#pragma once

#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"

namespace DSImage {

	template <typename T, typename TIdx>
	DSTypes::ImageType readPNG(DSLib::Matrix<T, TIdx> &mat, const DSTypes::String &file);

	template <typename T, typename TIdx>
	void writePNG(DSLib::Matrix<T, TIdx> &mat, const DSTypes::String &file, const DSTypes::ImageType = DSTypes::itM8);

	template <typename T, typename TIdx = DSTypes::MatrixIdx>
	class ImagePNG : public DSLib::Matrix<T, TIdx> {
	private:
		DSTypes::String filename_;
		DSTypes::ImageType it_;
	public:
		ImagePNG() {it_ = DSTypes::itM8; filename_="";};
		virtual ~ImagePNG() {};
		
		virtual void read(std::istream &is);
		virtual void write(std::ostream &os) const;
		
		ImagePNG(DSLib::Matrix<T, TIdx> &img);
		ImagePNG(const DSTypes::String &filename, bool prefetch);
		ImagePNG(const DSTypes::String &filename, DSLib::Matrix<T, TIdx> &img = DSLib::Matrix<T, TIdx>(), DSTypes::ImageType preferredImageType = DSTypes::itM16);
		ImagePNG(const TIdx rowCount, const TIdx colCount, const T &initVal, DSTypes::ImageType preferredImageType= DSTypes::itM16);
		ImagePNG(const TIdx rowCount, const TIdx colCount, DSTypes::ImageType preferredImageType= DSTypes::itM16);

		void loadImage();
		void saveImage();
		void saveImage(const DSTypes::String filename);
		void saveImage(const DSTypes::String filename, const DSTypes::ImageType it);
		void freeImage();
		bool hasImage() const;

		DSLib::Matrix<T, TIdx> getImage();
		void setImage(DSLib::Matrix<T, TIdx> &img);
		DSTypes::String getFilename() const;
		void setFilename(const DSTypes::String &filename);
		DSTypes::ImageType getImageType() const;
		DSTypes::UInt32 getBitsPerChannel() const;
		bool isPlanar() const;
		DSTypes::UInt32 getChannelCount() const;
		DSTypes::UInt32 getImageWidth() const;
		DSTypes::UInt32 getImageHeight() const;
		DSTypes::String getExt() const{ return ".png"; };
		void setImageType(const DSTypes::ImageType imageType);
		DSLib::Matrix<T, TIdx> & mat() { return *this; };
		void convertRGBToPlanarV();
		ImagePNG<T, TIdx> getChannel(const TIdx n);
		
		ImagePNG(ImagePNG<T, TIdx> &&other) { clone(std::move(other)); };
		ImagePNG(const ImagePNG<T, TIdx> &other) { clone(other); };
		ImagePNG(DSLib::Matrix<T, TIdx> &&other);
		ImagePNG(const DSLib::Matrix<T, TIdx> &other);
		ImagePNG & operator=(const ImagePNG<T, TIdx> &other) { clone(other); return *this;};
		ImagePNG & operator=(ImagePNG<T, TIdx> &&other) { clone(std::move(other)); return *this;};

		void clone(const ImagePNG<T, TIdx> &other);
		void clone(ImagePNG<T, TIdx> &&other);
	};
}
