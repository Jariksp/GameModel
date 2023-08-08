#ifndef CRX_TREFLECTIONIMPLDEFS0_H_H_
#define CRX_TREFLECTIONIMPLDEFS0_H_H_

#include <CRX/Reflection/TReflectionImplTempalates.h>




// тут собраны разные дефайны и кое-какие темплейтные методы, которые облегчают жизнь в задании таблицы рефлекшена


#define REFLECTION_TABLE_TYPE_DEFINES(ClassType, ParentClassType) \
	typedef ClassType TClassType; \
	typedef ParentClassType TParentClassType;\



#define BEGIN_REFLECTION_TABLE_FUNCTION_HEADER() \
	if( !crx::TReflectionTypeFactory::CheckAndSetClassTypeAlreadyInited(crx::GetTypeName<TClassType>()) ) return; \
	auto classTypeFactory = crx::impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<TClassType>();\
	crx::TReflectionTypeFactory::Add( crx::impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<TClassType*>() );\
	crx::TReflectionTypeFactory::Add( crx::impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<std::shared_ptr<TClassType>>() );\
	crx::TReflectionTypeFactory::Add( crx::impl::SelectTemplateFactoryByPointerNonPointerSharedPtrType<std::weak_ptr<TClassType>>() );\
	crx::impl::AddCastToReflectionObject<TClassType>();\
	TClassType* class_ptr = 0;\
	std::string class_type = crx::GetTypeName<TClassType>();\
	(void)class_ptr;\
	crx::impl::TryCallInitReflectionTable<TParentClassType*>();\
	classTypeFactory->AddParent( crx::GetTypeName<TParentClassType>() );\
	crx::TReflectionTypeFactory::Add(classTypeFactory);\




#define BEGIN_REFLECTION_TABLE_STD(ClassType) \
	crx::TCallFunctionBeforMain ClassType::CallInitReflectionTable( &ClassType::InitReflectionTable );\
