//
// Created by Zhou,Baochuan on 18/6/7.
//

#include <regex>
#include "common.h"
#include "string_util.h"

using namespace work;
StringUtil::StringUtil ()
{
}
StringUtil::~StringUtil ()
{
}

wstring StringUtil::string2wstring(const string str)
{
    unsigned len = str.size() * 2;// 预留字节数
    setlocale(LC_CTYPE, "");     //必须调用此函数
    wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
    mbstowcs(p,str.c_str(),len);// 转换
    std::wstring str1(p);
    delete[] p;// 释放申请的内存
    return str1;
}


string StringUtil::wstring2string(const wstring str)
{
    unsigned len = str.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p,str.c_str(),len);
    std::string str1(p);
    delete[] p;
    return str1;
}

string StringUtil::strip_tag(const string str)
{
    regex rgx("<[a-zA-Z/]*>");
    return regex_replace(str, rgx, "");
}

vector<string> StringUtil::split(const string input, string separator)
{
    vector<string> ret;
    vector< wstring > splitVec;
    wstring winput = string2wstring(input);
    wstring wseparator = StringUtil::string2wstring(separator);
    boost::split(splitVec, winput, boost::is_any_of(wseparator));
    int size = splitVec.size();
    if ( size > 0) {
        for (int i = 0; i < size;i++) {
            ret.push_back(wstring2string(splitVec[i]));
        }
    }
    return ret;
}

