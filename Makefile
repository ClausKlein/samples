CC:=/opt/local/bin/clang
CXX:=/opt/local/bin/clang++
# CXX:=g++-mp-5
# CC:=gcc-mp-5

BOOST_ROOT:=/usr/local


CPPFLAGS:=-I${BOOST_ROOT}/include
# create dpendency file volatile.d collate.d
# Write a depfile containing user headers
CPPFLAGS+= -MMD -MP


CFLAGS:=-g -Wall -Wextra -std=c99
CFLAGS+= -Wconversion
CFLAGS+= -Wundef -Wno-unused-parameter


CXXFLAGS:=-g -Wall -Wextra -std=c++17
## -WCL4 -Wc++98-compat-pedantic -stdlib=libc++ -std=c++98
#!BOOST# CXXFLAGS+= -Wconversion
#!BOOST# CXXFLAGS+= -Wold-style-cast
#!BOOST# CXXFLAGS+= -Wundef
CXXFLAGS+= -Wno-unused-parameter -Wno-unused-local-typedef -Wno-unused-variable
#
# see http://clang.llvm.org/docs/index.html
##XXX CXXFLAGS+= -O1 -fsanitize=memory -fsanitize=address -fsanitize=thread -fno-omit-frame-pointer

#
# LDFLAGS:= -L /opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/
# export DYLD_LIBRARY_PATH=/opt/local/libexec/llvm-3.4/lib/clang/3.4/lib/darwin/
#
#TBD# LDFLAGS+=-L/opt/local/lib


### ls -1 *.{c,cpp}
SOURCES:= \
EreaseRemoveIdiom \
NonVirtualBaseClassTest.cpp \
SFINAE-cxx11 \
bitset.cpp \
byteorder.cpp \
collate.cpp \
copy-test.cpp \
copy.cpp \
copy_stdio.c \
exceptions.cpp \
filtpath.cpp \
getaddrinfo.cpp \
getline.cpp \
hash.cpp \
konto.cpp \
l1083.cpp \
map.cpp \ \
nested-classes.cpp \
point.cpp \
priority_queue \
result_of.cpp \
set2.cpp \
sets.cpp \
sorted_set.cpp \
sorted_map.cpp \
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
unordered_map.cpp \
unordered_set.cpp \
volatile.cpp \


CXX_SOURCES:=$(wildcard $(CURDIR)/*.cpp)
PROGRAMS:=$(notdir $(CXX_SOURCES:.cpp=))
C_SOURCES:=$(wildcard $(CURDIR)/*.c)
PROGRAMS+=$(notdir $(C_SOURCES:.c=))
DEPENDS:=$(PROGRAMS:=.d)


MAKEFLAGS+=--no-builtin-rules


.PHONY: all check test clean distclean
all: $(PROGRAMS) check

check: #TODO $(CXX_SOURCES)
	clang-tidy -checks='-cppcoreguidelines-pro-bounds-array-to-pointer-decay' ClonableBase.cpp

test: all
	./NonVirtualBaseClassTest
	./testiostream
	./byteorder
	./timevalue
	./teststdio
	./volatile
	./point
	./set2
	./copy-test

filepath: CPPFLAGS+= -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ALL_NO_LIB=1 # -DBOOST_SYSTEM_DYN_LINK=1 ## -DBOOST_ALL_NO_AUTO_LINK=1
filepath: LDLIBS+= -L$(BOOST_ROOT)/lib -lboost_filesystem-mt -lboost_system-mt

copy-test: CPPFLAGS+= -DBOOST_SYSTEM_NO_DEPRECATED
## copy-test: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
copy-test: LDLIBS+= $(BOOST_ROOT)/lib/libboost_unit_test_framework.a

volatile: CPPFLAGS+= -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ALL_NO_LIB=1 # -DBOOST_SYSTEM_DYN_LINK=1 ## -DBOOST_ALL_NO_AUTO_LINK=1
volatile: LDLIBS+= -L$(BOOST_ROOT)/lib -lboost_thread-mt -lboost_system-mt
# volatile: volatile.o
## volatile.o: .FORCE

## point: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## nested-classes: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## sorted_map: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## unordered_map: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## unordered_set: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## testclock: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## vector: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## map: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## l1083: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## result_of: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++
## SFINAE-cxx11: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++11 -stdlib=libc++
## EreaseRemoveIdiom: CXXFLAGS:= -g -Wextra -Wpedantic -std=c++14 -stdlib=libc++

clean:
	$(RM) $(PROGRAMS) *.exe *.o
	rm -rf *.dSYM

distclean: clean
	$(RM) $(PROGRAMS) *~ *.orig *.d

###TODO### -include $(DEPENDS)

# %.d: %.c Makefile
# 	@$(CPP) $(CPPFLAGS) -M -MP $< -o $@
# %.d: %.cpp Makefile
# 	@$(CPP) $(CPPFLAGS) -M -MP $< -o $@

%.o: %.cpp
	$(COMPILE.cc) $^ -o $@

%: %.o
	$(LINK.cc) $^ -o $@ $(LOADLIBES) $(LDLIBS)

%: %.c
	$(LINK.c) $^ -o $@ $(LOADLIBES) $(LDLIBS)


.FORCE::;
Makefile::;
%.d::;
%.h::;
%.c::;
%.cpp::;
%.hpp::;

