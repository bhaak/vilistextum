
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Option1 extends javax.swing.JPanel{


    /*members*/
    javax.swing.JLabel    label1;
    javax.swing.JLabel    label2;
    javax.swing.JTextField    image;
    javax.swing.JTextField    output_width;

    /*constructor*/
    public Option1(){
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*label1*/
        label1 = new javax.swing.JLabel();
        label1.setText("Default Image Name");
        this.add(label1,new GridBagConstraints(
            0,0,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,5,10),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*label2*/
        label2 = new javax.swing.JLabel();
        label2.setText("Width of output");
        this.add(label2,new GridBagConstraints(
            0,3,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(10,10,5,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*image*/
        image = new javax.swing.JTextField();
        image.setText("Image");
        this.add(image,new GridBagConstraints(
            0,1,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.HORIZONTAL,    //fill
                new Insets(0,10,10,10),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*output_width*/
        output_width = new javax.swing.JTextField();
        output_width.setText("72");
        this.add(output_width,new GridBagConstraints(
            0,4,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.HORIZONTAL,    //fill
                new Insets(0,10,10,10),    //Insets TLBR
            0,0            //ipadx, ipady
            ));

    }//end init()


		public java.util.ArrayList getParam()
		{
				java.util.ArrayList v = new java.util.ArrayList();
				v.add("--width="+output_width.getText());
				v.add("--defimage="+image.getText());
				return(v);
		}
}//end class
