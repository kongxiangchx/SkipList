#ifndef __LEXICALCAST_H__
#define __LEXICALCAST_H__

namespace chx {

template<class F, class T>
class LexicalCast {
public:
    T operator() (const F& v) {
        return boost::lexical_cast<T>(v);
    }
};

template<>
class LexicalCast<std::string, std::vector<std::string> > {
public:
    std::vector<std::string> operator()(const std::string& v) {
        std::string s = v.substr(1, v.length()-2);
        std::vector<std::string> vec;
        size_t start = 0;
        size_t pos = 0;
        while((pos = s.find(",", start)) != std::string::npos) {
            vec.push_back(s.substr(start, pos-start));
            start = pos+1;
        }
        vec.push_back(s.substr(start));
        return vec;
    }
};

template<>
class LexicalCast<std::vector<std::string>, std::string> {
public:
    std::string operator()(const std::vector<std::string>& v) {
        std::string s = "";
        s.append("[");
        int len = v.size();
        for(int i = 0; i < len; i++) {
            if(i) {
                s.append(",");
            }
            s.append(v[i]);
        }
        s.append("]");
        return s;
    }
};

template<>
class LexicalCast<std::unordered_set<std::string>, std::string> {
public:
    std::string operator()(const std::unordered_set<std::string>& v) {
        std::string s = "";
        s.append("{");
        for(auto it = v.begin(); it != v.end(); ++it) {
            if(it != v.begin()) {
                s.append(",");
            }
            s.append(*it);
        }
        s.append("}");
        return s;
    }
};

template<>
class LexicalCast<std::string, std::unordered_set<std::string> > {
public:
    std::unordered_set<std::string> operator()(const std::string& v) {
        std::string s = v.substr(1, v.length()-2);
        std::unordered_set<std::string> uset;
        size_t start = 0;
        size_t pos = 0;
        while((pos = s.find(",", start)) != std::string::npos) {
            uset.insert(s.substr(start, pos-start));
            start = pos+1;
        }
        uset.insert(s.substr(start));
        return uset;
    }
};

template<>
class LexicalCast<std::set<std::string>, std::string> {
public:
    std::string operator()(const std::set<std::string>& v) {
        std::string s = "";
        s.append("{");
        for(auto it = v.begin(); it != v.end(); ++it) {
            if(it != v.begin()) {
                s.append(",");
            }
            s.append(*it);
        }
        s.append("}");
        return s;
    }
};

template<>
class LexicalCast<std::string, std::set<std::string> > {
public:
    std::set<std::string> operator()(const std::string& v) {
        std::string s = v.substr(1, v.length()-2);
        std::set<std::string> set;
        size_t start = 0;
        size_t pos = 0;
        while((pos = s.find(",", start)) != std::string::npos) {
            set.insert(s.substr(start, pos-start));
            start = pos+1;
        }
        set.insert(s.substr(start));
        return set;
    }
};

template<>
class LexicalCast<std::unordered_map<std::string, std::string>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, std::string>& v) {
        std::string s = "";
        s.append("{");
        for(auto it = v.begin(); it != v.end(); ++it) {
            if(it != v.begin()) {
                s.append(",");
            }
            s.append((*it).first);
            s.append(":");
            s.append((*it).second);
        }
        s.append("}");
        return s;
    }
};

template<>
class LexicalCast<std::string, std::unordered_map<std::string, std::string> > {
public:
    std::unordered_map<std::string, std::string> operator()(const std::string& v) {
        std::string s = v.substr(1, v.length()-2);
        std::unordered_map<std::string, std::string> hash;
        size_t start = 0;
        size_t pos = 0;
        while((pos = s.find(",", start)) != std::string::npos) {
            std::string str = s.substr(start, pos-start);
            size_t pos_str = str.find(":");
            hash[str.substr(0,pos_str)] = str.substr(pos_str+1);
            start = pos+1;
        }
        std::string str = s.substr(start, pos-start);
        size_t pos_str = str.find(":");
        hash[str.substr(0,pos_str)] = str.substr(pos_str+1);
        return hash;
    }
};
    
}


#endif