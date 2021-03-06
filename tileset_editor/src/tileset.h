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
// tileset.h: declaration of the Tileset class.

#ifndef TILESET_H
#define TILESET_H

#include <QObject>
#include <QVector>

#include "tile.h"

class Tileset: public QObject {
	Q_OBJECT
 public:
	explicit Tileset(int tileSize, int divs, QObject *parent=NULL);

	void setName(const QString &name);
	QString getName() const;

	void setAuthor(const QString &author);
	QString getAuthor() const;

	int getTileSize() const;
	int getDivisions() const;

	void addTile(Tile *tile);
	void removeTile(Tile *tile);

	int getNumTiles() const;
	Tile* getTileById(int id) const;
	Tile* getTileByIndex(int i) const;

 private:
	QString m_Name;
	QString m_Author;

	int m_TileSize;
	int m_Divs;

	QVector<Tile*> m_Tiles;
};

#endif
