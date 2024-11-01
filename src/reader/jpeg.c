#include <stdio.h>
#include "jpeg.h"

unsigned long int jpeg_keywords(FILE *fh, enum JPEG_KEYWORD *k, unsigned long int *len, unsigned long int *pos, unsigned long int n) {
    if(n == 0 || fh == NULL) return 0;

    unsigned char segsize;
    unsigned char b;
    unsigned short s;
    unsigned long int i = 0;
    unsigned long int file_pos = ftell(fh);

    while(!feof(fh) && i < n) {
        if(fread(&b, 1, 1, fh) != 1) break;

        // Command byte
        if(b == 0xff) {
            s = 0xff00;

            if(fread(&b, 1, 1, fh) != 1) break;
            s += b;

            if((s & 0xfff8) == dRST) {
                k[i] = RST0 + (s & 7);
                segsize = 0;
            }
            else if((s & 0xfff0) == dAPP) {
                k[i] = APP0 + (s & 15);
            }
            else if (s == dSOI)
            {
                k[i] = SOI;
                segsize = 0;
            }
            else if (s == dSOF0)
            {
                k[i] = SOF0;
            }
            else if (s == dSOF2)
            {
                k[i] = SOF2;
            }
            else if (s == dDHT)
            {
                k[i] = DHT;
            }
            else if (s == dDQT)
            {
                k[i] = DQT;
            }
            else if (s == dDRI)
            {
                k[i] = DRI;
            }
            else if (s == dSOS)
            {
                k[i] = SOS;
            }
            else if (s == dCOM)
            {
                k[i] = COM;
            }
            else if (s == dEOI)
            {
                k[i] = EOI;
                segsize = 0;
            }
            else {
                continue;
            }

            pos[i] = ftell(fh) - 2;

            if(segsize) {
                if(fread(&b, 1, 1, fh) != 1) break;
                len[i] += (b << 8);
                if(fread(&b, 1, 1, fh) != 1) break;
                len[i] += b;

                fseek(fh, pos[i] + len[i], SEEK_SET);
            }
            else {
                len[i] = 0;
                segsize++;
            }

            i++;
        }
    }

    fseek(fh, file_pos, SEEK_SET);
    return i;
}

const char *jpeg_keyword_display(enum JPEG_KEYWORD k) {
    static const char *table[] = {
        "NON", "Start Of Image", "Start of Frame (baseline DCT)", "Start of Frame (progressive DCT)",
        "Define Huffman Table", "Define Quantization Table", "Define Restart Interval",
        "Start of Scan", "Text Comment", "End Of Image",
        "Restart Marker 0",
        "Restart Marker 1",
        "Restart Marker 2",
        "Restart Marker 3",
        "Restart Marker 4",
        "Restart Marker 5",
        "Restart Marker 6",
        "Restart Marker 7",
        "App-Specific Marker 0",
        "App-Specific Marker 1",
        "App-Specific Marker 2",
        "App-Specific Marker 3",
        "App-Specific Marker 4",
        "App-Specific Marker 5",
        "App-Specific Marker 6",
        "App-Specific Marker 7",
        "App-Specific Marker 8",
        "App-Specific Marker 9",
        "App-Specific Marker A",
        "App-Specific Marker B",
        "App-Specific Marker C",
        "App-Specific Marker D",
        "App-Specific Marker E",
        "App-Specific Marker F",
    };

    if (k > APPf) k = NON;

    return table[k];
}
