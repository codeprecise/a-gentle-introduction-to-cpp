HEADERS = $(wildcard *.h)

CPPOBJECTSDIR=cppobj
CPPSOURCES = $(wildcard *.cpp)
CPPOBJECTS = $(CPPSOURCES:%.cpp=$(CPPOBJECTSDIR)/%.o)

COBJECTSDIR=cobj
CSOURCES = $(wildcard *.c)
COBJECTS = $(CSOURCES:%.c=$(COBJECTSDIR)/%.o)

TARGET = Queue

CCOMPILER=gcc
CPPCOMPILER =g++
LINKER =g++

CCFLAGS=-O3

all: CREATE_OBJECTSDIR $(TARGET)

val: all
	valgrind ./$(TARGET) --leak-check=summary --leak-resolution=med

run: all
	./$(TARGET)

CREATE_OBJECTSDIR:
	mkdir -p $(CPPOBJECTSDIR); mkdir -p $(COBJECTSDIR)

$(TARGET): $(CPPOBJECTS) $(COBJECTS)
	$(LINKER) $(CPPOBJECTS) $(COBJECTS) -o $@

$(CPPOBJECTSDIR)/%.o: %.cpp $(HEADERS)
	$(CPPCOMPILER) $(CCFLAGS) -c $< -o $@

$(COBJECTSDIR)/%.o: %.c $(HEADERS)
	$(CCOMPILER) $(CCFLAGS) -c $< -o $@

clean: 
	rm -rf $(TARGET) $(COBJECTSDIR) $(CPPOBJECTSDIR)
