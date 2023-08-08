#include <CRX/Base/TParseFormat.h>

#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Reflection/TReflectionObjectCasts.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionGlobals.h>

#include <CRX/Base/TStringUtils.h>

namespace crx {
	
//static std::string s_sVariadicPropertyDelimeter = "|";
static std::string s_sDotDelimeter = ".";

static bool ParseImplementation(std::pair<TReflectionObject*, TReflectionClassPropertyData*>& data,
								TReflectionObject* owner,
								const std::vector<std::string>& relationPropertyData) {
	if(!owner) return false;

	for(auto& propertyName:relationPropertyData) {
		auto propertyData = owner->FindProperty(propertyName);
		if(!propertyData) return false;

		if( relationPropertyData.size()==1 ) {
			data = std::make_pair(owner, propertyData);

		} else {
			std::any var;
			owner->GetPropertyData(propertyName, var);

			auto isReflectable = TReflectionGlobals::GetInstance()->IsChildOf( propertyData->ClassType(), GetTypeName<TReflectionObject>() );

			if(isReflectable) {
				auto newTokRelation = relationPropertyData;
				RemoveFromVector(newTokRelation, propertyName);

				auto converter = propertyData->Cast();
				auto refl = converter?converter->Cast(var):nullptr;

				return ParseImplementation( data, refl, newTokRelation );

			} else {
				return ParseImplementation( data, owner, {propertyName} );
			}
		}
	} // for(propertyName)
	return true;
}

static bool IsContainDefaultValue(const std::pair<TReflectionObject*, TReflectionClassPropertyData*>& data) {
	auto owner = data.first;
	auto property = data.second;
	if(!owner || !property) return true;

	auto str = owner->GetPropertyToStringNoError( property->Name() );
	if( str.empty() ) return true;

	return crx::TFixPoint::FromString( GreedFixPointFormat(str) )==crx::TFixPoint{};
}

static std::vector<std::string> TryStrTok(const std::string& text, const std::string& delimeter) {
	auto tokens = crx::StrTok(text, delimeter);
	if( tokens.empty() ) tokens.push_back(text);
	return tokens;
}

std::map<std::string, std::pair<crx::TReflectionObject*, crx::TReflectionClassPropertyData*>> TParseFormat::Parse(	crx::TReflectionObject* owner,
																													const std::string& relationPropertyDataText,
																													const std::vector<std::string>& relationPropertyFoldData) {
	std::map<std::string, std::pair<crx::TReflectionObject*, crx::TReflectionClassPropertyData*>> data;

	if(!owner) return data;

	auto relationPropertyNames = crx::SubStrsBetween(relationPropertyDataText, OpenTokParse, CloseTokParse);

	for(auto& relationPropertyFullName:relationPropertyNames) {
		// for property "a.b.c|d.e" will generate
		// { "a.b.c|d.e", {{"a", "b", "c"}, {"d", "e"}} }
		auto relationProperty = relationPropertyFullName;

		for(auto& relationPropertyFold: relationPropertyFoldData) {
			auto foldPosition = relationProperty.find(relationPropertyFold);
			if( foldPosition!=std::string::npos ) relationProperty.replace(foldPosition, relationPropertyFold.size(), "");
		}

		// uncomment to allow variadic property
		// auto relationPropertyNameData = TryStrTok(relationProperty, s_sVariadicPropertyDelimeter); 
		std::vector<std::string> relationPropertyNameData = {relationProperty};

		for(auto& relationPropertyName:relationPropertyNameData) {
			auto relationPropertyData = TryStrTok(relationPropertyName, s_sDotDelimeter);

			std::pair<TReflectionObject*, TReflectionClassPropertyData*> propertyData;
			if( !ParseImplementation(propertyData, owner, relationPropertyData) ) continue;
			
			if( !data.count(relationPropertyFullName) || !IsContainDefaultValue(propertyData) ) {
				data[relationPropertyFullName] = propertyData;
			}
		} // for(relationPropertyName)
	} // for(relationPropertyFullName)

	return data;
}

static std::string ParseFormatImplementation(const std::string& relationPropertyName,
						const std::pair<TReflectionObject*, TReflectionClassPropertyData*>& data,
						const std::string& relationPropertyDataText,
						std::function<bool(std::string&, TReflectionObject*)> converter,
						const std::string& openTok,
						const std::string& closeTok) {
	std::string propertyStringData;

	auto object = data.first;
	auto property = data.second;

	if( object && property && property->StringConverter() ) {
		std::any variable;
		property->Get(object, variable);

		propertyStringData = object->GetPropertyToString(property);
	}

	auto acceptFormat = true;
	if(converter) acceptFormat = converter(propertyStringData, object);

	return acceptFormat?crx::ReplaceAll(relationPropertyDataText, openTok+relationPropertyName+closeTok, propertyStringData):relationPropertyDataText;
}

std::string TParseFormat::Format(	const std::map<std::string, std::pair<crx::TReflectionObject*, 
									crx::TReflectionClassPropertyData*>>& data,
									const std::string& relationPropertyDataText,
									std::function<bool(std::string&, TReflectionObject*)> converter) {

	auto result = relationPropertyDataText;
	for(auto& [propertyName, propertyData]:data) {
		result = ParseFormatImplementation(propertyName, propertyData, result, converter, OpenTokParse, CloseTokParse);
	}

	return result;
}

std::string TParseFormat::ParseFormat(	crx::TReflectionObject* owner,
										const std::string& relationPropertyDataText,
										std::function<bool(std::string&, TReflectionObject*)> converter,
										const std::vector<std::string>& relationPropertyFoldData) {
	
	auto formatedText = relationPropertyDataText;
	return Format( Parse(owner, formatedText, relationPropertyFoldData), formatedText, converter );
}

std::function<bool(std::string&, TReflectionObject*)> ArithmeticParseFormatConverter(bool forceFormat, const std::string& format) {
	return [forceFormat, format](std::string& str, TReflectionObject*) {
		if( str.empty() ) return false;
		str = crx::GreedFixPointFormat(str);

		auto acceptFormat = forceFormat || crx::TFixPoint::FromString(str)!=TFixPoint{};
		if(acceptFormat) str = crx::Format( format, str.c_str() );

		return acceptFormat;
	};
}

} /* namespace crx */
