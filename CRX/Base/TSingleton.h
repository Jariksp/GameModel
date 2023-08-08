#ifndef CRX_BASE_TSINGLETON_H_
#define CRX_BASE_TSINGLETON_H_

#include <CRX/Base/TDebug.h>

#include <typeinfo>
#include <mutex>

namespace crx {

template<class T>
struct TBaseSingleton {
	public:
	static bool IsInstance() {
		//lock_guard<recursive_mutex> guard(m_xSingletonMutex);
		return m_pSingletonInstance!=nullptr;
	}

	protected:
	static void NullifyInstance() {
		m_pSingletonInstance = nullptr;
	}


	protected:
	inline static T* m_pSingletonInstance = nullptr;
	inline static std::recursive_mutex m_xSingletonMutex;
};


template<class T>
class TCreativeSingleton : public TBaseSingleton<T> {
	public:
	static T* GetInstance() {
		//lock_guard<recursive_mutex> guard(TBaseSingleton<T>::m_xSingletonMutex);
		if(!TBaseSingleton<T>::m_pSingletonInstance) TBaseSingleton<T>::m_pSingletonInstance = new T();
		return TBaseSingleton<T>::m_pSingletonInstance;
	}

	static void FreeInstance() {
		//lock_guard<recursive_mutex> guard(TBaseSingleton<T>::m_xSingletonMutex);
		if(TBaseSingleton<T>::m_pSingletonInstance) { delete TBaseSingleton<T>::m_pSingletonInstance; TBaseSingleton<T>::NullifyInstance(); }
	}

	protected:
	static void NullifyInstance() {
		TBaseSingleton<T>::m_pSingletonInstance = nullptr;
	}

};


template<class T>
class THolderSingleton : public TBaseSingleton<T> {
	public:
	static T* GetInstance() {
		//lock_guard<recursive_mutex> guard(TBaseSingleton<T>::m_xSingletonMutex);
		if(!TBaseSingleton<T>::m_pSingletonInstance) _ERROR("THolderSingleton: No value in GetInstance");
		return TBaseSingleton<T>::m_pSingletonInstance;
	}


	static void FreeInstance() {
		//lock_guard<recursive_mutex> guard(TBaseSingleton<T>::m_xSingletonMutex);
		TBaseSingleton<T>::NullifyInstance();
	}

	protected:
	static void SetInstance(T* v) {
		//lock_guard<recursive_mutex> guard(TBaseSingleton<T>::m_xSingletonMutex);
		TBaseSingleton<T>::m_pSingletonInstance = v;
	}

};

}//crx




#endif /* BASE_TSINGLETON_H_ */