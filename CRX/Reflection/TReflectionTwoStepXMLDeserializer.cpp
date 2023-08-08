#include <CRX/Reflection/TReflectionTwoStepXMLDeserializer.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Reflection/TReflectionTypeFactory.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionObjectCasts.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/IReflectionSerializable.h>
#include <CRX/XParse/TXMNode.h>

namespace crx {

TReflectionObject* TReflectionTwoStepXMLDeserializer::LoadPointeredReflection(const std::string& data, NSerializerFlags flags) {
	std::any aobj;
	return LoadReflection(data, flags, aobj);

}

std::shared_ptr<TReflectionObject> TReflectionTwoStepXMLDeserializer::LoadSharedReflection(const std::string& data, NSerializerFlags flags) {
	std::any aobj;
	return LoadReflection(data, flags, aobj)->shared_from_this();
	/*
	std::any aobj;
	Load( aobj, data, flags );

	auto tn = crx::GetTypeNameFromAny(aobj);
	auto cast = crx::TReflectionGlobals::GetInstance()->FindCast(tn);
	return cast->Cast(aobj)->shared_from_this();
	*/
}

TReflectionObject* TReflectionTwoStepXMLDeserializer::LoadReflection(const std::string& data, NSerializerFlags flags, std::any& aobj) {
	Load( aobj, data, flags );

	auto tn = crx::GetTypeNameFromAny(aobj);
	auto cast = crx::TReflectionGlobals::GetInstance()->FindCast(tn);
	return cast->Cast(aobj);
}

void TReflectionTwoStepXMLDeserializer::Load(std::any& out, const std::string &inData, NSerializerFlags flags) {

	std::string tmp;

	if(flags&NNSerializerFlags::Base64) {
		tmp = crx::base64_decode(inData);
	}

	if(flags&NNSerializerFlags::Zip) {
		std::string dd = tmp.size()?tmp:inData;
		tmp.clear();
		crx::ZLibDecompress(dd, tmp);
	}


	const auto& data = tmp.size()?tmp:inData;


	auto null_node = std::make_shared<TXMNode>();
	null_node->Parse(data);
	auto node = null_node->At(0);

	auto rootID = node->Params["root"];

	auto global = TReflectionGlobals::GetInstance();

	std::map< std::string, SData* > dispatchs;

	for(unsigned i=0;i<node->Size();i++) {
		auto cc = node->At(i);

		auto it = cc->Find("typeid");
		if( !it ) _ERROR("No typeid node in: "<<cc->Name());
		auto typeName = it->Params["value"];

		auto fac = global->FindTypeFactory(typeName);
		if(!fac) {
			//_ERROR("Factory for: '"<<typeName<<"' not found");
			continue;
		}

		auto data = new SData();
		data->Any = fac->Create();
		data->Obj = fac->Cast()->Cast(data->Any);
		data->Node = cc;
		data->Serializable = dynamic_cast<IReflectionSerializable*>(data->Obj);

		if(!data->Obj) _ERROR("Can't cast "<<fac->TypeName()<<" to TReflectionObject* OR can't create TReflectionObject*");

		if(data->Serializable) data->Serializable->AfterConstrictionInDeserializer();

		dispatchs[cc->Name()] = data;
	}//for

	for(auto it:dispatchs) {
		auto obj = it.second->Obj;
		auto node = it.second->Node;

		if(it.second->Serializable) it.second->Serializable->BeforLoadInDeserializer();

		for(unsigned i=0;i<node->Size();i++) {
			auto n = node->At(i);
			if(n->Name()=="typeid") continue;

			auto cf = obj->FindProperty( n->Name() );
			if(!cf) continue;

			auto cfac = cf->TypeFactory();
			auto stringConv = DoFindStringConverter(cfac, n);

			if(stringConv) {
				std::any data;
				stringConv->Set( data, n->Params["value"] );
				cf->Set(obj, data);
			} else if( cfac->Cast() ) {
				auto unID = n->Params["value"];
				auto dit = dispatchs.find(unID);
				if(dit==dispatchs.end()) _ERROR("ReflectionObject with uid: "<<unID<<" not found");
				auto ddv = dit->second;

				std::any dispAny;
				cfac->Cast()->CastFromAnyToAny(ddv->Any, dispAny);
				cf->Set(obj, dispAny);

			} else if( cfac->ContainerManipulator() ) {
				std::any data_ptr;
				cf->Get(obj, data_ptr);
				auto container = cf->ContainerManipulator();

				container->Clear(data_ptr);
				auto vfac = container->ValueTypeFactory();
				auto ifac = container->IndexTypeFactory();

				auto isMapLike = container->IndexTypeName().size();

				for(unsigned i=0;i<n->Size();i++) {
					auto vnode = n->At(i);

					std::any index;
					if(isMapLike) {
						index = ifac->Create();
						ifac->StringConverter()->Set( index, vnode->Params["key"] );
					}

					if(vfac->StringConverter()) {
						auto value = vfac->Create();
						vfac->StringConverter()->Set( value, vnode->Params["value"] );
						container->AddElement(data_ptr, index, value);
					} else if(vfac->Cast()) {
						auto unID = vnode->Params["value"];
						std::any to;

						if(unID=="0") {
							vfac->Cast()->Cast(nullptr, to);
						} else {
							auto dit = dispatchs.find(unID);
							if(dit==dispatchs.end()) _ERROR("ReflectionObject with uid: "<<unID<<" not found");

							auto ddv = dit->second;
							vfac->Cast()->CastFromAnyToAny(ddv->Any, to);
						}


						container->AddElement(data_ptr, index, to);
					} else {
						_ERROR("Can't load vector data: "<<vfac->TypeName());
					}
				}//for all elements
				cf->Set( obj, data_ptr );

			} else {
				_ERROR("Unknown how to load: "<<cfac->TypeName());
			}
		}//for node

		if(it.second->Serializable) it.second->Serializable->AfterLoadInDeserializer();

	}//for dispatchs


	auto it = dispatchs.find(rootID);
	if(it==dispatchs.end()) _ERROR("rootID not found");
	out = it->second->Any;

	for(auto it:dispatchs) if(it.second->Serializable) it.second->Serializable->AfterAllLoadedInDeserializer();

	for(auto it:dispatchs) delete it.second;
	dispatchs.clear();
}


TReflectionStringConverter* TReflectionTwoStepXMLDeserializer::DoFindStringConverter(TReflectionTypeFactory *f, std::shared_ptr<TXMNode> n) {
	auto it = n->Params.find("value");

	if(FindStringConverter && it!=n->Params.end()) {
		return FindStringConverter(f, it->second);
	} else {
		return f->StringConverter();
	}
}

}//crx


