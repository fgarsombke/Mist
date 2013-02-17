.PHONY : clean
.IGNORE: clean
clean::
	\rm -f *.o *~ *.sdf
	\rm -r -d -f ./Debug ./Release ./x64 ./x86 ./Win32
