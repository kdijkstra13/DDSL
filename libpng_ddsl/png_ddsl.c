#include "png_ddsl.h"

int prepareReadPNG(png_structp *png_ptr, png_infop *info_ptr, png_FILE_p * fp, const char * file) {
	unsigned char header[8];	
	*fp = fopen(file, "rb");
	if (!*fp)
		return PNG_DDSL_CANNOT_OPEN;
	fread(header, 1, 8, *fp);
	if (png_sig_cmp(header, 0, 8))
		return PNG_DDSL_NOT_A_PNG_FILE;
	*png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!(*png_ptr))
		return PNG_DDSL_CREATE_READ_STRUCT_FAILED;
	*info_ptr = png_create_info_struct(*png_ptr);
	if (!(*info_ptr))
		return PNG_DDSL_CREATE_INFO_STRUCT_FAILED;
	if (setjmp(png_jmpbuf(*png_ptr)))
		return PNG_DDSL_ERROR_INIT_IO;
	png_init_io(*png_ptr, *fp);
	png_set_sig_bytes(*png_ptr, 8);
	png_read_info(*png_ptr, *info_ptr);
	return PNG_OK;
}

int readPNGInfo(unsigned int *h, unsigned int *w, unsigned char *c, unsigned char *bits, const char * file) {
	png_structp png_ptr;
	png_infop info_ptr;
	//png_inforp info_ptr;
	png_FILE_p f;
	int e;
	if ((e = prepareReadPNG(&png_ptr, &info_ptr, &f, file)) != PNG_OK) {
		if (e > PNG_DDSL_CANNOT_OPEN)
			fclose(f);
		return e;
	}
	*w = png_get_image_width(png_ptr, info_ptr);
	*h = png_get_image_height(png_ptr, info_ptr);
	*c = png_get_color_type(png_ptr, info_ptr);
	*bits = png_get_bit_depth(png_ptr, info_ptr);
	free(png_ptr);
	free(info_ptr);
	fclose(f);
	return PNG_OK;
}

int writeRawPNG(void * src, unsigned int h, unsigned int w, unsigned char c, unsigned char b, const char * file) {
	png_structp png_ptr;
	//png_inforp info_ptr;
	png_infop info_ptr;	
	FILE *f = fopen(file, "wb");
	if (!f)
		return PNG_DDSL_CANNOT_OPEN;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return PNG_DDSL_CREATE_READ_STRUCT_FAILED;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return PNG_DDSL_CREATE_INFO_STRUCT_FAILED;
	if (setjmp(png_jmpbuf(png_ptr)))
		return PNG_DDSL_ERROR_INIT_IO;
	png_init_io(png_ptr, f);
	if (setjmp(png_jmpbuf(png_ptr)))
		return PNG_DDSL_WRITE_HEADER_FAILED;
	png_set_IHDR(png_ptr, info_ptr, w, h, b, c, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	if (setjmp(png_jmpbuf(png_ptr)))
		return PNG_DDSL_WRITE_FAILED;
	
	png_write_image(png_ptr, src);
	
	if (setjmp(png_jmpbuf(png_ptr)))
		return PNG_DDSL_WRITE_FAILED;

	png_write_end(png_ptr, NULL);	
	//free(png_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
	free(info_ptr);
	fclose(f);
	return PNG_OK;
}

int readRawPNG(void * dst, const char * file) {
	png_structp png_ptr;
	//png_inforp info_ptr;
	png_infop info_ptr;
	png_FILE_p f;
	int e;
	if ((e = prepareReadPNG(&png_ptr, &info_ptr, &f, file)) != PNG_OK) {
		if (e > PNG_DDSL_CANNOT_OPEN)
			fclose(f);
		return e;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
		return PNG_DDSL_READ_IMG_ERROR;
	png_read_image(png_ptr, dst);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	free(info_ptr);
	fclose(f);
	return PNG_OK;
}
