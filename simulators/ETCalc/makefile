include ../../make/stdprefix.mk

NESTING := ../../
OUT_DIR := ../../lib

LIB = ETCalc
SOURCE_NAMES = ETCalc.cpp ETCalcParameters.cpp ETCalcParametersBuilder.cpp ETCalcPython.cpp
LSTATIC_LIBS:=mist_shared boost_python
ADTL_FLGS := -I$(PYTHONINC_DIR)

all : dirs $(OBJECTS) $(LIB)
	
include $(NESTING)/make/stdsuffix.mk
