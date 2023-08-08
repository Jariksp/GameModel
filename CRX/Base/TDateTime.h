#ifndef CRX_TDATETIME_H_
#define CRX_TDATETIME_H_

#include <string>
#include <time.h>
#include <sstream>


namespace crx {

class TDateInterval;

/// Work with two time representation - seconds and human-readable date/time.
class TDateTime {
	public:
	TDateTime();
	TDateTime(const std::string& date);
	
	
	public:
	void Set(const std::string& date);// support only yyyy-mm-dd && yyyy-mm-dd hh:mm && yyyy-mm-dd hh:mm:ss formats


	/// Returns SQL timestamp YYYYMMDDHHMMSS shortened to len.
	enum NFormat {
		FormatYMDHMS = 0,
		FormatYMDHM = 1,
		FormatYMD = 2
	};

	std::string Get(NFormat type=FormatYMDHMS) const;

	std::string SQLTime(NFormat type=FormatYMDHMS) const;

	struct SRet {
		std::string Day;
		std::string Month;
		std::string Year;
		std::string Min;
		std::string Hour;
		std::string Sec;

		int iDay;
		int iMonth;
		int iYear;
		int iMin;
		int iHour;
		int iSec;
	};

	SRet GetComponents() const;

	static std::string MonthByNumber(int m);


	void operator+=(const TDateInterval& aa);
	void operator-=(const TDateInterval& aa);
	bool operator<(const TDateTime& aa) const;
	bool operator>(const TDateTime& aa) const;

	bool operator<=(const TDateTime& aa) const;
	bool operator>=(const TDateTime& aa) const;

	bool operator!=(const TDateTime& aa) const;

	bool operator==(const TDateTime& aa) const;

	public:
	mutable int64_t secs;
};

std::istream& operator>>(std::istream& is, TDateTime& dt);
std::ostream& operator<<(std::ostream& st, const crx::TDateTime& rr);

}//crx



#endif
