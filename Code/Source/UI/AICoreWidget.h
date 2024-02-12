
#pragma once

#if !defined(Q_MOC_RUN)
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <QWidget>
#endif

namespace AICore
{
    class AICoreWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit AICoreWidget(QWidget* parent = nullptr);
    };
} // namespace AICore