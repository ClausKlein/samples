#
# Standard stuff
#
.SUFFIXES:

# Disable the built-in implicit rules.
MAKEFLAGS+= --no-builtin-rules

.PHONY: setup show all test lcov install check cmakelint format clean distclean

PROJECT_NAME:=$(shell basename $${PWD})

##################################################
# begin of config part
# see https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/
# and https://github.com/llvm-mirror/clang-tools-extra/blob/master/clang-tidy/tool/run-clang-tidy.py
#
### checkAllHeader:='include/spdlog/[acdlstv].*'
## checkAllHeader?='include/spdlog/[^f].*'
checkAllHeader?='$(CURDIR)/.*\.h$$'

# NOTE: there are many errors with boost::test, doctest, catch test framework! CK
CHECKS?='-*non-private-member-*,-cppcoreguidelines-pro-bounds-*,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-macro-usage,-cppcoreguidelines-avoid-*,-modernize-avoid-*,-readability-magic-numbers'
## CHECKS?='-*,cppcoreguidelines-*,-cppcoreguidelines-pro-*,-cppcoreguidelines-avoid-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay'
# CHECKS?='-*,portability-*,readability-*,misc-*,-readability-magic-numbers'
CHECKS?='-*,boost-*,cert-*,-cert-err58-cpp,misc-*,-misc-static-assert,-misc-unused-*,-misc-unconventional-*'

# prevent hard config of find_package(asio 1.14.1 CONFIG CMAKE_FIND_ROOT_PATH_BOTH)
ifeq (NO${CROSS_COMPILE},NO)
    # CC:=/usr/local/bin/clang
    # CXX:=/usr/local/bin/clang++

    CMAKE_INSTALL_PREFIX?=/usr/local
    export CMAKE_INSTALL_PREFIX
    CMAKE_STAGING_PREFIX:=$(CMAKE_INSTALL_PREFIX)
    # CMAKE_STAGING_PREFIX?=/tmp/staging/$(PROJECT_NAME)$(CMAKE_INSTALL_PREFIX)
    CMAKE_PREFIX_PATH?="$(CMAKE_STAGING_PREFIX);$(CMAKE_INSTALL_PREFIX);/usr/local/opt/boost;/opt/local;/usr"
else
    CMAKE_STAGING_PREFIX?=/tmp/staging/$(CROSS_COMPILE})$(PROJECT_NAME)
    CMAKE_PREFIX_PATH?="$(CMAKE_STAGING_PREFIX)"
endif


#NO! BUILD_TYPE?=Coverage
# NOTE: use on shell$> BUILD_TYPE=Coverage make lcov
BUILD_TYPE?=Debug
# BUILD_TYPE?=Release
GENERATOR?=Ninja
#options GENERATOR:=Xcode
# maybe: GENERATOR:='Eclipse CDT4 - Ninja'
# NO! GENERATOR?='Unix Makefiles'

# maybe: VERBOSE?=-v

# end of config part
##################################################


BUILD_DIR:=../.build-$(PROJECT_NAME)-${CROSS_COMPILE}$(BUILD_TYPE)
ifeq ($(BUILD_TYPE),Coverage)
    USE_LCOV=ON
    ifeq (NO${CROSS_COMPILE},NO)
        CC:=/usr/bin/gcc
        CXX:=/usr/bin/g++
    endif
else
    USE_LCOV=OFF
endif


all: setup .configure-$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -- $(VERBOSE)


test: all
	cd $(BUILD_DIR) && ctest -C $(BUILD_TYPE) --rerun-failed --output-on-failure .
	cd $(BUILD_DIR) && ctest -C $(BUILD_TYPE) .


# NOTE: we do only check the new cpp file! CK
run-clang-tidy: setup .configure-$(BUILD_TYPE) compile_commands.json
	run-clang-tidy.py -header-filter=$(checkAllHeader) -checks=$(CHECKS) 2>&1 | tee run-clang-tidy.log
	egrep '\b(warning|error):' run-clang-tidy.log | perl -pe 's/(^.*) (warning|error):/\2/' | sort -u

setup: $(BUILD_DIR) .clang-tidy compile_commands.json
	find . -name CMakeLists.txt -o -name '*.cmake' -o -name '*.cmake.in' -o -name '*meson*' > .buildfiles.log

# cmake setup
.configure-$(BUILD_TYPE): CMakeLists.txt
	cd $(BUILD_DIR) && cmake -G $(GENERATOR) -Wdeprecated -Wdev \
      -DUSE_LCOV=$(USE_LCOV) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
      -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH \
      -DCMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH) \
      -DCMAKE_STAGING_PREFIX=$(CMAKE_STAGING_PREFIX) \
      -DCMAKE_PROJECT_INCLUDE_BEFORE=${HOME}/Workspace/cmake/before_project_setup.cmake \
      -DCMAKE_PROJECT_INCLUDE=${HOME}/Workspace/cmake/build_options.cmake \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} $(CURDIR)
	touch $@

compile_commands.json: .configure-$(BUILD_TYPE)
	ln -sf $(BUILD_DIR)/compile_commands.json .

$(BUILD_DIR): GNUmakefile
	mkdir -p $@


format: .clang-format
	find . -type f \( -name '*.hxx' -o -name '*.hpp' -o -name '*.cxx' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -style=file -i


cmakelint:
	find $(PACKAGE) -type f \( -name '*.cmake' -o -name 'CMakeLists.txt' \) -print0 | xargs -0 cmakelint --linelength=120 2>&1 | tee cmakelint.log


show: setup
	cmake -S $(CURDIR) -B $(BUILD_DIR) -L --trace


check: setup .configure-$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) --target $@ 2>&1 | tee run-clang-tidy.log
	egrep '\b(warning|error):' run-clang-tidy.log | perl -pe 's/(^.*) (warning|error):/\2/' | sort -u


lcov: all .configure-Coverage
	cmake --build $(BUILD_DIR) --target $@


install: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target $@


clean: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target $@


distclean:
	rm -rf $(BUILD_DIR) .configure-* .buildfiles.log compile_commands.json *~ .*~ tags
	find . -name '*~' -delete


# These rules keep make from trying to use the match-anything rule below
# to rebuild the makefiles--ouch!

## CMakeLists.txt :: ;
GNUmakefile :: ;
.clang-tidy :: ;
.clang-format :: ;

# Anything we don't know how to build will use this rule.  The command is
# a do-nothing command.
% :: ;

