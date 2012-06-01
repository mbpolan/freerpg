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

ParseError::ParseError(const std::string &msg): std::runtime_error(msg.c_str()) { }

ConfigFile* ConfigFile::instance() throw(ParseError) {
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

std::string ConfigFile::getMapType() const {
	return m_ValueMap.find("map-type")->second;
}

std::string ConfigFile::getXMLMapFile() const {
	return m_ValueMap.find("map-xml-file")->second;
}

std::string ConfigFile::getStoreType() const {
	return m_ValueMap.find("store-type")->second;
}

std::string ConfigFile::getSQLite3File() const {
	return m_ValueMap.find("sqlite3-file")->second;
}

ConfigFile::ConfigFile(const std::string &file) throw(ParseError) {
	parse(file);
}

void ConfigFile::parse(const std::string &file) throw(ParseError) {
	xmlDocPtr doc=xmlReadFile(file.c_str(), NULL, 0);
	if (!doc)
		throw ParseError("Unable to open config file");

	xmlNodePtr root=xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*) "server")!=0) {
		xmlFreeDoc(doc);
		throw ParseError("Root element is unexpected");
	}

	xmlNodePtr info=root->children;
	while(info) {
		const char *name=(const char*) info->name;

		if (xmlStrcmp(info->name, (const xmlChar*) "store")==0)
			parseStoreData(info);

		else if (xmlStrcmp(info->name, (const xmlChar*) "map")==0)
			parseMapData(info);

		else
			m_ValueMap[std::string(name)]=std::string((const char*) xmlNodeGetContent(info));

		info=info->next;
	}

	xmlFreeDoc(doc);
}

void ConfigFile::parseMapData(void *node) throw(ParseError) {
	xmlNodePtr n=(xmlNodePtr) node;

	const char *ctype=(const char*) xmlGetProp(n, (const xmlChar*) "type");
	if (!ctype)
		throw ParseError("Missing \"type\" attribute for map tag");

	std::string type=std::string(ctype);
	if (type=="xml") {
		xmlNodePtr ptr=n->children;
		bool found=false;

		while(ptr) {
			if (xmlStrcmp(ptr->name, (const xmlChar*) "file")==0) {
				m_ValueMap["map-xml-file"]=std::string((const char*) xmlNodeGetContent(ptr));
				found=true;
			}

			ptr=ptr->next;
		}

		if (!found)
			throw ParseError("XML map tag requires a single \"file\" child tag");
	}

	m_ValueMap["map-type"]=type;
}

void ConfigFile::parseStoreData(void *node) throw(ParseError) {
	xmlNodePtr n=(xmlNodePtr) node;

	const char *ctype=(const char*) xmlGetProp(n, (const xmlChar*) "type");
	if (!ctype)
		throw ParseError("Missing \"type\" attribute for store tag");

	std::string type=std::string(ctype);
	if (type=="sqlite3") {
		xmlNodePtr ptr=n->children;
		bool found=false;

		while(ptr) {
			if (xmlStrcmp(ptr->name, (const xmlChar*) "file")==0) {
				m_ValueMap["sqlite3-file"]=std::string((const char*) xmlNodeGetContent(ptr));
				found=true;
			}

			ptr=ptr->next;
		}

		if (!found)
			throw ParseError("SQLite3 store tag requires a single \"file\" child tag");
	}

	m_ValueMap["store-type"]=type;
}
