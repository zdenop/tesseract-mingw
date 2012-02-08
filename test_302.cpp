/*
 *
 * $ g++ -o bin/test_302 test_302.cpp -Iinclude/tesseract/ -Iinclude/leptonica/ -Llib -Llibs -ltesseract -llept168 -lws2_32
 * $ bin\test_302
 */

#include <baseapi.h>
#include <allheaders.h>

Pix *pix;

int main(int argc, char *argv[])
{
  tesseract::TessBaseAPI *myTess = new tesseract::TessBaseAPI();

  printf("Tesseract-ocr version: %s\n",
         tesseract::TessBaseAPI::Version());
  printf("Leptonica version: %s\n",
         getLeptonicaVersion());

  if (argv[1] == NULL)
    {
      printf("Usage: tess_302 image_filename.\n");
      exit(1);
    }
  // create pix
  FILE *fp = fopen(argv[1], "r");
  if (fp)
    {
      fclose(fp);
      pix = pixRead(argv[1]);
      if (pix == NULL)
        {
          printf("Unsupported image type.\n");
          exit(3);
        }
      else
        {
          printf("Pix created\n");
        }
    }
  else
    {
      printf("file '%s' doesnt exist.\n", argv[1]);
      exit(3);
    }

  // initalize tesseract
  myTess->Init(NULL, "eng", tesseract::OEM_DEFAULT, NULL, 0, NULL, NULL, false);

  // run ocr
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
