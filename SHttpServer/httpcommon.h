#ifndef HTTPCOMMON_H
#define HTTPCOMMON_H
#include <QObject>

using HttpHeaderList = QHash<QByteArray,QByteArray>;
using ConnectionRoutineCallBack = std::function<bool ()>;
using ConnectionRoutineCallBackList = QList<ConnectionRoutineCallBack>;
class SHttpRequest;
using RequestRoutineCallBack = std::function<bool (SHttpRequest *)>;
using RequestRoutineCallBackList = QList<RequestRoutineCallBack>;
namespace Http {

    Q_NAMESPACE;



    enum Operation{
        HeadOperation       = 1,
        GetOperation        = 2,
        PutOperation        = 3,
        PostOperation       = 4,
        DeleteOperation     = 5,
        //CustomOperation     = 6,
        OptionsOperation    = 7
    };
    Q_ENUM_NS(Operation)



    enum StatusCode{


        //1xx Informational Response, An informational response indicates that the request was received and understood. It is issued on a provisional basis while request processing continues. It alerts the client to wait for a final response. The message consists only of the status line and optional header fields, and is terminated by an empty line. As the HTTP/1.0 standard did not define any 1xx status codes, servers must not[note 1] send a 1xx response to an HTTP/1.0 compliant client except under experimental conditions.
        Continute                         = 100, //    The server has received the request headers and the client should proceed to send the request body (in the case of a request for which a body needs to be sent; for example, a POST request). Sending a large request body to a server after a request has been rejected for inappropriate headers would be inefficient. To have a server check the request's headers, a client must send Expect: 100-continue as a header in its initial request and receive a 100 Continue status code in response before sending the body. If the client receives an error code such as 403 (Forbidden) or 405 (Method Not Allowed) then it should not send the request's body. The response 417 Expectation Failed indicates that the request should be repeated without the Expect header as it indicates that the server does not support expectations (this is the case, for example, of HTTP/1.0 servers)
        SwitchingProtocols                = 101, //    The requester has asked the server to switch protocols and the server has agreed to do so.
        Processing                        = 102, //    A WebDAV request may contain many sub-requests involving file operations, requiring a long time to complete the request. This code indicates that the server has received and is processing the request, but no response is available yet.[6] This prevents the client from timing out and assuming the request was lost.
        EearlyHints                       = 103, //    Used to return some response headers before final HTTP message


        //2xx Success, This class of status codes indicates the action requested by the client was received, understood, and accepted.[2]
        Ok                                = 200, //    Standard response for successful HTTP requests. The actual response will depend on the request method used. In a GET request, the response will contain an entity corresponding to the requested resource. In a POST request, the response will contain an entity describing or containing the result of the action.
        Created                           = 201, //    The request has been fulfilled, resulting in the creation of a new resource.
        Accepted                          = 202, //    The request has been accepted for processing, but the processing has not been completed. The request might or might not be eventually acted upon, and may be disallowed when processing occurs.
        NonAuthoritativeInformation       = 203, //    The server is a transforming proxy (e.g. a Web accelerator) that received a 200 OK from its origin, but is returning a modified version of the origin's response.
        NoContent                         = 204, //    The server successfully processed the request, and is not returning any content.
        ResetContent                      = 205, //    The server successfully processed the request, asks that the requester reset its document view, and is not returning any content.
        PartialContent                    = 206, //    The server is delivering only part of the resource (byte serving) due to a range header sent by the client. The range header is used by HTTP clients to enable resuming of interrupted downloads, or split a download into multiple simultaneous streams.
        MultiStatus                       = 207, //    The message body that follows is by default an XML message and can contain a number of separate response codes, depending on how many sub-requests were made.[16]
        AlreadyReported                   = 208, //    The members of a DAV binding have already been enumerated in a preceding part of the (multistatus) response, and are not being included again.
        IMUsed                            = 226, //    The server has fulfilled a request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.


