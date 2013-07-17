CC:=clang
CXX:=clang++

CPPFLAGS:=-I/opt/local/include

CFLAGS:=-g -Wall -Wextra -Wconversion -Wundef -std=c99

CXXFLAGS:=-g -Wall -Wextra -std=c++98
CXXFLAGS+= -Wconversion
CXXFLAGS+= -Wundef
CXXFLAGS+= -Wold-style-cast
CXXFLAGS+= -Weffc++

# ls *.{c,cpp}
SOUCES:= \
NonVirtualBaseClassTest.cpp \
copy.cpp \
exceptions.cpp \
getline.cpp \
sets.cpp \
sortedset.cpp \
test_filbuf.cpp \
test_gcount.cpp \
test_rdbuf.cpp \
testiostream.cpp \
teststdio.c \
timevalue.cpp \
vokable2.cpp \


CXX_SOURCES:=$(wildcard $(CURDIR)/*.cpp)
PROGRAMS:=$(notdir $(CXX_SOURCES:.cpp=))
C_SOURCES:=$(wildcard $(CURDIR)/*.c)
PROGRAMS+=$(notdir $(C_SOURCES:.c=))

.PHONY: all clean
all:	$(PROGRAMS)

clean:
	$(RM) $(PROGRAMS) *~ *.orig *.exe 
	rm -rf *.dSYM

