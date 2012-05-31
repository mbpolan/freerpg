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
// newdialog.cpp: definition of the NewDialog class.

#include "newdialog.h"

#include "ui/ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent): QDialog(parent) {
	ui=new Ui::NewDialog;
	ui->setupUi(this);
}

QString NewDialog::getName() const {
	return ui->nameEdit->text();
}

QString NewDialog::getAuthor() const {
	return ui->authorEdit->text();
}

int NewDialog::getTileSize() const {
	return ui->sizeSB->value();
}

int NewDialog::getSubdivisions() const {
	return ui->divSB->value();
}
