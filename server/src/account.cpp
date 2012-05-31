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
// account.cpp: definition of the Account class.

#include "account.h"

Account::Account(const std::string &username, const std::string &password) {
	m_Username=username;
	m_Password=password;
}

void Account::addCharacter(const std::string &name) {
	m_Characters.push_back(name);
}

std::vector<std::string> Account::getCharacters() const {
	return m_Characters;
}

std::string Account::getUsername() const {
	return m_Username;
}

std::string Account::getPassword() const {
	return m_Password;
}
