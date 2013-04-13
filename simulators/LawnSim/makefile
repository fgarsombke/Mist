include ../../make/stdprefix.mk

NESTING := ../..
OUT_DIR := ./bin
EXE := LawnSim
ADTL_FLGS := -I../Controllers/$(INC_DIR) -I../ETCalc/$(INC_DIR)
LFLAGS := 
LDIRS:=
LSTATIC_LIBS:=mist_controllers mist_shared boost_filesystem boost_chrono boost_date_time boost_program_options boost_thread boost_thread boost_random boost_exception boost_system
ADTLEXE_FLGS := -pthread 

SOURCE_NAMES = Constants.cpp DriftEntry.cpp LawnCoordinate.cpp LawnGenerator.cpp LawnSimStd.cpp main.cpp PerlinNoiseGenerator.cpp SimOptions.cpp Simulator.cpp Sprinkler.cpp Yard.cpp YardCell.cpp YardInfo.cpp

all : dirs $(OBJECTS) $(EXE)
	
include $(NESTING)/make/stdsuffix.mk
