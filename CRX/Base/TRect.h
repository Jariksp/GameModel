#ifndef CRX_TRECTH_H_
#define CRX_TRECTH_H_

#include <CRX/Base/TPoint.h>
#include <CRX/Base/TSize.h>
#include <sstream>


namespace crx {

class TRect {
	public:
	TRect() = default;
	TRect(int x, int y, int w, int h);
	TRect& operator&(const TRect&);
	void operator&=(const TRect&);

	static TRect Null;

	void Move(int x, int y);
	void Set(int x=0, int y=0, int w=0, int h=0);
	void Set(const TPoint& p);
	void Size(int w, int h);

	public:
	TSize Size() const;
	void Size(const TSize& s);
	TPoint Place() const;
	TPoint Origin() const;
	void Place(const TPoint& p);

	public:
	inline int X() const { return x; }
	inline int Y() const { return y; }
	inline int H() const { return h; }
	inline int W() const { return w; }
	inline int X2() const { return x+w; }
	inline int Y2() const { return y+h; }

	inline void X(int a) { x = a; }
	inline void Y(int a) { y = a; }
	inline void H(int a) { h = a; }
	inline void W(int a) { w = a; }
	inline void X2(int a) { w = a - x; }
	inline void Y2(int a) { h = a - y; }


	inline bool operator==(const TRect& r) const { return x==r.x && y==r.y && w==r.w && h==r.h; } 
	inline bool operator!=(const TRect& r) const { return !(operator==(r)); }
	inline bool IsEmpty() const { return (w<=0 || h<=0); }
	void Clear();
	bool IsInside(int x, int y) const;
	bool IsInside(const TRect&) const;
	bool IsIntersection(const TRect&) const;
	bool IsOutside(const TRect&) const;
	bool Clip(int& x, int&  y, TRect& sr) const;
	bool Clip(TRect& r) const;
	bool BltClip(TPoint& p, TRect& sr);

	TRect Intersection(const TRect&) const;
	TRect MaxRect(const TRect&) const;

	public:
	typedef int OutCode;
	static const int INSIDE = 0; // 0000
	static const int LEFT = 1;   // 0001
	static const int RIGHT = 2;  // 0010
	static const int BOTTOM = 4; // 0100
	static const int TOP = 8;    // 1000
	
	OutCode ComputeOutCode(int x, int y) const;
	bool ClipLine(TPoint& start, TPoint& end) const;

	public:
	enum {
		IndexX=0,
		IndexY=1,
		IndexW=2,
		IndexH=3,
		IndexMax=4
	};
	inline int& operator[](int idx) { return ((int*)this)[idx]; }
	inline const int& operator[](int idx) const { return ((int*)this)[idx]; }

	public:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	public:
	std::string ToString() const;
	void FromString(const std::string& data);
};

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TRect& rr);


#endif
