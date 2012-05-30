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
// NetworkManager.java: network communication class.

package client;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class NetworkManager extends Thread {
	
	private static NetworkManager instance=null;
	
	private NetworkListener listener;
	private Socket socket;
	
	public static void setInstance(NetworkManager mgr) {
		instance=mgr;
	}
	
	public static NetworkManager getInstance() {
		return instance;
	}
	
	public NetworkManager(String host, int port) throws UnknownHostException, IOException {
		this.listener=null;
		this.socket=new Socket(host, port);
	}
	
	public void setListener(NetworkListener listener) {
		this.listener=listener;
	}
	
	public void terminate() {
		try {
			socket.close();
		} 
		
		catch (IOException e) { }
	}
	
	public void run() {
		listener.onConnected();
		
		Packet p=new Packet();
		boolean loop=true;
		
		while(!socket.isClosed() && loop) {
			try {
				p.read(socket);
				handlePacket(p);
			} 
			
			catch (IOException e) {
				loop=false;
			}
			
			p.clear();
		}
		
		try {
			socket.close();
		} 
		
		catch (IOException e) { }
		
		listener.onDisconnected();
	}
	
	public void sendProtocolVersion() {
		try {
			Packet p=new Packet();
			p.addByte(ProtSpec.ID_PROTVER);
			p.addByte(ProtSpec.PROT_VERSION);
			
			p.write(socket);
		}
		
		catch (IOException ex) {
			try {
				socket.close();
			} 
			
			catch (IOException e) { }
		}
	}
	
	public void sendLogin(String username, String password) {
		try {
			Packet p=new Packet();
			p.addByte(ProtSpec.ID_LOGIN);
			p.addString(username);
			p.addString(password);

			p.write(socket);
		}

		catch (IOException ex) {
			try {
				socket.close();
			} 

			catch (IOException e) { }
		}
	}
	
	private void handlePacket(Packet p) {
		// get the header byte
		byte header=p.getByte();
		switch(header) {
			
			// protocol version verification
			case ProtSpec.ID_PROTVER: {
				byte result=p.getByte();
				listener.onVerification(result==ProtSpec.RES_OK);
			} break;
			
			// authentication result
			case ProtSpec.ID_LOGIN: {
				byte result=p.getByte();
				listener.onAuthentication(result==ProtSpec.RES_OK);
			} break;
			
			default: {
				System.out.println("Warning: unknown packet header "+header);
			} break;
		}
	}
}
