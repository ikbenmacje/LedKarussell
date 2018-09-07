# Static Code Checks

This directory contains static code checks. Eventually, they need to move to Makefile.am which is processed by autotools. For now, simply run the shell scripts from this directory and see the results in text and XML format. Each check can also be run directory from Qt Creator as an external tool.


## clang-format

This is a tool to format C/C++/Obj-C code. The script is called `./clang-format.sh` which reformats the source code. Run that before creating a git commit, but also before running the tools mentioned below. This needs installation with `sudo apt-get install clang-format`. For easy of use, there is also a shell script in this directory to run it on all source files at once.


## cloc

This tool counts, and computes differences of, lines of source code and comments. Install with `sudo apt-get install cloc` and run via `./cloc.sh`. The results are in `cloc.txt` and `cloc.xml`. See also https://github.com/AlDanial/cloc


## cppcheck

This is a tool for static C/C++ code analysis. Install with `sudo apt-get install cppcheck`, and run via `/.cppcheck.sh`. The results are in `cppcheck.txt` and `cppcheck.xml`. See also http://cppcheck.sourceforge.net


## vera++

This is a programmable verification and analysis tool for C++. Install with `sudo apt-get install vera++` and run via `./vera++.sh`. The results are in `vera++.txt` and `vera++.xml`. See also https://bitbucket.org/verateam/vera


## hpp2plantuml

To gain some insight in the class structure of the C++ code in an alternative way, [hpp2plantuml](https://pypi.org/project/hpp2plantuml/) can be used. This tool generates PlantUML class diagrams from parsing only the C++ headers files. Install with `sudo pip3 install --upgrade hpp2plantuml` and `sudo apt-get install plantuml`. Run it with `./hpp2plantuml.sh` and see the output in `hpp2plantuml.svg` and `hpp2plantuml.png`.


## Qt Creator

These static code checks and the code reformatter can also be run directly in Qt Creator as external tools, but needs investigation. See also https://sourceforge.net/projects/qtprojecttool


## Doxygen

TODO To investigate and add is reporting on doxygen coverage. See:
* https://github.com/psycofdj/coverxygen
* https://github.com/codecov/support/issues/364
* https://github.com/alobbs/doxy-coverage
* http://jessevdk.github.io/cldoc/
