#ifndef CRX_REFLECTION_IREFLECTIONINDIRECTCALLOPERATOR_H_
#define CRX_REFLECTION_IREFLECTIONINDIRECTCALLOPERATOR_H_

namespace crx {

class TReflectionObject;

template <typename ...Ts>
class IReflectionIndirectCallOperator {
	public:
	virtual ~IReflectionIndirectCallOperator()=default;

	virtual void IndirectCall(TReflectionObject* obj, Ts... xs)=0;
};

}//crx


#endif /* REFLECTION_IREFLECTIONINDIRECTCALLOPERATOR_H_ */
