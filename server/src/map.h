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
// map.h: declaration of the Map class.

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <map>
#include <vector>

class Tile;

class Map {

 public:
	~Map();

	Tile* getTile(int x, int y) const;

 private:
	Map(int width, int height);

	void addTilesetTile(int ts, Tile *tile);
	void putTile(int x, int y, int ts, int id);

	int m_Width;
	int m_Height;
	std::map<int, std::map<int, Tile*> > m_Tilesets;
	std::vector<Tile*> m_Map;

	friend class MapLoaderXML;

};

#endif
