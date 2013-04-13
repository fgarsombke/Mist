SOURCES = $(SOURCE_NAMES:%=$(SRC_DIR)/%)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LDIRARGS:=
ifneq ($(LDIRS),)
	LDIRARGS:=$(LDIRS:%=-L%)
endif

LSTATIC_ARGS:=

ifneq ($(LSTATIC_LIBS),)
	LSTATIC_ARGS:=$(LSTATIC_LIBS:%=-Wl,-l% -static)
endif

$(LIB) : $(OBJECTS)
	ar crs $(OUT_DIR)/lib$(LIB).a $^ 

$(EXE) : $(OBJECTS)
	$(CXX) -o $(OUT_DIR)/$(EXE) $^ $(ADTLEXE_FLGS) -L./$(NESTING)/lib $(LDIRARGS) $(LFLAGS) $(LSTATIC_ARGS) -Bdynamic
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp ./$(INC_DIR)/%.h
	$(CXX) $(CFLAGS) -c $(ADTL_FLGS) -I$(INC_DIR) -I./$(NESTING)/$(INC_DIR) $< -o $@

dirs:
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)

clean::
	\rm -f *.o *~ *.sdf
	\rm -r -d -f ./Debug ./Release ./x64 ./x86 ./Win32 ./obj ./lib

.PHONY : clean dirs all
.IGNORE: clean