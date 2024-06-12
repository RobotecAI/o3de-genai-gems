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
#include <QLayout>
#include <QWidget>
#include <QPushButton>
#include <QVector>

#include <AzCore/std/containers/vector.h>
#include <AzCore/Component/Entity.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#endif


namespace GenAIFramework
{
    class AIChatMessage : public QWidget
    {
        Q_OBJECT
    public:
        explicit AIChatMessage(QWidget* parent = nullptr,
                               const AZStd::string& message = "",
                               const AZStd::vector<AZStd::string>& details = {},
                               bool isAssistantReply = false);
        ~AIChatMessage() override;

    public slots:
        void OnDetailsButton();

    protected:
      void paintEvent(QPaintEvent* event) override;

    private:
        const AZStd::string message;
        const AZStd::vector<AZStd::string> details;
        const bool isAssistantReply;
        QLayout* m_layout;
        QPushButton* details_button;
        QVector<QLabel*> detail_labels;
        bool show_details;

        void setupShortLayout();
        void setupDetailsLayout();
    };
} // namespace GenAIFramework

