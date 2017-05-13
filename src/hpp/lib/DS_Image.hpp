#pragma once

#include "h/lib/DS_Image.h"

#include <limits>
#include <memory>

namespace DSImage {
	//Structs for casting memory buffer to convenient lay-outs
	template <typename T>struct LayoutRGB { T r;T g;T b; };
	template <typename T>struct LayoutRGBA { T r;T g;T b; T a;};
	template <typename T> struct LayoutBGR { T b;T g;T r; };
	template <typename T> struct LayoutBGRA { T b;T g;T r;T a; };
	template <typename T> struct LayoutARGB { T a;T r;T g;T b; };
	template <typename T> struct LayoutMonoA { T m;T a; };

	//Descr: BGRA pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class BGRA {
	private:
		T * buf_;
		const unsigned int byt_ = 4;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * byt_; }
		inline void operator++(int) { buf_ += byt_; }
		inline void operator--(int) { buf_ -= byt_; }
	};

	//Descr: RGBA pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class RGBA {
	private:
		T * buf_;
		const unsigned int byt_ = 4;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * byt_; }
		inline void operator++(int) { buf_ += byt_; }
		inline void operator--(int) { buf_ -= byt_; }
	};

	//Descr: ARGB pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class ARGB {
	private:
		T * buf_;
		const unsigned int byt_ = 4;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * byt_; }
		inline void operator++(int) { buf_ += byt_; }
		inline void operator--(int) { buf_ -= byt_; }
	};

	//Descr: RGB pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class RGB {
	private:
		T * buf_;
		const unsigned int byt_ = 3;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * byt_; }
		inline void operator++(int) { buf_ += byt_; }
		inline void operator--(int) { buf_ -= byt_; }
	};

	//Descr: RGBPlanar pixel traverser class (R | G | B)
	//T: Simple type of the channel
	template <typename T>
	class RGBPlanar {
	private:
		T * buf_r_;
		T * buf_g_;
		T * buf_b_;
		const unsigned int elm_ = 3;
	public:
		inline T * buf() { return (T*)buf_r_; }
		inline T * buf_r() const { return (T*)buf_r_; }
		inline T * buf_g() const { return (T*)buf_g_; }
		inline T * buf_b() const { return (T*)buf_b_; }
		inline T & r() const { return *buf_r_; }
		inline T & g() const { return *buf_g_; }
		inline T & b() const { return *buf_b_; }
		inline void buf(const void * buf, unsigned int w) { 
			unsigned char * b = (unsigned char *)buf;
			buf_r_ = (T*)b;
			buf_g_ = (T*)(b + w * sizeof(*buf_r_));
			buf_b_ = (T*)(b + 2*w * sizeof(*buf_r_));
		}
		inline void buf_r(const void * buf) { buf_r_ = (T*)buf; }
		inline void buf_g(const void * buf) { buf_g_ = (T*)buf; }
		inline void buf_b(const void * buf) { buf_b_ = (T*)buf; }
		inline unsigned int byt() { return sizeof(*buf_r_) * elm_; }
		inline void operator++(int) { buf_r_++; buf_g_++; buf_b_++; }
		inline void operator--(int) { buf_r_--; buf_g_--; buf_b_--; }
	};

	//Descr: RGBPlanarV pixel traverser class (R ^ G ^ B)
	//T: Simple type of the channel
	template <typename T>
	class RGBPlanarV {
	private:
		T * buf_r_;
		T * buf_g_;
		T * buf_b_;
		const unsigned int elm_ = 3;
	public:
		inline T * buf() { return (T*)buf_r_; }
		inline T * buf_r() const { return (T*)buf_r_; }
		inline T * buf_g() const { return (T*)buf_g_; }
		inline T * buf_b() const { return (T*)buf_b_; }
		inline T & r() const { return *buf_r_; }
		inline T & g() const { return *buf_g_; }
		inline T & b() const { return *buf_b_; }
		inline void buf(const void * buf, unsigned int w, unsigned int h) { 
			unsigned char * b = (unsigned char *)buf;
			buf_r_ = (T*)b;
			buf_g_ = (T*)(b + w * h * sizeof(*buf_r_));
			buf_b_ = (T*)(b + 2 * w * h * sizeof(*buf_r_));
		}
		inline void buf_r(const void * buf) { buf_r_ = (T*)buf; }
		inline void buf_g(const void * buf) { buf_g_ = (T*)buf; }
		inline void buf_b(const void * buf) { buf_b_ = (T*)buf; }
		inline unsigned int byt() { return sizeof(*buf_r_) * elm_; }
		inline unsigned int elm() { return elm_; }
		inline void operator++(int) { buf_r_++; buf_g_++; buf_b_++; }
		inline void operator--(int) { buf_r_--; buf_g_--; buf_b_--; }
	};

	//Descr: BGR pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class BGR {
	private:
		T * buf_;
		const unsigned int byt_ = 3;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * byt_; }
		inline void operator++(int) { buf_ += byt_; }
		inline void operator--(int) { buf_ -= byt_; }
	};

	//Descr: Mono pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class Mono {
	private:
		T * buf_;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_); }
		inline void operator++(int) { buf_++; }
		inline void operator--(int) { buf_--; }
	};

	//Descr: Mono + Alpha pixel traverser class
	//T: Simple type of the channel
	template <typename T>
	class MonoA {
	private:
		T * buf_;
	public:
		inline T * buf() { return (T*)buf_; }
		inline void buf(const void * buf) { buf_ = (T*)buf; }
		template <typename TLay> inline TLay & buf() { return *(reinterpret_cast<TLay*>(buf_)); }
		template <typename TLay> inline const TLay & buf() const { return *(reinterpret_cast<TLay*>(buf_)); }
		inline unsigned int byt() { return sizeof(*buf_) * 2; }
		inline void operator++(int) { buf_+=byt(); }
		inline void operator--(int) { buf_+=byt(); }
	};

	//Conversions to Mono<>
	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, Mono<TTo> &to) {
		to.template buf<TTo>() = (TTo)from.template buf<TFrom>();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, Mono<TTo> &to) {
		double r = (double)from.template buf<LayoutRGB<TFrom>>().r;
		double g = (double)from.template buf<LayoutRGB<TFrom>>().g;
		double b = (double)from.template buf<LayoutRGB<TFrom>>().b;
		to.template buf<TTo>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, Mono<TTo> &to) {
		double r = (double)from.template buf<LayoutBGR<TFrom>>().r;
		double g = (double)from.template buf<LayoutBGR<TFrom>>().g;
		double b = (double)from.template buf<LayoutBGR<TFrom>>().b;
		to.template buf<TTo>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, Mono<TTo> &to) {
		double r = (double)from.template buf<LayoutRGBA<TFrom>>().r;
		double g = (double)from.template buf<LayoutRGBA<TFrom>>().g;
		double b = (double)from.template buf<LayoutRGBA<TFrom>>().b;
		to.template buf<TTo>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const ARGB<TFrom> &from, Mono<TTo> &to) {
		double r = (double)from.template buf<LayoutARGB<TFrom>>().r;
		double g = (double)from.template buf<LayoutARGB<TFrom>>().g;
		double b = (double)from.template buf<LayoutARGB<TFrom>>().b;
		to.template buf<TTo>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, Mono<TTo> &to) {
		to.template buf<TTo>() = (TTo)from.template buf<LayoutMonoA<TFrom>>().m;
	};

	//Conversions to MonoA<>
	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, MonoA<TTo> &to) {
		to.template buf<LayoutMonoA<TTo>>().m = (TTo)from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutMonoA<TTo>>().a = (TTo)from.template buf<LayoutMonoA<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, MonoA<TTo> &to) {
		double r = (double)from.template buf<LayoutRGB<TFrom>>().r;
		double g = (double)from.template buf<LayoutRGB<TFrom>>().g;
		double b = (double)from.template buf<LayoutRGB<TFrom>>().b;
		to.template buf<LayoutMonoA<TTo>>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, MonoA<TTo> &to) {
		double r = (double)from.template buf<LayoutBGR<TFrom>>().r;
		double g = (double)from.template buf<LayoutBGR<TFrom>>().g;
		double b = (double)from.template buf<LayoutBGR<TFrom>>().b;
		to.template buf<LayoutMonoA<TTo>>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, MonoA<TTo> &to) {
		double r = (double)from.template buf<LayoutRGBA<TFrom>>().r;
		double g = (double)from.template buf<LayoutRGBA<TFrom>>().g;
		double b = (double)from.template buf<LayoutRGBA<TFrom>>().b;
		to.template buf<LayoutMonoA<TTo>>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	template <typename TFrom, typename TTo> void assign(const ARGB<TFrom> &from, MonoA<TTo> &to) {
		double r = (double)from.template buf<LayoutARGB<TFrom>>().r;
		double g = (double)from.template buf<LayoutARGB<TFrom>>().g;
		double b = (double)from.template buf<LayoutARGB<TFrom>>().b;
		to.template buf<LayoutMonoA<TTo>>() = (TTo)(0.3 * r + 0.59 * g + 0.11 * b);
	};

	//Conversions to RGB<>
	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = (TTo)from.template buf<LayoutRGB<TFrom>>().r;
		to.template buf<LayoutRGB<TTo>>().g = (TTo)from.template buf<LayoutRGB<TFrom>>().g;
		to.template buf<LayoutRGB<TTo>>().b = (TTo)from.template buf<LayoutRGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutRGB<TTo>>().g = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutRGB<TTo>>().b = (TTo)from.template buf<TFrom>();
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = from.template buf<LayoutBGR<TFrom>>().r;
		to.template buf<LayoutRGB<TTo>>().g = from.template buf<LayoutBGR<TFrom>>().g;
		to.template buf<LayoutRGB<TTo>>().b = from.template buf<LayoutBGR<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const BGRA<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = from.template buf<LayoutBGRA<TFrom>>().r;
		to.template buf<LayoutRGB<TTo>>().g = from.template buf<LayoutBGRA<TFrom>>().g;
		to.template buf<LayoutRGB<TTo>>().b = from.template buf<LayoutBGRA<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const ARGB<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = from.template buf<LayoutARGB<TFrom>>().r;
		to.template buf<LayoutRGB<TTo>>().g = from.template buf<LayoutARGB<TFrom>>().g;
		to.template buf<LayoutRGB<TTo>>().b = from.template buf<LayoutARGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutRGB<TTo>>().g = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutRGB<TTo>>().b = from.template buf<LayoutMonoA<TFrom>>().m;
	};

	template <typename TFrom, typename TTo> void assign(const RGBPlanar<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = (TTo)from.r();
		to.template buf<LayoutRGB<TTo>>().g = (TTo)from.g();
		to.template buf<LayoutRGB<TTo>>().b = (TTo)from.b();
	};

	template <typename TFrom, typename TTo> void assign(const RGBPlanarV<TFrom> &from, RGB<TTo> &to) {
		to.template buf<LayoutRGB<TTo>>().r = (TTo)from.r();
		to.template buf<LayoutRGB<TTo>>().g = (TTo)from.g();
		to.template buf<LayoutRGB<TTo>>().b = (TTo)from.b();
	};

	//Conversions to BGR<>
	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = (TTo)from.template buf<LayoutBGR<TFrom>>().r;
		to.template buf<LayoutBGR<TTo>>().g = (TTo)from.template buf<LayoutBGR<TFrom>>().g;
		to.template buf<LayoutBGR<TTo>>().b = (TTo)from.template buf<LayoutBGR<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutBGR<TTo>>().g = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutBGR<TTo>>().b = (TTo)from.template buf<TFrom>();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = from.template buf<LayoutRGB<TFrom>>().r;
		to.template buf<LayoutBGR<TTo>>().g = from.template buf<LayoutRGB<TFrom>>().g;
		to.template buf<LayoutBGR<TTo>>().b = from.template buf<LayoutRGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const BGRA<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = from.template buf<LayoutBGRA<TFrom>>().r;
		to.template buf<LayoutBGR<TTo>>().g = from.template buf<LayoutBGRA<TFrom>>().g;
		to.template buf<LayoutBGR<TTo>>().b = from.template buf<LayoutBGRA<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const ARGB<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = from.template buf<LayoutARGB<TFrom>>().r;
		to.template buf<LayoutBGR<TTo>>().g = from.template buf<LayoutARGB<TFrom>>().g;
		to.template buf<LayoutBGR<TTo>>().b = from.template buf<LayoutARGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = from.template buf<LayoutRGBA<TFrom>>().r;
		to.template buf<LayoutBGR<TTo>>().g = from.template buf<LayoutRGBA<TFrom>>().g;
		to.template buf<LayoutBGR<TTo>>().b = from.template buf<LayoutRGBA<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, BGR<TTo> &to) {
		to.template buf<LayoutBGR<TTo>>().r = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutBGR<TTo>>().g = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutBGR<TTo>>().b = from.template buf<LayoutMonoA<TFrom>>().m;
	};

	//Conversions to BGRA
	template <typename TFrom, typename TTo> void assign(BGRA<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = (TTo)from.template buf<LayoutBGRA<TFrom>>().r;
		to.template buf<LayoutBGRA<TTo>>().g = (TTo)from.template buf<LayoutBGRA<TFrom>>().g;
		to.template buf<LayoutBGRA<TTo>>().b = (TTo)from.template buf<LayoutBGRA<TFrom>>().b;
		to.template buf<LayoutBGRA<TTo>>().a = (TTo)from.template buf<LayoutBGRA<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutBGRA<TTo>>().g = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutBGRA<TTo>>().b = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutBGRA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = from.template buf<LayoutRGB<TFrom>>().r;
		to.template buf<LayoutBGRA<TTo>>().g = from.template buf<LayoutRGB<TFrom>>().g;
		to.template buf<LayoutBGRA<TTo>>().b = from.template buf<LayoutRGB<TFrom>>().b;
		to.template buf<LayoutBGRA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = from.template buf<LayoutBGR<TFrom>>().r;
		to.template buf<LayoutBGRA<TTo>>().g = from.template buf<LayoutBGR<TFrom>>().g;
		to.template buf<LayoutBGRA<TTo>>().b = from.template buf<LayoutBGR<TFrom>>().b;
		to.template buf<LayoutBGRA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const ARGB<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = from.template buf<LayoutARGB<TFrom>>().r;
		to.template buf<LayoutBGRA<TTo>>().g = from.template buf<LayoutARGB<TFrom>>().g;
		to.template buf<LayoutBGRA<TTo>>().b = from.template buf<LayoutARGB<TFrom>>().b;
		to.template buf<LayoutBGRA<TTo>>().a = from.template buf<LayoutARGB<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = from.template buf<LayoutRGBA<TFrom>>().r;
		to.template buf<LayoutBGRA<TTo>>().g = from.template buf<LayoutRGBA<TFrom>>().g;
		to.template buf<LayoutBGRA<TTo>>().b = from.template buf<LayoutRGBA<TFrom>>().b;
		to.template buf<LayoutBGRA<TTo>>().a = from.template buf<LayoutRGBA<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, BGRA<TTo> &to) {
		to.template buf<LayoutBGRA<TTo>>().r = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutBGRA<TTo>>().g = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutBGRA<TTo>>().b = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutBGRA<TTo>>().a = from.template buf<LayoutMonoA<TFrom>>().a;
	};

	//Conversions to ARGB
	template <typename TFrom, typename TTo> void assign(ARGB<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = (TTo)from.template buf<LayoutARGB<TFrom>>().r;
		to.template buf<LayoutARGB<TTo>>().g = (TTo)from.template buf<LayoutARGB<TFrom>>().g;
		to.template buf<LayoutARGB<TTo>>().b = (TTo)from.template buf<LayoutARGB<TFrom>>().b;
		to.template buf<LayoutARGB<TTo>>().a = (TTo)from.template buf<LayoutARGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutARGB<TTo>>().g = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutARGB<TTo>>().b = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutARGB<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = from.template buf<LayoutRGB<TFrom>>().r;
		to.template buf<LayoutARGB<TTo>>().g = from.template buf<LayoutRGB<TFrom>>().g;
		to.template buf<LayoutARGB<TTo>>().b = from.template buf<LayoutRGB<TFrom>>().b;
		to.template buf<LayoutARGB<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = from.template buf<LayoutBGR<TFrom>>().r;
		to.template buf<LayoutARGB<TTo>>().g = from.template buf<LayoutBGR<TFrom>>().g;
		to.template buf<LayoutARGB<TTo>>().b = from.template buf<LayoutBGR<TFrom>>().b;
		to.template buf<LayoutARGB<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = from.template buf<LayoutRGBA<TFrom>>().r;
		to.template buf<LayoutARGB<TTo>>().g = from.template buf<LayoutRGBA<TFrom>>().g;
		to.template buf<LayoutARGB<TTo>>().b = from.template buf<LayoutRGBA<TFrom>>().b;
		to.template buf<LayoutARGB<TTo>>().a = from.template buf<LayoutRGBA<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, ARGB<TTo> &to) {
		to.template buf<LayoutARGB<TTo>>().r = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutARGB<TTo>>().g = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutARGB<TTo>>().b = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutARGB<TTo>>().a = from.template buf<LayoutMonoA<TFrom>>().a;
	};

	//Conversions to RGBA
	template <typename TFrom, typename TTo> void assign(RGBA<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = (TTo)from.template buf<LayoutRGBA<TFrom>>().r;
		to.template buf<LayoutRGBA<TTo>>().g = (TTo)from.template buf<LayoutRGBA<TFrom>>().g;
		to.template buf<LayoutRGBA<TTo>>().b = (TTo)from.template buf<LayoutRGBA<TFrom>>().b;
		to.template buf<LayoutRGBA<TTo>>().a = (TTo)from.template buf<LayoutRGBA<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutRGBA<TTo>>().g = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutRGBA<TTo>>().b = (TTo)from.template buf<TFrom>();
		to.template buf<LayoutRGBA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = from.template buf<LayoutRGB<TFrom>>().r;
		to.template buf<LayoutRGBA<TTo>>().g = from.template buf<LayoutRGB<TFrom>>().g;
		to.template buf<LayoutRGBA<TTo>>().b = from.template buf<LayoutRGB<TFrom>>().b;
		to.template buf<LayoutRGBA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = from.template buf<LayoutBGR<TFrom>>().r;
		to.template buf<LayoutRGBA<TTo>>().g = from.template buf<LayoutBGR<TFrom>>().g;
		to.template buf<LayoutRGBA<TTo>>().b = from.template buf<LayoutBGR<TFrom>>().b;
		to.template buf<LayoutRGBA<TTo>>().a = std::numeric_limits<TTo>::max();
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = from.template buf<LayoutARGB<TFrom>>().r;
		to.template buf<LayoutRGBA<TTo>>().g = from.template buf<LayoutARGB<TFrom>>().g;
		to.template buf<LayoutRGBA<TTo>>().b = from.template buf<LayoutARGB<TFrom>>().b;
		to.template buf<LayoutRGBA<TTo>>().a = from.template buf<LayoutARGB<TFrom>>().a;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, RGBA<TTo> &to) {
		to.template buf<LayoutRGBA<TTo>>().r = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutRGBA<TTo>>().g = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutRGBA<TTo>>().b = from.template buf<LayoutMonoA<TFrom>>().m;
		to.template buf<LayoutRGBA<TTo>>().a = from.template buf<LayoutMonoA<TFrom>>().a;
	};

	//Conversions to RGBPlanar
	template <typename TFrom, typename TTo> void assign(const RGBPlanar<TTo> &from, RGBPlanar<TTo> &to) {
		to.r() = (TTo)from.r();
		to.g() = (TTo)from.g();
		to.b() = (TTo)from.b();
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, RGBPlanar<TTo> &to) {
		to.r() = (TTo)from.template buf<TFrom>();
		to.g() = (TTo)from.template buf<TFrom>();
		to.b() = (TTo)from.template buf<TFrom>();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, RGBPlanar<TTo> &to) {
		to.r() = from.template buf<LayoutRGB<TFrom>>().r;
		to.g() = from.template buf<LayoutRGB<TFrom>>().g;
		to.b() = from.template buf<LayoutRGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, RGBPlanar<TTo> &to) {
		to.r() = from.template buf<LayoutBGR<TFrom>>().r;
		to.g() = from.template buf<LayoutBGR<TFrom>>().g;
		to.b() = from.template buf<LayoutBGR<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, RGBPlanar<TTo> &to) {
		to.r() = from.template buf<LayoutARGB<TFrom>>().r;
		to.g() = from.template buf<LayoutARGB<TFrom>>().g;
		to.b() = from.template buf<LayoutARGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, RGBPlanar<TTo> &to) {
		to.r() = from.template buf<LayoutMonoA<TFrom>>().m;
		to.g() = from.template buf<LayoutMonoA<TFrom>>().m;
		to.b() = from.template buf<LayoutMonoA<TFrom>>().m;
	};

	//Conversions to RGBPlanarV
	template <typename TFrom, typename TTo> void assign(const RGBPlanar<TTo> &from, RGBPlanarV<TTo> &to) {
		to.r() = (TTo)from.r();
		to.g() = (TTo)from.g();
		to.b() = (TTo)from.b();
	};

	template <typename TFrom, typename TTo> void assign(const Mono<TFrom> &from, RGBPlanarV<TTo> &to) {
		to.r() = (TTo)from.template buf<TFrom>();
		to.g() = (TTo)from.template buf<TFrom>();
		to.b() = (TTo)from.template buf<TFrom>();
	};

	template <typename TFrom, typename TTo> void assign(const RGB<TFrom> &from, RGBPlanarV<TTo> &to) {
		to.r() = from.template buf<LayoutRGB<TFrom>>().r;
		to.g() = from.template buf<LayoutRGB<TFrom>>().g;
		to.b() = from.template buf<LayoutRGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const BGR<TFrom> &from, RGBPlanarV<TTo> &to) {
		to.r() = from.template buf<LayoutBGR<TFrom>>().r;
		to.g() = from.template buf<LayoutBGR<TFrom>>().g;
		to.b() = from.template buf<LayoutBGR<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const RGBA<TFrom> &from, RGBPlanarV<TTo> &to) {
		to.r() = from.template buf<LayoutARGB<TFrom>>().r;
		to.g() = from.template buf<LayoutARGB<TFrom>>().g;
		to.b() = from.template buf<LayoutARGB<TFrom>>().b;
	};

	template <typename TFrom, typename TTo> void assign(const MonoA<TFrom> &from, RGBPlanarV<TTo> &to) {
		to.r() = from.template buf<LayoutMonoA<TFrom>>().m;
		to.g() = from.template buf<LayoutMonoA<TFrom>>().m;
		to.b() = from.template buf<LayoutMonoA<TFrom>>().m;
	};

	//Descr: Traverse the pixels in row major continuous order
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderContinuous {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
	public:				
		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderContinuous &operator()(std::shared_ptr<TBuf> &buf) { buf = buf_;return *this; } //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() {px_.buf((void *)buf_.get());}; //Reset cursor position
		inline void operator++(int) {px_++;}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderContinuous(unsigned int h, unsigned int w, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_*w_*px_.byt()], OrderContinuous::del);
			rst();
		};
	};

	//Descr: Traverse the pixels in row major planar order
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderPlanar {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
	public:
		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderPlanar &operator()(std::shared_ptr<TBuf> &buf) { buf = buf_; return *this; } //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() { px_.buf((void*)buf_.get(), w_); x_ = 0; y_ = 0; }; //Reset cursor position
		
		inline void operator++(int) {
			if (x_ < w_ - 1) {
				x_++;
				px_++;
			} else {
				x_ = 0;
				y_++;
				unsigned char * b = (unsigned char *)buf_.get();
				b += y_ * w_ * px_.byt();
				px_.buf((void*)b, w_);
			}
		}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderPlanar(unsigned int h, unsigned int w, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_*w_*px_.byt()], OrderPlanar::del);
			rst();
		};
	};

	//Descr: Traverse the pixels in row major planar vertical order
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderPlanarV {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
	public:
		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderPlanarV &operator()(std::shared_ptr<TBuf> &buf) { buf = buf_; return *this; } //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() { px_.buf((void*)buf_.get(), w_, h_); x_ = 0; y_ = 0; }; //Reset cursor position
		
		inline void operator++(int) {
			if (x_ < w_ - 1) {
				x_++;
				px_++;
			} else {
				x_ = 0;
				y_++;
				unsigned char * b = (unsigned char *)buf_.get();
				b += y_ * w_ * (px_.byt() / px_.elm());
				px_.buf((void*)b, w_, h_);
			}
		}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderPlanarV(unsigned int h, unsigned int w, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_*w_*px_.byt()], OrderPlanarV::del);
			rst();
		};
	};

	//Descr: Traverse the pixels in row major array order (for png)
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderArrayOfArray {
	private:
		TPx px_;
		std::shared_ptr<TBuf*> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
	public:
		inline TPx &px() {return px_;} //Get current pixels
		inline unsigned int operator~() {return w_*h_; } //Count pixels
		inline OrderArrayOfArray &operator()(std::shared_ptr<TBuf*> &buf) { buf = buf_;return *this; } //Get the i/o buffer
		inline std::shared_ptr<TBuf*> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() { px_.buf((void *)buf_.get()[0]);x_=0;y_=0;}; //Reset cursor position
		inline void operator++(int) {
			if (x_ < w_-1) {
				x_++;
				px_++;
			} else {
				x_ = 0;
				y_++;
				px_.buf((void*)buf_.get()[y_]);
			}
		}; //Go to next pixel

		static void del(TBuf ** buf, unsigned int h) {
			for (unsigned int i=0;i<h;i++)
				delete[] buf[i];
			delete[] buf;
		}; //static deleter for shared pointer 

		OrderArrayOfArray(unsigned int h, unsigned int w, std::shared_ptr<TBuf *> buf = nullptr) : w_(w), h_(h), buf_(buf) {
			if (buf_ == nullptr) {
				TBuf* * bf;
				bf = new TBuf*[h_];
				for (unsigned int i=0;i<h_;i++)
					bf[i] = (TBuf*) new unsigned char[w_*px_.byt()];
				buf_.reset(bf, [=](TBuf** buf){OrderArrayOfArray::del(buf, h_);});
			}
			rst();
		};
	};

	//Descr: Traverse the pixels in row major array order upside down (for bmp,dib)
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderFlipped {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
	public:
		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderFlipped &operator()(std::shared_ptr<TBuf> &buf) { 
			buf = buf_;
			return *this;
		} //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() {
			x_=0;
			y_=h_-1;
			unsigned char *b = buf_.get();
			px_.buf((void *) &(b[y_ * w_ * px_.byt()]));
		}; //Reset cursor position
		inline void operator++(int) {
			if (x_ < w_ - 1) {
				x_++;
				px_++;
			} else {
				x_ = 0;
				y_--;
				unsigned char *b = buf_.get();
				px_.buf((void *) &(b[y_ * w_ * px_.byt()]));
			}
		}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderFlipped(unsigned int h, unsigned int w, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_*w_*px_.byt()], OrderFlipped::del);
			rst();
		};
	};

	//Descr:	Traverse the pixels in row major array with padding (for bmp,dib)
	//			(w * bytesPerPixel) % pad bytes is added to each line
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderPadded {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
		unsigned int pad_;
	public:
		unsigned int padding() { return ( pad_ - ((w_ * px_.byt()) % pad_));};
		unsigned int bytesPerLine() {return (w_ + padding()) * px_.byt(); };

		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderPadded &operator()(std::shared_ptr<TBuf> &buf) {
			buf = buf_;
			return *this;
		} //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() {
			x_ = 0;
			y_ = 0;
			unsigned char *b = buf_.get();
			px_.buf((void *) &(b[y_ * bytesPerLine()]));
		}; //Reset cursor position
		inline void operator++(int) {
			if (x_ < w_ - 1) {
				x_++;
				px_++;
			} else {
				x_ = 0;
				y_++;
				unsigned char *b = buf_.get();
				px_.buf((void *) &(b[y_ * bytesPerLine()]));
			}
		}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderPadded(unsigned int h, unsigned int w, unsigned int pad, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), pad_(pad), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_ * bytesPerLine()], OrderPadded::del);
			rst();
		};
	};

	//Descr:	Traverse the pixels in row major array with padding and flipping (for bmp,dib)
	//			(w * bytesPerPixel) % pad bytes is added to each line
	//TPx: The class type of the pixel (RGB, BGRA, etc)
	//TBuf: The simple type of the buffer for input/output (can be void)
	template <typename TPx, typename TBuf>
	class OrderPaddedFlipped {
	private:
		TPx px_;
		std::shared_ptr<TBuf> buf_;
		unsigned int h_;
		unsigned int w_;
		unsigned int x_;
		unsigned int y_;
		unsigned int pad_;
	public:
		unsigned int padding() { return (pad_ - ((w_ * px_.byt()) % pad_)); };
		unsigned int bytesPerLine() { return (w_ + padding()) * px_.byt(); };

		inline TPx &px() { return px_; } //Get current pixels
		inline unsigned int operator~() { return w_*h_; } //Count pixels
		inline OrderPaddedFlipped &operator()(std::shared_ptr<TBuf> &buf) {
			buf = buf_;
			return *this;
		} //Get the i/o buffer
		inline std::shared_ptr<TBuf> operator()() { return buf_; } //Get the i/o buffer		
		inline void rst() {
			x_ = 0;
			y_ = h_-1;
			unsigned char *b = buf_.get();
			px_.buf((void *) &(b[y_ * bytesPerLine()]));
		}; //Reset cursor position
		inline void operator++(int) {
			if (x_ < w_ - 1) {
				x_++;
				px_++;
			}
			else {
				x_ = 0;
				y_--;
				unsigned char *b = buf_.get();
				px_.buf((void *) &(b[y_ * bytesPerLine()]));
			}
		}; //Go to next pixel

		static void del(TBuf * buf) { delete[] buf; }; //static deleter for shared pointer 

		OrderPaddedFlipped(unsigned int h, unsigned int w, unsigned int pad, std::shared_ptr<TBuf> buf = nullptr) : w_(w), h_(h), pad_(pad), buf_(buf) {
			if (buf_ == nullptr)
				buf_.reset((TBuf*) new unsigned char[h_ * bytesPerLine()], OrderPaddedFlipped::del);
			rst();
		};
	};

	//Main function to convert image formats (Could also take temps)
	template <class TFrom, class TTo>
	inline bool convertBuffer(TFrom &&from, TTo &&to) {
		if (~from != ~to)
			return false;
		from.rst();
		to.rst();
		for (unsigned int i = 0;i<~from;i++, from++, to++)
			assign(from.px(), to.px());
		return true;
	};

	/** General sliding window methods **/
	template <typename T, typename TIdx>
	T aggrMean(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T sum = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval;
			}
		}
		return sum / (windowWidth * windowHeight);
	}

	template <typename T, typename TIdx>
	T aggrSum(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T sum = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval;
			}
		}
		return sum;
	}

	template <typename T, typename TIdx>
	T aggrMin(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T min = *pixelPtr;
		for (int winy = -(int)windowOriginY;winy < windowHeight - windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < windowWidth - windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				if (pxval < min)
					min = pxval;
			}
		}
		return min;
	}

	template <typename T, typename TIdx>
	T aggrConvolution(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX, const DSLib::Matrix<T, TIdx> &mask) {
		T sum = 0;
		int masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++,masky++) {
			int maskx = 0;
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++,maskx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval * mask.val(masky, maskx);
			}
		}
		return sum;
	}

	template <typename T, typename TIdx>
	T aggrSSIM(const T * const pixelPtr1, const T * const pixelPtr2, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY1, const TIdx imgStrideX1, const TIdx imgStrideY2, const TIdx imgStrideX2, const DSLib::Matrix<T, TIdx> &mask, const T C1, const T C2, const T C3) {
		//mean
		T mean1 = 0;
		T mean2 = 0;
		int masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++,masky++) {
			int maskx = 0;
			const int pre1 = (winy * imgStrideY1);
			const int pre2 = (winy * imgStrideY2);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++,maskx++) {				
				const T pxval1 = *(pixelPtr1 + winx + pre1);
				const T pxval2 = *(pixelPtr2 + winx + pre2);
				const T w = mask.val(masky, maskx);
				mean1 += pxval1 * w;				
				mean2 += pxval2 * w;
			}
		}
		
		//var and cov
		T var1 = 0;
		T var2 = 0;
		T cov = 0;
		masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++,masky++) {
			int maskx = 0;
			const int pre1 = (winy * imgStrideY1);
			const int pre2 = (winy * imgStrideY2);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++,maskx++) {
				const T pxval1 = *(pixelPtr1 + winx + pre1);
				const T pxval2 = *(pixelPtr2 + winx + pre2);
				const T w = mask.val(masky, maskx);
				var1 += (pxval1-mean1) * (pxval1-mean1) * w;
				var2 += (pxval2-mean2) * (pxval2-mean2) * w;
				cov += (pxval1-mean1) * (pxval2-mean2) * w;
			}
		}
		//const T sd1 = sqrt(var1);
		//const T sd2 = sqrt(var2);

		//ssim
		const T two = (T)2;
		const T mean1sq = mean1 * mean1;
		const T mean2sq = mean2 * mean2;
		const T num = (two * mean1 * mean2  + C1) * (two * cov + C2);
		const T divi = (mean1sq + mean2sq + C1) * (var1 + var1 + C2);
		return (num/divi);
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMean(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrMean(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrSum(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrSum(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T, typename TIdx>
	void drawGaussian(DSLib::Matrix<T, TIdx> &mat, const TIdx meanY, const TIdx meanX, const T sigma) {		
		//http://mathworld.wolfram.com/GaussianFunction.html
		T scale = T(1)/(T(2)*T(C_PI)*sigma*sigma);
		T mul = T(1)/(T(2)*sigma*sigma);
		T mnx = (T)meanX;
		T mny = (T)meanY;
		for (TIdx y=0;y<mat.rows.count();y++) {
			for (TIdx x=0;x<mat.cols.count();x++) {
				T parm = (x-mnx)*(x-mnx) + (y-mny)*(y-mny);
				mat.val(y, x) = scale * exp(-parm * mul);
			}
		}
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrConvolution(const DSLib::Matrix<T, TIdx> &mask, const TIdx windowOriginY, const TIdx windowOriginX) {
		TIdx windowHeight = mask.rows.count();
		TIdx windowWidth = mask.cols.count();
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrConvolution(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX, mask);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> makeAggrSSIM(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma, const T C1, const T C2, const T C3) {
		DSLib::Matrix<T, TIdx> mask(windowHeight, windowWidth);
		drawGaussian(mask, windowHeight/2, windowWidth/2, sigma);
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask, C1, C2, C3](const T * const p1, const TIdx imgStrideY1, const TIdx imgStrideX1, const T * const p2, const TIdx imgStrideY2, const TIdx imgStrideX2) {
			return aggrSSIM(p1, p2, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY1, imgStrideX1, imgStrideY2, imgStrideX2, mask, C1, C2, C3);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrGauss(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma) {
		DSLib::Matrix<T, TIdx> mask(windowHeight, windowWidth);
		drawGaussian(mask, windowHeight/2, windowWidth/2, sigma);
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrConvolution(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX, mask);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMin(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrMin(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T>
	std::function<T(const T)> makeFuncQuadratic(const T a, const T b, const T c) {
		return [a, b, c](const T x) {
			return a * x * x + b * x + c;
		};
	}

	template <typename T>
	std::function<T(const T)> makeFuncLinear(const T a, const T b) {
		return [a, b](const T x) {
			return a * x + b;
		};
	}

	template <typename T>
	T getSlideArea(const T height, const T width, const T windowHeight, const T windowWidth) {
		return (height * width) - (windowHeight * width) - (windowWidth * height) + (windowWidth * windowHeight);
	}

	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(DSLib::Matrix<T, TIdx> &mat, std::function<T(const T)> func) {
		const int startx = windowOriginX;
		const int imgw = mat.cols.count();
		const int endx = mat.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = mat.rows.count() - (windowHeight - windowOriginY);
		const int stridey = mat.getDataStride();
		T * const ptr = mat.getData();
		T sum = 0;

//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int pre = y * stridey;
			for (int x = startx;x<endx;x++) {
				ptr[pre + x] = func(ptr[pre + x]);
				sum += ptr[pre + x];
			}
		}
		return sum / getSlideArea(mat.rows.count(), mat.cols.count(), windowHeight, windowWidth);
	}

	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &src, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func) {
		if (src.order() != DSTypes::oRowMajor || dst.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for either source or destination");
		if (!dst.isSameSize(src))
			dst.resize(src.rows.count(), src.cols.count());

		const int startx = windowOriginX;
		const int imgw = src.cols.count();
		const int endx = src.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = src.rows.count() - (windowHeight - windowOriginY);
		//const int dststridex = 1;
		const int dststridey = dst.getDataStride();
		const int srcstridex = 1;
		const int srcstridey = src.getDataStride();

		const T * const srcPtr = src.getData();
		T * const dstPtr = dst.getData();
		T sum = 0;

//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int presrc = y * srcstridey;
			const int predst = y * dststridey;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtr = srcPtr + presrc + x;
				T val = aggr(pixelPtr, srcstridey, srcstridex);
				dstPtr[predst + x] = func(val);
				sum += dstPtr[predst + x];
			}
		}
		return sum / getSlideArea(src.rows.count(), src.cols.count(), windowHeight, windowWidth);
	}

	//Returns mean response
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &a, const DSLib::Matrix<T, TIdx> &b, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func) {
		if (a.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide2", "Invalid order for either a");
		if (b.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide2", "Invalid order for either b");
		if (dst.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide2", "Invalid order for either dst");
		if (!a.isSameSize(b))
			throw DSTypes::Error(DSTypes::ecParameter, "slide2", SS("a and b have different sizes " << a.printSize() << " != " << b.printSize()));
		if (!dst.isSameSize(a))
			dst.resize(a.rows.count(), a.cols.count());
		
		const int startx = windowOriginX;
		const int imgw = a.cols.count();
		const int endx = a.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = a.rows.count() - (windowHeight - windowOriginY);
		//const int dststridex = 1;
		const int dststridey = dst.getDataStride();
		const int astridex = 1;
		const int astridey = b.getDataStride();
		const int bstridex = 1;
		const int bstridey = b.getDataStride();

		const T * const aPtr = a.getData();
		const T * const bPtr = b.getData();
		T * const dstPtr = dst.getData();
		T sum = 0;

//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int prea = y * astridey;
			const int preb = y * bstridey;
			const int predst = y * dststridey;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtra = aPtr + prea + x;
				const T * pixelPtrb = bPtr + preb + x;
				T val = aggr(pixelPtra, astridey, astridex, pixelPtrb, bstridey, bstridex);
				dstPtr[predst + x] = func(val);
				sum += dstPtr[predst + x];
			}
		}
		return sum / getSlideArea(a.rows.count(), a.cols.count(), windowHeight, windowWidth);
	}
}
