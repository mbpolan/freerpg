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

#include <QObject>
#include <QPixmap>

class Tile: public QObject {
	Q_OBJECT

 public:
	enum Bit {
		CLOSED=	0,
		OPEN=	1,
		OVERDRAW=	2
	};

	typedef QVector<QVector<Tile::Bit> > BitMap;

 public:
	explicit Tile(int id, int divs, const QPixmap &img, QObject *parent=NULL);

	int getId() const;

	void setBit(int x, int y, const Bit &b);
	BitMap getBitMap() const;

	QPixmap getImage() const;

	void setIsAnimated(bool yes);
	bool isAnimated() const;

	void setNextFrame(int id);
	int getNextFrame() const;

	void setDelay(int delay);
	int getDelay() const;

	void setAlpha(char alpha);
	char getAlpha() const;

 private:
	int m_Id;
	BitMap m_BitMap;
	QPixmap m_Image;

	bool m_Animated;
	int m_NextFrame;
	int m_Delay;

	char m_Alpha;

};

#endif
