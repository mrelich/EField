SHELL := /bin/bash

RCXX=$(shell root-config --cflags)
RLIB=$(shell root-config --libs)

CXX		= g++
CFLAGS		= -pipe -g \
-D __SKIP_CLASS_DEF__ \
-I.. \
-Wall -Wextra -Winit-self -Wcast-align -Wno-comment -Wno-ignored-qualifiers \
-O2 -march=native -mfpmath=sse -fPIC \
-Wl,--no-as-needed

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

Targets = analysis dim3Ana

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
obj/TreeAnalyzer.so:	$(inc)/TreeAnalyzer.h
obj/Dim1.so:		$(inc)/Dim1.h 
obj/Tools.so:		$(inc)/Tools.h
obj/Analysis.so:	$(inc)/Analysis.h
obj/Dim3.so:		$(inc)/Dim3.h
obj/Analysis3D.so:	$(inc)/Analysis3D.h

#---- Analysis Code ----#
analysis: 			\
	obj/TreeAnalyzer.so 	\
	obj/Dim1.so		\
	obj/Tools.so		\
	obj/Analysis.so	
	$(CXX) $+ -o $@ $(CXXFLAGS) $(LDFLAGS) $(links)

dim3Ana:			\
	obj/Dim3.so		\
	obj/Tools.so		\
	obj/Analysis3D.so	
	$(CXX) $+ -o $@ $(CXXFLAGS) $(LDFLAGS) $(links)

clean:
	rm -f $(Targets)
	rm -f obj/*.so
	rm -f obj/*.so
	rm -f obj/*.cxx
	rm -f obj/*.h
