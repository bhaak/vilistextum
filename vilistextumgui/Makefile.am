JAVABINDIR = /usr/lib/jdk1.3/bin

bin_PROGRAMS = VilistextumGUI.jar

VilistextumGUI_jar_SOURCES = Buttons.java Debug.java ExecuteCommand.java Files.java Option1.java Option2.java Panel0.java Panel16.java VilistextumGUI.java

VilistextumGUI.jar:
	$(JAVABINDIR)/javac *.java
	rm -f VilistextumGUI.jar 
	$(JAVABINDIR)/jar -cmf mainclass VilistextumGUI.jar *.class		

