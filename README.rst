==================
Ninja
==================

*Ninja* is a small build system with a focus on speed.
Based on original version from http://martine.github.com/ninja/

.. contents::

---------------------------------------------------------------

.. note:: I have forked the orignal version to support `MINGW`_, `MSYS`_, and
          `CYGWIN`_ build too.

- My version support timestamps with 100s timestamps insteads of only sec.
- It contains a GNUmakefile and a CMakeLists.txt if you like to work `Using Ninja with an IDE:`_
- `ninja -V` show you a distinguish version number if you build it youself from develop branch.


See the manual -- http://ClausKlein.github.com/ninja/doc/manual.html or
`doc/manual.asciidoc <doc/manual.html>`_ included in the distribution -- for
background and more details.


Download the current *Ninja* source code as zip file from
https://github.com/ClausKlein/ninja/zipball/develop

Getting a source code version of *Ninja* with `git`_ to work with::

    git clone git://github.com/ClausKlein/ninja.git
    cd ninja
    make ninja


Building
=============

To build, run `python ./bootstrap.py`.  It first blindly compiles all non-test
source files together, then re-builds *Ninja* using itself and `./build.ninja`.
You should end up with a *Ninja* binary in the source root.  Run `./ninja -h`
for help.

The only file of interest to a user is the resulting *Ninja* binary.

You may install it yourself with::

  sudo install ninja /usr/local/bin
  # or
  sudo make install


Contributing
=============

If you want to work with code, you should use `git-flow`_ .
Fork the repository. Then, run::

    git clone --recursive git@github.com:<username>/ninja.git
    git branch develop origin/develop
    git flow init -d
    git flow feature start <your feature>

Then, do work and commit your changes::

    git flow feature publish <your feature>

When done, open a pull request to your feature branch.


HACKING :)
==================

.. include:: HACKING.rst

