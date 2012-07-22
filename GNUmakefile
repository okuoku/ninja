MAKEFLAGS+= --no-builtin-rules
export MAKE:=${MAKE} -${MAKEFLAGS}

prefix?=/opt/local

bindir:=$(prefix)/bin
libdir:=$(prefix)/lib
includedir:=$(prefix)/include

HOSTNAME:=$(shell hostname)
ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)
	gtestdir:=$(shell $(bindir)/grealpath ${HOME}/Workspace/cpp/gtest-1.6.0)
	CXX:=$(prefix)/libexec/ccache/g++
else
	CXX:=g++
	gtestdir:=gtest-1.6.0
	gtestarchive:=$(gtestdir).zip
endif

###XXX .SILENT:
.PHONY: test manual install clean distclean help ### all
.DEFAULT: all
all:: ninja

# bootstrap without installed ninja!
bootstrap.py: ;
ninja.bootstrap: bootstrap.py
	ls -lrtd --full-time
	if [ ! -d $(gtestdir) ] ; then \
	  if [ ! -f $(gtestarchive) ] ; then \
	    wget --verbose http://googletest.googlecode.com/files/$(gtestarchive); \
	  fi; \
	  unzip -o $(gtestarchive); \
	fi
	src/version.sh
	$(RM) build.ninja
	./$<
	cp -p -b ninja $@
	-$(RM) build.ninja

# bootstrap with install ninja!
ninja: ninja.bootstrap build.ninja
	./$<

# gnerate docu
manual: doc/manual.html index.html
	zip gh-pages.zip $?
doc/manual.html: doc/manual.asciidoc
	$(bindir)/asciidoc -a toc -a max-width=45em -o doc/manual.html doc/manual.asciidoc
index.html: README.rst HACKING.rst License.rst GNUmakefile $(bindir)/rst2html-2.7.py
	$(bindir)/rst2html-2.7.py -dg $< > $@

###FIXME -Wundef not usable with gtest-1.6.0! ck
###XXX	CPPFLAGS="-I$(gtestdir)/include -I$(includedir) -DUSE_TIME_T" \
###TODO CXXFLAGS are not used! ck

build.ninja: src/depfile_parser.cc src/lexer.cc
	CPPFLAGS="-I$(gtestdir)/include -I$(includedir)" \
	CXXFLAGS='-Wall -Wextra -Weffc++ -Wold-style-cast -Wcast-qual -Wundef -std=c++11' \
	CFLAGS='-Wsign-compare -Wconversion -Wpointer-arith -Wcomment -Wcast-align -Wcast-qual' \
	LDFLAGS="-L$(libdir)" \
	CXX="$(CXX)" ./configure.py --debug --with-gtest=$(gtestdir)


.PHONY: testcrossbuild
ifeq ($(HOSTNAME),claus-kleins-macbook-pro.local)

src/depfile_parser.in.cc: ;
src/depfile_parser.cc: src/depfile_parser.in.cc $(bindir)/re2c
	$(bindir)/re2c -b -i --no-generation-date -o $@ $<

src/lexer.in.cc: ;
src/lexer.cc: src/lexer.in.cc $(bindir)/re2c
	$(bindir)/re2c -b -i --no-generation-date -o $@ $<

.PHONY: testcmake testcmakebuild testcmakecross
testcmake:: testcmakecross testcmakebuild
testcmakebuild: ninja
	-$(RM) -f CMakeCache.txt
	cmake -G Ninja -DCMAKE_MAKE_PROGRAM:STRING="$(CURDIR)/ninja" -Dgtest=$(gtestdir) && ./ninja
	./ninja -v
	./ninja -d explain
	cpack -C CPackConfig.cmake -G PackageMaker

testcmakecross: ${HOME}/.cmake/cmake-cross.sh
	-$(RM) -f CMakeCache.txt
	$<

testcrossbuild:
	export CC=i386-mingw32-cc CXX=i386-mingw32-c++ AR=i386-mingw32-ar; \
      ./configure.py --platform=mysys && ./ninja

else

testcrossbuild:
	export CC=i586-mingw32msvc-cc CXX=i586-mingw32msvc-c++ AR=i586-mingw32msvc-ar; \
      ./configure.py --platform=mysys && ./ninja

src/depfile_parser.cc: ;
src/lexer.cc: ;

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
