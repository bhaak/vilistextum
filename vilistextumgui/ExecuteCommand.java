// adapted form Michael C. Daconta column "Java Traps" on JavaWorld
// http://www.javaworld.com/jw-12-2000/jw-1229-traps.html

import java.util.*;
import java.io.*;

class StreamGobbler extends Thread
{

InputStream is;
String type;
VilistextumGUI ec;
		
StreamGobbler(InputStream is, String type, VilistextumGUI ec)
{
		this.is = is;
		this.type = type;
		this.ec = ec;
}
		
public void run()
{
		String output="";

		try
		{
				InputStreamReader isr = new InputStreamReader(is);
				BufferedReader br = new BufferedReader(isr);
				String line=null;
				while ( (line = br.readLine()) != null)
						{ output += line; System.out.println(type + ">" + line); } 
				this.sendOutput(output);
		} 
		catch (IOException ioe)
		{
				ioe.printStackTrace(); 
		}
}

public void sendOutput(String output)
{
		int value;
		if (type.equals("ERROR"))
		{
			System.err.println("-"+output+"-");
			if (!output.equals("")) { ec.messageDialog(1, output); }
		} else if (type.equals("TITLE"))
		{
			ec.messageDialog(-1, output);
		}
}

} // end StreamGobbler

public class ExecuteCommand extends Thread
{
	String type;
  String args[];
	VilistextumGUI vg;

public ExecuteCommand(String type, String a[], VilistextumGUI vg)
{
		this.type = type;
		this.args = a;
		this.vg = vg;
}

public void run()
{
		if (args.length < 1)
		{
			System.out.println("USAGE: java GoodWindowsExec <cmd>");
			System.exit(1);
		}
				
		try
		{ 
				String osName = System.getProperty("os.name");
				System.err.println("os.name "+osName);
				String[] cmd;
				int add = 0;

				if(osName.equals("Windows NT"))
				{
						add = 2;
						cmd = new String[args.length+add];
						cmd[0] = "cmd.exe" ;
						cmd[1] = "/C" ;
						cmd[2] = args[0];
				}
				else if(osName.equals("Windows 95"))
				{
						add = 2;
						cmd = new String[args.length+add];
						cmd[0] = "command.com" ;
						cmd[1] = "/C" ;
						cmd[2] = args[0];
				}
				else // assume anything else is command line
				{
						cmd = new String[args.length];						
				}
						
				for (int i=0; i<args.length; i++) { cmd[i+add] = args[i]; System.err.println(args[i]);}

				Runtime rt = Runtime.getRuntime();
				System.out.println("Execing " + cmd[0] );
				Process proc = rt.exec(cmd);
				// any error message?
				StreamGobbler errorGobbler = new StreamGobbler(proc.getErrorStream(), "ERROR", vg); 
						
				// any output?
				StreamGobbler outputGobbler = new StreamGobbler(proc.getInputStream(), type, vg);
				
				// kick them off
				errorGobbler.start();
				outputGobbler.start();
						
				// any error???
				int exitVal = proc.waitFor();
				if (exitVal==0) 
				{ 
						if (!type.equals("TITLE"))
						{	vg.messageDialog(0,"Document successfully converted");}
				}
				//else { vg.messageDialog(exitVal, "Fehler aufgetreten"); }
				System.out.println("ExitValue: " + exitVal); 
				System.err.println("os.name "+osName);
		} 
		catch (Throwable t)
		{
			t.printStackTrace();
		}
} //end run

public static void main(String[] args)
{
	ExecuteCommand gwe = new ExecuteCommand("COMMAND", args, null);
 	gwe.start();
}

                                                             }
