EField
======

The code here is designed to calculate the properties of the electric field
producd from electromagnetic showers in Ice.  I have so far implemented the 
1-D parameterized form of calculating the field as well as a simple analytic
approximation around the Cherenkov angle.  The goal is to implement the full 
3-D parameterization where one uses the individual tracks in an event.

##########################################
#             CODE USAGE                 #
##########################################

Physics -- This code houses some useful
	physics constants.

Dim1 -- This piece has all the methods
     needed to perform the 1D analysis
     of a shower shop.  It takes as input
     a vector which is supposed to be 
     representative of the shower profile.
     the entries should be N(e-p) at each
     step in radiation length.  It can also
     implement the simple analytic approximation
     around the Cherenkov Angle

Analysis -- This class is where the analysis
	 is actually done.  It outputs the necessary
	 histograms and figures that are useful to 
	 study the properties.  The user could use this 
	 class and add methods, or use it as an example
	 on how to use Dim1 (and later Dim3).

TreeAnalyzer -- In order to do the event by event
	     analysis, I figured I would need convenient
	     access to the Geant4 data.  So I wrote a very
	     basic flat tree that has basic variables needed 
	     to analyze the shower profile.

##########################################
#              HOW TO RUN                #
##########################################

After compiling, the executable 'analysis'
will appear.  There are currently no options.
The user needs to toggle them off in 'main'
which can be found in Analysis.  I will update
this in the future as package grows

