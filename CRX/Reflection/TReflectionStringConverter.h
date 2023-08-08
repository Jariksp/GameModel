#ifndef CRX_TSTRINGCONVERTER_J__H_H_H_
#define CRX_TSTRINGCONVERTER_J__H_H_H_

#include <CRX/Reflection/TReflectionCommonTypeIdent.h>
#include <any>
#include <vector>


namespace crx {

class TReflectionStringConverter : public TReflectionCommonTypeIdent {
	public:
	TReflectionStringConverter()=default;
	virtual ~TReflectionStringConverter()=default;

	public:
	static void Add(TReflectionStringConverter* conv);
	static bool IsHave(const std::string& typeName);
	static void SetCustomFromString(const std::string& className, const std::string& publishAs, TReflectionStringConverter* conv);

	public:
	virtual void Set(std::any& out, const std::string& value)=0;//set data from string
	virtual void Get(std::string& out, const std::any& value)=0;//get string from data

	public:
	// это для эдитора, чтобы получить список всех назаний для энума
	class IStringSource {
		public:
		virtual ~IStringSource()=default;
		public:
		virtual const std::vector<std::string>& StringSource()=0;
		virtual const std::vector<std::pair<std::string, int> >& StringData()=0;//lower case name,value
		virtual int NameToInt(const std::string& name);
	};

};


}//crx

/*

#include <TDispatchData.h>
#include <vector>


class TStringConverter : public TCommonTypeIdent {
	public:
	TStringConverter();
	TStringConverter(const vector<string>& stringSource);
	TStringConverter(const vector<const char*>& stringSource);
	virtual ~TStringConverter();



	public:
	void Set(const TDispatchDataPtr& data, const string& value) const;
	void Get(string& data, const TDispatchDataPtr& value) const;

	public:
	bool IsHaveStringSource() const;
	const vector<string>& StringSource() const;

	public:
	virtual void DoSet(void* data, const std::string& value) const=0;
	virtual void DoGet(std::string& data, const void* value) const=0;

	protected:
	void InitSS(const vector<string>& ss);

	protected:
	bool m_bIsStringSource = false;
	vector<string> m_vStringSource;
	vector<string> m_vLoweredStringSource;
};

*/





#endif
