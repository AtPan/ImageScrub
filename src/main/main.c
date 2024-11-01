#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jpeg.h"

int main(int, char*[]);
unsigned long int strrnchr(const char *, char, unsigned long int);
void display_usage(void);

void display_usage() {
    printf("This program is meant to remove selected metadata from a picture file.\n"
        "Metadata is extra information that can be convienent, but can also be a security concern for many.\n"
        "Metadata can include (but is not limted to): Exact location of a picture, date picture was taken, and time a picture was taken.\n"
        "\nOptional Flags:\n"
        "\t--help: Displays this screen\n"
        "\t--date: Remove the date a picture was taken\n"
        "\t--time: Remove the time a picture was taken\n"
        "\t--location: Remove the location a picture was taken\n"
        "If no flag is passed, all metadata is removed. While the safest choice, it can have unintended side effects.\n"
        "Please make a backup of your picture in case the changes made are unsatisfactory and you wish to revert.");
}

int main(int argc, char *argv[]) {
    if(argc <= 2) {
        fprintf(stderr, "Usage: %s pictureName [flags]\n", argv[0]);
        fputs("Run with --help for more information\n", stderr);
        exit(1);
    }

    FILE *fh;
    if((fh = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "FATAL: Could not open file '%s'\n", argv[1]);
        exit(4);
    }

    unsigned long int filelen = strlen(argv[1]);
    unsigned long int extension = strrnchr(argv[1], '.', filelen);

    if(strncmp(argv[1] + extension + 1, "jpeg", filelen) == 0 || strncmp(argv[1] + extension + 1, "jpg", filelen) == 0) {
        for(int i = 2; i < argc; i++) {
            int arglen = strlen(argv[i]);

            if(strncmp(argv[i], "--help", arglen) == 0) {
                display_usage();
                exit(0);
            }

            if(strncmp(argv[i], "--all", arglen) == 0) {
                add_jpeg_metadata(JPEG_METADATA_ALL);
                break;
            }
            else if(strncmp(argv[i], "--location", arglen) == 0) {
                add_jpeg_metadata(JPEG_METADATA_LOCATION);
            }
            else if(strncmp(argv[i], "--date", arglen) == 0) {
                add_jpeg_metadata(JPEG_METADATA_DATE);
            }
            else if(strncmp(argv[i], "--time", arglen) == 0) {
                add_jpeg_metadata(JPEG_METADATA_TIME);
            }
        }

        jpeg_metadata_clear(fh);
    }
}

unsigned long int strrnchr(const char *s, char c, unsigned long int len) {
    unsigned long int pos = -1;
    for(unsigned long int i = 0; i < len; i++) {
        if(s[i] == c) pos = i;
    }
    return pos;
}
