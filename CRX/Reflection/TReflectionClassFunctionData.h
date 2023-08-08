#ifndef CRX_REFLECTION_TREFLECTIONCLASSFUNCTIONDATA_H_
#define CRX_REFLECTION_TREFLECTIONCLASSFUNCTIONDATA_H_

#include <string>


namespace crx {

class IReflectionFunctionCallOperator;


class TReflectionClassFunctionData {
	public:
	TReflectionClassFunctionData(const std::string& name, IReflectionFunctionCallOperator* op);
	virtual ~TReflectionClassFunctionData()=default;

	public:
	static void Add(TReflectionClassFunctionData* property);
	static TReflectionClassFunctionData* Add(const std::string& publishAs, IReflectionFunctionCallOperator* op);

	const std::string& Name();

	IReflectionFunctionCallOperator* Operator();

	std::string TypeName();
	const std::string& ClassTypeName();


	protected:
	std::string m_sName;
	IReflectionFunctionCallOperator* m_pOperator = nullptr;

};



}//crx






#endif /* REFLECTION_TREFLECTIONCLASSFUNCTIONDATA_H_ */
