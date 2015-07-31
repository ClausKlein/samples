# CC:=clang
# CXX:=clang++
CXX:=g++-mp-5
CC:=gcc-mp-5

CPPFLAGS:=-I/opt/local/include
CPPFLAGS+= -MMD -MP


CFLAGS:=-g -Wall -Wextra -std=c99
CFLAGS+= -Wconversion
CXXFLAGS+= -Wundef
##XXX CFLAGS+= -fsanitize=address

CXXFLAGS:=-g -Wall -Wextra -std=c++14
###TODO CXXFLAGS+= -Wconversion
##XXX CXXFLAGS+= -Wundef
##XXX CXXFLAGS+= -Wold-style-cast
CXXFLAGS+= -Weffc++
# see http://clang.llvm.org/docs/index.html
##XXX CXXFLAGS+= -O1 -fsanitize=memory -fsanitize=address -fsanitize=thread -fno-omit-frame-pointer

# LDFLAGS = -L /opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/
# export DYLD_LIBRARY_PATH=/opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/

### ls -1 *.{c,cpp}
SOURCES:= \
NonVirtualBaseClassTest.cpp \
bitset.cpp \
byteorder.cpp \
copy.cpp \
copy_stdio.c \
exceptions.cpp \
getaddrinfo.cpp \
getline.cpp \
hash.cpp \
konto.cpp \
l1083.cpp \
map.cpp \ \
point.cpp \
set2.cpp \
sets.cpp \
sortedset.cpp \
stripws.cpp \
test_filbuf.cpp \
test_gcount.cpp \
test_iomanip.cpp \
test_rdbuf.cpp \
testclock.cpp \
testiostream.cpp \
teststdio.c \
timevalue.cpp \
trim_example.cpp \
unorderedmap.cpp \


CXX_SOURCES:=$(wildcard $(CURDIR)/*.cpp)
PROGRAMS:=$(notdir $(CXX_SOURCES:.cpp=))
C_SOURCES:=$(wildcard $(CURDIR)/*.c)
PROGRAMS+=$(notdir $(C_SOURCES:.c=))
DEPENDS:=$(PROGRAMS:=.d)

.PHONY: all clean distclean
all:	$(PROGRAMS)

clean:
	$(RM) $(PROGRAMS) *.exe *.o
	rm -rf *.dSYM

distclean: clean
	$(RM) $(PROGRAMS) *~ *.orig *.d

-include $(DEPENDS)

# %.d: %.c Makefile
# 	@$(CPP) $(CPPFLAGS) -M -MP $< -o $@
# %.d: %.cpp Makefile
# 	@$(CPP) $(CPPFLAGS) -M -MP $< -o $@
# 
