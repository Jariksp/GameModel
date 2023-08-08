#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Base/TMiscUtils.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/TReflectionTwoStepXMLDeserializer.h>
#include <CRX/Reflection/TReflectionTwoStepXMLSerializer.h>
#include <CRX/Reflection/TReflectionTypeFactory.h>
#include <CRX/Reflection/TReflectionObjectCasts.h>

namespace crx {

std::string TReflectionObject::TypeName() const { return GetTypeName(*this); }

void TReflectionObject::InitReflectionTable() {}

TReflectionClassPropertyData* TReflectionObject::FindProperty(const std::string &fieldName) {
	return TReflectionGlobals::GetInstance()->FindPropertyData( TypeName(), fieldName );
}

TReflectionClassFunctionData* TReflectionObject::FindFunction(const std::string& fieldName) {
	return TReflectionGlobals::GetInstance()->FindFunctionData( TypeName(), fieldName );
}


void TReflectionObject::SetPropertyData(const std::string &fieldName, const std::any &data) {
	auto d = FindProperty(fieldName);
	if( !d ) _ERROR("No published field: "<<fieldName<<" in class "<<TypeName());
	d->Set(this, data);

}

void TReflectionObject::GetPropertyData(const std::string &fieldName, std::any &saveTo) {
	auto d = FindProperty(fieldName);
	if( !d ) _ERROR("No published field: "<<fieldName<<" in class "<<TypeName());
	d->Get(this, saveTo);
}


void TReflectionObject::SetPropertyFromString(const std::string &fieldName, const std::string &value) {
	auto cf = FindProperty(fieldName);
	if( !cf ) _ERROR("No published field: "<<fieldName<<" in class "<<TypeName());
	SetPropertyFromString(cf, value);
}

void TReflectionObject::SetPropertyFromString(TReflectionClassPropertyData *property, const std::string &value) {
	std::any data;
	property->StringConverter()->Set(data, value);
	property->Set( this, data );
}

std::string TReflectionObject::GetPropertyToStringNoError(const std::string& fieldName) {
	auto d = FindProperty(fieldName);
	if( !d ) return "";
	return GetPropertyToString(d);
}

std::string TReflectionObject::GetPropertyToString(const std::string &fieldName) {
	auto d = FindProperty(fieldName);
	if( !d ) _ERROR("No published field: "<<fieldName<<" in class "<<TypeName());
	return GetPropertyToString(d);

}

std::string TReflectionObject::GetPropertyToString(TReflectionClassPropertyData *cf) {
	std::any d;
	cf->Get(this, d);
	std::string str;
	cf->StringConverter()->Get(str, d);
	return str;
}

std::string TReflectionObject::SerializeShared(const std::string& rootNodeName) {
	return SerializeShared(NNSerializerFlags::None, rootNodeName);
}

std::string TReflectionObject::SerializeShared(NSerializerFlags flags, const std::string& rootNodeName) {
	crx::TReflectionTwoStepXMLSerializer ser;
	ser.Save(rootNodeName, shared_from_this(), flags);
	return ser.Data();
}

std::shared_ptr<TReflectionObject> TReflectionObject::DeserializeSharedObject(const std::string &data, NSerializerFlags flags) {
	crx::TReflectionTwoStepXMLDeserializer dser;
	return dser.LoadSharedReflection(data, flags);
}

TReflectionObject* TReflectionObject::DeserializePointeredObject(const std::string& data, NSerializerFlags flags) {
	crx::TReflectionTwoStepXMLDeserializer dser;
	return dser.LoadPointeredReflection(data, flags);
}

TReflectionObject* TReflectionObject::CloneThisReflection() {
	auto useTypeName = TypeName()+std::string("*");

	auto tf = crx::TReflectionGlobals::GetInstance()->FindTypeFactory( useTypeName );
	auto dnc = tf->Create();
	return tf->Cast()->Cast(dnc);
}


}//crx





/*
#include <TDispatch.h>
#include <TAbiExt.h>
#include <TGlobalDispatch.h>
#include <TClassPropertyData.h>
#include <TStringConverter.h>
#include <debug.h>
#include <CRX/XParse/TXMNode.h>
#include <TDispatchXMLSerializer.h>
#include <TDispatchXMLDeserializer.h>


void TDispatch::InitDispatchTable() {}





TClassFunctionData* TDispatch::FindFunction(const std::string& fn) {
	return TGlobalDispatch::GetInstance()->FindFunctionData( TypeName(), fn );
}



bool TDispatch::IsPropertyEqual(const string& fn, const TDispatchDataPtr& src) {
	TClassPropertyData* d = FindProperty(fn);
	if( !d ) _ERROR("No published field: "<<fn<<" in class "<<TypeName());
	return d->IsEqual( this, src );
}


//bool TDispatch::operator==(TDispatch& rhs) { return IsEqual(rhs); }
//
bool TDispatch::operator==(const TDispatch& rhs) const {
	TDispatch* dst = (TDispatch*)&rhs;
	TDispatch* src = (TDispatch*)this;
	return src->IsDispatchEqual(*dst);
}

bool TDispatch::IsDispatchEqual(TDispatch& r) {
	if( TypeName() != r.TypeName() ) return false;
	
	const map<string, TClassPropertyData*>* mm = TGlobalDispatch::GetInstance()->GetAllPropertys( TypeName() );
	map<string, TClassPropertyData*>::const_iterator it = mm->begin();
	

	for(;it!=mm->end();it++) {
		TClassPropertyData* d = it->second;
		if( !d->IsEqual(&r, this) ) return false;
	}
	
	return true;
}




TDispatch* TDispatch::Clone() {
	TDispatch* nc = (TDispatch*)TGlobalDispatch::GetInstance()->FindTypeFactory( TypeName() )->New();

	TDispatchXMLSerializer ser;
	ser.SaveObject("Obj", this);

	TDispatchXMLDeserializer dser;
	dser.Data( ser.Data() );
	dser.LoadThis("Obj", nc);

	return nc;
}

void TDispatch::Serialize(IDispatchSerializer* data) {
	data->Save(this);
}

void TDispatch::DeSerialize(IDispatchDeserializer* data) {
	data->Load(this);
}
*/




