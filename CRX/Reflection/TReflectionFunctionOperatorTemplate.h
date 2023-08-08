#ifndef CRX_REFLECTION_TREFLECTIONFUNCTIONOPERATORTEMPLATE_H_
#define CRX_REFLECTION_TREFLECTIONFUNCTIONOPERATORTEMPLATE_H_

#include <CRX/Reflection/IReflectionFunctionCallOperator.h>
#include <CRX/Reflection/IReflectionIndirectCallOperator.h>

namespace crx {

namespace impl {

template<class CL, typename ...Ts>
class TFunctionOperatorTemplate : public IReflectionFunctionCallOperator, public IReflectionIndirectCallOperator<Ts...> {
	public:
	typedef void(CL::*TCall)(Ts...);


	TFunctionOperatorTemplate() {
		m_sClassTypeName = GetTypeName<CL>();
	}


	virtual void IndirectCall(TReflectionObject* obj, Ts... xs) override {
		CL* cl = dynamic_cast<CL*>(obj);
		std::invoke( m_pCall, cl, xs... );
	}



	protected:
	TCall m_pCall = nullptr;
};


// ----------------------------------------- //


template<class CL>
class TFunctionOperatorVoid : public TFunctionOperatorTemplate<CL> {
	public:
	typedef void(CL::*TCall)(void);

	public:
	TFunctionOperatorVoid(TCall set) {
		this->m_pCall = set;
	}
	virtual ~TFunctionOperatorVoid()=default;

	public:
	virtual void Call(TReflectionObject* obj, std::vector<std::any>& agrs) override {
		this->IndirectCall(obj);
	}

};

// ----------------------------------------- //

template<class CL, class VA>
class TFunctionOperatorOne : public TFunctionOperatorTemplate<CL,VA> {
	public:
	typedef void(CL::*TCall)(VA);

	public:
	TFunctionOperatorOne(TCall set) {
		this->m_pCall = set;
		this->m_vArgsTypeNames.push_back( GetTypeName<VA>() );
	}

	virtual ~TFunctionOperatorOne()=default;

	public:
	virtual void Call(TReflectionObject* obj, std::vector<std::any>& agrs) override {
		this->IndirectCall( obj, std::any_cast<VA&>(agrs[0]) );
	}


};

// ----------------------------------------- //

template<class CL, class VA, class VB>
class TFunctionOperatorTwo : public TFunctionOperatorTemplate<CL,VA, VB> {
	public:
	typedef void(CL::*TCall)(VA, VB);

	public:
	TFunctionOperatorTwo(TCall set) {
		this->m_pCall = set;
		this->m_vArgsTypeNames.push_back( GetTypeName<VA>() );
		this->m_vArgsTypeNames.push_back( GetTypeName<VB>() );
	}
	virtual ~TFunctionOperatorTwo()=default;

	public:
	virtual void Call(TReflectionObject* obj, std::vector<std::any>& agrs) override {
		this->IndirectCall( obj, std::any_cast<VA&>(agrs[0]), std::any_cast<VB&>(agrs[1]) );
	}


};

}//impl



}//crx



#endif /* REFLECTION_TREFLECTIONFUNCTIONOPERATORTEMPLATE_H_ */
