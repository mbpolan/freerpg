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
// GamePane.java: interface for viewing the game screen.

package client;

import java.awt.BorderLayout;
import java.awt.Canvas;

import javax.swing.JPanel;
import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.LWJGLUtil;
import org.lwjgl.Sys;
import org.lwjgl.opengl.Display;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.GLContext;

import client.TransitionListener.State;

import static org.lwjgl.opengl.GL11.*;

public class GamePane extends JPanel implements GameListener {
	
	private static final long serialVersionUID = 1L;
	
	private Canvas canvas;
	private Thread renderThread;
	private TransitionListener transitioner;
	private boolean running;
	
	public GamePane(TransitionListener transitioner) {
		this.transitioner=transitioner;
		setLayout(new BorderLayout());
		
		canvas=new Canvas() {

			@Override
			public void addNotify() {
				super.addNotify();
				System.out.println("NOTIFY");
				init();
			}

			@Override
			public void removeNotify() {
				deinit();
				super.removeNotify();
			}
		};
		
		canvas.setSize(getWidth(), getHeight());
		canvas.setFocusable(true);
		canvas.setIgnoreRepaint(true);
		
		add(canvas);
		canvas.requestFocus();
	}
	
	public void terminate() {
		remove(canvas);
	}
	
	@Override
	public void onDisconnected() {
		transitioner.onStateTransition(State.GAME);
	}
	
	private void loop() {
		while(running) {
			renderFrame();
			Display.update();
		}
		
		Display.destroy();
	}
	
	private void renderFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	private void initGL() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, getWidth(), getHeight(), 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
	}
	
	private void init() {
		renderThread=new Thread() {
			
			@Override
			public void run() {
				running=true;
				
				try {
					Display.setParent(canvas);
					Display.create();
					initGL();
				}
				
				catch (LWJGLException e) {
					System.out.println(e.getMessage());
				}
				
				loop();
			}
		};
		
		renderThread.start();
	}
	
	private void deinit() {
		running=false;
		try {
			renderThread.join();
		}
		
		catch (InterruptedException ex) { }
	}
}
