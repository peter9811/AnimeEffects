#ifndef CTRL_UILOGGER_H
#define CTRL_UILOGGER_H

#include "ctrl/UILog.h"
#include "ctrl/UILogType.h"
#include <QString>

namespace ctrl {

class UILogger {
public:
    virtual ~UILogger() {}
    virtual void pushLog(const QString& aMessage, UILogType aType) = 0;
};

} // namespace ctrl

#endif // CTRL_UILOGGER_H
