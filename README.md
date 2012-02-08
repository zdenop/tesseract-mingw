This experimental test to compile tesseract 3.02 with mingw on Windows.
Library was created in Qt Creator+mingw with cmake (script is in cmake directory).

Leptonica library(build by Microsoft Visual Studio 2008) is from [leptonica.com page](http://leptonica.com/source/vs2008-1.68.zip).

"Tesseract include files" was identified by [Tom Powers](http://groups.google.com/group/tesseract-dev/msg/a59952174f78c000) in tesserac 3.01 - maybe some file is missing.

Nothing is guaranteed - use it on your own risk (or benefit ;-) )
Improvements and forks are welcomed!

Test
====

There is a test file (test_302.cpp) that show simple usage of library. If should be possible to compile with:

    g++ -o bin/test_302 test_302.cpp -Iinclude/tesseract/ -Iinclude/leptonica/ -Llib -Llibs -ltesseract -llept168 -lws2_32
    
Than run:
    bin\test_302 phototest.tif