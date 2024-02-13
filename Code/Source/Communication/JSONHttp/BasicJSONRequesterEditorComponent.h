
#pragma once

#include "Communication/JSONHttp/BasicJSONRequesterComponent.h"
#include "Communication/JSONHttp/BasicJSONRequesterController.h"
#include "ToolsComponents/EditorComponentAdapter.h"
#include "ToolsComponents/EditorComponentBase.h"
#include <AzCore/RTTI/ReflectContext.h>

namespace AICore
{
    using BasicJSONRequesterEditorComponentBase = AzToolsFramework::Components::
        EditorComponentAdapter<BasicJSONRequesterController, BasicJSONRequesterComponent, BasicJSONRequesterConfiguration>;

    class BasicJSONRequesterEditorComponent : public BasicJSONRequesterEditorComponentBase
    {
    public:
        AZ_COMPONENT(
            BasicJSONRequesterEditorComponent, "{1c4be1c2-dd90-4690-a829-785310b3bad6}", AzToolsFramework::Components::EditorComponentBase);

        BasicJSONRequesterEditorComponent() = default;
        explicit BasicJSONRequesterEditorComponent(const BasicJSONRequesterConfiguration& config);
        ~BasicJSONRequesterEditorComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////
    };
} // namespace AICore
