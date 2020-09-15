//
// Created by Zhou,Baochuan on 18/6/6.
//

#include "relation.h"
#include <gtest/gtest.h>

using namespace work;

Relation::Relation() {
    this->init_map_family();
    this->init_map_cache_had_relation();
    this->_http = new Http();
}

Relation::~Relation() {
    delete this->_http;
}

void Relation::add_id_url(string id, string url) {
    if (this->_map_id_url.count(id) > 0) {
        return;
    }
    this->_map_id_url[id] = url;
}

// 映射角色关系:关系=>逆向关系
void Relation::init_map_family() {
    string file_path = "./data/relation/D_map_family";
    ifstream in(file_path.c_str());
    ASSERT_FALSE(in.fail());
    string line;
    string relation_name; // 关系名
    string to_reverse_relation_name; // 对应的反向关系名
    vector<string> ret;
    int index = 0;
    while(getline(in, line, '\n')){
        if (index == 0) {
            index++;
            continue;
        }
        split(ret, line, boost::is_any_of("\t"));
        relation_name= ret[0];
        to_reverse_relation_name = ret[1];
        this->_map_family[relation_name] = to_reverse_relation_name;
    }
    in.close();

    in.open("./data/relation/F_map_family");
    ASSERT_FALSE(in.fail());
    while(getline(in, line, '\n')){
        split(ret, line, boost::is_any_of("\t"));
        this->_filter_map_family[ret[0]] = ret[0];
    }
    in.close();

}
// 初始化已有三元关系
void Relation::init_map_cache_had_relation() {
    string file_path = "data/relation/tmp_relationship_modules";
    ifstream in(file_path.c_str());
    ASSERT_FALSE(in.fail());
    string line, key;
    vector<string> ret;
    ptree root;
    while(getline(in, line, '\n')){
        split(ret, line, boost::is_any_of("\t"));
        if (ret.size() != 4) {
            continue;
        }

        try {
            stringstream str_stream(ret[3]);
            ptree root;
            read_json(str_stream, root);

            BOOST_FOREACH(ptree::value_type &v, root) {
                key = ret[1] + "_" + v.second.get_child("relate_id").get_value<string>();
                this->_map_cache_had_relation[key] = key;
            }
        } catch (ptree_error &e){
            std::cout<< "error:[" << e.what() << "]lemmaName=" << ret[0] <<endl;
        }
        ret.clear();
    }
    in.close();
}


string Relation::added_id_by_kg(const string target, const string name) {
    ptree root;
    read_ini("conf/work.ini", root);
    string url = root.get<string>("relationKg.url") + target + name;
    string response = conv::between(this->_http->get(url), "UTF-8", "GBK");
    string id, desc;

    try {
        stringstream str_stream(response);
        ptree root;
        read_json(str_stream, root);
        bool is_person_type = false; //是否是人物类词条
        double confidence = 0.0;
        BOOST_FOREACH(ptree::value_type &v, root.get_child("egl_ret")) {
            if (!equals(v.second.get_child("mention").get_value<string>(), name)) {
                continue;
            }
            BOOST_FOREACH(ptree::value_type &v1, v.second.get_child("category")) {
                if (v1.second.get_value<string>().find("人物", 0) != string::npos) {
                    is_person_type = true;
                }
            }
            if (!is_person_type) {
                continue;
            }

            if (v.second.get_child("confidence").get_value<double>() > confidence ) {
                id = v.second.get_child("_bdbkKgId").get_value<string>();
                confidence = v.second.get_child("confidence").get_value<double>();
            }
        }
    } catch (ptree_error &e){
        std::cout<< "error:[" << e.what() << "]url=" << url <<endl;
    }

    return id;
}

string filterSpecial(const string text, const string startMark, const string endMark)
{
    size_t start, end, length;
    string ret;
    if ((start = text.find(startMark)) != string::npos) {
        if ((end = text.find(endMark)) != string::npos) {
            if (start != 0) {
                ret = text.substr(0, start);
            }

            end = end + endMark.length();
            length = text.length() - end;
            ret += text.substr(end, length);
        }
    }
    return ret;
}

