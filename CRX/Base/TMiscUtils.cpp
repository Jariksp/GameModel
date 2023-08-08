#include <locale>
#include <codecvt>
#include <chrono>
//#include <zlib.h>
#include <thread>
#include <CRX/Base/TPoint.h>
#include <algorithm>

#include <CRX/Base/TMiscUtils.h>
#include <CRX/Base/TStringUtils.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <CRX/Base/TDebug.h>
#ifdef __GNUG__
#include <cxxabi.h>
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <Shlobj.h>
#else
#include <unistd.h>
#endif


namespace crx {


void OpenLinkInDefaultBrowser(const std::string &url) {
	std::string open = "xdg-open";
	#ifdef _MSC_VER
	open = "start";
	#else
	#if defined(__APPLE__) && defined(__MACH__)
	open = "open";
	#endif
	#endif

	open += std::string(" ") + url;

	system( open.c_str() );
}

uint64_t MicroTime() {
	auto tt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return tt;
}

uint64_t MilliTime() { 	
	auto tt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return tt; 
}

uint64_t Time() { 	
	auto tt = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return tt; 
}


bool IsNear(double a, double d, double eps) {  return fabs(a-d)<eps; }

double NormalizeAngle(double angl) {
	auto a = atan2( sin(angl), cos(angl) );
	//while(a < 0) a += 2 * M_PI;
	
	if(a<0) a += 2*M_PI;
	//if(a<0) return 0;
	if( IsNear(a, 2*M_PI) ) return 0;
	return a;
}

crx::TFixPoint NormalizeDegreeAngle(crx::TFixPoint in) {
	in *= 1000;
	int d = in.AsInt();
	d = d%360000;
	if(d<0) d += 360000;
	in = d;
	in /= 1000;
	return in;
}

double RadToGrad(double rad) { return (rad*180.0)/M_PI; }

double GradToRad(double grad) { return (grad*M_PI)/180.0; }

// void RotatePoint2D(float &pointX, float &pointY, float angle, float centerPointX, float centerPointY) {
// 	float s = sin(angle);
// 	float c = cos(angle);

// 	pointX -= centerPointX;
// 	pointY -= centerPointY;

// 	float xnew = pointX * c - pointY * s;
// 	float ynew = pointX * s + pointY * c;

// 	pointX = xnew + centerPointX;
// 	pointY = ynew + centerPointY;
// }

//void RotatePoint2D(float &pointX, float &pointY, float angle, float centerPointX, float centerPointY) {
//	float s = sin(angle);
//	float c = cos(angle);
//
//	pointX -= centerPointX;
//	pointY -= centerPointY;
//
//	float xnew = pointX * c - pointY * s;
//	float ynew = pointX * s + pointY * c;
//
//	pointX = xnew + centerPointX;
//	pointY = ynew + centerPointY;
//}
std::string EpochTimeToString(time_t epochTime, std::string format) {
	auto ts = *localtime(&epochTime);
	std::string strMon = (ts.tm_mon+1<10 ? "0" : "")+std::to_string(ts.tm_mon+1);
	std::string strMin = (ts.tm_min<10 ? "0" : "")+std::to_string(ts.tm_min);

	return crx::Format(format, std::to_string(ts.tm_mday), strMon, std::to_string(ts.tm_year+1900), std::to_string(ts.tm_hour), strMin);
}

TDateTime EpochTimeToDate(time_t epochTime) {
	TDateTime date;
	date.Set( EpochTimeToString(epochTime) );
	return date;
}

double Sqr(double a) { return a*a; }

static std::string RemovePtrSign(const std::string& str) {
	if( !str.size() || str[ str.size()-1 ]!='*' ) _ERROR("Can't remove ptr sign - it epsend");
	return str.substr( 0, str.size()-1 );
}

static void RemoveSpaceBetweenTemplates(std::string& s) {
	s = ReplaceAll(s, "> >", ">>");
}

static std::string TryConvert(const char* n) {
	if(n==NULL) return "";

	#ifdef __GNUG__
	int status;
	char* realname = abi::__cxa_demangle(n, 0, 0, &status);
	if( realname ) {
		std::string ss(realname);
		free(realname);
		RemoveSpaceBetweenTemplates(ss);
		return ss;
	}
	#else
	std::string ass = n;
	if (ass != "") {
		ass = ReplaceAll(ass, "class ", "");
		ass = ReplaceAll(ass, "struct ", "");
		ass = ReplaceAll(ass, " * __ptr64", "*");
		RemoveSpaceBetweenTemplates(ass);
		return ass;
	}
	#endif

	switch( n[0] ) {
		case 'i':
			return std::string("int");
		case 'j':
			return std::string("unsigned int");
		case 'd':
			return std::string("double");
		case 'f':
			return std::string("float");
		case 'c':
			return std::string("char");
		case 'b':
			return std::string("bool");
		case 's':
			return std::string("short");
		case 't':
			return std::string("unsigned short");
		case 'l':
			return std::string("long");
		case 'm':
			return std::string("unsigned long");
		case 'e':
			return std::string("long double");
		case 'h':
			return std::string("unsigned char");
		case 'y':
			return std::string("unsigned long long");
		case 'x':
			return std::string("long long");
	};

	_ERROR("Can't convert type name for "<<n);
	return std::string();

}


std::string ConvertTypeName(const char* n, bool rem) {
	std::string ss = TryConvert(n);
	if( rem ) ss = RemovePtrSign( ss );
	return ss;
}


void ZLibCompress(const std::string& str, std::string& outstring, int compressionlevel) {
	//z_stream zs;						// z_stream is zlib's control structure
	//memset(&zs, 0, sizeof(zs));

	//if (deflateInit(&zs, compressionlevel) != Z_OK)
	//	throw(std::runtime_error("deflateInit failed while compressing."));

	//zs.next_in = (Bytef*)str.data();
	//zs.avail_in = (uInt)str.size();		   // set the z_stream's input

	//int ret;
	//char outbuffer[32768];


	//// retrieve the compressed bytes blockwise
	//do {
	//	zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
	//	zs.avail_out = sizeof(outbuffer);

	//	ret = deflate(&zs, Z_FINISH);

	//	if (outstring.size() < zs.total_out) {
	//		// append the block to the output string
	//		outstring.append(outbuffer,
	//						 zs.total_out - outstring.size());
	//	}
	//} while (ret == Z_OK);

	//deflateEnd(&zs);

	//if (ret != Z_STREAM_END) {		  // an error occurred that was not EOF
	//	std::ostringstream oss;
	//	oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
	//	throw(std::runtime_error(oss.str()));
	//}
}

void ZLibDecompress(const std::string& str, std::string& outstring) {
	//z_stream zs;						// z_stream is zlib's control structure
	//memset(&zs, 0, sizeof(zs));

	//if (inflateInit(&zs) != Z_OK)
	//	throw(std::runtime_error("inflateInit failed while decompressing."));

	//zs.next_in = (Bytef*)str.data();
	//zs.avail_in = (uInt)str.size();

	//int ret;
	//char outbuffer[32768];

	//// get the decompressed bytes blockwise using repeated calls to inflate
	//do {
	//	zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
	//	zs.avail_out = sizeof(outbuffer);

	//	ret = inflate(&zs, 0);

	//	if (outstring.size() < zs.total_out) {
	//		outstring.append(outbuffer,
	//						 zs.total_out - outstring.size());
	//	}

	//} while (ret == Z_OK);

	//inflateEnd(&zs);

	//if (ret != Z_STREAM_END) {		  // an error occurred that was not EOF
	//	std::ostringstream oss;
	//	oss << "Exception during zlib decompression: (" << ret << ") "
	//		<< zs.msg;
	//	throw(std::runtime_error(oss.str()));
	//}

}

void MicroSleep(uint64_t microSecs) {
	std::this_thread::sleep_for( std::chrono::microseconds(microSecs) );
}

void MilliSleep(uint64_t milliSecs) { MicroSleep(milliSecs*1000); }

std::string GetTypeNameFromAny(const std::any &data) {
	return ConvertTypeName(data.type().name());
}

std::string RemoveNamespaceFromTypeName(const std::string& tn) {
	auto it = tn.find_last_of("::");
	if(it==std::string::npos) return tn;
	return tn.substr(it+2);
}

double AnglesDelta(double a, double b) {
	return atan2(sin(a-b), cos(a-b));
}

double MinAngleWithDir(double angle1, double angle2) {
	auto angleDelta = (angle1 < angle2) ? angle2 - angle1 : angle1 - angle2;
	if (angleDelta >= M_PI) {
		angle1 = (angle1 < 0) ? angle1 + 2 * M_PI : angle1 - 2 * M_PI;
	}
	
	return angle1;
}

bool IsAngelsNear(double a, double b, double eps) {
	return fabs(AnglesDelta(a, b))<=eps;
}

double AngleBetween(const TDPoint &src, double srcRotateAngle, const TDPoint &dst) {
	TDPoint d = dst;
	d -= src;
	TDPoint s( cos(srcRotateAngle), sin(srcRotateAngle) );

	auto cosa = ( (d.x*s.x) + (d.y*s.y) ) / (d.Len()*s.Len());

	return acos( cosa );
}

double AngleBetweenPlaneAndStraight(const T3DPoint& planeABC, const T3DPoint& straight) {
	auto angleSin = 	(planeABC.x*straight.x + planeABC.y*straight.y + planeABC.z*straight.z) /
						(sqrt(planeABC.x*planeABC.x + planeABC.y*planeABC.y + planeABC.z*planeABC.z) *
						sqrt(straight.x*straight.x + straight.y*straight.y + straight.z*straight.z));
	return asin(angleSin);
}

double FixPoint(double a, unsigned s) {
	auto i = floor(a);
	auto p = a - i;
	double ss = pow(10, s);
	p *= ss;
	p = floor(p);
	p /= ss;
	return i + p;
}

int iround(float a) { return iround( double(a) ); }

int iround(double a) { return (int)llround(a); }

unsigned uround(float a) { return uround( double(a) ); }

unsigned uround(double a) { return (unsigned)llround(a); }

std::vector<std::string> LoadStringVectorFromStringCommaSep(const std::string &pp) {
	return LoadVectorConfigFromString<std::string>(pp, ",");
}


std::string GetPathToExecutableFileOfCurrentProcess() {

	std::string path;

	#ifdef _MSC_VER
	
	TCHAR filePath[MAX_PATH + 1];
	GetModuleFileName(NULL, filePath, MAX_PATH + 1);
	path = filePath;

	path = ReplaceAll(path, "\\", "/");

	#else
	
	char arg1[20];
	char exepath[PATH_MAX + 1] = {0};

	sprintf( arg1, "/proc/%d/exe", getpid() );
	readlink( arg1, exepath, 1024 );
	path = exepath;

	#endif

	path = GetDirectoryPath(path);
	return path;

}

std::string GetIDEIndependedPathToRoot() {

	#ifdef _MSC_VER

	TCHAR filePath[MAX_PATH + 1];
	GetModuleFileName(NULL, filePath, MAX_PATH + 1);
	std::string path = filePath;
	
	path = ReplaceAll(path, "\\", "/");

	auto fne = GetFileNameFromPath(path, false);
	path = GetDirectoryPath(path);

	auto pos = path.find(fne);
	if(pos!=std::string::npos) path = path.substr(0, pos);

	return path;

	#else

	return GetPathToExecutableFileOfCurrentProcess();

	#endif

}

std::string GetDefaultSaveRootPath() {
	std::string path;

	#ifdef _MSC_VER
	PWSTR wpath;
	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &wpath);
	std::wstring strpath(wpath);
	CoTaskMemFree(wpath);

	path = to_utf8(strpath);
	path = ReplaceAll(path, "\\", "/");

	#else
	path = GetPathToExecutableFileOfCurrentProcess();
	path = AssemblePath(path, "Saves");
	#endif

	return path;
}


std::string GetIDEIndependedPathToVFSRoot(const std::string& rootFolder) {
	return AssemblePath( GetIDEIndependedPathToRoot(), rootFolder );
}

std::vector<std::vector<int>> GetAreaDirs(const crx::T3DIPoint &areaSize, int dimension) {
	std::vector<std::vector<int>> result;

	for(int i = 0; i < dimension; ++i) {
		auto remainder = !bool(areaSize[i] % 2);
		auto res = areaSize[i] / 2;

		//remainder = remainder == 0 ? 1 : 0;

		result.emplace_back();
		for(auto j = (0 - res) + remainder; j <= 0 + res; ++j) {
			result[i].push_back(j);
		}
	}

	return result;
}



} //crx








