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
#include "database.h"
#include "dbsqlite3.h"
#include "protocol.h"
#include "serversocket.h"

void connectionHandler(ServerSocket::ClientData *client) {
	int socket=client->getSocket();

	// first verify the client's protocol version
	Protocol *p=new Protocol(socket);
	if (p->verify()) {
		std::pair<std::string, std::string> login=p->getCredentials();
		Account *account=Database::instance()->getAccountByName(login.first);

		// try to load the user's account and verify it
		if (!account || account->getPassword()!=login.second)
			p->sendLoginResult(false);

		else {
			p->sendLoginResult(true);

			// send the character list for this account
			p->sendCharacterList(account->getCharacters());
			std::string character=p->getCharacter();

			// load the player
			Player *player=Database::instance()->getPlayerByName(character);
			p->setPlayer(player);
			p->loop();

			delete player;
		}

		delete account;
	}

	close(socket);

	delete p;
	delete client;
}

int main(int argc, char *argv[]) {
	try {
		ConfigFile *cfg=ConfigFile::instance();

		// attempt to load the correct database driver
		std::string db=cfg->getStoreType();

		// sqlite3 database
		if (db=="sqlite3")
			DBSqlite3::use(cfg->getSQLite3File());

		else
			throw DatabaseError("The only supported databases are: sqlite3");

		ServerSocket socket(cfg->getIPAddress(), cfg->getPort());

		socket.bind();
		socket.listen();

		std::cout << "FreeRPG server v1.0 running...\n";

		while(1) {
			ServerSocket::ClientData *client=socket.accept();
			boost::thread thread(connectionHandler, client);
		}
	}

	catch (const ParseError &e) {
		std::cout << "[ConfigFile]: " << e.what() << "\n";
	}

	catch (const DatabaseError &e) {
		std::cout << "[Database]: " << e.what() << "\n";
	}

	catch (const SocketError &e) {
		std::cout << "[Socket]: " << e.what() << "\n";
	}

	// clean up
	Database::close();

	return 0;
}
