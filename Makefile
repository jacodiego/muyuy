#OBJS specifies which files to compile as part of the project

# OBJS = $(wildcard main.cpp common/*.cpp engine/*.cpp  engine/*/*.cpp gui/*.cpp screens/*.cpp screens/*/*.cpp systems/*.cpp utils/*.cpp)
OBJS = $(wildcard main.cpp engine/*.cpp  engine/*/*.cpp gui/*.cpp screens/*.cpp screens/*/*.cpp systems/*.cpp utils/*.cpp)

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -g -w -std=c++20

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lvulkan -llua -lfreetype -lopenal -lalure

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Muyuy

INC=-I$(HOME)/Proyectos/Muyuy/ -I/usr/include/freetype2

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)