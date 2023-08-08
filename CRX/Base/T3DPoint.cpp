#include <math.h>
#include <CRX/Base/T3DPoint.h>
#include <CRX/Base/TMiscUtils.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>

namespace crx {

/*
T3DPoint::T3DPoint(double x1, double y1, double z1) {
	x = x1; y = y1; z = z1;
}
*/

T3DPoint T3DPoint::CalcRotation() const {
	T3DPoint rot;

	rot.x = atan2(z, y);
	rot.y = atan2(x, z);
	rot.z = atan2(y, x);

	return rot;
}

//static double sqr(double a) { return a*a; }

double T3DPoint::Distance(const T3DPoint &a) const {
	return sqrt( Sqr(a.x-x) + Sqr(a.y-y) + Sqr(a.z-z) );
}

double T3DPoint::DotProduct(const T3DPoint& other) const {
	return x * other.x + y * other.y + z * other.z;
}


std::ostream& operator<<(std::ostream& st, const crx::T3DPoint& rr) { return crx::OperatorToOS(st, rr); }
std::istream& operator>>(std::istream& is, crx::T3DPoint& dt) { return crx::OperatorFromIS(is, dt); }




double T3DPoint::Length() const { return sqrt( x*x + y*y + z*z); }

}//crx





