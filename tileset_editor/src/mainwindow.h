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
// mainwindow.h: declaration of the MainWindow class.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "tileset.h"

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow {
	Q_OBJECT

 public:
	MainWindow(QWidget *parent=NULL);
	~MainWindow();

 private slots:
	void onFileNew();
	void onFileOpen();
	void onFileSave();
	void onFileSaveAs();
	void onFileQuit();

	void onTileAdd();
	void onTileRemove();
	void onTileApplyAll();
	void onTileProperties();

	void onHelpAbout();

	void onToolToggled(bool);
	void onAnimToggled(bool yes);
	void onAlphaChanged(int value);
	void onTileSelected();

 private:
	int findRandomId() const;

	Ui::MainWindow *ui;

	bool m_Modified;
	QString m_SavePath;
	Tileset *m_Tileset;
};

#endif
