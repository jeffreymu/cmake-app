//
// Created by Zhou,Baochuan on 18/6/25.
//

#include "common.h"
#include "relation.h"
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace work;
using namespace boost;

int main(int argc, char** argv) {
    string cmd;

    // 命令行参数解析
    for (int i = 0; i < argc; i++) {
        if (i == 0) {
            continue;
        }

        if (strcmp(argv[i], "-g") == 0 && argc >= i + 1) {
            cmd = string(argv[i + 1]);
        }
    }

    // 训练样本
    filesystem::path dirCurrentPath = filesystem::current_path(); //取得当前程序所在文件夹
    filesystem::path file = dirCurrentPath / "data/relation/id_url";
    string s_train = "./data/relation/T_train";
    ifstream in(s_train.c_str());
    string line;
    // 生成id map映射文件
    if (equals(cmd, "id2url")) {
        string url, id;
        ptree root;
        ofstream of(file.string());
        // 先初始化id和url的映射关系
        while(getline(in, line, '\n')){
            if (line.empty()){
                continue;
            }
            try {
                stringstream str_stream(line);
                read_json(str_stream, root);
                id = root.get_child("lemmaId").get_value<string>();
                url = root.get_child("url").get_value<string>();
                of << id << "\t" << url << endl;
                //relation.add_id_url(id, url);
            } catch(ptree_error & e) {
            }
        }
        of.close();
        //in.clear();
        //in.seekg(0, ios::beg);
        return 0;
    }

    Relation relation;
    // 初始化id url映射
    ifstream inIdUrl(file.string());
    vector<string> splitVec;
    while (getline(inIdUrl, line , '\n')){
        splitVec = _T(line, "\t");
        if (splitVec.size() != 2) {
            continue;
        }
        relation.add_id_url(splitVec[0], splitVec[1]);
    }
    inIdUrl.close();

    while(getline(in, line, '\n')){
        if (line.empty()){
            continue;
        }
        relation.regular_extract_single_line(line);
        relation.regular_extract_wrap(line);
    }
    in.close();
    return 0;
}