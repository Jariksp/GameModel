#ifndef XPARSE_TMAPCONTAINERWRAPPER_H_
#define XPARSE_TMAPCONTAINERWRAPPER_H_

#include <map>
#include <string>

namespace crx {

class TMapContainerWrapper {
	public:
	TMapContainerWrapper(const std::map<std::string, std::string>& data);
	~TMapContainerWrapper()=default;

	public:
	bool IsUseLowerCase() const;
	void UseLowerCase(bool b);


	public:
	const std::map<std::string, std::string>& Data() const;
	const std::string& Value(const std::string& name) const;


	bool IsHaveKey(const std::string& key) const;
	void CheckForKey(std::string& name) const;

	int GetInt(const std::string name, int def) const;
	int GetInt(const std::string name) const;

	bool GetBool(const std::string name, bool def) const;
	bool GetBool(const std::string name) const;

	std::string GetString(const std::string name) const;
	std::string GetString(const std::string name, const std::string& def) const;


	protected:
	const std::map<std::string, std::string>& m_mData;
	bool m_bUseLowerCase = true;
};

} /* namespace crx */

#endif /* XPARSE_TMAPCONTAINERWRAPPER_H_ */
