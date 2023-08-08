#ifndef CRX_REFLECTION_TREFLECTIONTWOSTEPXMLSERIALIZER_H_
#define CRX_REFLECTION_TREFLECTIONTWOSTEPXMLSERIALIZER_H_


#include <map>
#include <vector>
#include <any>
#include <functional>
#include <string>
#include <CRX/Reflection/NNSerializerFlags.h>
#include <string>


namespace crx {

class TReflectionTypeFactory;
class TReflectionObject;
class TReflectionObjectCasts;
class TReflectionStringConverter;
class TReflectionClassPropertyData;

// этот класс берет TReflectionObject и сохраняет его в XML, при этом он делает 2 прохода - отдельно сохраняет именно все TReflectionObject
// а только потом уже все остальное, подставляет указатели вместо TReflectionObject. Таким образом оно может сохранить зацикленные ссылки
// и все указатели на один объект будут указывать на один объект
//
// НО! Сделано там внутри немного (на самом деле много) через жопку, нужно бы переписать, как руки дойдут
//
// добавил возможность кастомного определния StringConverter. Это необходимо для реализации сетевой передачи данных, когда
// в некоторых случаях нужно передаать полностью весь объект, а в некоторых - только его NetworkID

class TReflectionTwoStepXMLSerializer  {
	public:
	TReflectionTwoStepXMLSerializer()=default;
	virtual ~TReflectionTwoStepXMLSerializer()=default;


	public:
	std::string Data();

	public:
	void Save(const std::string& name, std::any data, NSerializerFlags flags=NNSerializerFlags::None);
	void Clear();


	public:
	// позволяет для выборочных TReflectionObject подменить полный парс на StringConverter
	std::function<TReflectionStringConverter*(TReflectionObject*)> FindStringConverter;

	protected:
	struct SData {
		SData() = default;
		SData(TReflectionObject* o, TReflectionObjectCasts* c);
		TReflectionObject* Object = nullptr;
		TReflectionObjectCasts* Cast = nullptr;
		std::any Any;
	};


	protected:
	void AddLine(const std::string& line, int addTabs=0);

	void SaveAllPtrsRecursive(SData* sdata);
	void SaveFull(TReflectionObject* d, TReflectionObjectCasts* cast);
	void SaveReference(const std::string& name, TReflectionObject* d, bool useKey=false, const std::string& keyString="");
	void SaveByData(const std::string& name, std::any& data, bool useKey=false, const std::string& keyString="");

	void BeforSave(const std::string& name, const std::string& params="");
	void AfterSave();

	void IsAcceptableType(TReflectionTypeFactory* fac, const std::string& fieldName);

	void AddLineString(const std::string& name, const std::string& data, bool useKey, const std::string& keyString);

	protected:
	TReflectionStringConverter* DoFindStringConverter(TReflectionTypeFactory* t, std::any& data);
	TReflectionStringConverter* DoFindStringConverter(TReflectionTypeFactory* t, TReflectionClassPropertyData* cf, TReflectionObject* in);

	int Index(TReflectionObject* obj);

	protected:
	std::vector<std::string> m_vLevel;
	std::string m_sData;
	std::map<uintptr_t, SData*> m_mObjects;

	std::map<TReflectionObject*, int> m_mIdxes;

};

}//crx

#endif /* REFLECTION_TDISPATCHXMLSERIALIZER_H_ */
