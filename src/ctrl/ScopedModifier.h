#ifndef CTRL_SCOPEDMODIFIER
#define CTRL_SCOPEDMODIFIER

#include "cmnd/ScopedUndoSuspender.h"
#include "core/Animator.h"
#include "core/Project.h"

namespace ctrl {

class ScopedModifier {
public:
    ScopedModifier(core::Project& aProject): mUndoSuspend(aProject.commandStack()), mAnimator(aProject.animator()) {
        mAnimator.suspend();
    }

    ~ScopedModifier() {
        mAnimator.resume();
    }

private:
    cmnd::ScopedUndoSuspender mUndoSuspend;
    core::Animator& mAnimator;
};

} // namespace ctrl

#endif // CTRL_SCOPEDMODIFIER
