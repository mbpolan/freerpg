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
// ProtSpec.java: protocol specification.

package client;

public final class ProtSpec {
	
	public static final byte MAP_TILES_WIDE=			11;
	public static final byte MAP_TILES_HIGH=			9;
	
	public static final byte PROT_VERSION=(byte)		0x01;
	
	public static final byte RES_FAIL=					0x00;
	public static final byte RES_OK=					0x01;
	
	public static final byte ID_PROTVER=(byte)			0xA0;
	public static final byte ID_LOGIN=	(byte)			0xA1;
	public static final byte ID_CHARLIST=(byte)			0xA2;
	public static final byte ID_LOGIN_COMPLETE=(byte)	0xA3;
	
	public static final byte ID_MAP_UPDATE=(byte)		0xA4;
	
	private ProtSpec() { }
}
