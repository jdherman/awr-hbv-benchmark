#*************************************************************
#* Makefile
#*
#* Purpose:
#*   Build HBV independently of e-NSGAII
#*
#* Version:
#*   May 2011 - Jon Herman. Most stolen from Josh's eNSGAII Makefile.
#**************************************************************


SHELL = /bin/sh
TARGET = hbv
MAIN_DIR = .
LIB_DIR = $(MAIN_DIR)
SHR_DIR = .

#**************************************************************
#Compiler
#**************************************************************

#Serial compilation (running on compute nodes)
#CC_SERIAL = g++
#When profiling on PCs...
#CC_SERIAL = C:/MinGW/bin/g++.exe
CC_SERIAL = icpc

#**************************************************************
#Compiler flags
#**************************************************************

#Optimized release code
#When profiling, use this set of flags
#C_FLAGS = -pg -O3 -mno-cygwin -DLINUX -DOUT_TO_SCREEN -DENKF_OUT_TO_SCREEN
C_FLAGS = -O3 -DLINUX -DOUT_TO_SCREEN

#Debugging code
C_FLAGS_DEBUG = -g -O0 -DLINUX -DOUT_TO_SCREEN

#***************************************************************
#HBV Source & Objects
#***************************************************************

HBV_DIR = $(MAIN_DIR)

HBV_SOURCE = $(HBV_DIR)/HBV.cpp  \
    $(HBV_DIR)/HBV.cpp      \
    $(HBV_DIR)/HBVMain.cpp  \
    $(HBV_DIR)/HBVInput.cpp  \
    $(SHR_DIR)/HamonEvap.cpp \
    $(SHR_DIR)/MOPEXData.cpp \
    $(SHR_DIR)/QuickSort.cpp  \
    $(SHR_DIR)/WaterObjectives.cpp \

HBV_OBJS = HBV.o  \
    HBV.o  \
    HBVMain.o  \
    HBVInput.o  \
    HamonEvap.o  \
    MOPEXData.o  \
    QuickSort.o  \
    WaterObjectives.o  \

HBV_INCLUDE  = -I$(HBV_DIR) -I$(SHR_DIR)

#**************************************************************
#Combine everything
#**************************************************************

LIB_BASE = libHBV
LIB_NAME = $(LIB_DIR)/$(LIB_BASE).a

#HBV
SOURCE = $(HBV_SOURCE)
OBJS = $(HBV_OBJS)
INCLUDES = $(HBV_INCLUDE)
DEFINES = -DHBVMOD -DRANGER_MEMORY_TRICK
LIBS = -lHBV

#**************************************************************
#Make Instructions:
#**************************************************************

#If profiling, add the -pg flag in the last line of these as well

all:

	$(CC_SERIAL) -c $(C_FLAGS) $(INCLUDES) -DSERIAL $(DEFINES) $(SOURCE)
	$(AR) cr $(LIB_NAME) $(OBJS)
	rm $(MAIN_DIR)/*.o
	$(CC_SERIAL) $(C_FLAGS) $(INCLUDES) -DSERIAL $(DEFINES) -c HBVMain.cpp
	$(CC_SERIAL) -o $(TARGET) HBVMain.o -L$(LIB_DIR) $(LIBS) -lm

serial_debug:
	$(CC_SERIAL) -c $(C_FLAGS_DEBUG) $(INCLUDES) -DSERIAL $(DEFINES) $(SOURCE)
	$(AR) cr $(LIB_NAME) $(OBJS)
	rm $(OBJS)
	$(CC_SERIAL) $(C_FLAGS_DEBUG) $(INCLUDES) -DSERIAL $(DEFINES) -c HBVMain.cpp
	$(CC_SERIAL) -o $(TARGET) HBVMain.o -L$(LIB_DIR) $(LIBS) -lm

#**************************************************************

clean:
	rm $(MAIN_DIR)/*.o; rm $(LIB_DIR)/*.a; rm $(MAIN_DIR)/$(TARGET)
