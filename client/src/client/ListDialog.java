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
// ListDialog.java: the ListDialog class.

package client;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

public class ListDialog extends JDialog {
	
	private static final long serialVersionUID = 1L;
	
	private JList list;
	private DefaultListModel<String> model;
	private JButton okButton;
	private JButton cancelButton;
	private boolean accepted;
	
	public ListDialog(String title, String label, List<String> lst) {
		setTitle(title);
		setModal(true);
		accepted=false;
		
		create(label);
		for (String name : lst)
			model.addElement(name);
		
		setLocationRelativeTo(null);
	}
	
	public boolean accepted() {
		return accepted;
	}
	
	public String getSelected() {
		return (String) list.getSelectedValue();
	}
	
	private void create(String label) {
		JPanel pane=(JPanel) getContentPane();
		setLayout(new BoxLayout(pane, BoxLayout.Y_AXIS));
		
		model=new DefaultListModel<String>();
		list=new JList(model);
		
		okButton=new JButton("OK");
		okButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				accepted=true;
				setVisible(false);
			}
			
		});
		
		cancelButton=new JButton("Cancel");
		cancelButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				setVisible(false);
			}
		});
		
		JPanel buttonBox=new JPanel();
		buttonBox.setLayout(new BoxLayout(buttonBox, BoxLayout.X_AXIS));
		
		buttonBox.add(okButton);
		buttonBox.add(cancelButton);
		
		pane.add(new JLabel(label));
		pane.add(new JScrollPane(list));
		pane.add(buttonBox);
		
		pack();
	}

}
