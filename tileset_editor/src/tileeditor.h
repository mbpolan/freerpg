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
// tileeditor.h: declaration of the TileEditor class.

#ifndef TILEEDITOR_H
#define TILEEDITOR_H

#include <QWidget>

#include "tile.h"

class TileEditor: public QWidget {
	Q_OBJECT
 public:
	TileEditor(QWidget *parent=NULL);

	void setCurrentBit(const Tile::Bit &bit);
	void setGrid(int lines);

	void setTile(Tile *tile);
	Tile* getTile() const;

	void fill();

 protected:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);

 private:
	Tile *m_Tile;
	Tile::Bit m_CurBit;
	QPixmap m_Image;
	int m_Lines;

};

#endif
