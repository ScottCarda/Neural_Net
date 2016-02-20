# Written Spring 2016 for CSC447 Artifical Intellegece.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = c++
# LIBS = -lpthread
FLAGS = -g -std=c++11 -Wall

# the build target executable:
TARGET = ANNtrain ANNtest CrossValidate

all: $(TARGET)

# specific targets
ANNtrain:	Parameters.h Data.h Neuron.h Net.h Parameters.cpp Data.cpp Neuron.cpp Net.cpp ANNtrain.cpp
		$(CC) $(FLAGS) -o $@ $^ $(LIBS) -lm

ANNtest:	Parameters.h Data.h Neuron.h Net.h Parameters.cpp Data.cpp Neuron.cpp Net.cpp ANNtest.cpp
		$(CC) $(FLAGS) -o $@ $^ $(LIBS) -lm

CrossValidate:	Parameters.h Data.h Neuron.h Net.h Parameters.cpp Data.cpp Neuron.cpp Net.cpp CrossValidate.cpp
		$(CC) $(FLAGS) -o $@ $^ $(LIBS) -lm

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
