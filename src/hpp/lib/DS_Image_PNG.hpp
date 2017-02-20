#pragma once

#include "h/lib/DS_Image_PNG.h"

#include "hpp/lib/DS_Image.hpp"
extern "C" {
	#include "png_ddsl.h"
}
using namespace std;
using namespace DSTypes;
using namespace DSLib;

namespace DSImage {
	template <typename T, typename TIdx>
	void writePNG(DSLib::Matrix<T, TIdx> &mat, const DSTypes::String &file, const DSTypes::ImageType it) {
		mat.breakUp();
		if (mat.order() != oRowMajor)
			throw Error(ecUnsupported, "writePNG", "Can only write a RowMajor matrix as image");
		TIdx h = mat.rows.count();
		TIdx w = mat.cols.count();
		std::shared_ptr<T> src(mat.getData(), [](T*){});
		int err;
		if (it == itM8) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderContinuous<Mono<T>, T>(h, w, src), OrderArrayOfArray<Mono<UInt8>, UInt8>(h, w)(dst));
			err = writeRawPNG(dst.get(), h, w, PNG_COLOR_TYPE_GRAY, 8, file.c_str());
		} else if (it == itM16) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderContinuous<Mono<T>, T>(h, w, src), OrderArrayOfArray<Mono<UInt16>, UInt16>(h, w)(dst));
			err = writeRawPNG(dst.get(), h, w, PNG_COLOR_TYPE_GRAY, 16, file.c_str());		
		} else if (it == itMA8) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderContinuous<Mono<T>, T>(h, w/2, src), OrderArrayOfArray<Mono<UInt8>, UInt8>(h, w/2)(dst));
			err = writeRawPNG(dst.get(), h, w/2, PNG_COLOR_TYPE_GRAY_ALPHA, 8, file.c_str());		
		} else if (it == itMA16) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderContinuous<Mono<T>, T>(h, w/2, src), OrderArrayOfArray<Mono<UInt16>, UInt16>(h, w/2)(dst));
			err = writeRawPNG(dst.get(), h, w/2, PNG_COLOR_TYPE_GRAY_ALPHA, 16, file.c_str());		
		} else if (it == itRGB8) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderContinuous<RGB<T>, T>(h, w / 3, src), OrderArrayOfArray<RGB<UInt8>, UInt8>(h, w / 3)(dst));
			err = writeRawPNG(dst.get(), h, w/3, PNG_COLOR_TYPE_RGB, 8, file.c_str());		
		} else if (it == itRGBA8) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderContinuous<RGBA<T>, T>(h, w / 4, src), OrderArrayOfArray<RGBA<UInt8>, UInt8>(h, w / 4)(dst));
			err = writeRawPNG(dst.get(), h, w/4, PNG_COLOR_TYPE_RGB_ALPHA, 8, file.c_str());
		} else if (it == itRGB16) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderContinuous<RGB<T>, T>(h, w / 4, src), OrderArrayOfArray<RGB<UInt16>, UInt16>(h, w / 3)(dst));
			err = writeRawPNG(dst.get(), h, w/3, PNG_COLOR_TYPE_RGB, 16, file.c_str());		
		} else if (it == itRGBA16) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderContinuous<RGBA<T>, T>(h, w / 4, src), OrderArrayOfArray<RGBA<UInt16>, UInt16>(h, w / 4)(dst));
			err = writeRawPNG(dst.get(), h, w / 4, PNG_COLOR_TYPE_RGB_ALPHA, 16, file.c_str());
		} else if (it == itRGB8Planar) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderPlanar<RGBPlanar<T>, T>(h, w / 3, src), OrderArrayOfArray<RGB<UInt8>, UInt8>(h, w / 3)(dst));
			err = writeRawPNG(dst.get(), h, w / 3, PNG_COLOR_TYPE_RGB, 8, file.c_str());
		} else if (it == itRGB16Planar) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderPlanar<RGBPlanar<T>, T>(h, w / 3, src), OrderArrayOfArray<RGB<UInt16>, UInt16>(h, w / 3)(dst));
			err = writeRawPNG(dst.get(), h, w / 3, PNG_COLOR_TYPE_RGB, 16, file.c_str());
		} else if (it == itRGB8PlanarV) {
			std::shared_ptr<UInt8*> dst;
			convertBuffer(OrderPlanarV<RGBPlanarV<T>, T>(h / 3, w, src), OrderArrayOfArray<RGB<UInt8>, UInt8>(h / 3, w)(dst));
			err = writeRawPNG(dst.get(), h / 3, w, PNG_COLOR_TYPE_RGB, 8, file.c_str());
		} else if (it == itRGB16PlanarV) {
			std::shared_ptr<UInt16*> dst;
			convertBuffer(OrderPlanarV<RGBPlanarV<T>, T>(h / 3, w, src), OrderArrayOfArray<RGB<UInt16>, UInt16>(h / 3, w)(dst));
			err = writeRawPNG(dst.get(), h / 3, w, PNG_COLOR_TYPE_RGB, 16, file.c_str());
		} else
			throw Error(ecNotImplemented, "writePNG", "Not supported");
		if (err != PNG_OK)
			throw Error(ecNotFound, "WritePNG", SS("writePNGInfo returned: " << err << ". Does the folder exist to write " << file << "?"));
	}

	template <typename T, typename TIdx>
	ImageType readPNG(DSLib::Matrix<T, TIdx> &mat, const DSTypes::String &file) {
		unsigned int h, w;
		unsigned char c, bits;
		int m = readPNGInfo(&h, &w, &c, &bits, file.c_str());
		if (m != PNG_OK)
			throw Error(ecNotFound, "ReadPNG", SS("readPNGInfo returned: " << m << ". Does the file exist: " << file << "?"));
		mat.clear();
		mat.setOrder(oRowMajor);
		if (c == PNG_COLOR_TYPE_GRAY) {
			mat.resize(h, w);
			std::shared_ptr<T> dst(mat.getData(), [](T*){});
			if (bits == 8) {
				OrderArrayOfArray<Mono<UInt8>, UInt8> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<Mono<T>, T>(h, w, dst));
				return itM8;
			} else if (bits == 16) {
				OrderArrayOfArray<Mono<UInt16>, UInt16> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<Mono<T>, T>(h, w, dst));
				return itM16;
			}
		} else if (c == PNG_COLOR_TYPE_GRAY_ALPHA) {
			mat.resize(h, w * 2);
			std::shared_ptr<T> dst(mat.getData(), [](T*){});
			if (bits == 8) {
				OrderArrayOfArray<MonoA<UInt8>, UInt8> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<MonoA<T>, T>(h, w, dst));
				return itMA8;
			} else if (bits == 16) {
				OrderArrayOfArray<MonoA<UInt16>, UInt16> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<MonoA<T>, T>(h, w, dst));
				return itMA16;
			}
		} else if (c == PNG_COLOR_TYPE_RGB) {
			mat.resize(h, w * 3);
			std::shared_ptr<T> dst(mat.getData(), [](T*){});
			if (bits == 8) {
				OrderArrayOfArray<RGB<UInt8>, UInt8> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<RGB<T>, T>(h, w, dst));
				return itRGB8;
			} else if (bits == 16) {
				OrderArrayOfArray<RGB<UInt16>, UInt16> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<RGB<T>, T>(h, w, dst));
				return itRGB16;
			}
		} else if (c == PNG_COLOR_TYPE_RGB_ALPHA) {
			mat.resize(h, w * 4);
			std::shared_ptr<T> dst(mat.getData(), [](T*){});
			if (bits == 8) {
				OrderArrayOfArray<RGBA<UInt8>, UInt8> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<RGBA<T>, T>(h, w, dst));
				return itRGBA8;
			} else if (bits == 16) {
				OrderArrayOfArray<RGBA<UInt16>, UInt16> src(h, w);
				readRawPNG(src().get(), file.c_str());
				convertBuffer(src, OrderContinuous<RGBA<T>, T>(h, w, dst));
				return itRGBA16;
			}
		}
		throw Error(ecNotImplemented, "readPNG", SS("Unsupported PNG type:" << " ColorType = " << TIdx(c) << " BPP = " << bits));
	}

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx>::ImagePNG(const TIdx rowCount, const TIdx colCount, const T &initVal, DSTypes::ImageType preferredImageType) : DSLib::Matrix<T, TIdx>(rowCount, colCount, initVal, DSTypes::oRowMajor) {
		setImageType(preferredImageType);
	};

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx>::ImagePNG(const String &filename, DSLib::Matrix<T, TIdx> &img, ImageType preferredImageType) {
		setFilename(filename);
		setImage(img);
		setImageType(preferredImageType);
	}

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx>::ImagePNG(const String &filename, bool prefetch) {
		setFilename(filename);
		if (prefetch)
			loadImage();
		else {
			Matrix<T, TIdx> img;
			setImage(img);
			setImageType(itM16);
		}
	}

	template <typename T, typename TIdx>
	ImageType ImagePNG<T, TIdx>::getImageType() const {
		return it_;
	}

	template <typename T, typename TIdx>
	UInt32 ImagePNG<T, TIdx>::getChannelCount() const {
		if (it_ == itM8 || it_ == itM16)
			return 1;
		if (it_ == itMA8 || it_ == itMA16) 
			return 2;
		if (it_ == itRGB8 || it_ == itRGB16 || it_ == itRGB8Planar || it_ == itRGB8PlanarV || it_ == itRGB16Planar || it_ == itRGB16PlanarV)
			return 3;
		return 4;
	}

	template <typename T, typename TIdx>
	UInt32 ImagePNG<T, TIdx>::getBitsPerChannel() const {
		if (it_ == itMA8 || it_ == itM8 || it_ == itRGB8 || it_ == itRGBA8 || it_ == itRGB8Planar || it_ == itRGB8PlanarV)
			return 8;
		if (it_ == itMA16 || it_ == itM16 || it_ == itRGB16 || it_ == itRGBA16 || it_ == itRGB16Planar || it_ == itRGB16PlanarV)
			return 16;
		throw Error(ecInternal, "getBitsPerChannel", SS("Unknown bits per channel for " << etos(it_)));
	}

	template <typename T, typename TIdx>
	bool ImagePNG<T, TIdx>::isPlanar() const {
		return (it_ == itRGB8Planar || it_ == itRGB16Planar || it_ == itRGB8PlanarV || it_ == itRGB16PlanarV);
	}

	template <typename T, typename TIdx>
	UInt32 ImagePNG<T, TIdx>::getImageWidth() const {
		if (it_ == itRGB8 || it_ == itRGB16 || it_ == itRGB8Planar ||  it_ == itRGB16Planar)
			return this->cols.count() / 3;
		if (it_ == itRGBA8 || it_ == itRGBA16)
			return this->cols.count() / 4;
		return this->cols.count();
	}

	template <typename T, typename TIdx>
	UInt32 ImagePNG<T, TIdx>::getImageHeight() const {
		if (it_ == itRGB8PlanarV || it_ == itRGB16PlanarV)
			return this->rows.count() / 3;
		return this->rows.count();
	}


	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::setImageType(const ImageType imageType) {
		it_ = imageType;
	}

	template <typename T, typename TIdx>
	DSLib::Matrix<T, TIdx> ImagePNG<T, TIdx>::getImage() {
		loadImage();
		return *this;
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::setImage(DSLib::Matrix<T, TIdx> &img) {
		*((Matrix<T, TIdx> *)this) = img;
	}

	template <typename T, typename TIdx>
	String ImagePNG<T, TIdx>::getFilename() const {
		return filename_;
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::setFilename(const String &filename) {
		filename_ = filename;
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::loadImage() {
		if (!hasImage())
			it_ = DSImage::readPNG(*this, filename_);
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::saveImage() {
		DSImage::writePNG(*this, filename_, it_);
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::saveImage(const String filename) {
		setFilename(filename);
		saveImage();
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::saveImage(const String filename, const ImageType it) {
		setFilename(filename);
		setImageType(it);
		saveImage();
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::freeImage() {
		this->clear();
	}

	template <typename T, typename TIdx>
	bool ImagePNG<T, TIdx>::hasImage() const {
		return this->isInitialized();
	}
	
	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::clone(const ImagePNG &other) { 
		filename_ = other.filename_;
		it_ = other.it_;
		Matrix<T>::clone(other);
	};

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::clone(ImagePNG &&other) {
		filename_ = other.filename_;
		it_ = other.it_;
		Matrix<T>::clone(std::move(other));
	};

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::read(istream &is) {
		DSUtil::read(is, filename_);
		DSUtil::read(is, it_);
		Matrix<T, TIdx>::read(is);
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::write(ostream &os) const {		
		Matrix<T, TIdx> img;
		if (!hasImage() && filename_ != "")
			DSImage::readPNG(img, filename_);
		else
			img = *this;
		DSUtil::write(os, filename_);
		DSUtil::write(os, it_);
		img.write(os);
	}

	template <typename T, typename TIdx>
	void ImagePNG<T, TIdx>::convertRGBToPlanarV() {	
		loadImage();
		if (getBitsPerChannel() == 8 && getChannelCount() == 3) {
			TIdx h = this->rows.count();
			TIdx w = this->cols.count() / 3;
			Matrix<T> dstmat(h * 3, w);
			std::shared_ptr<T> dst(dstmat.getData(), [](T*){});
			std::shared_ptr<T> src(this->getData(), [](T*){});
			convertBuffer(DSImage::OrderContinuous<DSImage::RGB<T>, T>(h, w, src), DSImage::OrderPlanarV<DSImage::RGBPlanarV<T>, T>(h, w, dst));
			static_cast<Matrix<T, TIdx>&>(*this) = dstmat;
			it_ = itRGB8PlanarV;
		} else if (getBitsPerChannel() == 16 && getChannelCount() == 3) {
			TIdx h = this->rows.count();
			TIdx w = this->cols.count() / 3;
			Matrix<T> dstmat(h * 3, w);
			std::shared_ptr<T> dst(dstmat.getData(), [](T*){});
			std::shared_ptr<T> src(this->getData(), [](T*){});
			convertBuffer(DSImage::OrderContinuous<DSImage::RGB<T>, T>(h, w, src), DSImage::OrderPlanarV<DSImage::RGBPlanarV<T>, T>(h, w, dst));
			static_cast<Matrix<T, TIdx>&>(*this) = dstmat;
			it_ = itRGB16PlanarV;
		}
	}

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx> ImagePNG<T, TIdx>::getChannel(const TIdx n) {
		loadImage();
		ImagePNG<T, TIdx> ret(getImageHeight(), getImageWidth(), (T)0, (getBitsPerChannel() == 8)?itM8:itM16);
		if (it_ == itRGB8Planar || it_ == itRGB16Planar) {
			ret.mat() = mat()(TIdx(0), getImageHeight(), n * getImageWidth(), getImageWidth());  
		} else if (it_ == itRGB8Planar || it_ == itRGB16Planar) {
			ret.mat() = mat()(n * getImageHeight(), getImageHeight(), TIdx(0), getImageWidth());  
		} else {
			TIdx channels = getChannelCount();
			if (channels < n)
				throw Error(ecRangeError, "getChannel", SS("Cannot get channel " << n << " from an image with " << n << " channels"));
			for (TIdx y = 0; y<ret.getImageHeight(); y++) {
				for (TIdx x = 0; x < ret.getImageWidth(); x++) {
					ret.mat().val(y, x) = mat().val(y, (x * channels) + n);
				}
			}
		}
		return ret;
	}

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx>::ImagePNG(Matrix<T, TIdx> &&other) {
		try {
			clone(dynamic_cast<ImagePNG<T, TIdx>&&>(other));
			Matrix<T, TIdx>::clone(other);
		} catch (const exception e){
			throw Error(ecInternal, "ImagePNG copy constructor from Matrix", String(e.what()));
		}
	}

	template <typename T, typename TIdx>
	ImagePNG<T, TIdx>::ImagePNG(const Matrix<T, TIdx> &other) {
		try {
			clone(dynamic_cast<const ImagePNG<T, TIdx>&>(other));
			Matrix<T, TIdx>::clone(other);
		} catch (const exception e){
			throw Error(ecInternal, "ImagePNG copy constructor from Matrix", String(e.what()));
		}
	}
};