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
// database.h: declaration of the Database class.

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <stdexcept>
#include <sqlite3.h>

#include "account.h"

class DatabaseError: public std::runtime_error {

 public:
	explicit DatabaseError(const std::string &msg);
};

class Database {

	typedef Database* const Handle;

 public:
	static Handle instance();
	static void close();

	virtual Account* getAccountByName(const std::string &username)=0;
	virtual Player* getPlayerByName(const std::string &name)=0;

 protected:
	Database();

	static Database *g_Instance;

};

#endif
