#ifndef CRX_TDATEINTERVAL_H_H_H_
#define CRX_TDATEINTERVAL_H_H_H_

#include <string>


namespace crx {

class TDateInterval {
	public:
	TDateInterval() = default;
	TDateInterval(const std::string& s);
	TDateInterval(unsigned secs);

	void Set(const std::string& s);
	void Set(unsigned secs);

	unsigned Get() const;

	protected:
	unsigned m_uSecs = 0;

};

}









#endif

