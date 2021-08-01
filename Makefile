#OBJS specifies which files to compile as part of the project
OBJS = source/main.c source/definitions/SDL_helpers.c source/definitions/parameters.c source/definitions/game_instructions.c source/definitions/game.c source/definitions/maze.c

#CC specifies which compiler we're using
CC = gcc

# include directories
INC_DIR = source/declarations

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -I${INC_DIR}

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = program.out

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
