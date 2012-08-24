Build Dependencies
--------------------

**Requiered** packages:

- `python`_
- `re2c`_

*Recomended* packages to use:

- `ccache`_
- `git-flow`_
- `docutils`_

*Requiered packages for development* are:

- `git`_
- `gtest`_
- `doxygen`_
- `asciidoc`_

Adjusting build flags and use `ccache`_ at `build.ninja`::

  CFLAGS=-O3 CXX='/opt/local/bin/ccache g++' ./configure.py \
    --with-gtest=./gtest-1.6.0

and rebuild.

.. attention:: `ninja -t clean` removes generated source files!

    You need `re2c`_ to build it again with your installed `ninja`.

    To bootstap it again you have to restore this generated sources::

        git checkout src/depfile_parser.cc src/lexer.cc

The build times for ninja on a **macbok pro** with 2 CPU cores are:

+---------------------------+-----------------------+-------------------------+
| 1.) build without ccache  |  rebuild with ccache  |   build "no work to do" |
+===========================+=======================+=========================+
|    real   0m9.444s        |     real  0m0.849s    |  real 0m0.005s          |
+---------------------------+-----------------------+-------------------------+
|    user   0m14.492s       |     user  0m0.202s    |  user 0m0.002s          |
+---------------------------+-----------------------+-------------------------+
|     sys   0m1.838s        |      sys  0m0.248s    |   sys 0m0.003s          |
+---------------------------+-----------------------+-------------------------+


Test-driven development
------------------------

Depends on `gtest`_ source pack - Google's framework for writing C++ tests

Installing gtest
................

- On newer Ubuntus it's distributed as source::

    sudo apt-get install libgtest-dev
    ./configure --with-gtest=/usr/src/gtest

- Otherwise you need to download it, unpack it:

    wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
    unzip /gtest-1.6.0.zip

- And set your build command to::

  ./configure.py --with-gtest=./gtest-1.6.0
  ./ninja ninja_test && ./ninja_test --gtest_filter=MyTest.Name

Now you can repeatedly build and run `ninja_test` while developing until the
tests pass.  Remember to build "all" before committing to verify the other
source still works!


Testing performance impact of changes
......................................

If you have a Chrome build handy, it's a good test case.
Otherwise, https://github.com/martine/ninja/downloads has a copy of
the Chrome build files (and depfiles). You can untar that, then run::

  path/to/my/ninja chrome

and compare that against a baseline Ninja.

There's a script at misc/measure.py that repeatedly runs a command like
the above (to address variance) and summarizes its runtime.  E.g.::

  path/to/misc/measure.py path/to/my/ninja chrome

For changing the depfile parser, you can also build `parser_perftest`
and run that directly on some representative input files.


How to successfully make changes to Ninja
..........................................

