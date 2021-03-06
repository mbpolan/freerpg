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
// configfile.h: declaration of the ConfigFile class.

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <iostream>
#include <stdexcept>
#include <map>

class ParseError: public std::runtime_error {

 public:
	explicit ParseError(const std::string &msg);
};

class ConfigFile {

 public:
	static ConfigFile* instance() throw(ParseError);

	std::string getIPAddress() const;
	int getPort() const;

	std::string getMapType() const;
	std::string getXMLMapFile() const;

	std::string getStoreType() const;
	std::string getSQLite3File() const;

 private:
	ConfigFile(const std::string &file) throw(ParseError);

	void parse(const std::string &file) throw(ParseError);
	void parseMapData(void *node) throw(ParseError);
	void parseStoreData(void *node) throw(ParseError);

	std::map<std::string, std::string> m_ValueMap;

	static ConfigFile *g_Instance;
};

#endif
