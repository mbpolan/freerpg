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

#include <QFileDialog>

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

	connect(ui->tileTree, SIGNAL(itemSelectionChanged()), this, SLOT(onTileSelected()));

	connect(ui->openRB, SIGNAL(toggled(bool)), this, SLOT(onToolToggled(bool)));
	connect(ui->blockRB, SIGNAL(toggled(bool)), this, SLOT(onToolToggled(bool)));
	connect(ui->overdrawRB, SIGNAL(toggled(bool)), this, SLOT(onToolToggled(bool)));

	connect(ui->animEnabledCB, SIGNAL(toggled(bool)), this, SLOT(onAnimToggled(bool)));
	connect(ui->alphaSB, SIGNAL(valueChanged(int)), this, SLOT(onAlphaChanged(int)));

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

		if (m_Tileset)
		    delete m_Tileset;

		// allocate a new tileset
		m_Tileset=new Tileset(diag.getTileSize(), diag.getSubdivisions(), this);
		m_Tileset->setName(name);

		// update the editor's grid
		ui->editor->setGrid(diag.getSubdivisions());
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
	QStringList lst=QFileDialog::getOpenFileNames(this, tr("Open Image"), "", tr("Images (*.png)"));
	QStringList failures;

	for (int i=0; i<lst.size(); i++) {
		QString file=lst[i];

		// load the image first
		QPixmap pixmap(file);
		if (pixmap.isNull() || pixmap.width()!=m_Tileset->getTileSize() ||
		    pixmap.height()!=m_Tileset->getTileSize())
		    failures.push_back(file);

		// create a new tile for this image
		int id=findRandomId();
		Tile *t=new Tile(id, m_Tileset->getDivisions(), pixmap, m_Tileset);
		m_Tileset->addTile(t);

		// add it to the tree as well
		QTreeWidgetItem *item=new QTreeWidgetItem(ui->tileTree);
		item->setText(0, QString("%1").arg(id));
		item->setIcon(1, QIcon(pixmap));

		ui->tileTree->addTopLevelItem(item);
	}
}

void MainWindow::onTileRemove() {

}

void MainWindow::onTileApplyAll() {
	ui->editor->fill();
}

void MainWindow::onHelpAbout() {

}

void MainWindow::onToolToggled(bool) {
	if (ui->openRB->isChecked())
		ui->editor->setCurrentBit(Tile::OPEN);
	else if (ui->blockRB->isChecked())
		ui->editor->setCurrentBit(Tile::CLOSED);
	else
		ui->editor->setCurrentBit(Tile::OVERDRAW);
}

void MainWindow::onAnimToggled(bool yes) {
	Tile *t=ui->editor->getTile();
	if (t) {
		t->setIsAnimated(yes);
		t->setNextFrame(ui->nextFrameSB->value());
		t->setDelay(ui->delaySB->value());
	}
}

void MainWindow::onAlphaChanged(int value) {
	Tile *t=ui->editor->getTile();
	if (t)
		t->setAlpha((char) ui->alphaSB->value());
}

void MainWindow::onTileSelected() {
	QTreeWidgetItem *item=ui->tileTree->currentItem();
	if (item) {
		int id=item->text(0).toInt();
		Tile *t=m_Tileset->getTileById(id);

		if (t) {
			ui->editor->setTile(t);
			ui->animEnabledCB->setChecked(t->isAnimated());

			if (t->isAnimated()) {
				ui->nextFrameSB->setEnabled(true);
				ui->nextFrameSB->setValue(t->getNextFrame());

				ui->delaySB->setEnabled(true);
				ui->delaySB->setValue(t->getDelay());
			}

			else {
				ui->nextFrameSB->setEnabled(false);
				ui->delaySB->setEnabled(false);
			}
		}
	}
}

int MainWindow::findRandomId() const {
	int id=0;
	while(m_Tileset->getTileById(id))
	    id++;

	return id;
}
