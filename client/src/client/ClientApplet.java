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
// ClientApplet.java: entry point into the client program.

package client;
import java.awt.Dimension;

import javax.swing.JApplet;
import javax.swing.SwingUtilities;


public class ClientApplet extends JApplet implements TransitionListener {
	
	private static final long serialVersionUID = 1L;
	
	private LoginPane loginPane;
	private GamePane gamePane;
	private State state;
	
	public void init() {
		try {
			SwingUtilities.invokeLater(new Runnable() {

				@Override
				public void run() {
					state=State.LOGIN;
					setSize(new Dimension(640, 480));
					
					// the initial screen is the login pane
					loginPane=new LoginPane(ClientApplet.this);
					add(loginPane);
					
					gamePane=new GamePane(ClientApplet.this);
					gamePane.setVisible(false);
				}
				
			});
		}
		
		catch (Exception ex) {
			System.out.println(ex.getMessage());
		}
	}
	
	public void destroy() {
		if (state==State.GAME) {
			gamePane.terminate();
			super.destroy();
		}
	}
	
	@Override
	public void onStateTransition(State completed) {
		if (completed==State.LOGIN){ 
			// remove and hide the login screen
			remove(loginPane);
			loginPane.setVisible(false);
			
			// and replace it with the game screen instead
			add(gamePane);
			gamePane.setVisible(true);
		}
		
		else {
			// remove and hide the game screen
			remove(gamePane);
			gamePane.setVisible(false);
			
			// and replace it with the login screen
			add(loginPane);
			loginPane.setVisible(true);
		}
	}

}
