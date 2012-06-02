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
// tilesetloader.cpp: definition of TilesetLoader class.

#include <cstring>

#include "tilesetloader.h"

const char *MAGIC_NUM="TLS10";

TilesetLoaderError::TilesetLoaderError(const std::string &msg):
	std::runtime_error(msg.c_str()) {
}

///////////////////////////////////////////////////////////////////////////////

TilesetLoader::Header::Header(int tileSize, int divs) {
	m_TileSize=tileSize;
	m_Divisions=divs;
}

int TilesetLoader::Header::getTileSize() const {
	return m_TileSize;
}

int TilesetLoader::Header::getDivisions() const {
	return m_Divisions;
}

///////////////////////////////////////////////////////////////////////////////

TilesetLoader::TilesetLoader(const std::string &file) throw(TilesetLoaderError) {
	m_CurTile=1;

	m_File=fopen(file.c_str(), "rb");
	if (!m_File)
		throw TilesetLoaderError("Unable to open tileset");

	char mn[strlen(MAGIC_NUM)+1];
	mn[strlen(MAGIC_NUM)]='\0';
	fread(mn, sizeof(char), strlen(MAGIC_NUM), m_File);

	if (strcmp(mn, MAGIC_NUM)!=0)
		throw TilesetLoaderError("This is not a TLS file");

	// skip the tileset name
	int len;
	fread(&len, sizeof(int), 1, m_File);
	fseek(m_File, len, SEEK_CUR);

	// skip the author's name
	fread(&len, sizeof(int), 1, m_File);
	fseek(m_File, len, SEEK_CUR);

	// read the header data
	int tileSize, divs;
	fread(&tileSize, sizeof(int), 1, m_File);
	fread(&divs, sizeof(int), 1, m_File);
	fread(&m_Count, sizeof(int), 1, m_File);

	std::cout << m_Count << "\n";

	m_Header=TilesetLoader::Header(tileSize, divs);
}

TilesetLoader::~TilesetLoader() {
	if (m_File) {
		fclose(m_File);
		m_File=NULL;
	}
}

TilesetLoader::Header TilesetLoader::getHeader() {
	return m_Header;
}

bool TilesetLoader::hasNext() const {
	return (m_CurTile<=m_Count);
}

std::pair<int, Tile::BitMap> TilesetLoader::readTile() {
	int id=readId();
	Tile::BitMap bmap=readBitMap();

	m_CurTile++;
	return std::make_pair(id, bmap);
}

int TilesetLoader::readId() {
	int id;
	fread(&id, sizeof(int), 1, m_File);

	return id;
}

Tile::BitMap TilesetLoader::readBitMap() {
	int size=m_Header.getDivisions();

	// skip the pixel data
	int len;
	fread(&len, sizeof(int), 1, m_File);
	fseek(m_File, len, SEEK_CUR);

	// read the bitmap
	Tile::BitMap bmap;
	bmap.resize(size);

	for (int i=0; i<size; i++) {
		bmap[i].resize(size);

		for (int j=0; j<size; j++) {
			char b=fgetc(m_File);
			bmap[i][j]=(Tile::Bit) b;
		}
	}

	// skip the remaining meta data
	char anim=fgetc(m_File);
	if (anim)
		fseek(m_File, sizeof(int)*2, SEEK_CUR);

	fseek(m_File, sizeof(char), SEEK_CUR);
	return bmap;
}
