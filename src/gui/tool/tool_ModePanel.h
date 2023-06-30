#ifndef GUI_TOOL_MODEPANEL_H
#define GUI_TOOL_MODEPANEL_H

#include "ctrl/ToolType.h"
#include "gui/GUIResources.h"
#include "gui/tool/tool_FlowLayout.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QPushButton>
#include <functional>
#include <vector>

namespace gui {
namespace tool {

    class ModePanel: public QGroupBox {
        Q_OBJECT
    public:
        typedef std::function<void(ctrl::ToolType, bool)> PushDelegate;

        ModePanel(QWidget* aParent, GUIResources& aResources, const PushDelegate& aOnPushed);

        int updateGeometry(const QPoint& aPos, int aWidth);
        QPushButton* button(ctrl::ToolType aId) {
            return mButtons[aId];
        }
        const QPushButton* button(ctrl::ToolType aId) const {
            return mButtons[aId];
        }

        void pushButton(ctrl::ToolType aId);

    private:
        void addButton(ctrl::ToolType aType, const QString& aIconName, const QString& aToolTip);
        void onThemeUpdated(theme::Theme&);

        GUIResources& mGUIResources;
        QButtonGroup* mGroup;
        std::vector<QPushButton*> mButtons;
        FlowLayout mLayout;
        PushDelegate mOnPushed;
    };

} // namespace tool
} // namespace gui

#endif // GUI_TOOL_MODEPANEL_H
