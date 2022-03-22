#include <cstring>

#include "main/http/http.hpp"

#include "main/Buffer.hpp"

using namespace Chaining::net::Http;

// ------------------------------- Request -------------------------------------------//

HttpRequest::HttpRequest()
    : parseState_ { ParseStatus::RequestLine }
{
}

HttpRequest::ParseStatus
HttpRequest::parseLine(std::string_view line)
{
    if (parseState_ == ParseStatus::HeaderLine) {
        return parseHeaderLine(line);
    } else if (parseState_ == ParseStatus::RequestLine) {
        return parseRequestLine(line);
    }
    return parseState_;
}

HttpRequest::ParseStatus
HttpRequest::parseHeaderLine(std::string_view line)
{
    static const std::unordered_map<std::string, Method> str2Method {
        { "GET", Method::GET }, { "PUT", Method::PUT }, { "POST", Method::POST },
        { "HEAD", Method::HEAD }, { "TRACE", Method::TRACE }, { "DELETE", Method::DELETE },
        { "OPTIONS", Method::OPTIONS }, { "CONNECT", Method::CONNECT }, { "OTHER", Method::OTHER }
    };

    const char* str = line.data();
    const char* space = ::strchr(str, ' ');
    if (space == nullptr) {
        return ParseStatus::Error;
    }
    space = '\0';
    space++;

    std::string method { str, ::strlen(str) };
    if (str2Method.find(method) != std::end(str2Method)) {
        requestLine_.setMethod(str2Method.at(method));
    } else {
        requestLine_.setMethod(str2Method.at("OTHER"));
    }

    if (requestLine_.method == Method::OTHER)
        return ParseStatus::Error;
}

HttpRequest::ParseStatus
HttpRequest::parseRequestLine(std::string_view line)
{
}

//----------------------------------- Response ---------------------------------------//

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
    , useBody_ { false }
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

HttpResponse& HttpResponse::setBody(std::string_view str)
{
    this->body_ = str;
    useBody_ = true;
}

void HttpResponse::StatusLine::Serilization(Buffer* buff)
{
    switch (version) {
    case Version::v09: {
        buff->append("HTTP/0.9");
        break;
    }
    case Version::v10: {
        buff->append("HTTP/1.0");
        break;
    }
    default: {
        buff->append("HTTP/1.1");
        break;
    }
    };

    buff->append(" ");
    buff->append(std::to_string(static_cast<uint16_t>(code)));
    buff->append(" ");
    buff->append(statusMessage);
    buff->append("\r\n");
}

void HttpResponse::Serialization(Buffer* buff)
{
    this->statusLine_.Serilization(buff);
    for (auto&& req_line : requestHeader_) {
        buff->append(req_line.first);
        buff->append(": ");
        buff->append(req_line.second);
        buff->append("\r\n");
    }
    buff->append("\r\n");

    if (useBody_)
        buff->append(body_);
}