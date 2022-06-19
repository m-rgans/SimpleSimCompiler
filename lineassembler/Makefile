OBJS = Hello_World.cpp inpost.cpp mystack.cpp

OBJ_NAME = func

CC = g++

#make sure -I precedes every entry
INCLUDE_PATHS = #-I../../SDL-32BIT/include/SDL2 -I../../SDLIMAGE-32BIT/include/SDL2/

#make sure -L precedes every entry
LIBRARY_PATHS = #-LC../../SDL-32BIT/lib -LC../../SDLIMAGE-32BIT/lib/

#COMPILER FLAGS WOULD GO HERE
COMPILER_FLAGS = #-lSDL2_image

#LINKER FLAGS
LINKER_FLAGS = #-lSDL2main -lSDL2 #-lmingw32

all : $(OBJS) #default make target
		$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

test : $(OBJS) #testing target
		$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
		./$(OBJ_NAME)

release : $(OBJS) #release target
		$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -O2
		#./$(OBJ_NAME)
