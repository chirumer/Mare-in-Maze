#OBJS specifies which files to compile as part of the project
OBJS = source/main.c source/definitions/SDL_helpers.c source/definitions/game_instructions.c source/definitions/game.c source/definitions/maze.c source/definitions/game_helpers.c source/definitions/queue.c source/definitions/game_audio.c source/definitions/game_graphics.c source/definitions/game_state.c source/definitions/constants.c source/definitions/resource_paths.c source/definitions/debug.c source/definitions/game_end_screen.c

#CC specifies which compiler we're using
CC = gcc

# include directories
INC_DIR = source/declarations

#COMPILER_FLAGS specifies the additional compilation options we're using
# -Wall includes all warnings
COMPILER_FLAGS = -Wall -I${INC_DIR}

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = program.out

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
