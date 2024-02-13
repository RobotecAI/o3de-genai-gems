
#pragma once

#if !defined(Q_MOC_RUN)
#include <AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h>
#include <AzQtComponents/Components/Widgets/SegmentControl.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <Communication/JSONHttp/BasicJSONRequester.h>
#include <QWidget>
#include <UI/PropertyEditor/ReflectedPropertyEditor.hxx>
#include <qwidget.h>
#endif

namespace AICore
{
    class RequesterTab
        : public QWidget
        , public AzToolsFramework::IPropertyEditorNotify
    {
        Q_OBJECT
    public:
        explicit RequesterTab(QWidget* parent = nullptr);

    private:
        void CreatePropertyEditor(QWidget* parent);

        void BeforePropertyModified(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void AfterPropertyModified(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SealUndoStack() override;

        AzQtComponents::SegmentControl* m_segmentControl;

        AzToolsFramework::ReflectedPropertyEditor* m_propertyEditor;

        // AZStd::pair<AZStd::string, BasicJSONRequester> m_requester_test = { "Requester 1", BasicJSONRequester() };
        AZStd::vector<BasicJSONRequester> m_requester_test;
    };
} // namespace AICore