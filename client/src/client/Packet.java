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
// Packet.java: network packet encapsulation.

package client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class Packet {
	
	private byte[] buffer;
	private int size;
	private int pos;
	
	public Packet() {
		// assume a size of 1KB
		this.buffer=new byte[1024];
		clear();
	}
	
	public Packet(int length) {
		this.buffer=new byte[1024];
		clear();
	}
	
	public void clear() {
		pos=2;
		size=0;
	}
	
	/**
	 * Appends a single byte to the end of this message.
	 * @param b The byte.
	 */
	public void addByte(byte b) {
		buffer[pos++]=b;
		size+=1;
	}
	
	/**
	 * Retrieves a byte from this message.
	 * @return A byte.
	 */
	public byte getByte() {
		return buffer[pos++];
	}
	
	public void addString(String str) {
		addUint16((short) str.length());
		
		for (int i=0; i<str.length(); i++) {
			byte b=(byte) str.charAt(i);
			addByte(b);
		}
	}
	
	public String getString() {
		short length=getUint16();
		String str="";
		
		for (int i=0; i<length; i++)
			str+=(char) getByte();
		
		return str;
	}
	
	public void addUint16(short n) {
		buffer[pos++]=(byte) n;
		buffer[pos++]=(byte) (n >> 8);
		
		size+=2;
	}
	
	public short getUint16() {
		short n=(short) (buffer[pos] | (buffer[pos+1] << 8));
		pos+=2;
		
		return n;
	}
	
	public void addUint32(int n) {
		buffer[pos++]=(byte) n;
		buffer[pos++]=(byte) (n >> 8);
		buffer[pos++]=(byte) (n >> 16);
		buffer[pos++]=(byte) (n >> 24);
		
		size+=4;
	}
	
	public int getUint32() {
		int n=(buffer[pos] | (buffer[pos+1] << 8) | 
			  (buffer[pos+2] << 16) | (buffer[pos+3] << 24));
		pos+=4;
		
		return n;
	}
	
	/**
	 * Clears this message's contents and reads in a new one from the assigned socket.
	 * @param socket The socket to read from.
	 * @throws IOException If a network error occurs.
	 */
	public void read(Socket socket) throws IOException {
		InputStream in=socket.getInputStream();

		// first read only the length to avoid reading in multiple packets
		byte[] msg=new byte[2];

		int read=0;
		do {
			int n=in.read(msg, read, 2);
			read+=n;
		}
		while(read<2);

		// this is how many bytes the payload is
		size=(msg[0] | msg[1] >> 8);

		// corrupt packet?
		if (size<0)
			throw new IOException("Corrupt length ("+size+")");

		// if the length is zero, this is nothing more than a keep-alive packet
		if (size==0)
			return;
		
		// read the entire payload
		read=0;
		do {
			int n=in.read(buffer, read+2, size-read);
			read+=n;
		} while(read<size);
	}
	
	/**
	 * Writes the contents of this message to the socket.
	 * @param socket The socket to write to.
	 * @throws IOException If a network error occurs.
	 */
	public void write(Socket socket) throws IOException {
		try {
			OutputStream out=socket.getOutputStream();
			
			// adjust the size of the packet
			buffer[0]=(byte) size;
			buffer[1]=(byte) (size << 8);
			
			// we're all set -- send the data out
			out.write(buffer, 0, size+2);
			
		}
		
		catch (IOException e) {
			throw e;
		}
	}
}
