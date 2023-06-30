#ifndef GUI_PROP_PROJECTPANEL_H
#define GUI_PROP_PROJECTPANEL_H

#include "core/ObjectNode.h"
#include "core/Project.h"
#include "gui/prop/prop_AttrGroup.h"
#include "gui/prop/prop_Items.h"
#include "gui/prop/prop_KeyGroup.h"
#include "gui/prop/prop_Panel.h"

namespace gui {
namespace prop {

    class ProjectPanel: public Panel {
    public:
        ProjectPanel(core::Project& aProject, QWidget* aParent);

    private:
        void build();

        core::Project& mProject;

        AttrGroup* mAttributes;
        // DecimalItem* mDepth;
    };

} // namespace prop
} // namespace gui

#endif // GUI_PROP_PROJECTPANEL_H
