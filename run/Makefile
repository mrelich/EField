SHELL := /bin/bash

RCXX=$(shell root-config --cflags)
RLIB=$(shell root-config --libs)

CXX		= g++
CFLAGS		= -pipe -g \
-D __SKIP_CLASS_DEF__ \
-I.. \
-Wall -Wextra -Winit-self -Wcast-align -Wno-comment -Wno-ignored-qualifiers \
-O2 -march=native -mfpmath=sse -fPIC 

#---- Paths ----#
inc = ../include
src = ../src

#---- Links ----#
links = -I$(inc)

CXXFLAGS	= $(CFLAGS) $(RCXX) -lTreePlayer
LDFLAGS		= -lm -L. $(RLIB)

#---- Packages Dependencies ----#
DEPS = 		\
Analysis.h	\
Dim1.h 		\
Physics.h

Targets = analysis

all: Makefile $(Targets)


obj/%.so: $(src)/%.cxx
	$(CXX) -c $< -o $@ $(SOFLAGS) $(CXXFLAGS) $(links)

#---- My Trees ----#
# Keep in case I want Trees later
#obj/MyTree_Dict.cxx: $(ELS_ana)/MyTree.h $(ELS_ana)/LinkDef.h
#	rootcint -f $@ -c -I$(ELS_ana) $+
#obj/MyTree_Dict.so: obj/MyTree_Dict.cxx $(TREE_DEPS)
#	$(CXX) -c $< -o $@ $(SOFLAGS) $(CXXFLAGS) -Os -g1 -I$(ELS_ana)

#---- Additional Objects ----#
obj/Dim1.so:		$(inc)/Dim1.h 
obj/Analysis.so:	$(inc)/Analysis.h

#---- Analysis Code ----#
analysis: 		\
	obj/Dim1.so	\
	obj/Analysis.so	
	$(CXX) $+ -o $@ $(CXXFLAGS) $(LDFLAGS) $(links)

clean:
	rm -f $(Targets)
	rm -f obj/*.so
	rm -f obj/*.so
	rm -f obj/*.cxx
	rm -f obj/*.h
