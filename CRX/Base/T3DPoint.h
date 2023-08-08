#ifndef CRX_BASE__T3DPOINT_H_
#define CRX_BASE__T3DPOINT_H_

#include <sstream>
#include <CRX/Base/TBaseNumberPointClass.h>

// может содержать как координаты в 3D Space, так и углы поворота вокруг осей
// rot - в радианах, вокруг каждой оси. Т.е. rot.x - это угол поворота вокруг оси Х
// соответственно, угол в плоскости XY - это вращение вокруг оси Z и сохранен он будет в T3DPoint::z
// используется правосторонняя система координат

namespace crx {

class T3DPoint : public TBaseNumberPointClass<double, T3DPoint> {
	public:
	inline T3DPoint()=default;

	inline T3DPoint(double x1, double y1, double z1=0) {
		x = x1; y = y1; z = z1;
	}

	public:
	// возвращает углы поворота векторов относительно осей
	T3DPoint CalcRotation() const;

	double Distance(const T3DPoint& a) const;

	double Length() const;

	double DotProduct(const T3DPoint& other) const;
	public:
	//void operator*=(double d) { OperatorMulAssign(d); }
	//void operator+=(const T3DPoint& p) { OperatorAddAssign(p); }
	//void operator-=(const T3DPoint& p) { OperatorSubAssign(p); }

	//bool operator==(const T3DPoint& p) const { return OperatorEqual(p); }

	//T3DPoint operator+(const T3DPoint& p) const { return OperatorAdd(this, p); }
	//T3DPoint operator-(const T3DPoint& p) const { return OperatorSub(this, p); }


	//double& operator[](int idx) { return OperatorIndexGet(idx); }
	//const double& operator[](int idx) const { return OperatorIndexGet(idx); }

};

std::ostream& operator<<(std::ostream& st, const crx::T3DPoint& rr);
std::istream& operator>>(std::istream& is, crx::T3DPoint& dt);


}//crx



#endif /* BASE__T3DPOINT_H_ */
