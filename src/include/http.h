//
// Created by Zhou,Baochuan on 18/6/5.
//

#ifndef WORK_HTTP_H
#define WORK_HTTP_H

#include "common.h"
#include <curl/curl.h>

namespace work {
    class Http {
    public:
        Http();
        ~Http();
        static string get(string url, unsigned retries = 3);
    };
}

#endif //WORK_HTTP_H