        //3xx Redirection, This class of status code indicates the client must take additional action to complete the request. Many of these status codes are used in URL redirection.[2]
        MultipleChoices                   = 300, //    Indicates multiple options for the resource from which the client may choose (via agent-driven content negotiation). For example, this code could be used to present multiple video format options, to list files with different filename extensions, or to suggest word-sense disambiguation.
        MovedPermanently                  = 301, //    This and all future requests should be directed to the given URI.
        Found                             = 302, //    Tells the client to look at (browse to) another URL. The HTTP/1.0 specification (RFC 1945) required the client to perform a temporary redirect with the same method (the original describing phrase was "Moved Temporarily"),[21] but popular browsers implemented 302 redirects by changing the method to GET. Therefore, HTTP/1.1 added status codes 303 and 307 to distinguish between the two behaviours
        SeeOther                          = 303, //    The response to the request can be found under another URI using the GET method. When received in response to a POST (or PUT/DELETE), the client should presume that the server has received the data and should issue a new GET request to the given URI.
        NotModified                       = 304, //    Indicates that the resource has not been modified since the version specified by the request headers If-Modified-Since or If-None-Match. In such case, there is no need to retransmit the resource since the client still has a previously-downloaded copy
        UseProxy                          = 305, //    The requested resource is available only through a proxy, the address for which is provided in the response. For security reasons, many HTTP clients (such as Mozilla Firefox and Internet Explorer) do not obey this status code.
        SwitchProxy                       = 306, //    No longer used. Originally meant "Subsequent requests should use the specified proxy.
        TemporaryRedirect                 = 307, //    In this case, the request should be repeated with another URI; however, future requests should still use the original URI. In contrast to how 302 was historically implemented, the request method is not allowed to be changed when reissuing the original request. For example, a POST request should be repeated using another POST request.[
        PermanentRedirect                 = 308, //    This and all future requests should be directed to the given URI. 308 parallel the behaviour of 301, but does not allow the HTTP method to change. So, for example, submitting a form to a permanently redirected resource may continue smoothly.[

        //4xx client errors
        BadRequest                        = 400, //    The server cannot or will not process the request due to an apparent client error (e.g., malformed request syntax, size too large, invalid request message framing, or deceptive request routing).
        Unauthorized                      = 401, //    Similar to 403 Forbidden, but specifically for use when authentication is required and has failed or has not yet been provided. The response must include a WWW-Authenticate header field containing a challenge applicable to the requested resource. See Basic access authentication and Digest access authentication.[31] 401 semantically means "unauthorised",[32] the user does not have valid authentication credentials for the target resource.
        PaymentRequired                   = 402, //    Reserved for future use. The original intention was that this code might be used as part of some form of digital cash or micropayment scheme, as proposed, for example, by GNU Taler,[33] but that has not yet happened, and this code is not widely used. Google Developers API uses this status if a particular developer has exceeded the daily limit on requests.[34] Sipgate uses this code if an account does not have sufficient funds to start a call.[35] Shopify uses this code when the store has not paid their fees and is temporarily disabled.[36] Stripe uses this code for failed payments where parameters were correct, for example blocked fraudulent payments.
        Forbidden                         = 403, //    The request contained valid data and was understood by the server, but the server is refusing action. This may be due to the user not having the necessary permissions for a resource or needing an account of some sort, or attempting a prohibited action (e.g. creating a duplicate record where only one is allowed). This code is also typically used if the request provided authentication by answering the WWW-Authenticate header field challenge, but the server did not accept that authentication. The request should not be repeated.
        NotFound                          = 404, //    The requested resource could not be found but may be available in the future. Subsequent requests by the client are permissible.
        MethodNotAllowed                  = 405, //    A request method is not supported for the requested resource; for example, a GET request on a form that requires data to be presented via POST, or a PUT request on a read-only resource.
        NotAcceptable                     = 406, //    The requested resource is capable of generating only content not acceptable according to the Accept headers sent in the request.[38] See Content negotiation.
        ProxyAuthenticationRequired       = 407, //    The client must first authenticate itself with the proxy.
        RequestTimeout                    = 408, //    The server timed out waiting for the request. According to HTTP specifications: "The client did not produce a request within the time that the server was prepared to wait. The client MAY repeat the request without modifications at any later time."
        Conflict                          = 409, //    Indicates that the request could not be processed because of conflict in the current state of the resource, such as an edit conflict between multiple simultaneous updates.
        Gone                              = 410, //    Indicates that the resource requested is no longer available and will not be available again. This should be used when a resource has been intentionally removed and the resource should be purged. Upon receiving a 410 status code, the client should not request the resource in the future. Clients such as search engines should remove the resource from their indices.[41] Most use cases do not require clients and search engines to purge the resource, and a "404 Not Found" may be used instead.
        LengthRequired                    = 411, //    The request did not specify the length of its content, which is required by the requested resource.
        PreconditionFailed                = 412, //    The server does not meet one of the preconditions that the requester put on the request header fields.
        PayloadTooLarge                   = 413, //    The request is larger than the server is willing or able to process. Previously called "Request Entity Too Large".
        URITooLong                        = 414, //    The URI provided was too long for the server to process. Often the result of too much data being encoded as a query-string of a GET request, in which case it should be converted to a POST request.[45] Called "Request-URI Too Long" previously.
        UnsupportedMediaType              = 415, //    The request entity has a media type which the server or resource does not support. For example, the client uploads an image as image/svg+xml, but the server requires that images use a different format.
        RangeNotSatisfiable               = 416, //    The client has asked for a portion of the file (byte serving), but the server cannot supply that portion. For example, if the client asked for a part of the file that lies beyond the end of the file.[48] Called "Requested Range Not Satisfiable" previously.
        ExpectationFailed                 = 417, //    The server cannot meet the requirements of the Expect request-header field.
        //ImATeaPot                       = 418, //    This code was defined in 1998 as one of the traditional IETF April Fools' jokes, in RFC 2324, Hyper Text Coffee Pot Control Protocol, and is not expected to be implemented by actual HTTP servers. The RFC specifies this code should be returned by teapots requested to brew coffee.[51] This HTTP status is used as an Easter egg in some websites, such as Google.com's I'm a teapot easter egg.
        MisdirectedRequest                = 421, //    The request was directed at a server that is not able to produce a response[54] (for example because of connection reuse).
        UnprocessableEntity               = 422, //    The request was well-formed but was unable to be followed due to semantic errors.
        Locked                            = 423, //    The resource that is being accessed is locked.
        FailedDependency                  = 424, //    The request failed because it depended on another request and that request failed (e.g., a PROPPATCH).
        TooEarly                          = 425, //    Indicates that the server is unwilling to risk processing a request that might be replayed.
        UpgradeRequired                   = 426, //    The client should switch to a different protocol such as TLS/1.3, given in the Upgrade header field.[56]
        PreconditionRequired              = 428, //    The origin server requires the request to be conditional. Intended to prevent the 'lost update' problem, where a client GETs a resource's state, modifies it, and PUTs it back to the server, when meanwhile a third party has modified the state on the server, leading to a conflict.
        TooManyRequests                   = 429, //    The user has sent too many requests in a given amount of time. Intended for use with rate-limiting schemes.
        RequestHeaderFieldsTooLarge       = 431, //    The server is unwilling to process the request because either an individual header field, or all the header fields collectively, are too large.
        UnavailableForLegalReasons        = 451, //    A server operator has received a legal demand to deny access to a resource or to a set of resources that includes the requested resource.[58] The code 451 was chosen as a reference to the novel Fahrenheit 451 (see the Acknowledgements in the RFC).

