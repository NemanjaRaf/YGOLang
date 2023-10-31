CXX = g++
CXXFLAGS = -Wall -g -std=c++11
INCLUDES = -I.

SRCS = main.cpp lexer.cpp token.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)
OBJDIR = dist
OBJS_WITH_DIR = $(addprefix $(OBJDIR)/,$(OBJS))
MAIN = ygolang
EXE = $(OBJDIR)/$(MAIN)

.PHONY: clean run

all: $(EXE)
	@echo Program has been compiled

$(EXE): $(OBJS_WITH_DIR)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(EXE) $(OBJS_WITH_DIR)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(EXE)
	$(EXE)

clean:
	@$(RM) -r $(OBJDIR)
