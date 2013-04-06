include ./make/stdprefix.mk

OUT_DIR := ./lib

LIB = mist_shared
SOURCE_NAMES = HtmlSource.cpp MistDataSource.cpp MistSchedule.cpp MistStd.cpp

all : dirs $(OBJECTS) $(LIB)
	
include $(NESTING)/make/stdsuffix.mk
