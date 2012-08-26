/**********************************************************************
* File:        ApiExample.cpp
* Description: Example usege of Tesseract-OCR API
* Author:      Zdenko Podobny
* Created:     2012-08-26
*
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*
**********************************************************************/

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(int argc, char *argv[])
{
    const char *language = "eng";
    const char *datapath = "../tessdata/";
    const char* filename = NULL;
    PIX  *pix;

    tesseract::TessBaseAPI *tAPI = new tesseract::TessBaseAPI();

    printf("Tesseract-ocr version: %s\n",
           tesseract::TessBaseAPI::Version());
    printf("Leptonica version: %s\n",
           getLeptonicaVersion());

    if (argv[1] == NULL) {
        filename = "../phototest.tif";
    } else {
        filename = argv[1];
    }

    // Create pix
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        pix = pixRead(filename);
        if (pix == NULL) {
            printf("Unsupported image type.\n");
            exit(3);
        }
        else {
            printf("Pix created\n");
        }
    } else {
        printf("file '%s' doesn't exist.\n", filename);
        exit(3);
    }

    // Initalize tesseract
    int rc = tAPI->Init(datapath, language, tesseract::OEM_DEFAULT, NULL, 0, NULL, NULL, false);
    if (rc) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      exit(1);
    }

    // Set PIX for OCR
    tAPI->SetImage(pix);

    // Optional: set rectangle to OCR
    tAPI->SetRectangle(30, 90, 600, 173);

    // run ocr
    char* outText = tAPI->GetUTF8Text();
    printf("OCR output:\n\n");
    printf(outText);

    tAPI->Clear();
    tAPI->End();
    delete [] outText;
    pixDestroy(&pix);
    return 0;
}
