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
// tilesetloader.h: declaration of the TilesetLoader class.

#ifndef TILESETLOADER_H
#define TILESETLOADER_H

#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "tile.h"

class TilesetLoaderError: public std::runtime_error {

 public:
	explicit TilesetLoaderError(const std::string &msg);
};

class TilesetLoader {

 public:
	class Header {

	 public:
		Header(int tileSize=0, int divs=0);

		int getTileSize() const;
		int getDivisions() const;

	 private:
		int m_TileSize;
		int m_Divisions;
	};

 public:
	explicit TilesetLoader(const std::string &file) throw(TilesetLoaderError);
	~TilesetLoader();

	Header getHeader();
	bool hasNext() const;
	std::pair<int, Tile::BitMap> readTile();

 private:
	int readId();
	Tile::BitMap readBitMap();

	FILE *m_File;
	Header m_Header;

	int m_Count;
	int m_CurTile;
};

#endif
