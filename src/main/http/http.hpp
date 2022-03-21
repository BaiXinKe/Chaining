#ifndef MQTT_HTTP_HPP__
#define MQTT_HTTP_HPP__

#include <string>
#include <string_view>
#include <unordered_map>

namespace Chaining::net::Http {

enum class Version {
    v09,
    v10,
    v11
};

class HttpRequest {
public:
private:
    enum class Method {
        OPTIONS,
        GET,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        CONNECT,
        OTHER
    };

    using HeaderField = std::unordered_map<std::string, std::string>;

    struct RequestLine {
        Method method;
        std::string RequestURI;
        Version version;
    };

    RequestLine requestLine_;
    HeaderField requestHeader_;
    std::string body_;
};

class HttpResponse {
public:
    enum class StatusCode {
        c100, // Continue
        c101, // Switching Protocols
        c200, // OK
        c201, // Created
        c202, // Accepted
        c203, // Non-Authoritative Information
        c204, // No Content
        c205, // Reset Content
        c206, // Partial Content
        c300, // Multiple Choices
        c301, // Moved Permanently
        c302, // Found
        c303, // See Other
        c304, // Not Modified
        c305, // Use Proxy
        c307, // Temporary Redirect
        c400, // Bad Request
        c401, // Unauthorized
        c402, // Payment Required
        c403, // Forbidden
        c404, // Not Found
        c405, // Method Not Allowed
        c406, // Not Acceptable
        c407, // Proxy Authentication Required
        c408, // Request Time-out
        c409, // Conflict
        c410, // Gone
        c411, // Length Required
        c412, // Precondition Failed
        c413, // Request Entity Too Large
        c414, // Request-URI Too Large
        c415, // Unsupported Media Type
        c416, // Requested range not satisfiable
        c417, // Expectation Failed
        c500, // Internal Server Error
        c501, // Not Implemented
        c502, // Bad Gateway
        c503, // Service Unavailable
        c504, // Gateway Time-out
        c505 // HTTP Version not supported
    };

public:
    HttpResponse();

    HttpResponse& addHeader(const std::string& name, const std::string& value);
    HttpResponse& setStatusCode(StatusCode code);

    ~HttpResponse();

private:
    using HeaderField = std::unordered_map<std::string, std::string>;
    struct StatusLine {
        Version version;
        StatusCode code;
        std::string statusMessage;

        static std::string StatusCode2Message(StatusCode code);
        StatusLine(Version v, StatusCode c, std::string msg);
        StatusLine& setVersion(Version v);
        StatusLine& setCode(StatusCode code);
    };

    StatusLine statusLine_;
    HeaderField requestHeader_;
    std::string body_;
};
}
#endif