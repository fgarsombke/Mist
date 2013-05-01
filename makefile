include ./make/stdprefix.mk

OUT_DIR := ./lib

LIB = mist_shared
SOURCE_NAMES = PAL.cpp Feedback.cpp HtmlSource.cpp MistDataSource.cpp MistSchedule.cpp MistStd.cpp PushPullBuffer.cpp WeatherData.cpp

mistshared : dirs $(OBJECTS) $(LIB)

all : controllers ETCalc LawnSim mistshared

controllers : 
	make -C simulators/controllers

ETCalc :
	make -C simulators/ETCalc

LawnSim :
	make -C simulators/LawnSim

	
include $(NESTING)/make/stdsuffix.mk
