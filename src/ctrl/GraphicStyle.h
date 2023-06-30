#ifndef CTRL_GRAPHICSTYLE_H
#define CTRL_GRAPHICSTYLE_H

#include <QIcon>
#include <QRect>
#include <QString>

namespace ctrl {

class GraphicStyle {
public:
    virtual ~GraphicStyle() {}
    virtual QFont font() const = 0;
    virtual QRect boundingRect(const QString& aText) const = 0;
    virtual QIcon icon(const QString& aName) const = 0;
};

} // namespace ctrl

#endif // CTRL_GRAPHICSTYLE_H
