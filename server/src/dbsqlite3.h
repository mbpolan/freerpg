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
// dbsqlite3.h: declaration of the DBSqlite3 class.

#ifndef DBSQLITE3_H
#define DBSQLITE3_H

#include <iostream>

#include "database.h"

class DBSqlite3: public Database {

 public:
	static void use(const std::string &file) throw(DatabaseError);

	Account* getAccountByName(const std::string &username);
	Player* getPlayerByName(const std::string &name);

 private:
	DBSqlite3(const std::string &file) throw(DatabaseError);
	~DBSqlite3();

	sqlite3 *m_Handle;
};

#endif
