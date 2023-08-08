#ifndef CRX_BASE_TMISCUTILS_H_
#define CRX_BASE_TMISCUTILS_H_

#include <cstddef>
#include <CRX/Base/T3DPoint.h>
#include <CRX/Base/T3DIPoint.h>
#include <typeinfo>
#include <string>
#include <any>
#include <CRX/Base/TDPoint.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TFixPoint.h>
#include <CRX/Base/TDateTime.h>

namespace crx {
	class TPoint;

	uint64_t MicroTime();
	uint64_t MilliTime();
	uint64_t Time();
	void MicroSleep(uint64_t microSecs);
	void MilliSleep(uint64_t milliSecs);

	void OpenLinkInDefaultBrowser(const std::string& url);

	bool IsNear(double a, double d, double eps=10e-4);
	double NormalizeAngle(double angl);//return in range [0;2PI);
	double AnglesDelta(double a, double b);

	crx::TFixPoint NormalizeDegreeAngle(crx::TFixPoint in);//from 0 to 360

	//angle1-target angle, angle2-current angle
	//return min target angle (with considering dir) 
	double MinAngleWithDir(double angle1, double angle2);

	bool IsAngelsNear(double a, double b, double eps=0.01);

	double AngleBetween(const TDPoint& src, double srcRotateAngle, const TDPoint& dst);
	double AngleBetweenPlaneAndStraight(const T3DPoint& planeABC, const T3DPoint& straight);

	double RadToGrad(double rad);
	double GradToRad(double grad);

	//void RotatePoint2D(float &pointX, float &pointY, float angle, float centerPointX, float centerPointY);
	template<typename T>
	void RotatePoint2D(T &pointX, T &pointY, float angle, float centerPointX, float centerPointY) {
		float s = sin(angle);
		float c = cos(angle);

		pointX -= T(centerPointX);
		pointY -= T(centerPointY);

		float xnew = float(pointX) * c - float(pointY) * s;
		float ynew = float(pointX) * s + float(pointY) * c;

		pointX = T(xnew + centerPointX);
		pointY = T(ynew + centerPointY);
	}

	template<template <typename> typename ContainerType, typename KeyType, typename ValType>
	ContainerType<KeyType> MapKeysToVector(const std::map<KeyType, ValType> &map) {
		ContainerType<KeyType> result;

		for(auto mapVal : map) {
			result.insert(result.end(), mapVal.first);
		}

		return result;
	}

	double Sqr(double a);

	// type info
	std::string ConvertTypeName(const char* n, bool fromPtr=false);

	template<class T>
	std::string GetTypeNameFromPtr(T* t) { return ConvertTypeName( typeid(t).name(), true ); }

	template<class T>
	std::string GetTypeName(T& t) { return ConvertTypeName( typeid(t).name() ); }

	template<class T>
	std::string GetTypeName() { return ConvertTypeName( typeid(T).name() ); }

	std::string GetTypeNameFromAny(const std::any& data);

	std::string RemoveNamespaceFromTypeName(const std::string& tn);

	void ZLibCompress(const std::string& in, std::string& out, int compressionlevel = 9);//9 - best
	void ZLibDecompress(const std::string& in, std::string& out);


	// два разный typename, чтобы можно было передавать классы-потомки в obj
	template<class TT, class VV>
	void RemoveFromVector(std::vector<TT>& vv, const VV& obj) {
		auto it = std::find(vv.begin(), vv.end(), obj);
		if(it!=vv.end()) vv.erase(it);
	}
	
	template<class TT>
	void RemoveFromVectorByIdx(std::vector<TT>& vv, std::size_t idx) {
		if( idx<vv.size() ) vv.erase(vv.begin()+idx);
	}

	template<class TT, class VV>
	void AddToVector(std::vector<TT>& vv, const VV& obj) {
		auto it = std::find(vv.begin(), vv.end(), obj);
		if(it==vv.end()) vv.push_back(obj);
	}

	template<class TT, class VV>
	bool IsHaveInVector(const std::vector<TT>& vv, const VV& obj) {
		auto it = std::find(vv.begin(), vv.end(), obj);
		return it!=vv.end();
	}

	template<class TT>
	void DeleteVector(std::vector<TT*>& invv) {
		auto vv = invv;
		for(auto& a:vv) delete a;
		invv.clear();
	}


	template<class T>
	T sign(T v) {
		if(v==0) return 0;
		return v<0?-1:+1;
	}

	std::vector<std::vector<int>> GetAreaDirs(const crx::T3DIPoint &areaSize, int dimension=2);

	double FixPoint(double value, unsigned singsAfterPoint);

	template<class TT, class BB>
	std::vector<std::shared_ptr<TT>> ObjectsFromVectorByType(const std::vector<std::shared_ptr<BB>>& source) {
		std::vector<std::shared_ptr<TT>> vv;
		for(auto a:source) {
			auto n = std::dynamic_pointer_cast<TT>(a);
			if(n) vv.push_back(n);
		}
		return vv;
	}

	template<typename TO, typename FROM>
	bool IsDynamicPointerCastPossible(std::shared_ptr<FROM> &object) {
		return std::dynamic_pointer_cast<TO>(object) != nullptr;
	}

	// обертки над округлялками, т.к. нет iround или uround, только lround
	int iround(float a);
	int iround(double a);

	unsigned uround(float a);
	unsigned uround(double a);



	template<class T>
	std::vector<T> LoadVectorConfigFromString(const std::string & pp, const std::string& sep=";") {
		std::vector<T> vv;

		auto svv = crx::StrTok(pp, sep);
		for(auto& a:svv) {
			T t = crx::from_string<T>(a);
			vv.push_back(t);
		}//for

		return vv;
	}

	std::vector<std::string> LoadStringVectorFromStringCommaSep(const std::string & pp);

	// ------------------------------------ //
	// platform depended, implemented for linux & windows only
	std::string GetPathToExecutableFileOfCurrentProcess();

	// a.exe in root, for MSVC check, if a.exe in root or in MS build dir
	std::string GetIDEIndependedPathToRoot();

	std::string GetDefaultSaveRootPath();

	std::string GetIDEIndependedPathToVFSRoot(const std::string& rootFolder="Data");

	template<typename ContainerType, typename ObjectType>
	bool IsContain(const ContainerType &container, const ObjectType &value) {
		return (std::find(container.begin(), container.end(), value) != container.end());
	}


	std::string EpochTimeToString(time_t epochTime, std::string format = "%s.%s.%s %s:%s");
	TDateTime EpochTimeToDate(time_t epochTime);
}


#endif /* BASE_TMISCUTILS_H_ */
