
#include "RequesterTab.h"
#include "AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h"
#include "AzQtComponents/Components/Widgets/SegmentControl.h"
#include "Communication/JSONHttp/BasicJSONRequester.h"

#include <AzCore/Utils/Utils.h>
#include <QLabel>
#include <QVBoxLayout>
#include <qnamespace.h>

namespace AICore
{
    RequesterTab::RequesterTab(QWidget* parent)
        : QWidget(parent)
    {
        m_segmentControl = new AzQtComponents::SegmentControl(this);
        m_segmentControl->setTabPosition(AzQtComponents::SegmentControl::West);
        m_segmentControl->setTabOrientation(Qt::Vertical);
        m_segmentControl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        CreatePropertyEditor(this);

        blockSignals(true);
        m_propertyEditor->ClearInstances();

        m_propertyEditor->AddInstance(&m_requester_test.second);
        m_propertyEditor->AddInstance(&m_requester_test2.second);

        m_propertyEditor->InvalidateAll();
        blockSignals(false);
    }
    void RequesterTab::CreatePropertyEditor(QWidget* parent)
    {
        // QVBoxLayout* verticalLayout = new QVBoxLayout(parent);
        // verticalLayout->setContentsMargins(0, 0, 0, 0);
        // verticalLayout->setSpacing(0);

        AZ::SerializeContext* m_serializeContext;
        AZ::ComponentApplicationBus::BroadcastResult(m_serializeContext, &AZ::ComponentApplicationRequests::GetSerializeContext);
        AZ_Assert(m_serializeContext, "Failed to retrieve serialize context.");

        const int propertyLabelWidth = 250;
        m_propertyEditor = new AzToolsFramework::ReflectedPropertyEditor(parent);
        m_propertyEditor->Setup(m_serializeContext, this, true, propertyLabelWidth);
        m_propertyEditor->show();
        m_propertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_propertyEditor->SetAutoResizeLabels(true);

        m_segmentControl->addTab(m_propertyEditor, QStringLiteral("Requester 1"));
        m_segmentControl->addTab(m_propertyEditor, QStringLiteral("Requester 2"));

        // verticalLayout->addWidget(m_propertyEditor);
    }

    void RequesterTab::BeforePropertyModified(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }

    void RequesterTab::AfterPropertyModified(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }

    void RequesterTab::SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }

    void RequesterTab::SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* /*node*/)
    {
        m_setregManager.SaveSystemConfiguration<BasicJSONRequester>(
            m_requester_test.second,
            m_requester_test.first,
            [](const BasicJSONRequester& obj1, AICoreSettingsRegistryManager::Result result)
            {
                if (result == AICoreSettingsRegistryManager::Result::Success)
                {
                    std::cout << "Good" << std::endl;
                }
                else
                {
                    std::cout << "bad" << std::endl;
                }
            });

        m_setregManager.SaveSystemConfiguration<BasicJSONRequester>(
            m_requester_test2.second,
            m_requester_test2.first,
            [](const BasicJSONRequester& obj1, AICoreSettingsRegistryManager::Result result)
            {
                if (result == AICoreSettingsRegistryManager::Result::Success)
                {
                    std::cout << "Good" << std::endl;
                }
                else
                {
                    std::cout << "bad" << std::endl;
                }
            });
    }

    void RequesterTab::SealUndoStack()
    {
    }
} // namespace AICore