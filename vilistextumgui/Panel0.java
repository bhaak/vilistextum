
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Panel0 extends javax.swing.JPanel{


    /*members*/
    Files    files;
    Panel16    panel16;
		ActionListener al;

    /*constructor*/
    public Panel0(ActionListener a){
				this.al = a;
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
        this.addListeners();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*files*/
        files = new Files();
        files.setToolTipText("files");
        this.add(files,new GridBagConstraints(
            0,2,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.HORIZONTAL,    //fill
                new Insets(0,0,5,0),    //Insets TLBR
            0,31            //ipadx, ipady
            ));


        /*panel16*/
        panel16 = new Panel16(al);
        panel16.setToolTipText("panel16");
        this.add(panel16,new GridBagConstraints(
            0,3,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,0,0,0),    //Insets TLBR
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
				java.util.ArrayList al = panel16.getParam();
				java.util.Iterator i = files.getParam().iterator();
				while (i.hasNext()) { al.add(i.next()); }
				return(al);
		}

}//end class
