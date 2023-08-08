#ifndef CRX_TDPOINT_H_H_
#define CRX_TDPOINT_H_H_

#include <sstream>


namespace crx {

class TPoint;

class TDPoint {
	public:
	TDPoint() = default;
	TDPoint(const TPoint&);
	TDPoint(double x1, double y1);

	bool operator==(const TDPoint& p) const;
	bool operator<(const TDPoint& p) const;
	TDPoint operator+(const TDPoint& p) const;
	TDPoint operator-(const TDPoint& p) const;
	void operator+=(const TDPoint& p);
	void operator-=(const TDPoint& p);

	void operator*=(double p);
	TDPoint operator/(double p);

	TDPoint& operator=(const TDPoint &point);

	public:
	double x = 0.0;
	double y = 0.0;
	
	inline double operator[](unsigned idx) const { return ((double*)this)[idx]; }
	inline double& operator[](unsigned idx) { return ((double*)this)[idx]; }
	
	public:
	void Set(double x1, double y1);
	void Set(const TPoint& p);
	void Set(const TDPoint& p);

	double Distance(const TDPoint& p) const;

	// возвращает квадрат дистанции
	double Distance2(const TDPoint& p) const;
	double Distance2(double x, double y) const;

	// поворацивает вектор на указанный угол
	void Rotate(double angle);

	// производит перенос системы координат. используется для прямого и обратного переноса (задается с помозью dir)
	void Translate(const TDPoint& p, double dir=-1);

	// возвращает cos между вектором вдоль оси OX и моим
	double DotProductOX() const;
	double Len() const;
	// находит уравнение  прямой, заданной точками this & linePoint, после чего
	// вычисляет координаты точки на этой прямой отстоящие от this на distance и
	// их возвращает
	TDPoint CalcPointAt(double distance, const TDPoint& linePoint) const;
	TDPoint CalcPointAt(double distance, double angle) const;

	double Dot(const TDPoint &p) const;
	void Normalize();
	double SqrLen() const;
	void Scale(const double scalar);
	double SqrDist(const TDPoint &p) const;
	void ScaleAndAdd(const TDPoint &p1, const TDPoint &p2, double scalar);

	bool IsZero() const;

	double Cross2D(const TDPoint &p1) const;

	// direction is optional; defaults to counter-clockwise
	void Perp2D(const bool clockwise = false);


	bool IsParallel(const TDPoint &p) const;


};

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TDPoint& rr);

#endif
