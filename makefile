ifndef APAMA_HOME
$(warning Did you remember to source the apama_env script?)
$(error APAMA_HOME is not defined)
endif
ifndef APAMA_WORK
$(warning Did you remember to source the apama_env script?)
$(error APAMA_WORK is not defined)
endif

# C++ compiler
CXX := /usr/bin/clang++ 
DEFINES := -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
CXXFLAGS := -std=c++11 -w -fPIC $(DEFINES)
INCLUDES := "-I$(APAMA_HOME)/include"
LDFLAGS := -shared -L"$(APAMA_HOME)/lib"

BINDIR := ./bin
OBJDIR := ./obj

# List each of the C++ source files
FILEPLUGIN_SRC_CPP := \
	file_transport.cpp

FILEPLUGIN_OBJ := FilePlugin
FILEPLUGIN_DEP := $(patsubst %.cpp, %.d, $(FILEPLUGIN_SRC_CPP))

fileplugin: $(OBJDIR) $(BINDIR)/libFilePlugin.so install

.PHONY: fileplugin clean

$(OBJDIR)/$(FILEPLUGIN_OBJ):$(FILEPLUGIN_SRC_CPP)
	$(CXX) -c $^ -o $(OBJDIR)/$(FILEPLUGIN_OBJ) $(CXXFLAGS) $(INCLUDES)

$(BINDIR)/libFilePlugin.so: $(OBJDIR)/$(FILEPLUGIN_OBJ)
	mkdir -p $(BINDIR)
	$(CXX) -o $@ $(OBJDIR)/$(FILEPLUGIN_OBJ) $(LDFLAGS)

install:
	mkdir -p $(APAMA_HOME)/lib
	cp $(BINDIR)/*.so $(APAMA_HOME)/lib

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
