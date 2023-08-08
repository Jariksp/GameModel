#include <CRX/Reflection/TReflectionTwoStepXMLSerializer.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Reflection/TReflectionTypeFactory.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionObjectCasts.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Base/TMiscUtils.h>
#include <CRX/Reflection/IReflectionSerializable.h>

namespace crx {

TReflectionTwoStepXMLSerializer::SData::SData(TReflectionObject* o, TReflectionObjectCasts* c){
	Object = o; 
	Cast = c;
}

// ---------------------------------------------------------- //

void TReflectionTwoStepXMLSerializer::BeforSave(const std::string& name, const std::string& params) {
	if(name.empty()) _ERROR("beforSave empty name");
	AddLine(std::string("<")+name+params+std::string(">"));
	m_vLevel.push_back(name);
}


void TReflectionTwoStepXMLSerializer::AfterSave() {
	if( m_vLevel.empty() ) _ERROR("Empty level, can't up");
	auto was = *m_vLevel.rbegin();
	m_vLevel.pop_back();
	if(was.size()) AddLine(std::string("</")+was+std::string(">"));
}


void TReflectionTwoStepXMLSerializer::AddLine(const std::string& line, int addTabs) {
	if(m_sData.size()) m_sData += "\n";
	int cnt = int(m_vLevel.size()) + addTabs;
	for(int i=0;i<cnt;i++) m_sData += "\t";
	m_sData += line;
}


std::string TReflectionTwoStepXMLSerializer::Data() { return m_sData; }


// ---------------------------------------------------------- //

void TReflectionTwoStepXMLSerializer::Clear() {
	m_sData.clear();
	m_vLevel.clear();
	m_mObjects.clear();
}

void TReflectionTwoStepXMLSerializer::Save(const std::string& name, std::any data, NSerializerFlags flags) {
	Clear();
	auto rootData = new SData();
	rootData->Any = data;
	rootData->Cast = TReflectionGlobals::GetInstance()->FindCast( GetTypeNameFromAny(rootData->Any) );
	rootData->Object = rootData->Cast->Cast(data);

	auto id = Index(rootData->Object);//uintptr_t
	BeforSave(name, std::string(" root=")+std::to_string(id)+std::string(" ") );

	SaveAllPtrsRecursive(rootData);

	for(auto& a:m_mObjects) {
		BeforSave( std::to_string(a.first) );
		auto iser = dynamic_cast<IReflectionSerializable*>(a.second->Object);
		if(iser) iser->BeforSaveInSerializer();
		SaveFull(a.second->Object, a.second->Cast);
		if(iser) iser->AfterSaveInSerializer();
		AfterSave();
	}

	AfterSave();

	for(auto it:m_mObjects) delete it.second;
	m_mObjects.clear();

	if(flags&NNSerializerFlags::Zip) {
		std::string out;
		crx::ZLibCompress(m_sData, out);
		m_sData = out;
	}

	if(flags&NNSerializerFlags::Base64) {
		m_sData = crx::base64_encode(m_sData);
	}

}


void TReflectionTwoStepXMLSerializer::IsAcceptableType(TReflectionTypeFactory* fac, const std::string& fieldName) {
//	if( fac->IsComplex() && !fac->IsPointer() ) {
//		_ERROR("Can't serialize non-pointer to TDispatch. FieldName: "<<fieldName<<" in "<<fac->TypeName());
//	}
}

void TReflectionTwoStepXMLSerializer::AddLineString(const std::string& name, const std::string& data, bool useKey, const std::string& keyString) {
	auto put = std::string("<")+name+std::string(" value=\"")+data+std::string("\" ");
	if(useKey) put += std::string(" key=\"")+keyString+std::string("\" ");
	put += std::string(" />");
	AddLine( put );
}


void TReflectionTwoStepXMLSerializer::SaveReference(const std::string& name, TReflectionObject* data, bool useKey, const std::string& keyString) {
	if( data && m_mIdxes.find(data)==m_mIdxes.end() ) _ERROR("Invalid reference to object: "<<data);

	auto ssid = std::to_string(Index(data));//uintptr_t
	AddLineString(name, ssid, useKey, keyString);
}

static bool IsWriteAndReadAble(TReflectionClassPropertyData* cf) {
	return cf->IsWritable() && cf->IsReadable();
}

void TReflectionTwoStepXMLSerializer::SaveFull(TReflectionObject* data, TReflectionObjectCasts* cast) {
	const auto& mm = *TReflectionGlobals::GetInstance()->GetAllPropertys(data->TypeName());

	auto typeName = data->TypeName();
	if(cast) {
		if(cast->IsPointer()) {
			typeName += std::string("*");
		} else if(cast->IsSmartPtr()) {
			typeName = cast->Wrapper()+std::string("<")+typeName+std::string(">");
		}
	}


	AddLine( std::string("<typeid value=\"")+typeName+std::string("\" />") );

	std::any dispData;
	for(auto it=mm.begin();it!=mm.end();it++) {
		auto cf = it->second;
		if( !IsWriteAndReadAble(cf) || cf->IsLazyInitializationNullPtr(data) ) continue;

		cf->Get(data, dispData);

		if( cf->Cast() && !DoFindStringConverter(cf->TypeFactory(), dispData) ) {
			auto disp = cf->Cast()->Cast(dispData);
			if(disp) SaveReference(cf->Name(), disp);
		} else {
			SaveByData(cf->Name(), dispData);
		}

	}
}// SaveFull


void TReflectionTwoStepXMLSerializer::SaveByData(const std::string& name, std::any& data, bool useKey, const std::string& keyString) {
	auto gi = TReflectionGlobals::GetInstance();
	auto typeName = GetTypeNameFromAny(data);
	auto typeFac = gi->FindTypeFactory(typeName);

	auto strConv = DoFindStringConverter(typeFac, data);
	if( strConv ) {
		std::string str;
		strConv->Get(str, data);
		AddLineString(name, str, useKey, keyString);
	} else if( typeFac->ContainerManipulator() ) {
		BeforSave(name);
		auto con = typeFac->ContainerManipulator();
		auto size = con->Size(data);

		std::any value;
		std::any index;
		bool isMapLike = con->IndexTypeName().size();
		for(unsigned i=0;i<size;i++) {
			con->ValueAt(data, i, value);
			if(isMapLike) {
				con->IndexAt(data, i, index);

				// ну это жесть какая-то - а если нет конертера или это указатель на TReflectionObject??
				auto indexStrConv = gi->FindStringConverter( con->IndexTypeName() );
				std::string str;
				indexStrConv->Get( str, index );

				SaveByData("element", value, true, str);
			} else {
				SaveByData("element", value);
			}
		}//for

		AfterSave();
	} else if( typeFac->Cast() ) {
		auto disp = typeFac->Cast()->Cast(data);
		SaveReference(name, disp, useKey, keyString);
	} else {
		std::string path;
		for(auto a : m_vLevel) path += std::string("/")+a;
		_ERROR( "Unknown type in: "<<name<<"  TypeName: "<<typeName<<" path: "<<path );
	}


}//SaveByData


void TReflectionTwoStepXMLSerializer::SaveAllPtrsRecursive(SData* sdata) {
	if(!sdata || !sdata->Object) return;

	TReflectionObject* data = sdata->Object;

	auto id = Index(data);//uintptr_t
	if( m_mObjects.find(id)!=m_mObjects.end() ) {
		// приоритет shared_ptr поинтера выше, если ранее встречался raw_ptr 
		if( sdata->Cast->IsSmartPtr() && !sdata->Cast->IsWeakPtr() ) m_mObjects[id] = sdata;
		return;//prevent cycling
	}

	auto global = TReflectionGlobals::GetInstance();
	if( !sdata->Cast->IsWeakPtr() ) m_mObjects[id] = sdata;// нельзя сохранять weak_ptr - его нельзя создать

	const auto& mm = *global->GetAllPropertys(data->TypeName());

	for(auto it=mm.begin();it!=mm.end();it++) {
		auto cf = it->second;
		if( !IsWriteAndReadAble(cf) || cf->IsLazyInitializationNullPtr(data) ) continue;

		auto fac = cf->TypeFactory();
		if(!fac) _ERROR("TypeFactory for '"<<cf->TypeName()<<"' not found");
		IsAcceptableType(fac, it->first);

		if( DoFindStringConverter(fac, cf, data) ) continue;

		if( cf->Cast() ) {
			auto ndata = new SData();
			ndata->Cast = cf->Cast();
			cf->Get(data, ndata->Any);
			ndata->Object = cf->Cast()->Cast(ndata->Any);
			SaveAllPtrsRecursive( ndata );
		} else if( fac->ContainerManipulator() ) {
			auto conv = global->FindContainerManipulator( fac->TypeName() );
			if(!conv) _ERROR("Container manipulator for "<<fac->TypeName()<<" not found!");
			auto vfac = global->FindTypeFactory( conv->ValueTypeName() );

			IsAcceptableType(vfac, it->first);

			if( vfac->Cast() ) {
				std::any container_ptr;
				cf->Get(data, container_ptr);
				auto con = fac->ContainerManipulator();
				auto size = con->Size(container_ptr);
				for(unsigned i=0;i<size;i++) {
					auto ndata = new SData();
					con->ValueAt(container_ptr, i, ndata->Any);
					ndata->Object = vfac->Cast()->Cast(ndata->Any);
					ndata->Cast = vfac->Cast();
					SaveAllPtrsRecursive(ndata);
				}//for
			}//if
		}//if container
	}//for

}//SaveAllPtrsRecursive


TReflectionStringConverter* TReflectionTwoStepXMLSerializer::DoFindStringConverter(TReflectionTypeFactory *t, std::any &data) {
	if(!FindStringConverter || !t->Cast()) return t->StringConverter();
	auto r = t->Cast()->Cast(data);
	return FindStringConverter(r);
}

TReflectionStringConverter* TReflectionTwoStepXMLSerializer::DoFindStringConverter(TReflectionTypeFactory *t, TReflectionClassPropertyData *cf, TReflectionObject* in) {
	if(!FindStringConverter || !t->Cast()) return t->StringConverter();
	std::any data;
	cf->Get(in, data);
	auto r = t->Cast()->Cast(data);
	return FindStringConverter(r);
}


int TReflectionTwoStepXMLSerializer::Index(TReflectionObject* obj) {
	if(!obj) return 0;

	auto it = m_mIdxes.find(obj);
	if(it==m_mIdxes.end()) {
		m_mIdxes[obj] = int(m_mIdxes.size())+1;
	}
	return m_mIdxes[obj];
}

}//crx


