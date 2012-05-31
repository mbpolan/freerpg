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

#include <QPainter>

#include "tileeditor.h"

TileEditor::TileEditor(QWidget *parent): QWidget(parent) {
}

void TileEditor::setImage(const QPixmap &image) {
	m_Image=image;
	repaint();
}

void TileEditor::paintEvent(QPaintEvent *e) {
	QWidget::paintEvent(e);
	QPainter p(this);

	p.save();

	p.setPen(Qt::black);
	p.drawRect(0, 0, width()-1, height()-1);

	p.restore();

	p.drawPixmap(QPoint(0,0), m_Image);
}
