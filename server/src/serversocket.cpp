/***************************************************************************
 *   Copyright (C) 2012 by FreeRPG Team                                    *
 *   http://freerpg.sf.net                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
// serversocket.cpp: definition of the ServerSocket class.

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sstream>

#include "serversocket.h"

SocketError::SocketError(const std::string &msg): std::runtime_error(msg.c_str()) { };

ServerSocket::ClientData::ClientData(const std::string &ip, int socket) {
	m_IP=ip;
	m_Socket=socket;
}

std::string ServerSocket::ClientData::getIP() const {
	return m_IP;
}

int ServerSocket::ClientData::getSocket() const {
	return m_Socket;
}

ServerSocket::ServerSocket(const std::string &ip, int port) {
	m_IP=ip;
	m_Port=port;
	m_Socket=-1;
}

ServerSocket::~ServerSocket() {
	if (m_Socket!=-1)
		close(m_Socket);
}

void ServerSocket::bind() throw(SocketError) {
	struct addrinfo hints, *list;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	std::stringstream ss;
	ss << m_Port;

	int ok;
	if ((ok=getaddrinfo(m_IP.c_str(), ss.str().c_str(), &hints, &list))!=0)
		throw SocketError("Unable to getaddrinfo");

	struct addrinfo *p=list;
	bool init=false;
	while(p && !init) {
		if ((m_Socket=socket(p->ai_family, p->ai_socktype, p->ai_protocol))<0) {
			close(m_Socket);

			p=p->ai_next;
			continue;
		}

		int yes=1;
		if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))<0) {
			close(m_Socket);

			p=p->ai_next;
			continue;
		}

		if (::bind(m_Socket, p->ai_addr, p->ai_addrlen)<0) {
			close(m_Socket);

			p=p->ai_next;
			continue;
		}

		init=true;
	}

	freeaddrinfo(list);

	if (!init)
		throw SocketError("Unable to bind to any interface");
}

void ServerSocket::listen() throw(SocketError) {
	if (::listen(m_Socket, 10)<0)
		throw SocketError("Unable to listen");
}

ServerSocket::ClientData* ServerSocket::accept() throw(SocketError) {
	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	char ip[INET6_ADDRSTRLEN];

	int sock;
	if ((sock=::accept(m_Socket, (struct sockaddr*) &client, &len))<0)
		throw SocketError("Accept failed");

	inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(ip));

	return new ClientData(std::string(ip), sock);
}
