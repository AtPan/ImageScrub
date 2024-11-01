#ifndef __JPEG_H
#define __JPEG_H

#include <stdio.h>

/* All information gathered from Wikipedia
 * Link is Here: https://en.wikipedia.org/wiki/JPEG
 */

// Section Headers -------------------------------------------
// Relevant Wikipedia Article:
// https://en.wikipedia.org/wiki/JPEG#Syntax_and_structure

#define  dSOI 0xffd8     /* Start Of Image */
#define dSOF0 0xffc0     /* Start Of Frame (baseline DCT) */
#define dSOF2 0xffc2     /* Start Of Frame (progressive DCT) */
#define  dDHT 0xffc4     /* Define Huffman Table(s) */
#define  dDQT 0xffdb     /* Define Quantization Table(s) */
#define  dDRI 0xffdd     /* Define Restart Interval */
#define  dSOS 0xffda     /* Start Of Scan */
#define  dCOM 0xfffe     /* COMment */
#define  dEOI 0xffd9     /* End Of Image */

#define dRST 0xffd0     /* ReStarT marker */
/* The Restart Marker is placed every r macroblocks, where r is defined at DRI.
 * Not used if DRI is unused.
 * The low three bits cycles from zero to seven.
 */

#define dAPP 0xffe0      /* APPlication-Specific, used mostly for metadata */

enum JPEG_KEYWORD {
    NON,
    SOI,
    SOF0,
    SOF2,
    DHT,
    DQT,
    DRI,
    SOS,
    COM,
    EOI,
    RST0,
    RST1,
    RST2,
    RST3,
    RST4,
    RST5,
    RST6,
    RST7,
    APP0,
    APP1,
    APP2,
    APP3,
    APP4,
    APP5,
    APP6,
    APP7,
    APP8,
    APP9,
    APPa,
    APPb,
    APPc,
    APPd,
    APPe,
    APPf
};

typedef unsigned short jpeg_metadata_t;
#define      JPEG_METADATA_ALL ((jpeg_metadata_t)0)
#define     JPEG_METADATA_DATE ((jpeg_metadata_t)1)
#define     JPEG_METADATA_TIME ((jpeg_metadata_t)2)
#define JPEG_METADATA_LOCATION ((jpeg_metadata_t)4)
#define  JPEG_METADATA_COMMENT ((jpeg_metadata_t)8)
// ------------------------------------------------------------



// Data Structures --------------------------------------------
typedef FILE jpeg;
typedef unsigned short jpeg_restart_interval_t;
typedef unsigned short jpeg_keyword_t;
// ------------------------------------------------------------



// JPEG Interface ---------------------------------------------
void add_jpeg_metadata(jpeg_metadata_t);
void remove_jpeg_metadata(jpeg_metadata_t);

/* Returns a String version of the given JPEG Section Header */
const char *jpeg_keyword_display(enum JPEG_KEYWORD);

/* Finds and saves all JPEG Section Headers and their lengths.
 * Must make sure that the passed arrays are at least n elements in length.
 *
 * Element i in HEADERS will have its size saved at element i in LENGTHS.
 * i.e. Size of HEADERS[i] = LENGTHS[i], where 0 <= i < n
 *
 *      JPEG - JPEG File
 *   HEADERS - Array of JPEG Section Headers to save to
 *   LENGTHS - Array of JPEG Section lengths to save to
 * POSITIONS - Array of JPEG Section positions to save to
 *         n - Maximum amount of sections to save
 *   Returns - Number of sections saved
 */
unsigned long int jpeg_keywords(FILE *JPEG, enum JPEG_KEYWORD *HEADERS, unsigned long int *LENGTHS, unsigned long int *POSITIONS, unsigned long int n);


/* Creates a new jpeg file that has the specified metadata removed.
 * New file will be called 'new.jpeg'; if a file has the same name it will be destroyed.
 *
 * All metadata supported can be accessed using 'JPEG_METADATA_x', where x is some information.
 * Currently only data, time, location, and text comments are supported.
 * To do more than one, you can binary-or the flags together (ex: JPEG_METADATA_x | JPEG_METADATA_x).
 * To remove ALL metadata (which could have unintended results), use JPEG_METADATA_ALL.
 *
 * JPEG - Jpeg file to remove metadata from
 * META - List of metadata information to remove.
 */
void jpeg_metadata_clear(FILE *JPEG);
// ------------------------------------------------------------

#endif
