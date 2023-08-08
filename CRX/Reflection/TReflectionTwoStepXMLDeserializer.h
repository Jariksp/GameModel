#ifndef CRX_REFLECTION_TREFLECTIONTWOSTEPXMLDESERIALIZER_H_
#define CRX_REFLECTION_TREFLECTIONTWOSTEPXMLDESERIALIZER_H_

#include <any>
#include <string>
#include <functional>
#include <memory>
#include <CRX/Reflection/NNSerializerFlags.h>

namespace crx {

class TReflectionObject;
class TXMNode;
class TReflectionClassPropertyData;
class TReflectionTypeFactory;
class TReflectionStringConverter;
class IReflectionSerializable;

class TReflectionTwoStepXMLDeserializer {
	public:
	TReflectionTwoStepXMLDeserializer()=default;
	virtual ~TReflectionTwoStepXMLDeserializer()=default;

	public:
	void Load(std::any& out, const std::string& data, NSerializerFlags flags=NNSerializerFlags::None);

	std::shared_ptr<TReflectionObject> LoadSharedReflection(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None);
	TReflectionObject* LoadPointeredReflection(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None);

	template<class TT>
	std::shared_ptr<TT> LoadSharedObject(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None) {
		return std::dynamic_pointer_cast<TT>( LoadSharedReflection(data, flags) );
	}

	template<class TT>
	TT* LoadPointeredObject(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None) {
		return dynamic_cast<TT*>( LoadPointeredReflection(data, flags) );
	}


	public:
	std::function<TReflectionStringConverter*(TReflectionTypeFactory*, const std::string&)> FindStringConverter;

	protected:
	struct SData {
		std::any Any;
		TReflectionObject* Obj = nullptr;
		std::shared_ptr<TXMNode> Node = nullptr;
		IReflectionSerializable* Serializable = nullptr;
	};


	protected:
	TReflectionStringConverter* DoFindStringConverter(TReflectionTypeFactory* f, std::shared_ptr<TXMNode> n);

	TReflectionObject* LoadReflection(const std::string& data, NSerializerFlags flags, std::any& store);


};

}//crx

#endif /* REFLECTION_TREFLECTIONTWOSTEPXMLDESERIALIZER_H_ */
