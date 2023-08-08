#ifndef IREFLECTIONSERIALIZABLE_H_
#define IREFLECTIONSERIALIZABLE_H_

namespace crx {

// используется в TReflectionTwoStepXMLDeserializer для отправки нотифи о процессе серелизации/десеррелизации
// нужно просто унаследоваться от этого класса  (и от TReflectionObject, только его пытаются прикастить к этому классу)

class IReflectionSerializable {
	public:
	IReflectionSerializable()=default;
	virtual ~IReflectionSerializable()=default;

	public:
	virtual void AfterConstrictionInDeserializer();
	virtual void BeforLoadInDeserializer();
	virtual void AfterLoadInDeserializer();
	virtual void AfterAllLoadedInDeserializer();

	public:
	virtual void BeforSaveInSerializer();
	virtual void AfterSaveInSerializer();

};

} /* namespace crx */

#endif /* IREFLECTIONSERIALIZABLE_H_ */
