PROJDIR::=$(shell pwd)
OBJDIR::=$(PROJDIR)/obj
OUTDIR::=$(PROJDIR)/bin
RAYLIBDIR::=$(PROJDIR)/vendor/raylib-cpp/include

INCLUDEDIRS::=$(PROJDIR)/src $(RAYLIBDIR)

target::=$(OUTDIR)/sweep

LDLIBS::=-lraylib #-lpthread -lstdc++fs
INCLUDEFLAGS::=$(patsubst %, -I%, $(realpath $(INCLUDEDIRS)))

CXXFLAGS::=-std=c++17 -Wall -Wextra $(INCLUDEFLAGS)
CXXDEBUGFLAGS::=-g -O0 -DDEBUG
CXXRELEASEFLAGS::=-O2


folders::=$(sort $(dir $(shell find src/)))
cpp::=$(foreach var,$(folders),$(wildcard $(var)*.cpp))
obj::=$(patsubst %,$(OBJDIR)/%,$(cpp:.cpp=.o))

.PHONY: all clean debug release setup submodules print

all: debug 

setup: submodules

submodules:
	git submodule update --init

debug: CXXFLAGS += $(CXXDEBUGFLAGS)
debug: $(target)

release: CXXFLAGS += $(CXXRELEASEFLAGS)
release: $(target)


$(target): $(obj)
	@mkdir -p $(OUTDIR)
	@echo [$(CXX)] $@
	@$(CXX) $(LDFLAGS) -o $(target) $(obj) $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@echo [$(CXX)] $^
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	@rm -rf $(obj) $(target)

print:
	@echo $(folders)
	@echo $(cpp)
	@echo $(obj)
	
install:
	@echo "Installing not needed"
	
run:
	$(target)
