#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Base/T3DIPoint.h>
#include <CRX/Base/T3DBoolPoint.h>
#include <CRX/Base/T3DPoint.h>
#include <CRX/Base/TDateTime.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Reflection/TReflectionTypeFactory.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionClassFunctionData.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Reflection/TReflectionStringConverterTemplate.h>
#include <CRX/Reflection/TReflectionTypeFactoryTemplate.h>
#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Reflection/TReflectionObjectCastsTemplate.h>
#include <memory>
#include <regex>


namespace crx {

//template<class T>
//void AddDefaultFactoryAndConverter(TReflectionGlobals* gi) {
//	gi->Add( new TReflectionStringConverterTemplate<T>() );
//	gi->Add( new TTReflectionTypeFactoryTemplate<T>() );
//}

template<class T>
void AddDefaultFactoryAndConverter(TReflectionGlobals* gi, std::function<T(const std::string&)> from=nullptr, std::function<std::string(const T&)> to=nullptr) {
	gi->Add( new impl::TReflectionStringConverterTemplate<T>(from, to) );
	gi->Add( impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<T>() );
}

bool TReflectionGlobals::IsClassTypeInited(const std::string &classType) {
	return m_mInitedClassType.find(ToLowerCase(classType))!=m_mInitedClassType.end();
}

void TReflectionGlobals::SetClassTypeInited(const std::string &classType) {
	m_mInitedClassType[ToLowerCase(classType)] = "";
}

TReflectionGlobals::TReflectionGlobals() {
	AddDefaultFactoryAndConverter<int>(this, &to_int);
	AddDefaultFactoryAndConverter<unsigned>(this, &to_uint);
	AddDefaultFactoryAndConverter<uint64_t>(this, &to_uint64);
	AddDefaultFactoryAndConverter<int64_t>(this, &to_int64);
	AddDefaultFactoryAndConverter<double>(this, &to_double);
	AddDefaultFactoryAndConverter<float>(this, &to_float);

	AddDefaultFactoryAndConverter<uintptr_t>(this);

	AddDefaultFactoryAndConverter<bool>(this,
			[](const std::string& s)->bool { return s=="true" || s=="1"; },
			[](const bool& b)->std::string { return b?"true":"false"; }
	);

	AddDefaultFactoryAndConverter<std::string>(this,
			[](const std::string& from)->std::string {
				return std::regex_replace(from, std::regex("&quot;"), "\"");
			},
			[](const std::string& to)->std::string {
				return std::regex_replace(to, std::regex("\""), "&quot;");
			}
	);



	AddDefaultFactoryAndConverter<crx::T3DPoint>(this);
	AddDefaultFactoryAndConverter<crx::T3DIPoint>(this);
	AddDefaultFactoryAndConverter<crx::T3DBoolPoint>(this);

	AddDefaultFactoryAndConverter<TDateTime>(this);

	Add( new impl::TReflectionObjectCastsPointerTemplate<TReflectionObject*>() );
	Add( new impl::TReflectionObjectCastsTemplate<TReflectionObject>() );
	Add( new impl::TReflectionObjectSharedPtrCastsTemplate<std::shared_ptr<TReflectionObject>>() );
	Add( new impl::TReflectionObjectWeakPtrCastsTemplate<std::weak_ptr<TReflectionObject>>() );

	Add( impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<TReflectionObject>() );
	Add( impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<TReflectionObject*>() );


}

// --------------------------------------------- //

void TReflectionGlobals::Add(TReflectionTypeFactory* f, bool force) {
	auto tn = ToLowerCase(f->TypeName());
	if( m_mTypeFactories.find(tn)!=m_mTypeFactories.end() && !force ) return;
	m_mTypeFactories[tn] = f;
}

template<class T>
void EraseFromCacheIfAny(std::map< std::string, std::map<std::string, T*>* >& mm, const std::string& ct) {
	auto it = mm.find(ct);
	if( it!=mm.end() ) {
		delete it->second;
		mm.erase(it);
	}
}//EraseFromCachIfAny

void TReflectionGlobals::Add(const std::string& inct, TReflectionClassPropertyData* cfd) {
	auto ct = ToLowerCase(inct);
	m_mFieldData[ct][ ToLowerCase(cfd->Name()) ] = cfd;
	EraseFromCacheIfAny(m_mGetAllFieldsCache, ct);
}

void TReflectionGlobals::Add(const std::string& inct, TReflectionClassFunctionData* cfd) {
	auto ct = ToLowerCase(inct);
	m_mFunctionData[ct][ ToLowerCase(cfd->Name()) ] = cfd;
	EraseFromCacheIfAny(m_mGetAllFunctionsCache, ct);
}

void TReflectionGlobals::Add(TReflectionStringConverter* conv) {
	auto tn = ToLowerCase(conv->TypeName());
	m_mStringConverters[tn] = conv;
}


void TReflectionGlobals::Add(TReflectionContainerManipulator* manipulator) {
	auto tn = ToLowerCase(manipulator->TypeName());
	m_mContainerManipulators[tn] = manipulator;
}

void TReflectionGlobals::Add(TReflectionObjectCasts *cast) {
	auto tn = ToLowerCase(cast->TypeName());
	m_mCasts[tn] = cast;
}


// --------------------------------------------- //

template<class T>
T* FindInCache(const std::string& tn, std::map<std::string, T*>& mm) {
	auto it = mm.find( ToLowerCase(tn) );
	if( it!=mm.end() ) return it->second;
	return nullptr;
}

TReflectionTypeFactory* TReflectionGlobals::FindTypeFactory(const std::string& tn) {
	return FindInCache(tn, m_mTypeFactories);
}


template<class T>
T* FindClassEntity(const std::map<std::string, T*>* mm, const std::string& inpa) {
	if(!mm) return nullptr;
	std::string fn = ToLowerCase( inpa );
	auto it = mm->find( fn );
	if( it!=mm->end() ) return it->second;
	return nullptr;
}



TReflectionClassPropertyData* TReflectionGlobals::FindPropertyData(const std::string& inct, const std::string& inpa) {
	return FindClassEntity( GetAllPropertys(inct), inpa );
}

TReflectionClassFunctionData* TReflectionGlobals::FindFunctionData(const std::string& inct, const std::string& inpa) {
	return FindClassEntity( GetAllFunctions(inct), inpa );
}


TReflectionStringConverter* TReflectionGlobals::FindStringConverter(const std::string& tn) {
	return FindInCache(tn, m_mStringConverters);
}


TReflectionContainerManipulator* TReflectionGlobals::FindContainerManipulator(const std::string& tn) {
	return FindInCache(tn, m_mContainerManipulators);
}

TReflectionObjectCasts* TReflectionGlobals::FindCast(const std::string &tn) {
	return FindInCache(tn, m_mCasts);
}


// --------------------------------------------- //

template<class TT>
static void get_all_fields(const std::string& ct, const std::map< std::string, std::map<std::string, TT> >& fds, const std::map<std::string, TReflectionTypeFactory*>& cls, std::map<std::string, TT>& mm) {
	auto it = cls.find( ct );
	if( it!=cls.end() ) {
		for(auto aa:it->second->Parents()) get_all_fields( ToLowerCase(aa), fds, cls, mm);
	}

	auto i2 = fds.find( ct );
	if( i2!=fds.end() ) {
		const auto& src = i2->second;
		for(auto it=src.begin();it!=src.end();it++) {
			/*
			auto fid = mm.find( it->first );
			if(fid!=mm.end()) {
				_ERROR("Duplicate field: '"<<it->first<<"' in '"<<ct<<"'");
			}
			mm.insert(*it);
			*/
			mm[it->first] = it->second;
		}
	}//for
}

template<class TT>
std::map<std::string, TT*>* _GetAllClassEntitys(const std::string& cc, std::map< std::string, std::map<std::string, TT*>* >& cache, const std::map< std::string, std::map<std::string, TT*> >& fds, const std::map<std::string, TReflectionTypeFactory*>& cls) {
	std::string cl = ToLowerCase(cc);
	auto it = cache.find( cl );
	if( it!=cache.end() ) return it->second;

	auto mm = new std::map<std::string, TT*>();
	get_all_fields( cl, fds, cls, *mm);
	cache[cl] = mm;
	return mm;
}

const std::map<std::string, TReflectionClassPropertyData*>* TReflectionGlobals::GetAllPropertys(const std::string& cc) {
	return _GetAllClassEntitys(cc, m_mGetAllFieldsCache, m_mFieldData, m_mTypeFactories);
}

const std::map<std::string, TReflectionClassFunctionData*>* TReflectionGlobals::GetAllFunctions(const std::string& cc) {
	return _GetAllClassEntitys(cc, m_mGetAllFunctionsCache, m_mFunctionData, m_mTypeFactories);
}

const std::map<std::string, TReflectionClassPropertyData*>& TReflectionGlobals::GetAllPropertysInThisClassOnly(const std::string& cc) {
	return m_mFieldData[ ToLowerCase(cc) ];
}

const std::map<std::string, TReflectionTypeFactory*>& TReflectionGlobals::GetAllTypeFactories() {
	return m_mTypeFactories;
}

static bool is_child_of(const std::string& tn, const std::string& ptn, const std::map<std::string, crx::TReflectionTypeFactory*>& mm) {
	if(tn==ptn) return true;
	auto it = mm.find( tn );
	if( it==mm.end() ) return false;
	for(auto aa:it->second->Parents()) if( is_child_of(ToLowerCase(aa), ptn, mm) ) return true;
	return false;
}


bool TReflectionGlobals::IsChildOf(const std::string& tn1, const std::string& ptn1) {
	auto tn = crx::ToLowerCase( tn1 );
	auto ptn = crx::ToLowerCase( ptn1 );
	return is_child_of(tn, ptn, m_mTypeFactories);
}

void TReflectionGlobals::Fill(TReflectionObject* obj, const std::map<std::string, std::string>& data) {
	for(const auto& a:data) {
		obj->SetPropertyFromString(a.first, a.second);
	}
}


std::string TReflectionGlobals::ToString(const std::any& data) {
	auto tn = GetTypeNameFromAny(data);
	auto conv = FindStringConverter(tn);
	std::string str;
	conv->Get(str, data);
	return str;
}

void TReflectionGlobals::SetCustomStringConverter(const std::string& classType, const std::string& publishedAs, TReflectionStringConverter *conv) {
	auto prop = FindPropertyData(classType, publishedAs);
	if(!prop) _ERROR("Can't add custom string converter to "<<classType<<"::"<<publishedAs);
	prop->SetCustomStringConverter(conv);
}

TReflectionObject* TReflectionGlobals::CreateReflection(const std::string &typeName) {
	auto ff = FindTypeFactory(typeName+"*");// а то оно не через new будет создавать
	auto obj = ff->Create();
	return ff->Cast()->Cast(obj);
}

}//crx



