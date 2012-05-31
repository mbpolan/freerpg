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
// serversocket.h: declaration of the ServerSocket class.

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <iostream>
#include <stdexcept>

class SocketError: public std::runtime_error {

 public:
	explicit SocketError(const std::string &msg);
};

class ServerSocket {

 public:
	class ClientData {

	 public:
			ClientData(const std::string &ip, int socket);

			std::string getIP() const;
			int getSocket() const;

	 private:
			std::string m_IP;
			int m_Socket;
	};

 public:
	ServerSocket(const std::string &ip, int port);
	~ServerSocket();

	void bind() throw(SocketError);
	void listen() throw(SocketError);
	ClientData* accept() throw(SocketError);

 private:
	std::string m_IP;
	int m_Port;
	int m_Socket;
};

#endif
