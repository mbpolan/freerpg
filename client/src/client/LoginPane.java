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
// LoginPane.java: interface and controller for logging into a server.

package client;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.UnknownHostException;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

public class LoginPane extends JPanel implements NetworkListener {
	
	private static final long serialVersionUID = 1L;
	
	private JTextField userEdit;
	private JTextField passEdit;
	private JTextField serverEdit;
	private JButton loginButton;
	private ImageIcon bg;
	private JLabel statusBar;
	
	public LoginPane() {
		create();
		
		setOpaque(false);
		bg=new ImageIcon(LoginPane.class.getResource("bg.png"));
	}

	@Override
	public void onConnected() {
		statusBar.setText("Connected, authenticating...");
		
		// send the protocol version
		NetworkManager.getInstance().sendProtocolVersion();
	}

	@Override
	public void onDisconnected() {
		statusBar.setText("Disconnected from server");
	}

	@Override
	public void onError(String msg) {
	}
	
	@Override
	public void onVerification(boolean success) {
		NetworkManager mgr=NetworkManager.getInstance();
		
		if (success) {
			statusBar.setText("Protocol ok");
			mgr.sendLogin(userEdit.getText(), passEdit.getText());
		}
		
		else
			mgr.terminate();
	}
	
	@Override
	public void onAuthentication(boolean success) {
		NetworkManager mgr=NetworkManager.getInstance();
		
		if (success) {
			statusBar.setText("Login ok");
		}
		
		else {
			JOptionPane.showMessageDialog(this, "Login was invalid");
			mgr.terminate();
		}
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		g.drawImage(bg.getImage(), 0, 0, null);
		super.paintComponent(g);
	}
	
	private void onLogin() {
		String host;
		int port;

		String[] comp=serverEdit.getText().split(":");
		if (comp==null) {
			JOptionPane.showMessageDialog(this, "Server address must be in form \"xxx.xxx.xxx.xxx:yyyy\"");
			return;
		}

		host=comp[0];
		port=Integer.parseInt(comp[1]);

		statusBar.setText("Connecting to server...");
		
		try {
			NetworkManager mgr=new NetworkManager(host, port);
			NetworkManager.setInstance(mgr);
			mgr.setListener(this);
			
			mgr.start();
		} 
		
		catch (UnknownHostException e) {
			JOptionPane.showMessageDialog(this, e.getMessage());
		} 
		
		catch (IOException e) {
			JOptionPane.showMessageDialog(this, e.getMessage());
		}
		
	}
	
	private void create() {
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		JPanel ctr=new JPanel();
		
		GridLayout grid=new GridLayout(0, 2);
		ctr.setLayout(grid);
		
		// create the gui components
		userEdit=new JTextField();
		passEdit=new JPasswordField();
		serverEdit=new JTextField("127.0.0.1:9090");
		
		loginButton=new JButton("Login");
		loginButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				onLogin();
			}
		});
		
		statusBar=new JLabel("...");
		statusBar.setForeground(Color.WHITE);
		
		// and lay them out
		ctr.add(new JLabel("Username"));
		ctr.add(userEdit);
		ctr.add(new JLabel("Password"));
		ctr.add(passEdit);
		ctr.add(new JLabel("Server"));
		ctr.add(serverEdit);
		ctr.add(loginButton);
		
		ctr.setBorder(BorderFactory.createLineBorder(Color.BLACK, 1));
		ctr.setMaximumSize(ctr.getPreferredSize());
		ctr.setAlignmentX(Component.CENTER_ALIGNMENT);
		
		add(Box.createVerticalGlue());
		add(ctr);
		add(Box.createVerticalGlue());
		add(statusBar);
	}

}
