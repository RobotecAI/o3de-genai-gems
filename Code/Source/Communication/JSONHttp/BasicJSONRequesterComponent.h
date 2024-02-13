
#pragma once

#include "BasicJSONRequesterController.h"
#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace AICore
{
    using BasicJSONRequesterComponentBase =
        AzFramework::Components::ComponentAdapter<BasicJSONRequesterController, BasicJSONRequesterConfiguration>;

    class BasicJSONRequesterComponent : public BasicJSONRequesterComponentBase
    {
    public:
        AZ_COMPONENT(BasicJSONRequesterComponent, "{aad62d35-c628-4141-b759-0d3764013b29}", AZ::Component)

        BasicJSONRequesterComponent() = default;
        BasicJSONRequesterComponent(const BasicJSONRequesterConfiguration& config);
        ~BasicJSONRequesterComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////
    };
} // namespace AICore
