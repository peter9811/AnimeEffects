#ifndef GUI_TOOL_SRTPANEL_H
#define GUI_TOOL_SRTPANEL_H

#include "ctrl/SRTParam.h"
#include "gui/GUIResources.h"
#include "gui/tool/tool_Items.h"
#include "util/Signaler.h"
#include <QGroupBox>
#include <functional>
#include <vector>

namespace gui {
namespace tool {

    class SRTPanel: public QGroupBox {
        Q_OBJECT
    public:
        SRTPanel(QWidget* aParent, GUIResources& aResources);

        int updateGeometry(const QPoint& aPos, int aWidth);

        const ctrl::SRTParam& param() const {
            return mParam;
        }

        // boost like signals
        util::Signaler<void(bool)> onParamUpdated; // a argment will be true when the layout is changed.

    private:
        void createMode();
        void updateTypeParam(int aType);

        GUIResources& mResources;
        ctrl::SRTParam mParam;
        QScopedPointer<SingleOutItem> mTypeGroup;
        QScopedPointer<CheckBoxItem> mAddMove;
        QScopedPointer<CheckBoxItem> mAddRotate;
        QScopedPointer<CheckBoxItem> mAddScale;
        QScopedPointer<CheckBoxItem> mAdjust;
    };

} // namespace tool
} // namespace gui

#endif // GUI_TOOL_SRTPANEL_H
