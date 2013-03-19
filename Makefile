

CXX=g++

TARGET=mspr
OBJDIR=build
SOURCE=src
CXXFLAGS+=-std=gnu++11
CFLAGS+=

ifdef DEBUG
	CFLAGS+=-O3
endif


SRC=$(shell find $(SOURCE) -name *.cpp)
OBJ=$(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

all: $(OBJDIR) target

clean:
	rm -rf $(OBJDIR)
distclean: clean

$(OBJDIR):
	mkdir -p $(OBJDIR)

target: $(OBJDIR) $(OBJ)
	$(CXX) -o $(OBJDIR)/$(TARGET) $(OBJ)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<


