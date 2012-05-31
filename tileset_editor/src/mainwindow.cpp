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
#include <QMessageBox>

#include "iotileset.h"
#include "mainwindow.h"
#include "newdialog.h"

#include "ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
	ui=new Ui::MainWindow;
	ui->setupUi(this);
	m_Tileset=NULL;
	m_Modified=false;
	m_SavePath=QString::null;

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
	if (m_Tileset) {
		delete m_Tileset;
		m_Tileset=NULL;
	}
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

		m_Modified=true;
	}
}

void MainWindow::onFileOpen() {
	QString path=QFileDialog::getOpenFileName(this, tr("Open Tileset"), tr("Tilesets (*.tls)"));
	if (path.isNull() || path.isEmpty())
		return;

	IOTileset io;
	if (!(m_Tileset=io.load(path)))
		QMessageBox::critical(this, tr("Error"), tr("Unable to load tileset!"), QMessageBox::Ok);

	else {
		setWindowTitle(QString("FreeRPG Tileset Editor - ")+m_Tileset->getName());

		ui->tileTree->clear();
		ui->editor->setGrid(m_Tileset->getDivisions());

		for (int i=0; i<m_Tileset->getNumTiles(); i++) {
			Tile *t=m_Tileset->getTileByIndex(i);

			QTreeWidgetItem *item=new QTreeWidgetItem(ui->tileTree);
			item->setText(0, QString("%1").arg(t->getId()));
			item->setIcon(1, QIcon(t->getImage()));

			ui->tileTree->addTopLevelItem(item);
		}

		centralWidget()->setEnabled(true);
		m_Modified=true;
		m_SavePath=path;
	}

}

void MainWindow::onFileSave() {
	if (!m_SavePath.isNull()) {
		IOTileset io(m_Tileset);
		if (!io.save(m_SavePath))
			QMessageBox::information(this, tr("Error"), tr("Unable to save tileset!"), QMessageBox::Ok);
	}

	else
		onFileSaveAs();
}

void MainWindow::onFileSaveAs() {
	QString path=QFileDialog::getSaveFileName(this, tr("Save Tileset"), "", tr("Tilesets (*.tls)"));
	if (path.isNull() || path.isEmpty())
		return;

	IOTileset io(m_Tileset);
	if (!io.save(path))
		QMessageBox::information(this, tr("Error"), tr("Unable to save tileset!"), QMessageBox::Ok);

	m_Modified=false;
	m_SavePath=path;
}

void MainWindow::onFileQuit() {
	if (m_Tileset && m_Modified) {
		int res=QMessageBox::question(this, tr("Save"),
								tr("Your tileset has been modified. Would you like to save?"),
								QMessageBox::Yes, QMessageBox::No);
		if (res==QMessageBox::Yes)
			onFileSave();
	}

	qApp->quit();
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
		m_Modified=true;
	}
}

void MainWindow::onTileRemove() {
	QTreeWidgetItem *item=ui->tileTree->currentItem();
	if (item) {
		int id=item->text(0).toInt();
		Tile *t=m_Tileset->getTileById(id);

		m_Tileset->removeTile(t);
		ui->tileTree->takeTopLevelItem(ui->tileTree->indexOfTopLevelItem(item));

		m_Modified=true;
	}
}

void MainWindow::onTileApplyAll() {
	ui->editor->fill();
	m_Modified=true;
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

		m_Modified=true;
	}
}

void MainWindow::onAlphaChanged(int value) {
	Tile *t=ui->editor->getTile();
	if (t) {
		t->setAlpha((char) ui->alphaSB->value());
		m_Modified=true;
	}
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
