
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/*Your Code*/
public class Files extends javax.swing.JPanel{


    /*members*/
    javax.swing.JTextField    outputTextField;
    javax.swing.JTextField    inputTextField;
    javax.swing.JButton    inputFileRequester;
    javax.swing.JLabel    label3;
    javax.swing.JLabel    label4;
    javax.swing.JButton    outputFileRequester;

    /*constructor*/
    public Files(){
        this.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
        this.setLayout(new GridBagLayout());
        this.init();
        this.addListeners();
    }//end constructor

    /*init() to initialize Components*/
    public void init() {

        /*outputTextField*/
        outputTextField = new javax.swing.JTextField();
        outputTextField.setText("");
        outputTextField.setToolTipText("outputTextField");
        this.add(outputTextField,new GridBagConstraints(
            0,4,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.1,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.BOTH,    //fill
                new Insets(5,10,5,10),    //Insets TLBR
            0,6            //ipadx, ipady
            ));


        /*inputTextField*/
        inputTextField = new javax.swing.JTextField();
        inputTextField.setText("");
        inputTextField.setToolTipText("inputTextField");
        this.add(inputTextField,new GridBagConstraints(
            0,2,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.1,0.0,    //weightx, weighty
            GridBagConstraints.CENTER,    //anchor
            GridBagConstraints.BOTH,    //fill
                new Insets(5,10,5,10),    //Insets TLBR
            0,6            //ipadx, ipady
            ));


        /*inputFileRequester*/
        inputFileRequester = new javax.swing.JButton();
        inputFileRequester.setText("...");
        inputFileRequester.setToolTipText("inputFileRequester");
        this.add(inputFileRequester,new GridBagConstraints(
            10,2,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.EAST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,0,0,5),    //Insets TLBR
            10,0            //ipadx, ipady
            ));


        /*label3*/
        label3 = new javax.swing.JLabel();
        label3.setText("Input File");
        label3.setToolTipText("label3");
        this.add(label3,new GridBagConstraints(
            0,1,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*label4*/
        label4 = new javax.swing.JLabel();
        label4.setText("Output File");
        label4.setToolTipText("label4");
        this.add(label4,new GridBagConstraints(
            0,3,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.WEST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(5,10,0,0),    //Insets TLBR
            0,0            //ipadx, ipady
            ));


        /*outputFileRequester*/
        outputFileRequester = new javax.swing.JButton();
        outputFileRequester.setText("...");
        outputFileRequester.setToolTipText("outputFileRequester");
        this.add(outputFileRequester,new GridBagConstraints(
            10,4,    //gridx, gridy
            1,1,    //gridwidth, gridheight
            0.0,0.0,    //weightx, weighty
            GridBagConstraints.EAST,    //anchor
            GridBagConstraints.REMAINDER,    //fill
                new Insets(0,0,0,5),    //Insets TLBR
            10,0            //ipadx, ipady
            ));

    }//end init()



    /*addListeners*/
    public void addListeners() {

            /*inputFileRequester...addActionListener*/
            inputFileRequester.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e){
                    inputFileRequester_actionPerformed(e);
                }
            });

            /*outputFileRequester...addActionListener*/
            outputFileRequester.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(ActionEvent e){
                    outputFileRequester_actionPerformed(e);
                }
            });

    }//end addListeners()

		JFileChooser fc;
		java.io.File infile=null;
		java.io.File outfile=null;
    /*EventHandlers*/
    public void inputFileRequester_actionPerformed(ActionEvent e)
		{
				Debug.println(e);
				fc = new JFileChooser(infile);
            
				int returnVal = fc.showOpenDialog(this);
				infile=fc.getSelectedFile();
				if (infile!=null) { inputTextField.setText(infile.getAbsolutePath()); }
		}
    public void outputFileRequester_actionPerformed(ActionEvent e)
		{
				fc = new JFileChooser(outfile);
				int returnVal = fc.showSaveDialog(this);
				outfile=fc.getSelectedFile();
				if (outfile!=null) { outputTextField.setText(outfile.getAbsolutePath()); }
		}

    //end EventHandlers

		public java.util.ArrayList getParam()
		{
				java.util.ArrayList v = new java.util.ArrayList();
				v.add(inputTextField.getText());
				v.add(outputTextField.getText());
				return(v);
		}
}//end class
