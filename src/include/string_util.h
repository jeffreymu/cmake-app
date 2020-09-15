//
// Created by Zhou,Baochuan on 18/6/7.
//

#ifndef WORK_STRING_UTIL_H
#define WORK_STRING_UTIL_H

#include "common.h"

namespace work{
    class StringUtil {
    public:
        StringUtil();
        ~StringUtil();
        static wstring string2wstring(const string str);
        static string wstring2string(const wstring str);
        static string strip_tag(const string str);
        // 字符串分割
        static vector<string> split(const string text, const string separator);
    };
    inline wstring _W(const string str){
        return StringUtil::string2wstring(str);
    }
    inline string _S(const wstring wstr){
        return StringUtil::wstring2string(wstr);
    }
    inline vector<string> _T(const string text, const string separator){
        return StringUtil::split(text, separator);
    }
}


#endif //WORK_STRING_UTIL_H
