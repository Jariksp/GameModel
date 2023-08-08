#ifndef CRX_TVFSROOT_H_
#define CRX_TVFSROOT_H_

#include <CRX/Base/TSingleton.h>
#include <string>
#include <vector>


namespace crx {

// ну это уже не совсем VFS, просто есть указатель на корень, откуда брать данные
// и ещё %Locale% заменяет. Может в будущем таки допишу ноды, чтобы брало из zip

class TVFSRoot : public TCreativeSingleton<TVFSRoot> {
	public:
	TVFSRoot()=default;
	virtual ~TVFSRoot()=default;

	void SetLocale(const std::string& l);

	public:
	// all pathes in VFS
	bool IsExists(const std::string& fn);
	bool IsDirectory(const std::string& fn);
	bool IsFile(const std::string& fn);

	bool IsFileExists(const std::string& fn);
	bool IsDirectoryExists(const std::string& fn);

	std::string FindExists(const std::vector<std::string>& in);// проверяет, есть ли в VFS такое вхождение. возвращает первое найденное

	std::vector<std::string> FilesList(const std::string& fn);

	public:// указываем папку и имя файла БЕЗ расширения. Ищут первое вхождение с подходящим расширение
	std::string FindImage(const std::string dir, const std::string& fileName);// .jpg, .jpeg, .png
	std::string FindVideo(const std::string dir, const std::string& fileName);// .mp4, .avi
	std::string FindAudio(const std::string dir, const std::string& fileName);// .ogg, .wav

	std::string FindWithExtention(const std::string dir, const std::string& fileName, const std::vector<std::string>& exts);// .ogg, .wav

	std::string LoadTxtFile(const std::string& fn);
	void SaveTxtFile(const std::string& fn, const std::string& data);
	void SaveTxtFile(const std::string& dir, const std::string& fn, const std::string& data);

	// загуржает файл из VFS. Находит все вхождения вида
	// #include fileName (перевод строки обязателен!)
	// и заменяет его на содержимое указанного файла.
	// если fileName начинается с /, то это абсолютный путь
	// если не с /, то это путь относительно файла, в котром найдем #include
	std::string LoadWithIncludes(const std::string& fileName, bool checkForComments=true);

	public:
	void BasePath(const std::string& s);
	const std::string& BasePath();
	std::string PrepareFileName(const std::string& fn);// IN - path in VFS, out - path in real file system



	public:
	static std::string PrepareVFSFileName(const std::string& infn);

	protected:

	protected:
	std::string m_sBasePath;
	std::string m_sLocale;

};

}//crx

#endif /* TVFSROOT_H_ */
