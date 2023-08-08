#ifndef BASE_THANDLERSPOOL_H_H_
#define BASE_THANDLERSPOOL_H_H_


namespace crx {

template<typename T>
class THandlersPool {
	protected:
	std::vector<std::pair<void*, T>> m_vPool;
		
	public:
	void Clear() {
		m_vPool.clear();
	}
		
	template<typename... Args>
	void SendHandlers(Args... args) {
		auto vv = m_vPool;
		for(auto& h : vv) h.second(args...);
	}
		
	void SendAndClearHandlers() {
		auto vv = m_vPool;
		m_vPool.clear();
		for(auto& h : vv) h.second();
	}
		
	void AddHandler(void* owner, T h) {
		m_vPool.emplace_back(owner, h);
	}
		
	void RemoveHandler(void* owner) {
		for(auto i = 0; i<int(m_vPool.size()); i++) {
			if(m_vPool[i].first == owner) {
				m_vPool.erase(m_vPool.begin() + i);
				break;
			}
		}
	}
		
	void RemoveAllHandlers(void* owner) {
		for(auto i = 0; i<int(m_vPool.size()); i++) {
			if(m_vPool[i].first == owner) {
				m_vPool.erase(m_vPool.begin() + i);
				i--;
			}
		}
	}
};
}

#endif
