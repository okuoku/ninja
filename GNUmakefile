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

prefix?=/opt/local

bindir:=$(prefix)/bin
libdir:=$(prefix)/lib
includedir:=$(prefix)/include

CXX:=$(shell which g++)
CMAKE:=$(shell which cmake)
CPACK:=$(shell which cpack)
WGET:=$(shell which wget)
UNZIP:=$(shell which unzip)
ZIP:=$(shell which zip)
ASCIIDOC:=$(shell which asciidoc)
HOSTNAME:=$(shell hostname)

gtestdir:=gtest-1.6.0
gtestarchive:=$(gtestdir).zip

ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)
#================================================
	###XXX gtestdir:=$(shell $(bindir)/grealpath ${HOME}/Workspace/cpp/$(gtestdir))
	export CXX=$(prefix)/libexec/ccache/g++
	export CC=$(prefix)/libexec/ccache/gcc
#================================================
endif

###XXX .SILENT:
.PHONY: test bench manual install clean distclean help all testbuilds
.DEFAULT: all
all:: ninja test bench testbuilds

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
	./$<

# configure to build ninja with gtest
build.ninja: src/depfile_parser.cc src/lexer.cc
	CPPFLAGS="-I$(gtestdir)/include -I$(includedir)" \
	CXXFLAGS='-Wall -Wextra -Weffc++ -Wold-style-cast -Wcast-qual -Wundef -std=c++11' \
	CFLAGS='-Wsign-compare -Wconversion -Wpointer-arith -Wcomment -Wcast-align -Wcast-qual' \
	LDFLAGS="-L$(libdir)" \
	CXX="$(CXX)" ./configure.py --debug --with-gtest=$(gtestdir)

.PHONY: testcrossbuild
testbuilds: testcrossbuild

ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)
#================================================

# gnerate docu
manual: doc/manual.html index.html
	zip gh-pages.zip $?
doc/manual.html: doc/manual.asciidoc
	$(ASCIIDOC) -a toc -a max-width=45em -o doc/manual.html doc/manual.asciidoc
index.html: README.rst HACKING.rst License.rst GNUmakefile $(bindir)/rst2html-2.7.py
	$(bindir)/rst2html-2.7.py -dg $< > $@

src/depfile_parser.in.cc: ;
src/depfile_parser.cc: src/depfile_parser.in.cc $(bindir)/re2c
	$(bindir)/re2c -b -i --no-generation-date -o $@ $<

src/lexer.in.cc: ;
src/lexer.cc: src/lexer.in.cc $(bindir)/re2c
	$(bindir)/re2c -b -i --no-generation-date -o $@ $<

.PHONY: testcmake testcmakebuild testcmakecross
testbuilds: testcmake
testcmake:: testcmakecross testcmakebuild
testcmakebuild: ninja
	-$(RM) -f CMakeCache.txt
	"$(CMAKE)" -G Ninja -DCMAKE_MAKE_PROGRAM:STRING="$(CURDIR)/ninja" \
		-DCMAKE_CXX_COMPILER:FILEPATH="${CXX}" -Dgtest="$(gtestdir)" && ./$<
	./$< -v
	./$< -d explain
	$(CPACK) -C CPackConfig.cmake -G PackageMaker

testcmakecross: ${HOME}/.cmake/cmake-cross.sh
	-$(RM) -f CMakeCache.txt
	$<

testcrossbuild: ninja
	export CC=i386-mingw32-cc CXX=i386-mingw32-c++ AR=i386-mingw32-ar; \
      ./configure.py --platform=msys && ./$<

#================================================
else
#================================================

testcrossbuild: ninja
	export CC=i586-mingw32msvc-cc CXX=i586-mingw32msvc-c++ AR=i586-mingw32msvc-ar; \
      ./configure.py --platform=msys && ./$<

src/depfile_parser.cc: ;
src/lexer.cc: ;

#================================================
endif

test:: ninja_test
	./$<

test:: parser_perftest
	./$< build/*.d

test:: build_log_perftest
	./$<

bench:: hash_collision_bench
	./$<

help: ninja
	./ninja -t targets

clean: build.ninja
	-$(RM) build/*.o build.ninja

distclean: ###XXX clean
	find . \( -name '*~' -o -name '.*~' -o -name '*.pyc' \) -delete
	rm -rf CMakeCache.txt CMakeFiles build *.orig *~ tags ninja ninja_test *_perftest \
		hash_collision_bench *.exe *.pdb *.ninja .ninja_log doc/doxygen/html \
		ninja.bootstrap CTestTestfile.cmake cmake_install.cmake *.a *.lib \
		CPack*.cmake install_manifest.txt _CPack_Packages
	-git status --ignored --short

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
