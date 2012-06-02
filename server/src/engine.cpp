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
// engine.cpp: definition of the Engine class.

#include "engine.h"

Engine* Engine::g_Instance=NULL;

void Engine::create(Map *map) {
	Engine::g_Instance=new Engine(map);
}

Engine* Engine::instance() {
	return Engine::g_Instance;
}

void Engine::addPlayer(Player *player) {
	// TODO: actual player positioning
	int x, y;
	x=y=3;

	player->getProtocol()->sendMapUpdate(m_Map->getAreaAround(x,y));
}

Map* Engine::getMap() const {
	return m_Map;
}

Engine::Engine(Map *map) {
	m_Map=map;
}
