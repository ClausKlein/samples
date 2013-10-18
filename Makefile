# CC:=clang-mp-3.4
# CXX:=clang++-mp-3.4

CPPFLAGS:=-I/opt/local/include

CFLAGS:=-g -Wall -Wextra -Wconversion -Wundef -std=c99
##XXX CFLAGS+= -fsanitize=address

CXXFLAGS:=-g -Wall -Wextra -std=c++98
CXXFLAGS+= -Wconversion
CXXFLAGS+= -Wundef
CXXFLAGS+= -Wold-style-cast
CXXFLAGS+= -Weffc++
# see http://clang.llvm.org/docs/index.html
##XXX CXXFLAGS+= -O1 -fsanitize=memory -fsanitize=address -fsanitize=thread -fno-omit-frame-pointer

# LDFLAGS = -L /opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/
# export DYLD_LIBRARY_PATH=/opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/

# ls *.{c,cpp}
SOUCES:= \
NonVirtualBaseClassTest.cpp \
byteorder.cpp \
copy.cpp \
exceptions.cpp \
getaddrinfo.cpp \
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

