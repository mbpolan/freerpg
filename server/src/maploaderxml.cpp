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
// maploaderxml.cpp: definition of MapLoaderXML class.

#include <boost/lexical_cast.hpp>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "map.h"
#include "maploaderxml.h"

MapLoaderXML::MapLoaderXML(const std::string &file) {
	m_File=file;
}

Map* MapLoaderXML::load() throw(MapLoaderError) {
	xmlDocPtr doc=xmlParseFile(m_File.c_str());
	if (!doc)
		return NULL;

	int width, height;
	xmlNodePtr root=xmlDocGetRootElement(doc);

	// get map attributes
	width=boost::lexical_cast<int>((const char*) xmlGetProp(root, (const xmlChar*) "width"));
	height=boost::lexical_cast<int>((const char*) xmlGetProp(root, (const xmlChar*) "height"));

	// allocate a map object temporarily
	Map *map=new Map(width, height);

	xmlNodePtr ptr=root->children;
	while(ptr) {
		// tileset data
		if (xmlStrcmp(ptr->name, (const xmlChar*) "tilesets")==0) {
			try {
				parseTilesetData(map, ptr);
			}

			catch (const TilesetLoaderError &e) {
				throw MapLoaderError(std::string("Unable to load tileset: ")+e.what());
			}
		}

		// tile data
		else if (xmlStrcmp(ptr->name, (const xmlChar*) "tiles")==0)
			parseTileData(map, ptr);

		ptr=ptr->next;
	}

	xmlFreeDoc(doc);
	return map;
}

void MapLoaderXML::parseTilesetData(Map *map, void *node) throw(MapLoaderError, TilesetLoaderError) {
	xmlNodePtr n=(xmlNodePtr) node;
	xmlNodePtr ptr=n->children;

	// read each tile set
	while(ptr) {
		if (xmlStrcmp(ptr->name, (const xmlChar*) "tileset")==0) {
			int id=boost::lexical_cast<int>((const char*) xmlGetProp(ptr, (const xmlChar*) "id"));
			std::string file=std::string((const char*) xmlGetProp(ptr, (const xmlChar*) "file"));

			char *dir=dirname(const_cast<char*>(m_File.c_str()));
			std::string tsFile=std::string(dir)+"/"+file;

			// load the tileset itself
			TilesetLoader loader(tsFile);
			while(loader.hasNext()) {
				std::pair<int, Tile::BitMap> data=loader.readTile();

				// create a new tile object and add it to the file set
				Tile *tile=new Tile(id, data.first, data.second);
				map->addTilesetTile(id, tile);
			}
		}

		ptr=ptr->next;
	}
}

void MapLoaderXML::parseTileData(Map *map, void *node) throw(MapLoaderError) {
	xmlNodePtr n=(xmlNodePtr) node;
	xmlNodePtr ptr=n->children;

	// read each tile
	while(ptr) {
		if (xmlStrcmp(ptr->name, (const xmlChar*) "tile")==0) {
			int x=boost::lexical_cast<int>((const char*) xmlGetProp(ptr, (const xmlChar*) "x"));
			int y=boost::lexical_cast<int>((const char*) xmlGetProp(ptr, (const xmlChar*) "y"));
			int ts=boost::lexical_cast<int>((const char*) xmlGetProp(ptr, (const xmlChar*) "ts"));
			int id=boost::lexical_cast<int>((const char*) xmlGetProp(ptr, (const xmlChar*) "id"));

			map->putTile(x, y, ts, id);
		}

		ptr=ptr->next;
	}
}
