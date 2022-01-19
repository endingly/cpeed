#include "down.h"
#include <cpprest/http_client.h>

namespace down
{
    // 解析 HTTP 请求
    Response Reslove(Request *req)
    {
        Response ret;
        web::http::client::http_client client(req->URI);
        auto rsp = client.request(req->Method).get();
        auto body = rsp.extract_string().get();
        std::wcout << rsp.status_code() << "\n"
                   << body << std::endl;
        auto headers = rsp.headers();
        if (headers.has(L"Content-Disposition") == true)
        {
            ret.Name = headers.find(L"Content-Disposition")->second;
        }
        //解析文件大小
        ret.szie = headers.content_length();

        //判断是否支持分段下载
        ret.Partial = ret.szie > 0 && rsp.status_code() == 206;
        return ret;
    };
}