Github pull requests are convenient for me to merge (I can just click
a button and it's all handled server-side), but I'm also comfortable
accepting pre-github git patches (via `send-email` etc.).

Good pull requests have all of these attributes:

* Are scoped to one specific issue
* Include a test to demonstrate their correctness
* Update the docs where relevant
* Match the Ninja coding style (see below)
* Don't include a mess of "oops, fix typo" commits

These are typically merged without hesitation.  If a change is lacking
any of the above I usually will ask you to fix it, though there are
obvious exceptions (fixing typos in comments don't need tests).

I am very wary of changes that increase the complexity of Ninja (in
particular, new build file syntax or command-line flags) or increase
the maintenance burden of Ninja.  Ninja is already successfully in use
by hundreds of developers for large projects and it already achieves
(most of) the goals I set out for it to do.  It's probably best to
discuss new feature ideas on the mailing list_ before I shoot down your
patch.

.. _list: http://groups.google.com/group/ninja-build


Generally it's the `Google C++ coding style`_, but in brief:
.............................................................

* Function name are camelcase.
* Member methods are camelcase, expect for trivial getters which are
  underscore separated.
* Local variables are underscore separated.
* Member variables are underscore separated and suffixed by an extra
  underscore.
* Two spaces indentation.
* Opening braces is at the end of line.
* Lines are 80 columns maximum.
* All source files should have the Google Inc. license header.

.. _`Google C++ coding style`: http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml


Documentation
-------------

Style guidelines
................

* Use `///` for doxygen.
* Use `\a` to refer to arguments.
* It's not necessary to document each argument, especially when they're
  relatively self-evident (e.g. in `CanonicalizePath(string* path, string* err)`,
  the arguments are hopefully obvious)

Building the manual
...................

You need `asciidoc`_ to do it::

  sudo apt-get install asciidoc --no-install-recommends
  ./ninja manual


Building the code documentation
...............................

You need `doxygen`_ to do it::

    sudo apt-get install doxygen
    ./ninja doxygen


Windows development on Linux / Mac OS X
----------------------------------------

- Get the `gtest`_ source, unpack it into your source dir::

    sudo apt-get install gcc-mingw32 wine
    #or export CC=i586-mingw32msvc-cc CXX=i586-mingw32msvc-c++ AR=i586-mingw32msvc-ar

    CC=i386-mingw32-cc CXX=i386-mingw32-c++ AR=i386-mingw32-ar \
      ./configure.py --platform=mysys --with-gtest=./gtest-1.6.0

- Build ninja::

    /path/to/host/ninja

- Run: ./ninja.exe  (implicitly runs through `wine`_ **(!)** )


Windows development on Windows (this is kind of hacky right now)
-----------------------------------------------------------------

- install `mingw`_, `msys`_, and `python`_
- in the mingw shell, put Python in your path, and: `python bootstrap.py`
- to reconfigure, run `python configure.py`
- remember to strip the resulting executable if size matters to you
- you'll need to rename `ninja.exe` into `my-ninja.exe` during development,
  otherwise ninja won't be able to overwrite itself when building


Using `clang`_
---------------

- Enable colors manually::

    CXX='/path/to/llvm/Release+Asserts/bin/clang++ -fcolor-diagnostics' ./configure.py


Using Ninja with an IDE:
------------------------

You can configure the `make`_ program as *<path/to>/ninja* within a custom project
in **Eclipse**, **NetBeans**, or **Visual Studio 2005**.

.. hint:: `cmake`_ starting with version 2.8.9 supports on all platforms ninja
          as buildsytem.  You may generate projects for:

            - 'Eclipse CDT4 - Ninja'
            - 'KDevelop3 - Ninja'
            - 'CodeBlocks - Ninja'

There is a `CMakeLists.txt` available which can be used to simply create a
project file to develop on ninja::

    cd build && \
        cmake -G "Eclipse CDT4 - Ninja" -DCMAKE_MAKE_PROGRAM:STRING="${PWD}/ninja" \
        -DCMAKE_CXX_COMPILER:FILEPATH="/opt/local/bin/g++" -Dgtest="${PWD}/gtest-1.6.0" .. && \
        ../ninja && ../ninja package


But you may always use a Makefile "stub" and import your project as a **Makefile
based project**. Use a Makefile like this:

.. include:: .Makefile
             :literal:


-------------------------------

.. target-notes::

.. _`asciidoc`: http://www.methods.co.nz/asciidoc/
.. _`ccache`: http://ccache.samba.org/
.. _`clang`: http://clang.llvm.org/
.. _`cmake`: http://cmake.org/
.. _`cygwin`: http://www.cygwin.com/
.. _`docutils`: http://docutils.sourceforge.net/
.. _`doxygen`: http://www.doxygen.org/
.. _`git-flow`: http://jeffkreeftmeijer.com/2010/why-arent-you-using-git-flow/
.. _`git`: http://git-scm.com/documentation/
.. _`gtest`: http://code.google.com/p/googletest/
.. _`make`: http://www.gnu.org/software/make/make.html
.. _`mingw`: http://mingw.org/
.. _`msys`: http://mingw.org/wiki/MSYS/
.. _`python`: http://python.org/
.. _`re2c`: http://re2c.sf.net/
.. _`wine`: http://winehq.org/

