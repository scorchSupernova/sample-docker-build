#include "stdafx.h"

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

class HTTPRequest {
public:
    HTTPRequest(const std::string& url);
    ~HTTPRequest();
    void post(const std::string& payload);

private:
    CURL* curl;
    std::string url;
};

#endif /* HTTPREQUEST_H */