void Relation::output(ptree root, string L, vector<map<string, string> > v_m_P2)
{
    try{
        string P1 = root.get<string>("lemmaTitle");
        string id, key, P2, url;
        vector<string> splitVecP2;
        //boost::regex reg("\uff08[^\uff09)]+\uff09");
        for (int i = 0, size = v_m_P2.size(); i < size; i++) {
            for (map<string, string>::iterator iter = v_m_P2[i].begin(); iter != v_m_P2[i].end(); iter++) {
                if (iter->second.find("（") == string::npos) {
                    P2 = iter->second;
                } else {
                    P2 = filterSpecial(iter->second, "\uff08", "\uff09");
                }
                if (P2.empty()) {
                    continue;
                }

                id = iter->first;
                if (id.empty() || equals(id, "null")) {
                    id =  added_id_by_kg(P1, P2);
                }
                if (id.empty()) {
                    continue;
                }
                // 已有的关系就不做导出
                key = root.get<string>("lemmaId") + "_" + id;
                if (this->_map_cache_had_relation.count(key) > 0) {
                    continue;
                } else {
                   this->_map_cache_had_relation[key] = key;
                }


                // 如果命中特有关系，check下是否是合法关系
                if (!this->is_valid_map(P1, L, P2)) {
                    continue;
                }
                cout << root.get<string>("lemmaId") << "\t" << P1 << "\t";
                cout << root.get<string>("url") << "\t" << L << "\t";
                cout << P2 << "\t" << id << "\t";
                if (this->_map_id_url.count(id) > 0) {
                    url = this->_map_id_url[id];
                } else {
                   url =  "https://baike.baidu.com/item/" + P2 + "/" + id;
                }
                cout << url << endl;
            }
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}

// 单行规则提取
string Relation::extract_single_L(ptree::value_type &node) {
    string L;
    string tag = node.second.get_child("tag").get_value<string>();
    if (!equals(tag, "paragraph")) {
        return L;
    }

    // 遍历数组中的元素
    vector< wstring > splitVec;
    wstring sep = _W("：");
    string text;
    unsigned int i = 0;
    BOOST_FOREACH (ptree::value_type& v, node.second.get_child("content")) {
        i++;
        if (i > 1) {
            break;
        }

        if (equals(v.second.get_child("tag").get_value<string>(), "text")) {
            if (text.length() > 40) {
                break;
            }
            text = StringUtil::strip_tag(v.second.get_child("text").get_value<string>());
            wstring line = _W(text);
            boost::split(splitVec, line, boost::is_any_of(sep));
            if (splitVec.size() > 1) {
                L = StringUtil::wstring2string(splitVec[0]);
                if (this->_map_family.count(L) > 0) {
                    return L;
                }
            }
            splitVec.clear();
        }
    }
    return "";
}
vector<RelationMap> Relation::regular_extract_single_line(const string &text) {

    vector<RelationMap> ret;
    stringstream str_stream(text);
    try {
        ptree root;
        read_json(str_stream, root);
        string P1 = root.get_child("lemmaTitle").get_value<string>();
        string L, P2;
        // 卡片提取
        if (!root.get_child("card").empty()) {
            BOOST_FOREACH(ptree::value_type &v1, root.get_child("card")) {
                if (this->_map_family.count(v1.second.get_child("name").get_value<string>()) > 0) {
                    L = v1.second.get_child("name").get_value<string>();
                    vector<map<string, string> > v_m_P2;
                    BOOST_FOREACH(ptree::value_type &v2, v1.second.get_child("value")) {
                            string value = v2.second.get_value<string>();
                            if (value.find("、", 0) == string::npos) {
                                map<string, string> m_id_name;
                                m_id_name[""] = value;
                                v_m_P2.push_back(m_id_name);
                            } else { // 二道切割
                                vector<string> splitVecP2 = StringUtil::split(value, "、");
                                for (int i = 0, size = splitVecP2.size(); i < size; i++) {
                                    map<string, string> m_id_name;
                                    m_id_name[""] = splitVecP2[i];
                                    v_m_P2.push_back(m_id_name);
                                }
                            }
                    }
                    output(root, L, v_m_P2);
                }
            }
        }

        // 单行内容提取
        L = "";
        unsigned int i = 0;
        BOOST_FOREACH(ptree::value_type &v, root.get_child("contentStructured")) {
            if (equals(v.second.get_child("tag").get_value<string>(), "paragraph")) {
                L = this->extract_single_L(v);
                if (L.empty()) {
                    continue;
                }
                i = 0;
                vector<map<string, string> > v_m_P2;
                BOOST_FOREACH(ptree::value_type &v1, v.second.get_child("content")) {
                    if (i == 0) {
                        i++;
                        continue;
                    }
                    // 只保留有内链的关系人P2
                    if (equals(v1.second.get_child("tag").get_value<string>(), "innerlink")) {
                        map<string, string> m_id_name;
                        m_id_name[v1.second.get_child("lemmaId").get_value<string>()] = v1.second.get_child("lemmaTitle").get_value<string>();
                        v_m_P2.push_back(m_id_name);
                    }
                }
                output(root, L, v_m_P2);
            }
        }

        // 段落提取
    } catch(ptree_error & e) {
        std::cout<<e.what()<<endl;
    } catch(runtime_error& e) {
        std::cout<<e.what()<<endl;
    }
    return ret;
}

// 以下是换行三元关系提取相关功能=====================================
// 获取角色
string Relation::extract_wrap_L(ptree::value_type &node) {
    string L;
    string tag = node.second.get_child("tag").get_value<string>();
    if (equals(tag, "paragraph")) {
        return L;
    }

    if (equals(tag, "header")) {
        L = node.second.get_child("title").get_value<string>();
    } else if (equals(tag, "list")) {
        if (equals(node.second.get_child("type").get_value<string>(), "unordered")) {
            // 用于删除html标签正则
            regex rgx("<[a-zA-Z/]*>");
            // 遍历数组中的元素
            BOOST_FOREACH (ptree::value_type& v, node.second.get_child("list")) {
                BOOST_FOREACH (ptree::value_type& v1, v.second) {
                    if (equals(v1.second.get_child("tag").get_value<string>(), "text")) {
                        L = regex_replace(v1.second.get_child("text").get_value<string>(), rgx, "");
                    }
                }
            }
        }
    }
    if (this->_map_family.count(L) > 0) {
        return L;
    }
    return "";
}

// 获取人物名称
vector<map<string, string > > extract_P2(ptree::value_type &node) {
    vector<map<string, string> > ret;

    // filter
    if (!equals(node.second.get_child("tag").get_value<string>(), "paragraph")) {
        return ret;
    }
    if (node.second.get_child("content").empty()) {
        return ret;
    }

    // 用于删除html标签正则
    regex rgx("<[a-zA-Z/]*>");
    // 遍历数组中的元素
    BOOST_FOREACH (ptree::value_type& v, node.second.get_child("content")) {
        if (equals(v.second.get_child("tag").get_value<string>(), "innerlink")) {
            map<string, string> P2;
            P2[v.second.get_child("lemmaId").get_value<string>()] = regex_replace(v.second.get_child("text").get_value<string>(), rgx, "");
            ret.push_back(P2);
        }
    }

    return ret;
}

bool Relation::is_valid_map(string P1, string L, string P2)
{
    if (this->_filter_map_family.count(L) > 0) {
        wstring wP2 = _W(P2);
        if (wP2.length() > 2) {
            return true;
        }
        wstring wP1 = _W(P1);
        wstring wSurnameP1, wSurnameP2;
        wSurnameP1 = wP1.substr(0, 1);
        wSurnameP2 = wP2.substr(0, 1);
        return equals(wSurnameP1, wSurnameP2);
    }

    return true;
}


vector<RelationMap> Relation::regular_extract_wrap(const string &text) {
    vector<RelationMap> ret;
    if (text.empty()) {
        return ret;
    }

    stringstream str_stream(text);
    try {
        ptree root;
        read_json(str_stream, root);
        string L, P2;
        vector<map<string, string> > v_m_P2;
        BOOST_FOREACH(ptree::value_type &v, root.get_child("contentStructured")) {
           if (!equals(v.second.get_child("tag").get_value<string>(), "paragraph")) {
               L = this->extract_wrap_L(v);
           } else {
               if (!L.empty()) {
                   v_m_P2 = extract_P2(v);
                   if (v_m_P2.size() > 0) {
                       output(root, L, v_m_P2);
                   }
               }
           }
        }
    } catch(ptree_error & e) {
        std::cout << "regular_extract_wrap()" << e.what() << "\ttext=" << text <<endl;
    } catch(runtime_error& e) {
        std::cout << "regular_extract_wrap()" << e.what() << "\ttext=" << text <<endl;
    }
    return ret;
}