        //5xx server errors, The server failed to fulfil a request.

        InternalServerError               = 500, //    A generic error message, given when an unexpected condition was encountered and no more specific message is suitable.
        NotImplemented                    = 501, //    The server either does not recognize the request method, or it lacks the ability to fulfil the request. Usually this implies future availability (e.g., a new feature of a web-service API)
        BadGateway                        = 502, //    The server was acting as a gateway or proxy and received an invalid response from the upstream server.
        ServiceUnavailable                = 503, //    The server cannot handle the request (because it is overloaded or down for maintenance). Generally, this is a temporary state.
        GatewayTimeout                    = 504, //    The server was acting as a gateway or proxy and did not receive a timely response from the upstream server.
        HTTPVersionNotSupported           = 505, //    The server does not support the HTTP protocol version used in the request.
        VariantAlsoNegotiates             = 506, //    Transparent content negotiation for the request results in a circular reference.
        InsufficientStorage               = 507, //    The server is unable to store the representation needed to complete the request.
        LoopDetected                      = 508, //    The server detected an infinite loop while processing the request (sent instead of 208 Already Reported).
        NotExtended                       = 510, //    Further extensions to the request are required for the server to fulfil it.
        NetworkAuthenticationRequired     = 511, //    The client needs to authenticate to gain network access. Intended for use by intercepting proxies used to control access to the network (e.g., "captive portals" used to require agreement to Terms of Service before granting full Internet access via a Wi-Fi hotspot).



    };

    Q_ENUM_NS(StatusCode)



    Operation methodtoEnum(const QString &op);

}
#endif // HTTPCOMMON_H
