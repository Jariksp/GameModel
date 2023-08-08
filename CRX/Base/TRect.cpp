#include <CRX/Base/TPoint.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TRect.h>
#include <CRX/Base/TStringUtils.h>


namespace crx {

TRect TRect::Null(-65535, -65535, -65535, -65535);

TRect::TRect(int x1, int y1, int w1, int h1) { Set(x1, y1, w1, h1); }



TRect& TRect::operator&(const TRect& rr) {
	operator=( Intersection(rr) );
	return *this;
}

void TRect::operator&=(const TRect& tt) { operator&(tt); }

void TRect::Move(int x1, int y1) {
	x = x1;
	y = y1;
}

void TRect::Set(int x1, int y1, int w1, int h1) {
	x = x1;
	y = y1;
	w = w1;
	h = h1;
}

void TRect::Set(const TPoint& p) { x = p.x; y = p.y; }

void TRect::Size(int w1, int h1) { w = w1; h = h1; }

//bool TRect::operator==(const TRect& r) const { return x==r.x && y==r.y && w==r.w && h==r.h; }
//bool TRect::operator!=(const TRect& r) const { return !(operator==(r)); }
//bool TRect::IsEmpty() const { return (w<=0 || h<=0); }
void TRect::Clear() { Set(0, 0, 0, 0); }



bool TRect::IsInside(int xa, int ya) const {
	return ( xa>=x && xa<X2() && ya>=y && ya<Y2() );
}

bool TRect::IsIntersection(const TRect& r) const {
	return !( r.X2()<=x || r.Y2()<=y || r.x>=X2() || r.y>=Y2() );
}

bool TRect::IsInside(const TRect& r) const {
	return ( r.x>=x && r.y>=y && r.X2()<=X2() && r.Y2()<=Y2() );
}

bool TRect::IsOutside(const TRect& r) const {
	return ( r.X2()<=x || r.Y2()<=y || r.x>=X2() || r.y>=Y2() );
}


TRect TRect::Intersection(const TRect& rr) const {
	if( !IsIntersection(rr) ) return TRect();

	TRect sr = rr;
	
	//left && top side
	if(sr.x<x) { int a = sr.X2(); sr.x = x; sr.X2(a); }
	if(sr.y<y) { int a = sr.Y2(); sr.y = y; sr.Y2(a); }

	//right && bottom side
	if( sr.X2()>X2() ) { sr.X2( X2() ); }
	if( sr.Y2()>Y2() ) { sr.Y2( Y2() ); }
	
	return sr;	
}

TRect TRect::MaxRect(const TRect& rr) const {
	if( IsEmpty() ) return rr;
	if( rr.IsEmpty() ) return *this;
	
	TRect rect = rr;
	if(x<rr.x) { int a = rect.X2(); rect.x = x; rect.X2(a); }
	if(y<rr.y) { int a = rect.Y2(); rect.y = y; rect.Y2(a); }
	if( X2()>rr.X2() ) rect.X2( X2() );
	if( Y2()>rr.Y2() ) rect.Y2( Y2() );

	return rect;
}


bool TRect::Clip(int& x, int&  y, TRect& sr) const {
	TRect rr( x, y, sr.w, sr.h );
	rr = Intersection(rr);
	if( rr.IsEmpty() ) return false;
	int xa = rr.x - x;
	int ya = rr.y - y;
	x += xa;
	y += ya;
	sr.Set( sr.x+xa, sr.y+ya, rr.w, rr.h );
	return true;
}

bool TRect::Clip(TRect& sr) const {
	sr = Intersection(sr);
	if( sr.IsEmpty() ) return false;
	return true;
}


bool TRect::BltClip(TPoint& p, TRect& sr) {
	TRect rr(p.x, p.y, sr.w, sr.h);
	rr = Intersection(rr);
	if( rr.IsEmpty() ) return false;
	int xa = rr.x - p.x;
	int ya = rr.y - p.y;
	p.x += xa;
	p.y += ya;
	sr.Set( sr.x+xa, sr.y+ya, rr.w, rr.h );
	if(sr.w<=0 || sr.h<=0) return false;
	return true;

}


TRect::OutCode TRect::ComputeOutCode(int ix, int iy) const {
	OutCode code;
	code = INSIDE;

	if(ix < x) code |= LEFT; else if( ix > X2() ) code |= RIGHT;
	if(iy < y) code |= BOTTOM; else if ( iy > Y2() ) code |= TOP;

	return code;
}

bool TRect::ClipLine(TPoint& start, TPoint& end) const {
	double x0 = start.x;
	double y0 = start.y;
	double x1 = end.x;
	double y1 = end.y;

	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	TRect::OutCode outcode0 = ComputeOutCode( (int)x0, (int)y0 );
	TRect::OutCode outcode1 = ComputeOutCode( (int)x1, (int)y1 );

	bool accept = false;

	while (true) {
		if( !(outcode0 | outcode1) ) { accept = true; break; }
		if (outcode0 & outcode1) break;

		double xa, ya;

		TRect::OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

		if (outcodeOut & TRect::TOP) {           // point is above the clip rectangle
			xa = x0 + (x1 - x0) * (Y2() - y0) / (y1 - y0);
			ya = Y2();
		} else if (outcodeOut & TRect::BOTTOM) { // point is below the clip rectangle
			xa = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
			ya = y;
		} else if (outcodeOut & TRect::RIGHT) {  // point is to the right of clip rectangle
			ya = y0 + (y1 - y0) * (X2() - x0) / (x1 - x0);
			xa = X2();
		} else if (outcodeOut & TRect::LEFT) {   // point is to the left of clip rectangle
			ya = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
			xa = x;
		}

		if (outcodeOut == outcode0) {
			x0 = xa;
			y0 = ya;
			outcode0 = ComputeOutCode((int)x0, (int)y0);
		} else {
			x1 = xa;
			y1 = ya;
			outcode1 = ComputeOutCode((int)x1, (int)y1);
		}
	}//while

	start.x = (int)x0;
	start.y = (int)y0;

	end.x = (int)x1;
	end.y = (int)y1;

	return accept;
}

TSize TRect::Size() const { return TSize(w,h); }
void TRect::Size(const TSize& s) { w = s.w; h = s.h; }
TPoint TRect::Place() const { return TPoint(x,y); }
void TRect::Place(const TPoint& p) { x = p.x; y = p.y; }

TPoint TRect::Origin() const {
	return TPoint(x + w/2, y + h/2);
}

std::string TRect::ToString() const {
	std::string s;
	s = std::to_string(x)+std::string(";")+std::to_string(y)+std::string(";")+std::to_string(w)+std::string(";")+std::to_string(h);
	return s;
}

void TRect::FromString(const std::string& data) {
	auto vv = StrTok(data, ";");
	if(vv.size()!=4) _ERROR("Wrong string data count: "<<vv.size());
	x = to_int(vv[0]);
	y = to_int(vv[1]);
	w = to_int(vv[2]);
	h = to_int(vv[3]);
}

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TRect& rr) {
	st<<"x="<<rr.x<<" y="<<rr.y<<" w="<<rr.w<<" h="<<rr.h;
	return st;
}


