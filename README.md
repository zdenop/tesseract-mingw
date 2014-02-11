This experimental test to compile tesseract 3.02 (svn revision 738) and leptonica 1.69 with mingw on Windows (XP SP3).
Library was created in mingw+msys environment,

**Tesseract include files** was identified by [Tom Powers](http://groups.google.com/group/tesseract-dev/msg/a59952174f78c000) in tesserac-ocr 3.01 - maybe some file is missing.

Nothing is guaranteed - use it on your own risk (or benefit ;-) ).
Improvements and forks are welcomed!


ApiExample
=======

There is the `ApiExample.cpp` file. It shows a simple usage of library. It can be compiled with:

   ``g++ ApiExample.cpp -o bin/ApiExample -D__BLOB_T_DEFINED -llept -ltesseract``

Then run:

   ``bin\ApiExample phototest.tif``

There is also the project file for QT Creator `ApiExample.pro`. It shows how to use tesseract-ocr and leptonica libraries within QT Creator (there is no QT4 usage example). Unfortunately QT Creator do not show console output within its Application Output.
