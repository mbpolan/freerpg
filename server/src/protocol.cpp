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
// protocol.cpp: definition of the Protocol class.

#include "packet.h"
#include "player.h"
#include "protspec.h"
#include "protocol.h"

Protocol::Protocol(int socket) {
	m_Socket=socket;
}

void Protocol::setPlayer(Player *player) {
	m_Player=player;
}

bool Protocol::verify() {
	// first verify the client's protocol version
	Packet p, r;
	p.read(m_Socket);

	char id=p.byte();
	char version=p.byte();
	if (id!=ProtSpec::ID_PROTVER || version!=ProtSpec::PROT_VERSION) {
		std::cout << "Protver: " << std::ios::hex << version << std::ios::dec << ", but expected " << ProtSpec::PROT_VERSION << "\n";

		r.addByte(ProtSpec::ID_PROTVER);
		r.addByte(ProtSpec::RES_FAIL);
		r.write(m_Socket);

		return false;
	}

	r.addByte(ProtSpec::ID_PROTVER);
	r.addByte(ProtSpec::RES_OK);
	r.write(m_Socket);

	return true;
}

std::pair<std::string, std::string> Protocol::getCredentials() {
	Packet p;
	p.read(m_Socket);

	char id=p.byte();
	if (id!=ProtSpec::ID_LOGIN)
		return std::make_pair("", "");

	std::string username=p.string();
	std::string password=p.string();

	return std::make_pair(username, password);
}

void Protocol::sendLoginResult(bool ok) {
	Packet p;

	p.addByte(ProtSpec::ID_LOGIN);
	p.addByte(ok ? ProtSpec::RES_OK : ProtSpec::RES_FAIL);
	p.write(m_Socket);
}

void Protocol::sendCharacterList(const std::vector<std::string> &lst) {
	Packet p;

	p.addByte(ProtSpec::ID_CHARLIST);
	p.addUint16(lst.size());

	for (int i=0; i<lst.size(); i++)
		p.addString(lst[i]);

	p.write(m_Socket);
}

void Protocol::sendLoginComplete() {
	Packet p;

	p.addByte(ProtSpec::ID_LOGIN_COMPLETE);
	p.write(m_Socket);
}


void Protocol::sendMapUpdate(const Map::IDMap &area) {
	Packet p;

	p.addByte(ProtSpec::ID_MAP_UPDATE);
	p.addMapArea(area);
	p.write(m_Socket);
}

std::string Protocol::getCharacter() {
	Packet p;
	p.read(m_Socket);

	char id=p.byte();
	if (id!=ProtSpec::ID_CHARLIST)
		return "";

	return p.string();
}

void Protocol::loop() {
	while(1) {
		Packet p;
		Packet::Result res=p.read(m_Socket);
		if (res!=Packet::NoError)
			break;

		handlePacket(p);
	}
}

void Protocol::handlePacket(Packet &p) {
	char header=p.byte();

}
