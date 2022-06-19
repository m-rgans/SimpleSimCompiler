#
# PROGRAM:    Assignment 9
# PROGRAMMER: your name
# LOGON ID:   your z-id
# DATE DUE:   due date of program
#

# Compiler variables
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

# Rule to link object code files to create executable file
all: scc simplesim
TARGFILE = prime.s
test: scc
	./scc < $(TARGFILE)


exam: scc simplesim
	-./scc < $(TARGFILE) > scc.out
	-./scc_check < $(TARGFILE) > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key

fullexam:
	echo "Running BIGDATA"
	./scc < bigdata.s > scc.out
	-./scc_check < bigdata.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGM"
	./scc < bigpgm.s > scc.out
	-./scc_check < bigpgm.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMCMD"
	./scc < bigpgmcmd.s > scc.out
	-./scc_check < bigpgmcmd.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMSTACK"
	./scc < bigpgmstack.s > scc.out
	-./scc_check < bigpgmstack.s > scc.key
	-diff scc.out scc.key
	echo "Running BIGPGMvar"
	./scc < bigpgmvar.s > scc.out
	-./scc_check < bigpgmvar.s > scc.key
	-diff scc.out scc.key
	echo "Running end"
	./scc < end.s > scc.out
	./scc_check < end.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running max"
	./scc < max.s > scc.out
	./scc_check < max.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running max"
	./scc < max.s > scc.out
	./scc_check < max.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running prime"
	./scc < prime.s > scc.out
	./scc_check < prime.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running read"
	./scc < read.s > scc.out
	./scc_check < read.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running rw"
	./scc < rw.s > scc.out
	./scc_check < rw.s > scc.key
	-diff scc.out scc.key
	./simplesim_check < scc.out > simpleCheck.out
	./simplesim_check < scc.key > simpleCheck.key
	-diff simpleCheck.out simpleCheck.key
	echo "Running sum"
	./scc < sum.s > scc.out
	./scc_check < sum.s > scc.key
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
