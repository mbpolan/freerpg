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
// dbsqlite3.h: definition of the DBSqlite3 class.

#include <boost/lexical_cast.hpp>
#include <sstream>

#include "dbsqlite3.h"

void DBSqlite3::use(const std::string &file) throw(DatabaseError){
	Database::g_Instance=new DBSqlite3(file);
}

Account* DBSqlite3::getAccountByName(const std::string &username) {
	int rows, cols;
	std::string sql="SELECT * FROM accounts WHERE username=\""+username+"\"";
	char **table, *msg;

	// if this call fails or such an account does not exist, report failure
	if (sqlite3_get_table(m_Handle, sql.c_str(), &table, &rows, &cols, &msg)!=SQLITE_OK || rows!=1)
		return NULL;

	Account *account=new Account(std::string(table[3]), std::string(table[4]));
	int acc=boost::lexical_cast<int>(table[5]);

	sqlite3_free_table(table);

	// gather a list of characters in this account
	std::stringstream ss;
	ss << "SELECT name FROM characters WHERE account=" << acc;
	if (sqlite3_get_table(m_Handle, ss.str().c_str(), &table, &rows, &cols, &msg)!=SQLITE_OK) {
		delete account;
		return NULL;
	}

	for (int i=0; i<rows; i++)
		account->addCharacter(std::string(table[i+1]));

	sqlite3_free_table(table);
	return account;
}

Player* DBSqlite3::getPlayerByName(const std::string &name) {
	// TODO
	return new Player(name);
}

DBSqlite3::DBSqlite3(const std::string &file) throw(DatabaseError) {
	if (sqlite3_open(file.c_str(), &m_Handle)!=SQLITE_OK) {
		m_Handle=NULL;
		throw DatabaseError("Unable to open database");
	}
}

DBSqlite3::~DBSqlite3() {
	if (m_Handle) {
		sqlite3_close(m_Handle);
		m_Handle=NULL;
	}
}
