#ifndef CRX_TXMNODEH_H_
#define CRX_TXMNODEH_H_

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>

namespace crx {

class TXMNode : public std::enable_shared_from_this<TXMNode> {
	public:
	TXMNode();
	TXMNode(const std::string& name, const std::string& content);	std::shared_ptr<TXMNode> operator[](unsigned index);
	const std::shared_ptr<TXMNode> operator[](unsigned index) const;

	virtual ~TXMNode();

	public:
	std::shared_ptr<TXMNode> get_this();

	public:
	std::shared_ptr<TXMNode> Find(const std::string& nodeName);
	void Clear();
	inline unsigned Size() const { return (unsigned)m_vNodes.size(); }
	inline std::vector<std::shared_ptr<TXMNode>>& Nodes() { return m_vNodes; }
	inline std::shared_ptr<TXMNode> At(unsigned x) { return m_vNodes[x]; }
	inline const std::shared_ptr<TXMNode> At(unsigned x) const { return m_vNodes[x]; }
	std::shared_ptr<TXMNode> Clone(std::shared_ptr<TXMNode> parent);

	public:
	// устанавливает колбек, если он возвращает true, то внутренности ноды нужно так же распарсить как ноду. нужно где-то в парсере HTML
	void ContentAsNode(std::function<bool(std::shared_ptr<TXMNode>)> hh);
	void ForceLowerCase(bool force);


	virtual void Parse(const std::string& content, bool load_as_cxm=false);
	virtual std::string ToString();
	void ContentContainerName(const std::string& name);

	public:
	std::map<std::string, std::string> Params;
	char ParamsSeparator;
	
	void Name(const std::string&);
	void Content(const std::string&);
	void DerivedFrom(const std::string&);
	const std::string& Name() const;
	const std::string& RealName() const;
	const std::string& Content() const;
	const std::string& DerivedFrom() const;
	bool IsDerived() const;

	const std::shared_ptr<TXMNode> Parent() const;
	std::shared_ptr<TXMNode> Parent();
	void Parent(std::shared_ptr<TXMNode> p);

	std::shared_ptr<TXMNode> FindUp(const std::string& name);
	std::shared_ptr<TXMNode> FindDown(const std::string& name);

	bool IsCXM() const;

	void Deriving(const std::shared_ptr<TXMNode> node);
	void Deriving(const std::map<std::string, std::string>& m);

	void EraseAbstract();


	protected:
	void FillNode(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node);
	void GetContent(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node);
	void CheckInheritens(std::string& name, std::shared_ptr<TXMNode> node);
	bool SkipComment(const std::string& str, unsigned& pos);
	void EndOfNode(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node);
	bool ParseXMNodeInner(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node);

	std::string _ToLowerCase(const std::string& s);

	void ParseContentInCXM(std::shared_ptr<TXMNode> innode);

	size_t find_first_of(const std::string& str, unsigned pos, const char c);
	size_t find_first_of(const std::string& str, unsigned pos, unsigned size, const char c);
	std::string FastLowerCase(const std::string& str, unsigned offset, unsigned size);


	protected:
	std::weak_ptr<TXMNode> m_xParent;
	std::string m_sName;
	std::string m_sRealName;
	std::string m_sContent;
	std::string m_sDerivedFrom;
	bool m_bCXM = false;
	bool m_bContentAsNode = false;
	std::string m_sContentContainerName;
	std::vector<std::shared_ptr<TXMNode>> m_vNodes;
	std::function<bool(std::shared_ptr<TXMNode>)> m_hContentAsNode = nullptr;
	bool m_bForceLowerCase = true;
};

}//crx






#endif
