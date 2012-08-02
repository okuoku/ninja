# Copyright 2012 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

MAKEFLAGS+= --no-builtin-rules
export MAKE:=${MAKE} -${MAKEFLAGS}
export LANG=C

prefix?=/opt/local

bindir:=$(prefix)/bin
libdir:=$(prefix)/lib
includedir:=$(prefix)/include

CXX:=$(shell which g++)
CMAKE:=$(shell which cmake)
WGET:=$(shell which wget)
UNZIP:=$(shell which unzip)
ZIP:=$(shell which zip)
GIT:=$(shell which git)
RE2C:=$(shell which re2c)
ASCIIDOC:=$(shell which asciidoc)
HOSTNAME:=$(shell hostname)

gtestdir:=gtest-1.6.0
gtestarchive:=$(gtestdir).zip

ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)
#================================================
	export CXX=$(prefix)/libexec/ccache/g++
	export CC=$(prefix)/libexec/ccache/gcc
#================================================
endif

###XXX .SILENT:
.PHONY: test bench manual install clean distclean help all testbuilds
.DEFAULT: all
all:: ninja test bench manual testbuilds

# bootstrap without installed ninja!
bootstrap.py: ;
ninja.bootstrap: bootstrap.py
	ls -lrtd --full-time
	if [ ! -d $(gtestdir) ] ; then \
	  if [ ! -f "$(gtestarchive)" -a -n "$(WGET)" ] ; then \
	    "$(WGET)" --verbose http://googletest.googlecode.com/files/$(gtestarchive); \
	  fi; \
	  if [ -f "$(gtestarchive)" -a -n "$(UNZIP)" ] ; then \
	    "$(UNZIP)" -o $(gtestarchive); \
	  fi; \
	fi
	src/version.sh
	$(RM) build.ninja
	./$<
	cp -p -b ninja $@
	-$(RM) build.ninja

# rebuild ninja with ninja.bootstrap and needed gtest!
ninja: ninja.bootstrap build.ninja
	ls -lrtd --full-time src/lexer*.cc src/depfile_parser*.cc
	./$<

# configure to build ninja with gtest
build.ninja: src/depfile_parser.cc src/lexer.cc
	CPPFLAGS="-I$(gtestdir)/include -I$(includedir)" \
	CXXFLAGS='-Wall -Wextra -Weffc++ -Wold-style-cast -Wcast-qual -Wundef -std=c++11' \
	CFLAGS='-Wsign-compare -Wconversion -Wpointer-arith -Wcomment -Wcast-align -Wcast-qual' \
	LDFLAGS="-L$(libdir)" \
	CXX="$(CXX)" ./configure.py --debug --with-gtest="$(CURDIR)/$(gtestdir)"

.PHONY: testcmakebuild testcrossbuild
testbuilds: testcmakebuild testcrossbuild

ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)
#================================================

ifneq ($(ASCIIDOC),)
# gnerate docu
manual: doc/manual.html index.html
	zip gh-pages.zip $?
doc/manual.html: doc/manual.asciidoc
	$(ASCIIDOC) -a toc -a max-width=45em -o doc/manual.html doc/manual.asciidoc
index.html: README.rst HACKING.rst License.rst GNUmakefile $(bindir)/rst2html-2.7.py
	$(bindir)/rst2html-2.7.py -dg $< > $@
else
manual: ;
endif

ifneq ($(RE2C),)
src/depfile_parser.in.cc: ;
src/depfile_parser.cc: src/depfile_parser.in.cc
	$(RE2C) -b -i --no-generation-date -o $@ $<

src/lexer.in.cc: ;
src/lexer.cc: src/lexer.in.cc
	$(RE2C) -b -i --no-generation-date -o $@ $<
else
src/depfile_parser.cc: ;
src/lexer.cc: ;
endif

.PHONY: testcmake testcmakebuild testcmakecross
testbuilds: testcmake
testcmake:: testcmakecross testcmakebuild

testcmakecross: ${HOME}/.cmake/cmake-cross.sh
	-$(RM) -rf CMakeCache.txt CMakeFiles build/*
	$< -Dgtest="$(CURDIR)/$(gtestdir)" -Dplatform=windows

testcrossbuild: ninja
	export CC=i386-mingw32-cc CXX=i386-mingw32-c++ AR=i386-mingw32-ar; \
      ./configure.py --platform=msys --with-gtest="$(CURDIR)/$(gtestdir)" && ./$<
	-$(RM) build.ninja

#================================================
else
#================================================

testcrossbuild: ninja
	export CC=i586-mingw32msvc-cc CXX=i586-mingw32msvc-c++ AR=i586-mingw32msvc-ar; \
      ./configure.py --platform=msys --with-gtest="$(CURDIR)/$(gtestdir)" && ./$<
	-$(RM) build.ninja

src/depfile_parser.cc: ;
src/lexer.cc: ;

#================================================
endif

testcmakebuild: ninja
	./$< -V
ifneq ($(CMAKE),)
	-$(RM) -rf CMakeCache.txt CMakeFiles build/*
	cd build && \
	"$(CMAKE)" -G Ninja -DCMAKE_MAKE_PROGRAM:STRING="$(CURDIR)/ninja" \
		-DCMAKE_CXX_COMPILER:FILEPATH="${CXX}" -Dgtest="$(CURDIR)/$(gtestdir)" .. && \
		../$< && ../$< package
endif

test:: ninja
	./$< all

test:: ninja_test
	./$<

test:: parser_perftest
	./$< build/*.d

test:: build_log_perftest
	./$<

bench:: hash_collision_bench
	time ./$<

help: ninja
	./ninja -t targets

clean:
	-$(RM) build/*.o build.ninja
	-$(RM) -rf CMakeCache.txt CMakeFiles

distclean: clean
	find . \( -name '*~' -o -name '.*~' -o -name '*.pyc' \) -delete
	rm -rf CMakeCache.txt CMakeFiles build *.orig *~ tags ninja ninja_test *_perftest \
		hash_collision_bench *.exe *.pdb *.ninja .ninja_log doc/doxygen/html \
		ninja.bootstrap CTestTestfile.cmake cmake_install.cmake *.a *.lib \
		CPack*.cmake install_manifest.txt _CPack_Packages
ifneq ($(GIT),)
	${GIT} status --ignored --short
endif

install: ninja
	install ninja $(bindir)

# NoOp rules for extern dependencies
README.rst: ;
HACKING.rst: ;
License.rst: ;
GNUmakefile: ;
$(bindir)/asciidoc: ;
$(bindir)/ninja: ;
$(bindir)/re2c: ;
$(bindir)/rst2html-2.7.py: ;
${HOME}/.cmake/cmake-cross.sh: ;
# Anything we don't know how to build will use this rule.
#
% :: ninja
	./ninja $@
