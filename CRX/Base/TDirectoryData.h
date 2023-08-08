#ifndef BASE_TDIRECTORYDATA_H_
#define BASE_TDIRECTORYDATA_H_

#include <vector>
#include <string>
#include <memory>

namespace crx {


// загружает данные из VFS. Строит дерев директорий и файлов. Предназначено для
// упрощения всяких там поисков речи юнитов или картинок или ещё чего. Закидываешь в
// Find что-то типа {ToString(Country), ToString(UnitClass), ToString(SpeechType)} и получаешь ноду с файлами

class TDirectoryData : public std::enable_shared_from_this<TDirectoryData> {
	public:
	TDirectoryData()=default;
	virtual ~TDirectoryData()=default;

	public:
	std::vector<std::shared_ptr<TDirectoryData>> Subdirs;
	std::vector<std::string> Files;

	public:
	std::shared_ptr<TDirectoryData> Find(const std::vector<std::string>& path);

	public:
	const std::string& Name();
	const std::string& Path();

	std::vector<std::string> FilesWithFullPath();
	const std::string& RandomFile();
	std::string RandomFileWithFullPath();
	std::string RandomFileWithFullPath(const std::vector<std::string>& path);

	public:
	static std::shared_ptr<TDirectoryData> Load(const std::string& path);

	protected:
	void _Load(const std::string& path);


	protected:
	std::weak_ptr<TDirectoryData> m_xParent;
	std::string m_sName;
	std::string m_sPath;

};

} /* namespace crx */

#endif /* BASE_TDIRECTORYDATA_H_ */
