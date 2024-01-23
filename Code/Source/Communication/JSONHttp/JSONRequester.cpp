
#include "JSONRequester.h"
#include <aws/common/allocator.h>
#include <aws/core/utils/memory/stl/AWSString.h>

namespace AICore
{
    JSONRequester::JSONRequester(AZStd::string url)
        : m_url(url){};

    void JSONRequester::Request(
        const Aws::Utils::Json::JsonView& request, AZStd::function<void(const Aws::Utils::Json::JsonView&, bool)> callback)
    {
        HttpRequestor::Headers headers;
        headers.insert(AZStd::make_pair("Content-Type", "application/json"));

        HttpRequestor::Callback innerCallback =
            [callback](const Aws::Utils::Json::JsonView& jsonView, Aws::Http::HttpResponseCode responseCode)
        {
            callback(jsonView, responseCode == Aws::Http::HttpResponseCode::OK);
        };

        AZStd::string requestString = request.WriteCompact().c_str();

        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersAndBody,
            m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            requestString,
            innerCallback);
    };

} // namespace AICore