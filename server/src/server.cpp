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
// server.cpp: entry point into the server program

#include <boost/thread.hpp>
#include <iostream>

#include "configfile.h"
#include "protocol.h"
#include "serversocket.h"

void connectionHandler(ServerSocket::ClientData *client) {
	int socket=client->getSocket();

	Protocol *p=new Protocol(socket);
	if (p->authenticate())
		p->loop();

	close(socket);

	delete p;
	delete client;
}

int main(int argc, char *argv[]) {
	ConfigFile *cfg=ConfigFile::instance();
	ServerSocket socket(cfg->getIPAddress(), cfg->getPort());

	try {
		socket.bind();
		socket.listen();

		std::cout << "FreeRPG server v1.0 running...\n";

		while(1) {
			ServerSocket::ClientData *client=socket.accept();
			boost::thread thread(connectionHandler, client);
		}
	}

	catch (const std::runtime_error &e) {
		std::cout << e.what() << "\n";
	}

	return 0;
}
