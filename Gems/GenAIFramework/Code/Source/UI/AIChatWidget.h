/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <QVBoxLayout>
#include <QWidget>

#include <AzCore/Component/Entity.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

#endif

namespace Ui
{
    class AIChatWidgetUI;
}

namespace GenAIFramework
{
    class AIChatWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit AIChatWidget(QWidget* parent = nullptr, QString modelName = "", QString providerName = "");

    private slots:
        void OnRequestButton();

    private:
        void UiAppendChatMessage(const AZStd::string& message, const bool response = false);
        void UiClearMessages();

        Ui::AIChatWidgetUI* m_ui;
        QVBoxLayout* m_uiChatLayout;
    };
} // namespace GenAIFramework
