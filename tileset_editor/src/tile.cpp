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
// tile.cpp: definition of the Tile class.

#include "tile.h"

Tile::Tile(int id, int divs, const QPixmap &img, QObject *parent): QObject(parent) {
	m_Id=id;
	m_Image=img;
	m_Animated=false;
	m_NextFrame=0;
	m_Delay=0;
	m_Alpha=255;

	m_BitMap.resize(divs);
	for (int i=0; i<m_BitMap.size(); i++) {
		m_BitMap[i].resize(divs);

		for (int j=0; j<m_BitMap.size(); j++)
		    m_BitMap[i][j]=Tile::OPEN;
	}
}

int Tile::getId() const {
	return m_Id;
}

void Tile::setBit(int x, int y, const Bit &b) {
	m_BitMap[x][y]=b;
}

Tile::BitMap Tile::getBitMap() const {
	return m_BitMap;
}

QPixmap Tile::getImage() const {
	return m_Image;
}

void Tile::setIsAnimated(bool yes) {
	m_Animated=yes;
}

bool Tile::isAnimated() const {
	return m_Animated;
}

void Tile::setNextFrame(int id) {
	m_NextFrame=id;
}

int Tile::getNextFrame() const {
	return m_NextFrame;
}

void Tile::setDelay(int delay) {
	m_Delay=delay;
}

int Tile::getDelay() const {
	return m_Delay;
}

void Tile::setAlpha(char alpha) {
	m_Alpha=alpha;
}

char Tile::getAlpha() const {
	return m_Alpha;
}
