#pragma once
#include <string>
#include <map>
#include <vector>

#include <cpprest/http_client.h>

namespace down
{
    struct Request
    {
        web::http::method Method;
        web::http::uri URI;
        web::http::http_headers Header;
        std::vector<char> content;
    };

    struct Response
    {
        std::wstring Name;
        int64_t szie;
        bool Partial;
    };

    Response Reslove(Request *req);
    
    void Down(Request *req);
}
