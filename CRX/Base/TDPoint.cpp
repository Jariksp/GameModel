#include <CRX/Base/TPoint.h>
#include <math.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TDPoint.h>


namespace crx {

static const auto EPS = 0.00001;

TDPoint::TDPoint(const TPoint& p) { Set(p); }
TDPoint::TDPoint(double x1, double y1) { Set(x1, y1); }
void TDPoint::Set(double x1, double y1) { x = x1; y = y1; }

void TDPoint::operator+=(const TDPoint& p) {
	x += p.x;
	y += p.y;
}

bool TDPoint::operator==(const TDPoint& p) const {
	return fabs(p.x-x)<EPS && fabs(p.y-y)<EPS;
}


double TDPoint::Distance(const TDPoint& p) const {
	auto xx = p.x - x;
	auto yy = p.y - y;
	return sqrt( xx*xx + yy*yy );
}

double TDPoint::Distance2(const TDPoint& p) const {
	auto xx = p.x - x;
	auto yy = p.y - y;
	return ( xx*xx + yy*yy );

}
double TDPoint::Distance2(double inx, double iny) const {
	inx -= x;
	iny -= y;
	return ( inx*inx + iny*iny );
}


void TDPoint::Set(const TPoint& p) { x = p.x; y = p.y; }
void TDPoint::Set(const TDPoint& p) { x = p.x; y = p.y; }

void TDPoint::Rotate(double a) {
	auto ca = cos(a);
	auto sa = sin(a);
	
	auto xh = x*ca - y*sa;
	auto yh = x*sa + y*ca;

	x = xh;
	y = yh;
}


void TDPoint::Translate(const TDPoint& p, double dir) {
	x += p.x*dir;
	y += p.y*dir;
}


double TDPoint::Len() const { return sqrt(x*x + y*y); }

double TDPoint::DotProductOX() const { return x/Len(); }

TDPoint TDPoint::CalcPointAt(double ds, const TDPoint& end) const {
	if( *this==end ) _ERROR("Can't CalcPointAt - points is equal");

	auto se_len = Distance(end);
	TDPoint ret;
	ret.x = x + ((end.x-x)/se_len)*ds;
	ret.y = y + ((end.y-y)/se_len)*ds;
	return ret;
}

TDPoint TDPoint::CalcPointAt(double distance, double angle) const {
	TDPoint ret;
	ret.x = x + cos(angle)*distance;
	ret.y = y + sin(angle)*distance;
	return ret;
}



bool TDPoint::operator<(const TDPoint &p) const {
	if(x != p.x) return x < p.x;
	return y < p.y;
}

TDPoint TDPoint::operator+(const TDPoint &p) const {
	TDPoint newPoint;
	newPoint.x = x + p.x;
	newPoint.y = y + p.y;
	return newPoint;
}

TDPoint TDPoint::operator-(const TDPoint &p) const {
	TDPoint newPoint;
	newPoint.x = x - p.x;
	newPoint.y = y - p.y;
	return newPoint;
}

void TDPoint::operator-=(const TDPoint &p) {
	x -= p.x;
	y -= p.y;
}

double TDPoint::Dot(const TDPoint &p) const {
	return (x * p.x + y * p.y);
}

void TDPoint::Normalize() {

	auto len = this->SqrLen();

	if (len > 0)
		len = 1/sqrt(len);

	x *= len;
	y *= len;
}

double TDPoint::SqrLen() const {
	return x * x + y * y;
}

void TDPoint::Scale(const double scalar) {
	x *= scalar;
	y *= scalar;
}

double TDPoint::SqrDist(const TDPoint &p) const {
	auto lx = p.x - x;
	auto ly = p.y - y;
	return lx*lx + ly*ly;

}

void TDPoint::ScaleAndAdd(const TDPoint &p1, const TDPoint &p2, double scalar) {
	x = p1.x + p2.x * scalar;
	y = p1.y + p2.y * scalar;
}

bool TDPoint::IsZero() const {
	return ((std::abs(x) + std::abs(y)) <= EPS);
}

double TDPoint::Cross2D(const TDPoint &p) const {
	return ((x * p.y) - (y * p.x));
}

void TDPoint::Perp2D(const bool clockwise) {
	TDPoint temp;
	if(clockwise) {
		temp.x = y;
		temp.y = -x;
	} else {
		temp.x = -y;
		temp.y = x;
	}
	*this = temp;
}

bool TDPoint::IsParallel(const TDPoint &p) const {
	return (std::abs(Cross2D(p)) <= EPS);
}

void TDPoint::operator *=(double p) {
	x *= p;
	y *= p;
}


TDPoint TDPoint::operator/(double p) {
	TDPoint res;

	res.x = x/p;
	res.y = y/p;

	return res;
}

TDPoint& TDPoint::operator=(const TDPoint &point) {
	x = point.x;
	y = point.y;
	return *this;
}

}//crx


std::ostream& operator<<(std::ostream& st, const crx::TDPoint& rr) {
	st<<"x="<<rr.x<<" y="<<rr.y;
	return st;
}



