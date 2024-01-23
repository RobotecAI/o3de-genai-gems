
#pragma once

#include <AzCore/std/any.h>

namespace AICore
{
    template<typename R, typename P>
    class RequestGenerator
    {
    public:
        virtual ~RequestGenerator(){};

        virtual R EncodePrompt(const P& prompt) = 0;

        virtual P DecodePrompt(const R& request) = 0;
    };

} // namespace AICore