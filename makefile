#
# PROGRAM:    Assignment 9
# PROGRAMMER: Joshua Sulouff
# LOGON ID:   Z1867688
#

# Compiler variables
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11 -IHeaders

INPUT_DIR=TestInput

vpath %.h Headers
vpath %.cpp Src
vpath %.s $(INPUT_DIR)

# Rule to link object code files to create executable file
all: scc simplesim
TARGFILE = bigdata.s

test: scc
	./scc < $(TARGFILE)


exam: scc simplesim
	-./scc < $(INPUT_DIR)/$(TARGFILE) > scc.out
	-./scc_check < $(INPUT_DIR)/$(TARGFILE) > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key

fullexam: scc bigdata.s bigpgm.s bigpgmcmd.s bigpgmstack.s bigpgmvar.s end.s max.s \
		  prime.s read.s rw.s sum.s
	echo "Running BIGDATA"
	-./scc < $(INPUT_DIR)/bigdata.s > scc.out
	-./scc_check < $(INPUT_DIR)/bigdata.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGM"
	-./scc < $(INPUT_DIR)/bigpgm.s > scc.out
	-./scc_check < $(INPUT_DIR)/bigpgm.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMCMD"
	-./scc < $(INPUT_DIR)/bigpgmcmd.s > scc.out
	-./scc_check < $(INPUT_DIR)/bigpgmcmd.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMSTACK"
	-./scc < $(INPUT_DIR)/bigpgmstack.s > scc.out
	-./scc_check < $(INPUT_DIR)/bigpgmstack.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMvar"
	-./scc < $(INPUT_DIR)/bigpgmvar.s > scc.out
	-./scc_check < $(INPUT_DIR)/bigpgmvar.s > scc.key
	-diff scc.out scc.key
	echo "Running end"
	./scc < $(INPUT_DIR)/end.s > scc.out
	./scc_check < $(INPUT_DIR)/end.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running max"
	./scc < $(INPUT_DIR)/max.s > scc.out
	./scc_check < $(INPUT_DIR)/max.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running max"
	./scc < $(INPUT_DIR)/max.s > scc.out
	./scc_check < $(INPUT_DIR)/max.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running prime"
	./scc < $(INPUT_DIR)/prime.s > scc.out
	./scc_check < $(INPUT_DIR)/prime.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running read"
	./scc < $(INPUT_DIR)/read.s > scc.out
	./scc_check < $(INPUT_DIR)/read.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running rw"
	./scc < $(INPUT_DIR)/rw.s > scc.out
	./scc_check < $(INPUT_DIR)/rw.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running sum"
	./scc < $(INPUT_DIR)/sum.s > scc.out
	./scc_check < $(INPUT_DIR)/sum.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
scc: scc.o inpost.o mystack.o
	$(CXX) $(CXXFLAGS) -o scc $^

simplesim: simplesim_main.o simplesim.o
	$(CXX) $(CXXFLAGS) -o simplesim $^

# Rules to compile source code file to object code
scc.o: scc.cpp sml.h inpost.h
inpost.o: inpost.cpp inpost.h mystack.h
mystack.o: mystack.cpp mystack.h
simplesim_main.o: simplesim_main.cpp simplesim.h
simplesim.o: simplesim.cpp simplesim.h sml.h


# Pseudo-target to remove object code and executable files
clean:
	rm -f scc simplesim *.o
