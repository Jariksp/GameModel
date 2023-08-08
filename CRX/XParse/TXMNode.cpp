#include <CRX/XParse/TXMNode.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>


namespace crx {

static std::string s_openComment("!--");
static std::string s_closeComment("-->");

static std::string s_abstract("abstract");

static inline bool is_space(const char c) { return (c==' ' || c=='\t' || c=='\n' || c=='\r'); }
static inline bool is_quote(const char c) { return (c=='\'' || c=='\"'); }

static inline void SkipSpaces(const std::string& str, unsigned& pos) {
	while( pos<str.size() && is_space(str[pos]) ) pos++;
}
static inline void SkipQuotes(const std::string& str, unsigned& pos) {
	char q = str[pos];
	pos++;
	while( pos<str.size() && str[pos]!=q ) pos++;
	pos++;
}


TXMNode::TXMNode() { Clear();}

TXMNode::TXMNode(const std::string& n, const std::string& c) {
	Clear();
	m_sName = n;
	m_sContent = c;
}
TXMNode::~TXMNode() { m_vNodes.clear(); }

void TXMNode::Name(const std::string& s) { m_sName = s; }
void TXMNode::Content(const std::string& s) { m_sContent = s; }
const std::string& TXMNode::Name() const { return m_sName; }
const std::string& TXMNode::RealName() const { return m_sRealName; }
const std::string& TXMNode::Content() const { return m_sContent; }
void TXMNode::ContentAsNode(std::function<bool(std::shared_ptr<TXMNode>)> hh) { m_hContentAsNode = hh; }

void TXMNode::Clear() {
	ParamsSeparator = '=';
	m_vNodes.clear();
	m_sName.clear();
	m_sContent.clear();
	Params.clear();
	m_bCXM = false;
	m_sContentContainerName = "content";
}

std::shared_ptr<TXMNode> TXMNode::Find(const std::string& nn2) {
	std::string nn = _ToLowerCase(nn2);
	for(auto aa:m_vNodes) if(aa->Name()==nn) return aa;
	return nullptr;
}


const std::string& TXMNode::DerivedFrom() const { return m_sDerivedFrom; }
bool TXMNode::IsDerived() const { return m_sDerivedFrom.size(); }
void TXMNode::DerivedFrom(const std::string& s) { m_sDerivedFrom = s; }

const std::shared_ptr<TXMNode> TXMNode::Parent() const { return m_xParent.lock(); }
std::shared_ptr<TXMNode> TXMNode::Parent() { return m_xParent.lock(); }
void TXMNode::Parent(std::shared_ptr<TXMNode> n) { m_xParent = n; }

std::shared_ptr<TXMNode> TXMNode::FindUp(const std::string& name2) {
	std::string name = _ToLowerCase(name2);
	auto pp = Find(name);
	if(pp) return pp;
	auto parent = Parent();
	if(!parent) return nullptr;
	return parent->FindUp(name);
}

std::shared_ptr<TXMNode> TXMNode::FindDown(const std::string& name2) {
	std::string name = _ToLowerCase(name2);
	auto pp = Find(name);
	if(pp) return pp;
	for(auto aa:m_vNodes) {
		auto n = aa->FindDown(name);
		if(n) return n;
	}
	return nullptr;
}

bool TXMNode::IsCXM() const { return m_bCXM; }

void TXMNode::Deriving(const std::shared_ptr<TXMNode> node) { Deriving(node->Params); }


void TXMNode::Deriving(const std::map<std::string, std::string>& con) {
	std::map<std::string, std::string> mm = con;
	Override(mm, Params);
	Params = mm;
}


void TXMNode::EraseAbstract() {
	while(true) {
		auto it = m_vNodes.begin();
		for(;it!=m_vNodes.end();it++) {
			if( (*it)->Params.find(s_abstract)==(*it)->Params.end() ) continue;
			m_vNodes.erase(it);
			break;
		}//for
		if(it==m_vNodes.end()) break;
	}//while
	for(auto it:m_vNodes) it->EraseAbstract();
}


void TXMNode::Parse(const std::string& content, bool cxm) {
	Clear();
	m_bCXM = cxm;
	if( content.empty() ) return;

	unsigned pos = 0;
	FillNode(content, pos, get_this());
	EraseAbstract();

	// это затычка - если просто параметры, без ноды, оно не парсит. ну вот так будет хоть
	if(cxm && m_vNodes.empty() && Params.empty()) {
		auto ss = std::string("<root>") + content + std::string("</root>");
		pos = 0;
		FillNode(ss, pos, get_this());
		EraseAbstract();

		auto vv = m_vNodes;
		Params = vv[0]->Params;
		m_vNodes = vv[0]->m_vNodes;
	}


}


bool TXMNode::SkipComment(const std::string& str, unsigned& pos) {
	if( (str.size()-pos)<s_openComment.size() ) return false;
	auto ptr = str.c_str() + pos;
	for(unsigned i=0;i<s_openComment.size();i++) if( ptr[i]!=s_openComment[i] ) return false;

	size_t pos2 = str.find(s_closeComment, pos);
	if (pos2 == std::string::npos) _ERROR("can't sind close node");
	pos = unsigned(pos2);
	return true;
}

void TXMNode::ContentContainerName(const std::string& name) { m_sContentContainerName = name; }

void TXMNode::GetContent(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node) {
	unsigned conBegin = pos;
	while(str[pos]!='<' && pos<str.size()) pos++;
	if(pos>conBegin) {
		std::string str2 = str.substr(conBegin, pos-conBegin);;
		if( m_hContentAsNode && m_hContentAsNode(node) ) {
			auto n2 = std::make_shared<TXMNode>();
			n2->Name(m_sContentContainerName);
			n2->Content(str2);
			n2->Parent( node );
			node->m_vNodes.push_back(n2);
		} else {
			node->m_sContent += str2;
		}
	}//if con
}

void TXMNode::EndOfNode(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node) {
	pos++;
	SkipSpaces(str, pos);
	auto end = find_first_of(str, pos, '>');
	if(end == std::string::npos) _ERROR("Can't find a charater > for the end of node")
	auto sName = FastLowerCase( str, unsigned(pos), unsigned(end-pos) );

	if(sName!=node->Name()) _ERROR("Close unknown node '"<<sName<<"'");
	pos = unsigned(end);

	if(node->IsCXM()) {
		ParseContentInCXM( node );
		node->Content("");
	}
}

void TXMNode::ParseContentInCXM(std::shared_ptr<TXMNode> innode) {
	auto separator = ParamsSeparator;

	auto v = StrTok(innode->Content(), "\n");
	for(unsigned i=0;i<v.size();i++) {
		auto str = Trim(v[i]);
		if(str[0]==';') continue;
		if(str.size()>1 && str[0]=='/' && str[1]=='/') continue;

		std::string key("");
		std::string val("");
		auto p = str.find_first_of(separator);
		key = TrimR( str.substr(0, p) );
		if(p!=std::string::npos) val = TrimL( str.substr(p+1, str.size()) );
		if(key.size()!=0) innode->Params[_ToLowerCase(key)] = val;
	}//for

}

std::shared_ptr<TXMNode> TXMNode::Clone(std::shared_ptr<TXMNode> parent) {
	auto n = std::make_shared<TXMNode>();
	*n = *this;
	n->m_vNodes.clear();
	n->m_xParent = parent;
	for(auto a:m_vNodes) n->m_vNodes.push_back( a->Clone(n) );
	return n;
}

void TXMNode::CheckInheritens(std::string& name, std::shared_ptr<TXMNode> node) {
	auto postwo = name.find_first_of(':');
	if( postwo==std::string::npos ) return;
	if( postwo>=(name.size()-1) || name[postwo+1]==':' ) return;

	auto inheritFrom = StrTok(name, ":");
	name = *inheritFrom.begin();

	for(auto i = inheritFrom.rbegin();i<(inheritFrom.rend()-1);i++) {
		auto pn = node->Parent()->FindUp( *i );
		if(!pn) _ERROR("No parent node \""<<(*i)<<"\"");
		Override( node->Params, pn->Params );
		{
			auto ait = node->Params.find( s_abstract );
			if(ait!=node->Params.end()) node->Params.erase( ait );
		}
		if(m_bCXM) {
			for(auto aa:pn->m_vNodes) node->m_vNodes.push_back(aa->Clone(node));
		}
		node->DerivedFrom( pn->Name() );
	}//for all parent

}

void TXMNode::ForceLowerCase(bool force) { m_bForceLowerCase = force; }

// return true if single tag
bool TXMNode::ParseXMNodeInner(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node) {
	auto singleNode = false;
	std::string name;

	#define CHECK() if(pos>=str.size()) break;
	unsigned p1, p2;
	unsigned v1, v2;
	for(;pos<str.size();pos++) {
		SkipSpaces(str, pos);
		CHECK();
		p1 = pos;

		for(;pos<str.size();pos++) {
			if( is_quote(str[pos]) ) SkipQuotes(str, pos);
			CHECK();
			if( is_space(str[pos]) ) break;
			if(str[pos]=='>') break; 
		}//for

		if( p1==pos ) break;

		if( str[p1]=='/' ) { singleNode = true; SkipSpaces(str, pos); break; }

		if( name.empty() ) {
			node->m_sRealName = str.substr(p1, pos-p1);
			name = _ToLowerCase(node->m_sRealName);//FastLowerCase(str, p1, pos-p1);
			CheckInheritens(name, node);
			node->Name( name );
		} else {
			if( str[p1]=='/' ) { singleNode = true; break; }


			auto end_v1 = find_first_of(str, p1, pos-p1, '=');
			v1 = unsigned(end_v1);
			if( end_v1==std::string::npos ) {
				node->Params[ FastLowerCase(str, p1, pos-p1) ];
			} else if( (v1+1)==pos ) {
				node->Params[ FastLowerCase(str, p1, pos-v1) ];
			} else {
				if(v1==p1) _ERROR("No key for param \""<<str.substr(p1, pos-p1)<<"\"");

				p2 = v1;
				v2 = pos;

				if( is_quote(str[v1+1]) && (pos-v1)>2 && str[pos-1]==str[v1+1] ) { v1++; v2--; }
				node->Params[ FastLowerCase(str, p1, p2-p1) ] = str.substr(v1+1, v2-v1-1);

			}

		}//if

		if(str[pos]=='>') break;
		CHECK();

	}//for
	#undef CHECK
	if(str[pos]=='>') pos++; 
	return singleNode;
}


void TXMNode::FillNode(const std::string& str, unsigned& pos, std::shared_ptr<TXMNode> node) {
	if(pos>=str.size()) _ERROR("Unclosed node (end of file) "<<node->Name());
	while(pos<str.size()) {
		//SkipSpaces(str, pos);
		if(pos>=str.size()) break;//end of file


		for(;pos<str.size();pos++) {
			if(pos>=str.size()) _ERROR("Unclosed node (1) "<<node->Name());

			GetContent(str, pos, node);
			if(pos>=str.size()) break;//_ERROR("Unclosed node (2) "<<node.Name());


			if(str[pos]!='<') continue;


			pos++;
			SkipSpaces(str, pos);
			if(str[pos]=='/') {
				EndOfNode(str, pos, node);
				return;
			}

			if( SkipComment(str, pos) ) continue;


			auto newNode = std::make_shared<TXMNode>();
			newNode->Parent( node );
			newNode->m_bCXM = m_bCXM;
			newNode->m_hContentAsNode = m_hContentAsNode;
			newNode->m_bForceLowerCase = m_bForceLowerCase;

			auto singledTag = ParseXMNodeInner( str, pos, newNode );
			node->m_vNodes.push_back( newNode );
				
			if(singledTag) break;


			FillNode(str, pos, *node->m_vNodes.rbegin());


		}//for

	}//while


}


std::string TXMNode::ToString() {
	if( !Name().size() ) { 
		std::string xmlStr;
		for(auto it:m_vNodes) xmlStr += it->ToString();
		return xmlStr; 
	}

	std::string xmlStr = "<"+Name();
	std::map<std::string, std::string>::iterator paramIt = Params.begin();
	for(;paramIt!=Params.end();++paramIt) xmlStr += " " + paramIt->first + "=\"" + paramIt->second + "\"";
	
	if( m_vNodes.empty() && m_sContent.empty() ) return xmlStr + " />";

	xmlStr += ">";
	xmlStr += m_sContent;

	for(auto it:m_vNodes) xmlStr += it->ToString();

	xmlStr += "</" + Name() + ">";
	return xmlStr;
}




std::string TXMNode::_ToLowerCase(const std::string &s) {
	if(!m_bForceLowerCase) return s;
	return ToLowerCase(s);
}



std::string TXMNode::FastLowerCase(const std::string& str, unsigned offset, unsigned size) {
	std::string ss;
	ss.resize( size );
	auto ptr = str.c_str() + offset;
	if(m_bForceLowerCase) {
		for(unsigned i=0;i<size;i++) ss[i] = tolower( ptr[i] );
	} else {
		for(unsigned i=0;i<size;i++) ss[i] = ptr[i];
	}
	return ss;
}


size_t TXMNode::find_first_of(const std::string& str, unsigned pos, const char c) {
	for(;pos<str.size();pos++) if(str[pos]==c) return pos;
	return std::string::npos;
}

size_t TXMNode::find_first_of(const std::string& str, unsigned pos, unsigned size, const char c) {
	size += pos;
	for(;pos<str.size()&&pos<size;pos++) if(str[pos]==c) return pos;
	return std::string::npos;
}

std::shared_ptr<TXMNode> TXMNode::get_this() { return shared_from_this(); }

}//crx
