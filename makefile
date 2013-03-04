include ./make/stdprefix.mk

OUT_DIR := ./lib

LIB = mist_shared
SOURCE_NAMES = WeatherData.cpp WeatherDataSource.cpp

all : dirs $(OBJECTS) $(LIB)
	
include $(NESTING)/make/stdsuffix.mk