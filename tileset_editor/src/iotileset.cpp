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

#include <cstdio>
#include <fstream>
#include <QBuffer>
#include <QByteArray>
#include <QDebug>

#include "iotileset.h"

const char *MAGIC_NUM="TLS10";

IOTileset::IOTileset(QObject *parent): QObject(parent) {
	m_Tileset=NULL;
}

IOTileset::IOTileset(Tileset *tileset, QObject *parent): QObject(parent) {
	m_Tileset=tileset;
}

bool IOTileset::save(const QString &file) {
	FILE *out=fopen(file.toAscii(), "wb");
	if (!out)
		return false;

	fwrite(MAGIC_NUM, sizeof(char), strlen(MAGIC_NUM), out);

	// write header data
	std::string author=m_Tileset->getAuthor().toStdString();
	std::string name=m_Tileset->getName().toStdString();

	int len=name.size();
	fwrite(&len, sizeof(int), 1, out);
	fwrite(name.c_str(), sizeof(char), name.size(), out);

	len=author.size();
	fwrite(&len, sizeof(int), 1, out);
	fwrite(author.c_str(), sizeof(char), author.size(), out);

	int n=m_Tileset->getTileSize();
	fwrite(&n, sizeof(int), 1, out);

	n=m_Tileset->getDivisions();
	fwrite(&n, sizeof(int), 1, out);

	int count=m_Tileset->getNumTiles();
	fwrite(&count, sizeof(int), 1, out);

	for (int i=0; i<count; i++) {
		Tile *t=m_Tileset->getTileByIndex(i);

		// write the id
		n=t->getId();
		fwrite(&n, sizeof(int), 1, out);

		// and pixel data
		QPixmap img=t->getImage();
		QByteArray bytes;
		QBuffer buf(&bytes);

		buf.open(QIODevice::WriteOnly);
		img.save(&buf, "PNG");
		int size=bytes.size();

		fwrite(&size, sizeof(int), 1, out);
		fwrite(bytes.data(), sizeof(char), size, out);

		// write the bit map
		Tile::BitMap bmap=t->getBitMap();
		for (int j=0; j<bmap.size(); j++) {
			for (int k=0; k<bmap.size(); k++) {
				qint8 b=bmap[j][k];
				fputc(b, out);
			}
		}

		// write the remaining meta data
		qint8 anim=t->isAnimated();
		fputc(anim, out);

		// only write animation data if the tile is animated
		if (anim) {
		    n=t->getNextFrame();
		    fwrite(&n, sizeof(int), 1, out);

		    n=t->getDelay();
		    fwrite(&n, sizeof(int), 1, out);
		}

		qint8 alpha=t->getAlpha();
		fputc(alpha, out);
	}

	fclose(out);
	return true;
}

Tileset* IOTileset::load(const QString &file) {
	FILE *in=fopen(file.toAscii(), "rb");
	if (!in)
	    return false;

	// read the magic number
	char mn[strlen(MAGIC_NUM)+1];
	fread(mn, sizeof(char), strlen(MAGIC_NUM), in);
	mn[strlen(MAGIC_NUM)]='\0';

	if (strcmp(MAGIC_NUM, mn)!=0) {
		qDebug("Magic numbers mismatched\n");
		return NULL;
	}

	// read the name
	int len;
	fread(&len, sizeof(int), 1, in);

	char cname[len+1];
	fread(cname, sizeof(char), len, in);
	cname[len]='\0';

	// read the author
	fread(&len, sizeof(int), 1, in);

	char cauthor[len+1];
	fread(cauthor, sizeof(char), len, in);
	cauthor[len]='\0';

	// read the remaining header info
	int tileSize, divs, count;
	fread(&tileSize, sizeof(int), 1, in);
	fread(&divs, sizeof(int), 1, in);
	fread(&count, sizeof(int), 1, in);

	// allocate a new tileset for later
	Tileset *ts=new Tileset(tileSize, divs);
	ts->setAuthor(QString(cauthor));
	ts->setName(QString(cname));
	QStringList failures;

	// read each tile
	for (int i=0; i<count; i++) {
		Tile *t=NULL;
		int id;
		fread(&id, sizeof(int), 1, in);

		// read pixel data length
		fread(&len, sizeof(int), 1, in);

		// and the pixel data itself
		char pixels[len];
		fread(pixels, sizeof(char), len, in);
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
				qint8 b=fgetc(in);
				t->setBit(j, k, (Tile::Bit) b);
			}
		}

		// read animation data
		qint8 anim=fgetc(in);
		t->setIsAnimated(anim);

		// read animation data if need be
		if (anim) {
			int nextFrame, delay;
			fread(&nextFrame, sizeof(int), 1, in);
			fread(&delay, sizeof(int), 1, in);

			t->setNextFrame(nextFrame);
			t->setDelay(delay);
		}

		qint8 alpha=fgetc(in);
		t->setAlpha(alpha);

		ts->addTile(t);
	}

	return ts;
}
