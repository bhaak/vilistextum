
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Panel16 extends javax.swing.JPanel{


    /*members*/
    Option2    option2;
    Option1    option1;
    Buttons    buttons;
		ActionListener al;

    /*constructor*/
    public Panel16(ActionListener a){
				this.al = a;
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
        this.addListeners();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*option2*/
        option2 = new Option2();
        option2.setToolTipText("option2");
        this.add(option2,new GridBagConstraints(
            1,0,    //gridx, gridy
            10,6,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.HORIZONTAL,    //fill
                new Insets(5,5,5,5),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*option1*/
        option1 = new Option1();
        option1.setToolTipText("option1");
        this.add(option1,new GridBagConstraints(
            0,0,    //gridx, gridy
            1,6,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.BOTH,    //fill
                new Insets(5,5,5,0),    //Insets TLBR
            10,28            //ipadx, ipady
            ));


        /*buttons*/
        buttons = new Buttons(al);
        buttons.setToolTipText("buttons");
        this.add(buttons,new GridBagConstraints(
            0,7,    //gridx, gridy
            11,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.HORIZONTAL,    //fill
                new Insets(0,5,5,5),    //Insets TLBR
            0,0            //ipadx, ipady
            ));

    }//end init()



    /*addListeners*/
    public void addListeners() {

    }//end addListeners()


    /*EventHandlers*/

    //end EventHandlers

		public java.util.ArrayList getParam()
		{
				java.util.ArrayList al = option1.getParam();
				java.util.Iterator i = option2.getParam().iterator();
				while (i.hasNext()) { al.add(i.next()); }
				return(al);
		}
}//end class
