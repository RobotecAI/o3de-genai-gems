
#pragma once

#include <AICore/Communication/RequesterBus.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/functional.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/RTTI/TemplateInfo.h>

#ifdef AICoreEditor
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#endif

namespace AICore
{
    template<typename RequestType>
    class Requester
#ifdef AICoreEditor
        : public AzToolsFramework::Components::EditorComponentBase
#else
        : public AZ::Component
#endif
        , public RequesterBus<RequestType>::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(Requester, AZ::SystemAllocator);
#ifdef AICoreEditor
        AZ_RTTI((Requester, "{b036ab75-d5d4-47c5-8b59-fd7b9ec4b056}", RequestType), AzToolsFramework::Components::EditorComponentBase);
#else
        AZ_RTTI((Requester, "{0bcac4e2-09a9-4702-98dd-ef37006664a7}", RequestType), AZ::Component);
#endif

        Requester() = default;
        virtual ~Requester() = default;

        using Request = RequestType;

        static void Reflect(AZ::ReflectContext* context)
        {
            if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<Requester<RequestType>, AZ::Component>()->Version(0);
            }
        }

        void Activate() override
        {
            RequesterBus<RequestType>::Handler::BusConnect(GetEntityId());
        };

        void Deactivate() override
        {
            RequesterBus<RequestType>::Handler::BusDisconnect();
        };

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
        {
            provided.push_back(AZ_CRC_CE("AICoreRequesterService"));
        };
    };

} // namespace AICore

// namespace AZ
// {
//     AZ_TYPE_INFO_TEMPLATE(AICore::Requester, "{0bcac4e2-09a9-4702-98dd-ef37006664a7}", AZ_TYPE_INFO_TYPENAME);
// }