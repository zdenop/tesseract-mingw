/*
 * $ g++ -o bin/test_302 test_302.cpp -Iinclude/tesseract/ -Iinclude/leptonica/ -Llib -Llibs -ltesseract -llept168 -lws2_32
 * $ bin/test_302
*/

#include <baseapi.h>
#include <allheaders.h>

Pix *pix;

int main(int argc, char *argv[]) {
    tesseract::TessBaseAPI *myTess = new tesseract::TessBaseAPI();
    printf("Tesseract-ocr version: %s\n",
           tesseract::TessBaseAPI::Version());
    printf("Leptonica version: %s\n",
           getLeptonicaVersion());
    
    pix = pixRead("phototest.tif");
    myTess->Init(NULL, "eng", tesseract::OEM_DEFAULT, NULL, 0, NULL, NULL, false);
    myTess->SetImage(pix);
    
    char* outText = myTess->GetUTF8Text();
    printf("OCR output:\n\n");
    printf(outText);

    myTess->Clear();
    myTess->End();
    delete [] outText;
    pixDestroy(&pix);    
    return 0;
}
