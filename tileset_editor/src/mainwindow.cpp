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
// mainwindow.cpp: definition of the MainWindow class.

#include "mainwindow.h"
#include "newdialog.h"

#include "ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
	ui=new Ui::MainWindow;
	ui->setupUi(this);

	// connect actions
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(onFileNew()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onFileOpen()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onFileSave()));
	connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onFileQuit()));

	connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(onTileAdd()));
	connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(onTileRemove()));
	connect(ui->actionApply_To_All, SIGNAL(triggered()), this, SLOT(onTileApplyAll()));

	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onHelpAbout()));

	centralWidget()->setEnabled(false);
}

MainWindow::~MainWindow() {
}

void MainWindow::onFileNew() {
	NewDialog diag(this);
	if (diag.exec()==QDialog::Accepted) {
		QString name=diag.getName();

		centralWidget()->setEnabled(true);
		setWindowTitle(QString("FreeRPG Tileset Editor - ")+name);
	}
}

void MainWindow::onFileOpen() {

}

void MainWindow::onFileSave() {

}

void MainWindow::onFileSaveAs() {

}

void MainWindow::onFileQuit() {

}

void MainWindow::onTileAdd() {

}

void MainWindow::onTileRemove() {

}

void MainWindow::onTileApplyAll() {

}

void MainWindow::onHelpAbout() {

}
