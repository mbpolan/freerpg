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
// map.cpp: definition of the Map class.

#include "map.h"
#include "tile.h"

Map::Map(int width, int height) {
	m_Map.resize(width*height, NULL);
	m_Width=width;
	m_Height=height;
}

Map::~Map() {
	// then each tile itself
	std::map<int, std::map<int, Tile*> >::iterator it;
	for (it=m_Tilesets.begin(); it!=m_Tilesets.end(); ++it) {
		for (int i=0; i<it->second.size(); i++)
			delete it->second[i];
	}

	m_Tilesets.clear();
}

Tile* Map::getTile(int x, int y) const {
	return m_Map[x*m_Width+y];
}


void Map::addTilesetTile(int ts, Tile *tile) {
	if (m_Tilesets.find(ts)==m_Tilesets.end())
		m_Tilesets[ts]=std::map<int, Tile*>();

	m_Tilesets[ts][tile->getId()]=tile;
}

void Map::putTile(int x, int y, int ts, int id) {
	m_Map[x*m_Width+y]=m_Tilesets[ts][id];
}
