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
// tile.h: declaration of the Tile class.

#ifndef TILE_H
#define TILE_H

#include <vector>

class Tile {

 public:
	enum Bit {
		CLOSED=		0,
		OPEN=		1,
		OVERDRAW=	2
	};

	typedef std::vector<std::vector<Tile::Bit> > BitMap;

 public:
	Tile(int id, const BitMap &bmap);

	int getId() const;
	Tile::Bit getBit(int x, int y) const;

 private:
	int m_Id;
	Tile::BitMap m_BitMap;
};

#endif
