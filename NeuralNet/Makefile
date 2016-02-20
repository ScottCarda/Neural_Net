# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = c++
# LIBS = -lpthread
FLAGS = -g -std=c++11 -Wall

# the build target executable:
TARGET = ANNtrain

all: $(TARGET)

# specific targets
ANNtrain:	Parameters.h Data.h Neuron.h Net.h Parameters.cpp Data.cpp Neuron.cpp Net.cpp main.cpp
		$(CC) $(FLAGS) -o $@ $^ $(LIBS) -lm

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
