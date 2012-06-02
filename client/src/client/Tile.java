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
// Tile.java: the Tile class.

package client;

import static org.lwjgl.opengl.GL11.*;
import org.newdawn.slick.opengl.Texture;

public class Tile {
	
	private Texture texture;
	private int id;
	private int size;
	private int divs;
	private byte[] zMap;
	
	public Tile(int id, int tileSize, int divs, byte[] zMap, Texture texture) {
		this.id=id;
		this.size=64;
		this.divs=divs;
		this.zMap=zMap;
		this.texture=texture;
	}
	
	public int getId() {
		return id;
	}
	
	public void render(int x, int y) {
		int tid=texture.getTextureID();
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tid);
		
		glBegin(GL_QUADS); {
			glVertex2i(x, y);			glTexCoord2f(0.0f, 0.0f);
			glVertex2i(x+size, y);		glTexCoord2f(1.0f, 0.0f);
			glVertex2i(x+size, y+size);	glTexCoord2f(1.0f, 1.0f);
			glVertex2i(x, y+size);		glTexCoord2f(0.0f, 1.0f);
		}
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
	}
	
}
