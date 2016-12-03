

# define the C compiler to use
CXX = g++

CXXFLAGS 	= -std=c++11
# define any compile-time flags
CFLAGS 	= -w -Wall -g -fpermissive -fpic
LDFLAGS	= "-Wl,-rpath,/proj/i9cg/contrib/lib"
# define any directories containing header files other than /usr/include
#
INCLUDES = 	\
			-I../contrib/include \
			-I../contrib/include/gl \
			-I/usr/include/SDL2/
 
LFLAGS  = -L/proj/i9cg/contrib/lib
# -L/usr/lib/arm-linux-gnueabihf 
LIBS = -lGL -lGLEW -lSDL2 -lassimp -lSDL2_image

# define the C source files
SRCS = \
	exercise7.cpp




# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
TARGET = exercise7

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(LFLAGS) $(LDFLAGS) $(CXXFLAGS)  $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean: 
	$(RM) *.o *~ $(TARGET)

depend: 
	$(SRCS) makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
