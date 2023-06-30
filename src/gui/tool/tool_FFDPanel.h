#ifndef GUI_TOOL_FFDPANEL_H
#define GUI_TOOL_FFDPANEL_H

#include "ctrl/FFDParam.h"
#include "gui/GUIResources.h"
#include "gui/tool/tool_Items.h"
#include "util/Signaler.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include <functional>
#include <vector>

namespace gui {
namespace tool {

    class FFDPanel: public QGroupBox {
        Q_OBJECT
    public:
        FFDPanel(QWidget* aParent, GUIResources& aResources);

        int updateGeometry(const QPoint& aPos, int aWidth);

        const ctrl::FFDParam& param() const {
            return mParam;
        }

        // boost like signals
        util::Signaler<void(bool)> onParamUpdated;

    private:
        void createBrush();
        void updateTypeParam(ctrl::FFDParam::Type aType);

        GUIResources& mResources;
        ctrl::FFDParam mParam;
        QScopedPointer<SingleOutItem> mTypeGroup;
        QScopedPointer<SingleOutItem> mHardnessGroup;
        QScopedPointer<SliderItem> mRadius;
        QScopedPointer<SliderItem> mPressure;
        QScopedPointer<SliderItem> mBlur;
        QScopedPointer<SingleOutItem> mEraseHardnessGroup;
        QScopedPointer<SliderItem> mEraseRadius;
        QScopedPointer<SliderItem> mErasePressure;
    };

} // namespace tool
} // namespace gui

#endif // GUI_TOOL_FFDPANEL_H
