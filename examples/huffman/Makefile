TARGET			=	huffman
CXXFLAGS		=	-std=c++17 -Wall -pedantic -Werror -Wno-error=attributes -O3
INCLUDE_PATH	=	-I../../sgfx/include
LIB_PATH		=	-L../../sgfx/lib
LIBS			=	-lsgfx -lX11

SRCS			=	*.cpp
OBJS			=	$(patsubst %.cpp,%.o,$(wildcard $(SRCS)))


all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(OBJS) -o $(TARGET) $(LIB_PATH) $(LIBS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm $(TARGET) $(OBJS)

.PHONY: shared clean
