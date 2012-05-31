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
// tileeditor.cpp: definition of the TileEditor class.

#include <QMouseEvent>
#include <QPainter>

#include "tileeditor.h"

TileEditor::TileEditor(QWidget *parent): QWidget(parent) {
	m_Tile=NULL;
	m_Lines=1;
	m_CurBit=Tile::OPEN;
}

void TileEditor::setCurrentBit(const Tile::Bit &bit) {
	m_CurBit=bit;
}

void TileEditor::setGrid(int lines) {
	m_Lines=lines;
}

void TileEditor::setTile(Tile *tile) {
	m_Tile=tile;
	if (m_Tile)
		m_Image=tile->getImage().scaled(QSize(255, 255));

	repaint();
}

Tile* TileEditor::getTile() const {
	return m_Tile;
}

void TileEditor::fill() {
	if (m_Tile) {
		for (int x=0; x<m_Lines; x++) {
			for (int y=0; y<m_Lines; y++)
				m_Tile->setBit(x, y, m_CurBit);
		}
	}

	repaint();
}

void TileEditor::paintEvent(QPaintEvent *e) {
	QWidget::paintEvent(e);
	QPainter p(this);

	p.save();

	p.setPen(Qt::black);
	p.drawRect(0, 0, width()-1, height()-1);

	p.restore();

	if (m_Tile) {
		p.drawPixmap(QPoint(1,1), m_Image);

		// draw grid lines over the image
		int dx=width()/m_Lines;
		int dy=height()/m_Lines;

		for (int i=0; i<m_Lines; i++) {
			p.drawLine(i*dx, 0, i*dx, height());
			p.drawLine(0, i*dy, width(), i*dy);
		}

		p.save();

		// draw color commentary over each square
		for (int x=0; x<m_Lines; x++) {
			for (int y=0; y<m_Lines; y++) {
				Tile::Bit bit=m_Tile->getBitMap()[x][y];
				QColor color;

				switch(bit) {
					case Tile::OPEN: color=Qt::green; break;
					case Tile::CLOSED: color=Qt::red; break;
					case Tile::OVERDRAW: color=Qt::yellow; break;
					default: break;
				}

				p.fillRect(x*dx+1, y*dy+1, dx-1, dy-1, QBrush(color, Qt::Dense5Pattern));
			}
		}

		p.restore();
	}
}

void TileEditor::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);

    if (!isEnabled())
		return;

	// determine what cell we clicked in
	int x=e->x()/(width()/m_Lines);
	int y=e->y()/(height()/m_Lines);

	m_Tile->setBit(x, y, m_CurBit);

	repaint();
}
