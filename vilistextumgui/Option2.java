
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Option2 extends javax.swing.JPanel{


    /*members*/
    javax.swing.JCheckBox    palm;
    javax.swing.JCheckBox    shrink_lines;
    javax.swing.JCheckBox    remove_empty_alt_tags;
    javax.swing.JCheckBox    nomicrosoft;
    javax.swing.JCheckBox    show_links;
    javax.swing.JCheckBox    convert_tags;

    /*constructor*/
    public Option2(){
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*palm*/
        palm = new javax.swing.JCheckBox();
        palm.setText("Optimize for PDA");
        this.add(palm,new GridBagConstraints(
            1,3,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(10,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*shrink_lines*/
        shrink_lines = new javax.swing.JCheckBox();
        shrink_lines.setText("Only one empty line between lines");
        this.add(shrink_lines,new GridBagConstraints(
            1,4,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*remove_empty_alt_tags*/
        remove_empty_alt_tags = new javax.swing.JCheckBox();
        remove_empty_alt_tags.setText("Remove empty alt tags");
        remove_empty_alt_tags.setSelected(true);
        this.add(remove_empty_alt_tags,new GridBagConstraints(
            1,5,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*nomicrosoft*/
        nomicrosoft = new javax.swing.JCheckBox();
        nomicrosoft.setText("Don't convert Microsoft entities and characters");
        this.add(nomicrosoft,new GridBagConstraints(
            1,6,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*show_links*/
        show_links = new javax.swing.JCheckBox();
        show_links.setText("Show links");
        show_links.setSelected(true);
        this.add(show_links,new GridBagConstraints(
            1,7,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*convert_tags*/
        convert_tags = new javax.swing.JCheckBox();
        convert_tags.setText("Convert tags (eg B, I, U)");
        convert_tags.setSelected(true);				
        this.add(convert_tags,new GridBagConstraints(
            1,8,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,10,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));

    }//end init()


		public java.util.ArrayList getParam()
		{
				java.util.ArrayList v = new java.util.ArrayList();

				if (palm.isSelected()) { v.add("-p"); }// --palm
				if (shrink_lines.isSelected()) { v.add("-s");} // --shrink-lines"
				if (remove_empty_alt_tags.isSelected()) { v.add("-r");} // --remove-empty-alt
				if (nomicrosoft.isSelected()) { v.add("-m"); }// --nomicrosoft
				if (show_links.isSelected()) { v.add("-l"); }// --links
				if (convert_tags.isSelected()) { v.add("-c"); } // --convert-tags

				return(v);
		}
   
}//end class
