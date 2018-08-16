src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
EXEC_DIR = ./
OBJ_DIR = ../objs 
CC := g++
CXXFLAGS := -Wall -g -O2
LFLAGS := -lpthread
#target = test
#all:$(obj)
#	@mkdir -p $(EXEC_DIR)
#	$(CC)  $(obj) -o  $(EXEC_DIR)$(target) $(LFLAGS) 

all:$(obj)
	cp $(obj) $(OBJ_DIR)
	
%.o:%.c
	$(CC) $(CXXFLAGS) -c $< -o $@
	
.PHONY:clean
clean:
	-rm -f $(obj)
#	-rm -f $(obj) $(EXEC_DIR)$(target)
