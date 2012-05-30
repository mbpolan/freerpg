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
// configfile.h: definition of the ConfigFile class.

#include <boost/lexical_cast.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "configfile.h"

ConfigFile* ConfigFile::g_Instance=NULL;

ConfigFile* ConfigFile::instance() throw(std::runtime_error) {
	if (!ConfigFile::g_Instance)
		return new ConfigFile("config.xml");

	else
		return ConfigFile::g_Instance;
}

std::string ConfigFile::getIPAddress() const {
	return m_ValueMap.find("ip")->second;
}

int ConfigFile::getPort() const {
	try {
		return boost::lexical_cast<int>(m_ValueMap.find("port")->second);
	}

	catch (const boost::bad_lexical_cast &e) {
		return -1;
	}
}

ConfigFile::ConfigFile(const std::string &file) throw(std::runtime_error) {
	parse(file);
}

void ConfigFile::parse(const std::string &file) throw(std::runtime_error) {
	xmlDocPtr doc=xmlReadFile(file.c_str(), NULL, 0);
	if (!doc)
		throw std::runtime_error("Unable to open config file");

	xmlNodePtr root=xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*) "server")!=0) {
		xmlFreeDoc(doc);
		throw std::runtime_error("Root element is unexpected");
	}

	xmlNodePtr info=root->children;
	while(info) {
		const char *name=(const char*) info->name;
		m_ValueMap[std::string(name)]=std::string((const char*) xmlNodeGetContent(info));

		info=info->next;
	}

	xmlFreeDoc(doc);
}
