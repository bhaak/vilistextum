
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Buttons extends javax.swing.JPanel{


    /*members*/
    javax.swing.JButton    convert;
    javax.swing.JButton    quit;
		ActionListener al;

    /*constructor*/
    public Buttons(ActionListener a){
				this.al = a;
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
        this.addListeners();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*convert*/
        convert = new javax.swing.JButton();
        convert.setText("Convert");
        this.add(convert,new GridBagConstraints(
            1,1,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(5,0,5,40),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*quit*/
        quit = new javax.swing.JButton();
        quit.setText("Quit");
        this.add(quit,new GridBagConstraints(
            7,1,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(5,40,5,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));

    }//end init()



    /*addListeners*/
    public void addListeners() {

            /*convert...addActionListener*/
    				convert.addActionListener(al);
						/*
            convert.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e){
                    convert_actionPerformed(e);
                }
            });
						*/
            /*quit...addActionListener*/
            quit.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e){
                    quit_actionPerformed(e);
                }
            });

    }//end addListeners()

    /*EventHandlers*/
    public void convert_actionPerformed(ActionEvent e)
		{ 
				Debug.println(e); 
		}
    public void quit_actionPerformed(ActionEvent e)
		{ 
				System.exit(0); 
		}

    //end EventHandlers
}//end class
