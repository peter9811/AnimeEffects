#ifndef GUI_TOOL_MESHPANEL_H
#define GUI_TOOL_MESHPANEL_H

#include "ctrl/MeshParam.h"
#include "gui/GUIResources.h"
#include "gui/tool/tool_Items.h"
#include "util/Signaler.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QPushButton>

namespace gui {
namespace tool {

    class MeshPanel: public QGroupBox {
        Q_OBJECT
    public:
        MeshPanel(QWidget* aParent, GUIResources& aResources);

        int updateGeometry(const QPoint& aPos, int aWidth);

        const ctrl::MeshParam& param() const {
            return mParam;
        }

        // boost like signals
        util::Signaler<void(bool)> onParamUpdated;

    private:
        void createMode();

        GUIResources& mResources;
        ctrl::MeshParam mParam;
        QScopedPointer<SingleOutItem> mTypeGroup;
    };

} // namespace tool
} // namespace gui

#endif // GUI_TOOL_MESHPANEL_H
