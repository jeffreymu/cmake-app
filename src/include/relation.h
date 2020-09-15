//
// Created by Zhou,Baochuan on 18/6/6.
//

#ifndef WORK_RELATION_H
#define WORK_RELATION_H
#include "common.h"
#include "string_util.h"
#include "http.h"

namespace work {
    // 人物关系:三元关系
    struct RelationMap{
        string P1;
        string P2;
        string L;
    };

    class Relation {
    public:
        Relation();
        ~Relation();
        // 初始化家庭成员映射关系
        void init_map_family();
        // 初始化已有三元关系
        void init_map_cache_had_relation();
        // 输出
        void output(ptree root, string L, vector<map<string, string> > v_m_P2);
        // 添加id和url的映射关系
        void add_id_url(string id, string url);

        // 补充词条ID数据（通过KG接口)
        string added_id_by_kg(const string target, const string name);

        // 规则提取-单行
        string extract_single_L(ptree::value_type &node);
        vector<RelationMap> regular_extract_single_line(const string &text);
        // 规则提取-换行
        string extract_wrap_L(ptree::value_type &node);
        vector<RelationMap> regular_extract_wrap(const string &text);


    private:
        // 家庭成员关系映射
        map<string, string> _map_family;
        // 特殊家庭成员关系映射
        map<string, string> _filter_map_family;
        // 已经有的三元关系(缓存已有关系):P1的ID => P2的ID
        map<string, string> _map_cache_had_relation;
        // 词条id和url落地地址映射
        map<string, string> _map_id_url;
        Http *_http;
        // 校验是否是合法关系
        bool is_valid_map(string P1, string L, string P2);
    };
}
#endif //WORK_RELATION_H
