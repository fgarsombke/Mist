include ../../make/stdprefix.mk

NESTING := ../../
OUT_DIR := ../../lib

SHLIB = ETCalc
SOURCE_NAMES = ETCalc.cpp ETCalcParameters.cpp ETCalcPython.cpp 
LSHARED_LIBS:=mist_shared python2.7 boost_python boost_system
ADTL_FLGS := -fpic -I$(PYTHONINC_DIR)
LFLAGS:=-shared -fpic

all : dirs $(OBJECTS) $(SHLIB)
	
include $(NESTING)/make/stdsuffix.mk
