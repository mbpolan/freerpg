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
// account.h: declaration of the Account class.

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <vector>

#include "player.h"

class Account {

 public:
	Account(const std::string &username, const std::string &password);

	void addCharacter(const std::string &name);
	std::vector<std::string> getCharacters() const;

	std::string getUsername() const;
	std::string getPassword() const;

 private:
	std::string m_Username;
	std::string m_Password;
	std::vector<std::string> m_Characters;
};

#endif
