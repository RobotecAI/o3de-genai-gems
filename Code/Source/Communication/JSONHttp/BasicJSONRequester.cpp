
#include <AICore/Communication/JSONHttp/BasicJSONRequester.h>

#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace AICore
{
    BasicJSONRequester::BasicJSONRequester(AZStd::string url)
        : m_url(url)
    {
    }

    void BasicJSONRequester::Request(
        const Aws::Utils::Json::JsonView& request, AZStd::function<void(const Aws::Utils::Json::JsonView&, bool)> callback)
    {
        HttpRequestor::Headers headers;
        // headers["Content-Type"] = "application/json";

        HttpRequestor::Callback innerCallback =
            [callback](const Aws::Utils::Json::JsonView& jsonView, Aws::Http::HttpResponseCode responseCode)
        {
            // callback(jsonView, responseCode == Aws::Http::HttpResponseCode::OK);
        };

        auto requestString = request.WriteReadable();
        std::cout << std::endl << std::endl << "Request: " << requestString.c_str() << std::endl << std::endl;

        // AZStd::string requestString(request.WriteReadable(false).c_str());

        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersAndBody,
            m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            "",
            innerCallback);
    };
} // namespace AICore