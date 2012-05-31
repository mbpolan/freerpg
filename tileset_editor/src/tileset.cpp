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
// tileset.cpp: definition of the Tileset class.

#include "tileset.h"

Tileset::Tileset(int tileSize, int divs, QObject *parent): QObject(parent) {
	m_Name="";
	m_Author="";
	m_TileSize=tileSize;
	m_Divs=divs;
}

void Tileset::setName(const QString &name) {
	m_Name=name;
}

QString Tileset::getName() const {
	return m_Name;
}

void Tileset::setAuthor(const QString &author) {
	m_Author=author;
}

QString Tileset::getAuthor() const {
	return m_Author;
}

int Tileset::getTileSize() const {
	return m_TileSize;
}

int Tileset::getDivisions() const {
	return m_Divs;
}

void Tileset::addTile(Tile *tile) {
	m_Tiles.push_back(tile);
}

void Tileset::removeTile(Tile *tile) {
	for (int i=0; i<m_Tiles.size(); i++) {
		if (m_Tiles[i]==tile) {
			m_Tiles.remove(i);
			return;
		}
	}
}

Tile* Tileset::getTileById(int id) const {
	for (int i=0; i<m_Tiles.size(); i++) {
		if (m_Tiles[i]->getId()==id)
		    return m_Tiles[i];
	}

	return NULL;
}
