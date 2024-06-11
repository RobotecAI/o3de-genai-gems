/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QWidget>
#endif

#include <AzCore/Component/Entity.h>
#include <AzCore/std/containers/unordered_map.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

#include "GenAIFrameworkWidget.h"
#include "NewChatWidget.h"

namespace Ui
{
    class AIAssistantUI;
}

namespace GenAIFramework
{
    class AIAssistantWidget
        : public QMainWindow
        //, private GenAIFramework::GenAIFrameworkNotificationBus::Handler
    {
        Q_OBJECT
    public:
        explicit AIAssistantWidget(QWidget* parent = nullptr);
        // void UpdateModelAndProviderLists();
        ~AIAssistantWidget() override;

    private slots:
        void OnNewChatAction();
        void OnConfigureAction();
        void OnChatCreated(const QString& chatName, const QString& modelName, const QString& providerName);

    protected:
        // QWidget overrides
        void closeEvent(QCloseEvent* event) override;
        void showEvent(QShowEvent* event) override;

    private:

        Ui::AIAssistantUI* m_ui;
        GenAIFrameworkWidget* m_optionsWidget;
        NewChatWidget* m_newChatWidget;
    };
} // namespace GenAIFramework
