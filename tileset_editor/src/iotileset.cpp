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
// iotileset.cpp: definition of IOTileset class.

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QFile>

#include "iotileset.h"

const char *MAGIC_NUM="TLS10";

IOTileset::IOTileset(QObject *parent): QObject(parent) {
	m_Tileset=NULL;
}

IOTileset::IOTileset(Tileset *tileset, QObject *parent): QObject(parent) {
	m_Tileset=tileset;
}

bool IOTileset::save(const QString &file) {
	QFile f(file);
	if (!f.open(QIODevice::WriteOnly))
		return false;

	QDataStream out(&f);

	// write the magic number
	out.writeRawData(MAGIC_NUM, strlen(MAGIC_NUM));

	// write header data
	std::string author=m_Tileset->getAuthor().toStdString();
	std::string name=m_Tileset->getName().toStdString();

	out << name.size();
	out.writeRawData(name.c_str(), name.size());

	out << author.size();
	out.writeRawData(author.c_str(), author.size());

	out << m_Tileset->getTileSize() << m_Tileset->getDivisions();
	out << m_Tileset->getNumTiles();

	int count=m_Tileset->getNumTiles();
	for (int i=0; i<count; i++) {
		Tile *t=m_Tileset->getTileByIndex(i);

		// write the id
		out << t->getId();

		// and pixel data
		QPixmap img=t->getImage();
		QByteArray bytes;
		QBuffer buf(&bytes);

		buf.open(QIODevice::WriteOnly);
		img.save(&buf, "PNG");
		int size=bytes.size();

		out << size;
		out.writeRawData(bytes.data(), size);

		// write the bit map
		Tile::BitMap bmap=t->getBitMap();
		for (int j=0; j<bmap.size(); j++) {
			for (int k=0; k<bmap.size(); k++) {
				qint8 b=bmap[j][k];
				out << b;
			}
		}

		// write the remaining meta data
		qint8 anim=t->isAnimated();
		out << anim;

		// only write animation data if the tile is animated
		if (anim) {
			out << t->getNextFrame();
			out << t->getDelay();
		}

		qint8 alpha=t->getAlpha();
		out << alpha;
	}

	f.close();
	return true;
}

Tileset* IOTileset::load(const QString &file) {
	QFile f(file);
	if (!f.open(QIODevice::ReadOnly))
		return NULL;

	QDataStream in(&f);

	// read the magic number
	char mn[strlen(MAGIC_NUM)+1];
	in.readRawData(mn, strlen(MAGIC_NUM));
	mn[strlen(MAGIC_NUM)]='\0';

	if (strcmp(MAGIC_NUM, mn)!=0) {
		qDebug("Magic numbers mismatched\n");
		return NULL;
	}

	// read the name
	int len;
	in >> len;

	char cname[len+1];
	in.readRawData(cname, len);
	cname[len]='\0';

	// read the author
	in >> len;

	char cauthor[len+1];
	in.readRawData(cauthor, len);
	cauthor[len]='\0';

	// read the remaining header info
	int tileSize, divs, count;
	in >> tileSize;
	in >> divs;
	in >> count;

	// allocate a new tileset for later
	Tileset *ts=new Tileset(tileSize, divs);
	QStringList failures;

	// read each tile
	for (int i=0; i<count; i++) {
		Tile *t=NULL;
		int id;
		in >> id;

		// read the length of the pixel data
		in >> len;

		// and the pixel data itself
		char pixels[len];
		in.readRawData(pixels, len);
		QImage img=QImage::fromData((const uchar*) pixels, len, "PNG");

		if (img.isNull()) {
			failures.push_back(QString("%1").arg(id));
			continue;
		}

		// allocate the tile object
		t=new Tile(id, divs, QPixmap::fromImage(img));

		// read the bit map
		for (int j=0; j<divs; j++) {
			for (int k=0; k<divs; k++) {
				qint8 b;
				in >> b;

				t->setBit(j, k, (Tile::Bit) b);
			}
		}

		// read animation data
		qint8 anim;
		in >> anim;
		t->setIsAnimated(anim);

		// read animation data if need be
		if (anim) {
			int nextFrame, delay;
			in >> nextFrame;
			in >> delay;

			t->setNextFrame(nextFrame);
			t->setDelay(delay);
		}

		qint8 alpha;
		in >> alpha;
		t->setAlpha(alpha);

		ts->addTile(t);
	}

	return ts;
}
