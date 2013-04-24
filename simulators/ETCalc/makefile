include ../../make/stdprefix.mk

NESTING := ../../
OUT_DIR := ../../lib

SHLIB = ETCalc
SOURCE_NAMES = ETCalc.cpp ETCalcParameters.cpp ETCalcPython.cpp 
LSHARED_LIBS:=mist_shared boost_system-mt
ADTL_FLGS := -fpic
LFLAGS:=-shared -fpic

all : dirs $(OBJECTS) $(SHLIB)
	
include $(NESTING)/make/stdsuffix.mk
