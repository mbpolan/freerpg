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
// TilesetManager.java: tile and tileset manager class.

package client;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.newdawn.slick.opengl.Texture;
import org.newdawn.slick.opengl.TextureLoader;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class TilesetManager {
	
	private static final String MAGIC_NUM="TLS10";
	
	private Map<Integer, Tileset> tilesets;
	private boolean isLoaded;
	private static TilesetManager instance=null;
	
	public static TilesetManager getInstance()  {
		if (instance==null)
			instance=new TilesetManager();
		
		return instance;
	}
	
	public boolean isLoaded() {
		return isLoaded;
	}
	
	public Tile getTileFor(int tId, int id) {
		if (!tilesets.containsKey(tId))
			return null;
		
		Tileset ts=tilesets.get(tId);
		return ts.getTileById(id);
	}
	
	public void load() throws GraphicsException {
		try {
			DocumentBuilderFactory factory=DocumentBuilderFactory.newInstance();
			DocumentBuilder builder=factory.newDocumentBuilder();
			Document doc=builder.parse(TilesetManager.class.getResource("content/tilesets.xml").toString());
			
			NodeList nodes=doc.getDocumentElement().getChildNodes();
			for (int i=0; i<nodes.getLength(); i++) {
				Node n=nodes.item(i);
				if (n.getNodeType()==Node.ELEMENT_NODE) {
					Element e=(Element) n;
					
					int tid=Integer.parseInt(e.getAttribute("id"));
					String file=e.getAttribute("file");
					
					File f=new File(TilesetManager.class.getResource("content/"+file).toURI());
					RandomAccessFile rf=new RandomAccessFile(f, "r");
					loadTileset(tid, rf);
				}
			}
			
			isLoaded=true;
		}
		
		catch (ParserConfigurationException ex) {
			throw new GraphicsException(ex.getMessage());
		}
		
		catch (IOException ex) {
			throw new GraphicsException(ex.getMessage());
		}
		
		catch (SAXException ex) {
			throw new GraphicsException(ex.getMessage());
		} 
		
		catch (URISyntaxException e) { }
	}
	
	private void loadTileset(int tId, RandomAccessFile rf) throws GraphicsException {
		try {
			BinaryFile in=new BinaryFile(rf);
			
			// verify magic number
			String smn=in.readFixedString(MAGIC_NUM.length());
			if (!smn.equals(MAGIC_NUM))
				throw new GraphicsException("This is not a TLS file");
			
			// skip the author and name strings
			int len=(int) in.readDWord();
			rf.skipBytes(len);
			
			len=(int) in.readDWord();
			rf.skipBytes(len);
			
			// read header info
			int tileSize=(int) in.readDWord();
			int divs=(int) in.readDWord();
			int count=(int) in.readDWord();
			
			// create a new tileset object
			Tileset ts=new Tileset(tileSize, divs);
			
			for (int i=0; i<count; i++) {
				// read the id
				int id=(int) in.readDWord();
				
				// read the pixel data
				len=(int) in.readDWord();
				byte[] pixels=new byte[len];
				rf.read(pixels, 0, len);
				
				// create an opengl texture using these pixels
				ByteArrayInputStream bin=new ByteArrayInputStream(pixels);
				Texture texture=TextureLoader.getTexture("PNG", bin);
				
				// read the bit map
				byte[] z=new byte[divs*divs];
				for (int j=0; j<divs; j++) {
					for (int k=0; k<divs; k++) {
						byte b=rf.readByte();
						if (b==0x02)
							z[j*divs+k]=1;
						else
							z[j*divs+k]=0;
					}
				}
				
				// read animation data
				byte anim=rf.readByte();
				if (anim==0x1) {
					int nextFrame=(int) in.readDWord();
					int delay=(int) in.readDWord();
				}
				
				byte alpha=rf.readByte();
				
				// store this data as a tile
				Tile tile=new Tile(id, tileSize, divs, z, texture);
				ts.addTile(tile);
			}
			
			tilesets.put(tId, ts);
		} 
		
		catch (FileNotFoundException e) {
			e.printStackTrace();
		} 
		
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	
	private TilesetManager() {
		tilesets=new HashMap<Integer, Tileset>();
		isLoaded=false;
	}
}
