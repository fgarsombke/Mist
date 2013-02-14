#
# Defines the part type that this project uses.
#
PART=LM3S1968

#
# Set the processor variant.
#
VARIANT=cm3

#
# The base directory for StellarisWare.
#
ROOT=../../..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=../drivers

#
# Where to find header files that do not live in the source directory.
#
IPATH=..
IPATH+=../../..

#
# The default rule, which causes the uart_echo example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/uart_echo.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the uart_echo example.
#
${COMPILER}/uart_echo.axf: ${COMPILER}/jsmn.o
${COMPILER}/uart_echo.axf: ${COMPILER}/rit128x96x4.o
${COMPILER}/uart_echo.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/uart_echo.axf: ${COMPILER}/uart_echo.o
${COMPILER}/uart_echo.axf: ${ROOT}/driverlib/${COMPILER}-cm3/libdriver-cm3.a
${COMPILER}/uart_echo.axf: uart_echo.ld
SCATTERgcc_uart_echo=uart_echo.ld
ENTRY_uart_echo=ResetISR

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
