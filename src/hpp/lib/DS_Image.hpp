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
	
	/*
	float aggrMean(const float * const pixelPtr, const int radius, const int imgWidth) {
	float sum = 0;
	for (int winy=-radius;winy<=radius;winy++) {
		const int pre = (winy * imgWidth);
		for (int winx=-radius;winx<=radius;winx++) {
			const float pxval = *(pixelPtr + winx + pre);
			sum += pxval;
		}
	}
	return sum / ((2*radius+1)*(2*radius+1));
	}

	float aggrMin(const float * const pixelPtr, const int radius, const int imgWidth) {
		float min = *pixelPtr;
		for (int winy = -radius;winy <= radius;winy++) {
			const int pre = (winy * imgWidth);
			for (int winx = -radius;winx <= radius;winx++) {
				const float pxval = *(pixelPtr + winx + pre);
				if (pxval < min)
					min = pxval;
			}
		}
		return min;
	}


	function<float(const float)> makeModelQuadratic(const float a, const float b, const float c) {
		return [a,b,c](const float x) {
			return a * x * x + b * x + c;
		};
	}

	function<float(const float)> makeModelLinear(const float a, const float b) {
		return [a, b](const float x) {
			return a * x + b;
		};
	}

	template <typename T, const int radius>
	void model_tpl(const RealImage<T> &flowSrc, RealImage<T> &dst, function<T(const T * const, const int, const int)> aggr, function<T(const T)> model) {
		dst.Resize(HeightWidth(flowSrc.GetHeight(), flowSrc.GetWidth()));
		SetAllPixels(dst, 0.0f);

		const int startx = radius;
		const int imgw = flowSrc.GetWidth();
		const int endx = flowSrc.GetWidth() - radius;
		const int starty = radius;
		const int endy = flowSrc.GetHeight() - radius;
		
		const T * const flowSrcPtr = flowSrc.GetFirstPixelPtr();
		T * const dstPtr = dst.GetFirstPixelPtr();

		#pragma omp parallel for
		for (int y = starty;y<endy;y++) {
			const int pre = y * imgw;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtr = flowSrcPtr + pre + x;
				T val = aggr(pixelPtr, radius, imgw);
				dstPtr[pre + x] = model(val);
			}
		}
	}*/
}