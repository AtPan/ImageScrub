#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpeg.h"

#define MAX_ARR 255

static jpeg_metadata_t meta;

void add_jpeg_metadata(jpeg_metadata_t _m) {
    meta |= _m;
}

void remove_jpeg_metadata(jpeg_metadata_t _m) {
    meta &= ~_m;
}

void jpeg_metadata_clear(FILE *fh) {
    FILE *new;
    if((new = fopen("scrubbed.jpeg", "w")) == NULL) {
        fputs("FATAL: Could not open 'new.jpeg' for writing\n", stderr);
        exit(1);
    }

    enum JPEG_KEYWORD *k = malloc(sizeof(enum JPEG_KEYWORD) * MAX_ARR);
    unsigned long int *len = malloc(sizeof(unsigned long int) * MAX_ARR);
    unsigned long int *pos = malloc(sizeof(unsigned long int) * MAX_ARR);
    if(k == NULL || len == NULL || pos == NULL) {
        fputs("FATAL: Could not create buffer for file handeling\n", stderr);
        exit(2);
    }

    jpeg_keywords(fh, k, len, pos, MAX_ARR);

    unsigned long int curr_keyword = 0;
    unsigned long int curr_pos = 0;
    unsigned char b;

    while(!feof(fh)) {
        if(curr_pos == pos[curr_keyword]) {
            if((meta == JPEG_METADATA_ALL && (k[curr_keyword] == COM || (k[curr_keyword] >= APP0 && k[curr_keyword] <= APPf)))
             || (((meta & JPEG_METADATA_COMMENT)) && k[curr_keyword] == COM)) {
                curr_pos += (len[curr_keyword] + 2);
                fseek(fh, curr_pos, SEEK_SET);
            }
            else if(k[curr_keyword] == APP1) {
                unsigned char skipped = 0;
                char *buf = malloc(len[curr_keyword] + 2);
                if(buf == NULL) {
                    fputs("FATAL: Could not secure buffer for file writing\n", stderr);
                    exit(2);
                }

                fread(buf, 2, 1, fh);
                fread(buf, 6, 1, fh);
                fseek(fh, pos[curr_keyword], SEEK_SET);
                if(strncmp(buf, "Exif\0\0", 6) == 0) {
                    for(unsigned long int i = 0; i < len[curr_keyword] + 2; i++) {
                        fread(buf + i - skipped, 1, 1, fh);
                        // Location tag
                        if((meta & JPEG_METADATA_LOCATION) && (buf[i - skipped] == (char)0xa2)) {
                            fread(buf + i + 1 - skipped, 1, 1, fh);
                            if(buf[i + 1 - skipped] == 0x14) {
                                skipped += 4;
                            }
                        }
                    }
                    buf[3] -= skipped;
                }
                else {
                    fread(buf, 1, len[curr_keyword] + 2, fh);
                }

                fwrite(buf, 1, len[curr_keyword] + 2 - skipped, new);

                free(buf);

                curr_pos += (len[curr_keyword] + 2);
            }
            curr_keyword++;
        }
        fread(&b, 1, 1, fh);
        fwrite(&b, 1, 1, new);
        curr_pos++;
    }

    free(pos);
    free(len);
    free(k);

    fclose(new);
}
