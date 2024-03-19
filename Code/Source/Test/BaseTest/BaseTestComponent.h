
#pragma once

#include <AICore/AIComponentBase/AIComponentBase.h>

namespace AICore
{
    class AIComponent : public AIComponentBase
    {
    public:
        AZ_COMPONENT(AIComponent, "{649a1139-ed22-4fe3-a81d-76c77faeea4e}", AIComponentBase);

        AIComponent() = default;
        ~AIComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate() override;
        void Deactivate() override;

        void TestButton();

        AZStd::string m_modelOutput;
        AZStd::string m_modelInput;
    };
} // namespace AICore