void ClassType::InitReflectionTable() {\



#define BEGIN_REFLECTION_TABLE(ClassType, ParentClassType) \
	BEGIN_REFLECTION_TABLE_STD(ClassType)\
	REFLECTION_TABLE_TYPE_DEFINES(ClassType, ParentClassType)\
	BEGIN_REFLECTION_TABLE_FUNCTION_HEADER();


#define BEGIN_REFLECTION_TABLE_TEMPLATE(ClassType, ParentClassType) \
	static void InitReflectionTable() {\
	REFLECTION_TABLE_TYPE_DEFINES(ClassType, ParentClassType)\
	BEGIN_REFLECTION_TABLE_FUNCTION_HEADER();



// это хитрая штука - позволяет инициализировать таблицу темплейта в .CPP
#define BEGIN_REFLECTION_TABLE_TEMPLATE_IMPL(FN_NAME) \
	template<typename TClassType, typename TParentClassType> void FN_NAME() {\
	BEGIN_REFLECTION_TABLE_FUNCTION_HEADER();


// ебаная хуйня - для доступа к полю нужно брать от указателя на класс, а для доступа к методу - он типа класса
// в результате нельзя сделать один дефайн для всего, приходится делать два

#define ADD_PROPERTY(VisibleName, InternalName) \
	{ \
		crx::impl::CreateClassPropertyData<TClassType>(#VisibleName, class_ptr->InternalName, class_ptr->InternalName);\
		crx::impl::AddVectorManipulatorAndTypeFactoryIfNeed(class_ptr->InternalName);\
		crx::impl::AddMapManipulatorAndTypeFactoryIfNeed(class_ptr->InternalName);\
	}\


#define ADD_FUNCTION_PROPERTY(VisibleName, InternalName) \
	{ \
		crx::impl::CreateClassPropertyData<TClassType>(#VisibleName, &TClassType::InternalName, &TClassType::InternalName);\
	}\


#define ADD_PARENT_CLASS(ParentClassType) \
	{\
		crx::impl::TryCallInitReflectionTable<ParentClassType*>();\
		classTypeFactory->AddParent( crx::GetTypeName<ParentClassType>() );\
	}\



#define ADD_ENUM_TYPE(TypeName) crx::impl::TryAddEnumReflectionStringConverver<TypeName>();


#define ADD_CLASS_FUNCTION(VisibleName, InternalName) \
	{ \
		crx::impl::CreateClassFunctionData<TClassType>(#VisibleName, &TClassType::InternalName);\
	}\



#define _ADD_PROPERTY(InternalName) ADD_PROPERTY(InternalName, InternalName);
#define _ADD_FUNCTION_PROPERTY(InternalName) ADD_FUNCTION_PROPERTY(InternalName, InternalName);
#define _ADD_CLASS_FUNCTION(InternalName) ADD_CLASS_FUNCTION(InternalName, InternalName);

#define ADD_TYPE_FACTORY(TypeName) \
	{ \
		crx::impl::AddReflectionTypeFactoryTemplateIfNo<TypeName>(); \
	}

#define ADD_STRICTED_FUNCTION_PROPERTY(VisibleName, InternalName) \
	{ \
		crx::impl::CreateClassPropertyData<TClassType>(#VisibleName, &TClassType::InternalName);\
	}


#define ADD_FUNCTION_BOOL_PROPERTY(VisibleName, InternalName) \
	{ \
		crx::impl::CreateClassPropertyData<TClassType>(#VisibleName, &TClassType::Is##InternalName, &TClassType::InternalName);\
	}


#define _ADD_FUNCTION_BOOL_PROPERTY(InternalName) ADD_FUNCTION_BOOL_PROPERTY(InternalName, InternalName);
#define _ADD_STRICTED_FUNCTION_PROPERTY(InternalName) ADD_STRICTED_FUNCTION_PROPERTY(InternalName, InternalName);

// это объект с отложенным создание. необходимо добавить ссылку на поле, где он хранится, чтобы чекуть в серелизаторе
// и если nullptr, то не дергать
#define ADD_WEAK_PTR_PROPERTY(VisibleName, Function, InternalName) \
	{ \
		crx::impl::CreateClassPropertyDataWithWeak<TClassType>(#VisibleName, &TClassType::Function, &TClassType::Function, class_ptr->InternalName);\
	}



#define SET_CUSTOM_FROM_STRING(VisibleName, InternalName, FromString) \
	{ \
		crx::impl::SetCustomFromString<TClassType>(#VisibleName, class_ptr->InternalName, FromString);\
	}\


#define _SET_CUSTOM_FROM_STRING(InternalName, FromString) SET_CUSTOM_FROM_STRING(InternalName, InternalName, FromString);

#define END_REFLECTION_TABLE() }






#endif

/*




#define ADD_TYPE_FACTORY(TypeName) \
	{ \
		TTypeFactory::AddTypeFactory( new TTypeFactoryTemplate<TypeName>() ); \
	}






#define ADD_WEAK_PTR_PROPERTY(VisibleName, FunctionGet, InternalName) \
	{ \
		auto cls = CreateClassPropertyData<TClassType>(#VisibleName, &TClassType::FunctionGet);\
		cls->SetWeakPtrGet( CreatePropertyWeakPtr<TClassType>(class_ptr->InternalName) );\
	}








#define ADD_VECTOR_MANIPULATOR(ElementTypeName) \
	TContainerManipulator::AddContainerManipulator( new TVectorManipulator<ElementTypeName >() );

#define ADD_MAP_MANIPULATOR(KeyTypeName,ElementTypeName) \
	TContainerManipulator::AddContainerManipulator( new TMapManipulator<KeyTypeName,ElementTypeName >() );

#define ADD_VECTOR_MANIPULATOR_AND_TYPE_FACTORY(ElementTypeName) \
		ADD_VECTOR_MANIPULATOR(ElementTypeName);\
		ADD_TYPE_FACTORY(vector<ElementTypeName>);








*/

