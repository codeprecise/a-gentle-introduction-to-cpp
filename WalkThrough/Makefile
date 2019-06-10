HEADERS = $(wildcard *.h)

CPPOBJECTSDIR=cppobj
CPPSOURCES = $(wildcard *.cpp)
CPPOBJECTS = $(CPPSOURCES:%.cpp=$(CPPOBJECTSDIR)/%.o)

COBJECTSDIR = cobj
CSOURCES = $(wildcard *.c)
COBJECTS = $(CSOURCES:%.c=$(COBJECTSDIR)/%.o)

TARGET = prod

CCOMPILER = gcc
CPPCOMPILER = g++
LINKER = g++

CCFLAGS = -O0 -g
LINKFLAGS = -lpthread

all: CREATE_OBJECTSDIR $(TARGET)

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

run: all
	./$(TARGET)

CREATE_OBJECTSDIR:
	mkdir -p $(CPPOBJECTSDIR); mkdir -p $(COBJECTSDIR)

$(TARGET): $(CPPOBJECTS) $(COBJECTS)
	$(LINKER) $(CPPOBJECTS) $(COBJECTS) -o $@ $(LINKFLAGS)

$(CPPOBJECTSDIR)/%.o: %.cpp $(HEADERS)
	$(CPPCOMPILER) $(CCFLAGS) -c $< -o $@

$(COBJECTSDIR)/%.o: %.c $(HEADERS)
	$(CCOMPILER) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(COBJECTSDIR) $(CPPOBJECTSDIR)
