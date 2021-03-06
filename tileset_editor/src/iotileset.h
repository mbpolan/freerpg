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
// iotileset.h: declaration of IOTileset class.

#ifndef IOTILESET_H
#define IOTILESET_H

#include <QObject>

#include "tileset.h"

class IOTileset: public QObject {
	Q_OBJECT
 public:
    explicit IOTileset(QObject *parent=NULL);
	IOTileset(Tileset *tileset, QObject *parent=NULL);

	bool save(const QString &file);
	Tileset* load(const QString &file);

 private:
	Tileset *m_Tileset;

};

#endif // IOTILESET_H
