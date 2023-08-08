#include <CRX/Base/TPoint.h>
#include <math.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TDPoint.h>

namespace crx {

TPoint::TPoint(int x1, int y1) { Set(x1, y1); }
void TPoint::Set(int x1, int y1) { x = x1; y = y1; }
bool TPoint::IsEqual(const TPoint& p) const { return x==p.x && y==p.y; }
int TPoint::Distance(const TPoint& p) const {
	int xx = p.x - x;
	int yy = p.y - y;
	return (int)sqrt( xx*xx + yy*yy );
}

bool TPoint::operator==(const TPoint& p) const { return IsEqual(p); }
bool TPoint::operator!=(const TPoint& p) const { return !IsEqual(p); }
bool TPoint::operator<(const TPoint& p) const { return x!=p.x?x<p.x:y<p.y; }

void TPoint::operator=(const TDPoint& p) { Set(p); }
TPoint::TPoint(const TDPoint& p) { Set(p); }

void TPoint::Set(const TDPoint& p) {
	x = int(0.5+p.x);
	y = int(0.5+p.y);
}

void TPoint::Set(const TPoint& p) {
	x = p.x;
	y = p.y;
}

TPoint TPoint::operator+(const TPoint& p) const { return TPoint(x+p.x, y+p.y); }
TPoint TPoint::operator-(const TPoint& p) const { return TPoint(x-p.x, y-p.y); }
void TPoint::operator+=(const TPoint& p) { x += p.x; y += p.y; }
void TPoint::operator*=(const TPoint& p) { x *= p.x; y *= p.y; }

std::string TPoint::ToString() const {
	return std::to_string(x)+std::string(";")+std::to_string(y);
}

void TPoint::FromString(const std::string &str) {
	auto vv = crx::StrTok(str, ";");
	if(vv.size()!=2) _ERROR("mismatch params. req 2, got: "<<vv.size());
	x = crx::to_int(vv[0]);
	y = crx::to_int(vv[1]);
}

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TPoint& rr) {
	st<<"x="<<rr.x<<" y="<<rr.y;
	return st;
}

