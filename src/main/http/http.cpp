#include "http.hpp"

using namespace Chaining::net::Http;

std::string HttpResponse::StatusLine::StatusCode2Message(StatusCode code)
{
    static const std::unordered_map<StatusCode, std::string> code2Message {
        { StatusCode::c100, "Continue" },
        { StatusCode::c101, "Switching Protocols" },
        { StatusCode::c200, "OK" },
        { StatusCode::c201, "Created" },
        { StatusCode::c202, "Accepted" },
        { StatusCode::c203, "Non-Authoritative Information" },
        { StatusCode::c204, "No Content" },
        { StatusCode::c205, "Reset Content" },
        { StatusCode::c206, "Parital Content" },
        { StatusCode::c300, "Multiple Choices" },
        { StatusCode::c301, "Moved Permanently" },
        { StatusCode::c302, "Found" },
        { StatusCode::c303, "See Other" },
        { StatusCode::c304, "Not Modified" },
        { StatusCode::c305, "Use Proxy" },
        { StatusCode::c307, "Temporary Redirect" },
        { StatusCode::c400, "Bad Request" },
        { StatusCode::c401, "Unauthorized" },
        { StatusCode::c402, "Payment Required" },
        { StatusCode::c403, "Forbidden" },
        { StatusCode::c404, "Not Found" },
        { StatusCode::c405, "Method Not Allowed" },
        { StatusCode::c406, "Not Acceptable" },
        { StatusCode::c407, "Proxy Authentication Required" },
        { StatusCode::c408, "Request Time-out" },
        { StatusCode::c409, "Conflict" },
        { StatusCode::c410, "Gone" },
        { StatusCode::c411, "Length Required" },
        { StatusCode::c412, "Precondition Failed" },
        { StatusCode::c413, "Request Entity Too Large" },
        { StatusCode::c414, "Request-URI Too Large" },
        { StatusCode::c415, "Unsupported Media Type" },
        { StatusCode::c416, "Requested range not satisfiable" },
        { StatusCode::c417, "Expectation Failed" },
        { StatusCode::c500, "Internal Server Error" },
        { StatusCode::c501, "Not Implemented" },
        { StatusCode::c502, "Bad Gateway" },
        { StatusCode::c503, "Service Unavailable" },
        { StatusCode::c504, "Gateway Time-out" },
        { StatusCode::c505, "HTTP Version not supported" },
    };

    return code2Message.at(code);
}

HttpResponse::StatusLine::StatusLine(Version v, StatusCode c, std::string msg)
    : version { v }
    , code { c }
    , statusMessage { std::move(msg) }
{
}

HttpResponse::StatusLine& HttpResponse::StatusLine::setVersion(Version v)
{
    version = v;
    return *this;
}

HttpResponse::StatusLine& HttpResponse::StatusLine::setCode(StatusCode c)
{
    code = c;
    statusMessage = std::move(StatusCode2Message(code));
    return *this;
}

HttpResponse::HttpResponse()
    : statusLine_ { Version::v11, StatusCode::c200, StatusLine::StatusCode2Message(StatusCode::c200) }
{
}

HttpResponse& HttpResponse::addHeader(const std::string& name, const std::string& value)
{
    requestHeader_[name] = value;
    return *this;
}

HttpResponse& HttpResponse::setStatusCode(StatusCode code)
{
    statusLine_.setCode(code);
    return *this;
}