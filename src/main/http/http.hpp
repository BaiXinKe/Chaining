#ifndef MQTT_HTTP_HPP__
#define MQTT_HTTP_HPP__

#include <string>
#include <string_view>
#include <unordered_map>

namespace Chaining::net {

class Buffer;
namespace Http {

    enum class Version {
        v09,
        v10,
        v11
    };

    class HttpRequest {
    public:
        enum class ParseStatus {
            RequestLine,
            HeaderLine,
            Body,
            Error,
            Done
        };

    public:
        HttpRequest() = default;

        ParseStatus parseLine(std::string_view line);

        ~HttpRequest() = default;

    private:
        ParseStatus parseHeaderLine(std::string_view line);
        ParseStatus parseRequestLine(std::string_view line);

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

            void setMethod(Method m) { method = m; }
        };

        ParseStatus parseState_;

        RequestLine requestLine_;
        HeaderField requestHeader_;

        std::string body_;

        bool parse_done_ { false };
    };

    class HttpResponse {
    public:
        enum class StatusCode : uint16_t {
            c100 = 100, // Continue
            c101 = 101, // Switching Protocols
            c200 = 200, // OK
            c201 = 201, // Created
            c202 = 202, // Accepted
            c203 = 203, // Non-Authoritative Information
            c204 = 204, // No Content
            c205 = 205, // Reset Content
            c206 = 206, // Partial Content
            c300 = 300, // Multiple Choices
            c301 = 301, // Moved Permanently
            c302 = 302, // Found
            c303 = 303, // See Other
            c304 = 304, // Not Modified
            c305 = 305, // Use Proxy
            c307 = 307, // Temporary Redirect
            c400 = 400, // Bad Request
            c401 = 401, // Unauthorized
            c402 = 402, // Payment Required
            c403 = 403, // Forbidden
            c404 = 404, // Not Found
            c405 = 405, // Method Not Allowed
            c406 = 406, // Not Acceptable
            c407 = 407, // Proxy Authentication Required
            c408 = 408, // Request Time-out
            c409 = 409, // Conflict
            c410 = 410, // Gone
            c411 = 411, // Length Required
            c412 = 412, // Precondition Failed
            c413 = 413, // Request Entity Too Large
            c414 = 414, // Request-URI Too Large
            c415 = 415, // Unsupported Media Type
            c416 = 416, // Requested range not satisfiable
            c417 = 417, // Expectation Failed
            c500 = 500, // Internal Server Error
            c501 = 501, // Not Implemented
            c502 = 502, // Bad Gateway
            c503 = 503, // Service Unavailable
            c504 = 504, // Gateway Time-out
            c505 = 505 // HTTP Version not supported
        };

    public:
        HttpResponse();

        HttpResponse& addHeader(const std::string& name, const std::string& value);
        HttpResponse& setStatusCode(StatusCode code);

        void Serialization(Buffer* buff);

        HttpResponse& setBody(std::string_view str);

        ~HttpResponse() = default;

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

            void Serilization(Buffer* buff);
        };

        StatusLine statusLine_;
        HeaderField requestHeader_;

        bool useBody_;
        std::string body_;
    };
}
}
#endif