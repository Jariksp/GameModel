#include <sstream>
#include <iomanip>

#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CRX/Base/TDateInterval.h>
#include <CRX/Base/TDateTime.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TStringUtils.h>


namespace crx {

static std::string s_sMonths[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

std::string TDateTime::MonthByNumber(int m) { return s_sMonths[m]; }

TDateTime::TDateTime() {
	secs = 0;
}

TDateTime::TDateTime(const std::string& date) { Set(date); }

struct SRet3Date {
	int y;
	int d;
	int m;
};

static int64_t days_from_civil(int y, int m, int d) {
    y -= m <= 2;
    const int64_t era = (y >= 0 ? y : y-399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
    const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
    const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
    return era * 146097 + static_cast<int64_t>(doe) - 719468;
}

static SRet3Date civil_from_days(int64_t z) {
    z += 719468;
    const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
    const int64_t y = static_cast<int64_t>(yoe) + era * 400;
    const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
    const unsigned mp = (5*doy + 2)/153;                                   // [0, 11]
    const unsigned d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
    const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]
		SRet3Date ret;
		ret.y = (int)y + (m <= 2);
		ret.m = m;
		ret.d = d;
		return ret;
}


void TDateTime::Set(const std::string& date) {
	struct tm ti;
	memset(&ti, 0, sizeof(ti));

	if(date.size()==10) {
		sscanf( date.c_str(), "%u-%u-%u",  &ti.tm_year, &ti.tm_mon, &ti.tm_mday );
	} else if(date.size()==16) {
		sscanf( date.c_str(), "%u-%u-%u %u:%u",  &ti.tm_year, &ti.tm_mon, &ti.tm_mday, &ti.tm_hour, &ti.tm_min );
	} else {
		sscanf( date.c_str(), "%u-%u-%u %u:%u:%u",  &ti.tm_year, &ti.tm_mon, &ti.tm_mday, &ti.tm_hour, &ti.tm_min, &ti.tm_sec );
	}

	secs = days_from_civil(ti.tm_year, ti.tm_mon, ti.tm_mday)*24*3600;
	int add = ti.tm_hour*3600 + ti.tm_min*60 + ti.tm_sec;
	secs += add;
}

static std::string II(int a) {
	std::string r;
	if(a<10) r += "0";
	r += std::to_string(a);
	return r;
}

TDateTime::SRet TDateTime::GetComponents() const {
	SRet ret;

	int rr = secs%(24*3600);

	if( secs<0 && rr ) rr = (24*3600) + rr;

	int64_t days = secs/(24*3600);
	if(rr && secs<0) days--;
	SRet3Date ymd = civil_from_days(days);

	
	ret.Year = II(ymd.y);
	ret.Month = II(ymd.m);
	ret.Day = II(ymd.d);

	ret.iYear = ymd.y;
	ret.iMonth = ymd.m;
	ret.iDay = ymd.d;
	ret.iMin = 0;
	ret.iHour = 0;
	ret.iSec = 0;


	{
		int min = rr%3600;
		min = min/60;
		ret.Hour = II(rr/3600);
		ret.Min = II(min);
		ret.iHour = rr/3600;
		ret.iMin = min;

	}
	{
		int min = rr%3600;
		int ss = min%60;
		ret.Sec = II(ss);
		ret.iSec = ss;
	}
	return ret;

}

std::string TDateTime::Get(NFormat type) const {

	SRet dd = GetComponents();

	std::string ret;
	ret += dd.Year+std::string("-")+dd.Month+std::string("-")+dd.Day;

	if(type==FormatYMDHM || type==FormatYMDHMS) ret += std::string(" ")+dd.Hour+std::string(":")+dd.Min;
	if(type==FormatYMDHMS) ret += std::string(":")+dd.Sec;

	return ret;
}



void TDateTime::operator+=(const TDateInterval& aa) {
	secs += aa.Get();
}

void TDateTime::operator-=(const TDateInterval& aa) {
	secs -= aa.Get();
}

bool TDateTime::operator<(const TDateTime& aa) const { return secs<aa.secs; }
bool TDateTime::operator>(const TDateTime& aa) const { return secs>aa.secs; }

bool TDateTime::operator<=(const TDateTime& aa) const { return secs<=aa.secs; }
bool TDateTime::operator>=(const TDateTime& aa) const { return secs>=aa.secs; }

bool TDateTime::operator!=(const TDateTime& aa) const { return secs!=aa.secs; }
bool TDateTime::operator==(const TDateTime& aa) const { return secs==aa.secs; }

std::string TDateTime::SQLTime(NFormat type) const {
	std::string r = Get(type);
	r = ReplaceAll(r, "-", "");
	r = ReplaceAll(r, " ", "");
	r = ReplaceAll(r, ":", "");
	return r;
}

std::istream& operator>>(std::istream& is, TDateTime& dt) {
	std::string str;
	is>>str;
	dt.Set(str);
	return is;
}



std::ostream& operator<<(std::ostream& st, const crx::TDateTime& rr) {
	st<<rr.Get();
	return st;
}

}


