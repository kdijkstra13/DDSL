#pragma once
//#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

enum pngerr { PNG_OK = 0, PNG_DDSL_CANNOT_OPEN, PNG_DDSL_NOT_A_PNG_FILE, PNG_DDSL_CREATE_READ_STRUCT_FAILED, PNG_DDSL_CREATE_INFO_STRUCT_FAILED, PNG_DDSL_ERROR_INIT_IO, PNG_DDSL_READ_IMG_ERROR, PNG_DDSL_WRONG_TYPE, PNG_DDSL_WRITE_HEADER_FAILED, PNG_DDSL_WRITE_FAILED};

void abort_(const char * s, ...);

int prepareReadPNG(png_structp *png_ptr, png_infop *info_ptr, png_FILE_p * fp, const char * file);
int readPNGInfo(unsigned int *h, unsigned int *w, unsigned char *c, unsigned char *bits, const char * file);
int readRawPNG(void * dst, const char * file);
int writeRawPNG(void * src, unsigned int h, unsigned int w, unsigned char c, unsigned char b, const char * file);
