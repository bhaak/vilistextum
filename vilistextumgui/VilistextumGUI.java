
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;


public class VilistextumGUI extends javax.swing.JFrame implements ActionListener 
{ 
	Panel0  panel0;

public VilistextumGUI() 
{
	this.setLocation(50,50);
	panel0 = new Panel0(this);
	this.getContentPane().add(panel0);

	this.addWindowListener(new WindowAdapter()
	{
		public void windowClosing(WindowEvent e)
		{ 
				dispose();
				System.exit(0);
		}});

  String[] cmd = {"vilistextum","--version"}; 
	ExecuteCommand ec = new ExecuteCommand("TITLE", cmd, this);
	ec.start();
}

public void actionPerformed(ActionEvent e)
{
	java.util.ArrayList command = panel0.getParam();
	command.add(0, "--errorlevel=0");
	command.add(0, "vilistextum");
					
	System.err.println(command);
	try 
	{
		java.util.Iterator i = command.iterator();
		int j = 0;
		String[] str = new String[command.size()];
		while (i.hasNext()) { str[j++] = (String)i.next(); }
							
		ExecuteCommand ec = new ExecuteCommand("OUTPUT", str, this);
		ec.start();
	}
	catch (Exception ex)
	{
			System.err.println("Error: "+ex.getMessage());
	}
}

public void messageDialog(int exitValue, String msg)
{
		JOptionPane jop = new JOptionPane();
		if (exitValue==0) 
		{
			jop.showMessageDialog(this, msg, "Information", JOptionPane.INFORMATION_MESSAGE);
		} else if (exitValue==-1)
		{
			// a big Hack for setting the window title	
			this.setTitle(msg);
		} else {
			jop.showMessageDialog(this, msg, "Error", JOptionPane.ERROR_MESSAGE);
		}
}

public static void main(String args[])
{
	VilistextumGUI app = new VilistextumGUI();
	app.setSize(600,400);
	app.pack();
	app.show();
}//end main-method

}//end class VilistextumGUI
