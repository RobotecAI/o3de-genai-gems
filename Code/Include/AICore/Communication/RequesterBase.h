
#pragma once

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/RTTI/TemplateInfo.h>

namespace AICore
{
    class RequesterBase
    {
    public:
        AZ_RTTI(RequesterBase, "{0dfe52c6-e936-44a4-8451-3931c96cf0d0}");
        virtual ~RequesterBase() = default;
    };
} // namespace AICore