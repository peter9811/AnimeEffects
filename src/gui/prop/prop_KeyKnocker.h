#ifndef GUI_PROP_KEYKNOCKER_H
#define GUI_PROP_KEYKNOCKER_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <functional>

namespace gui {
namespace prop {

    class KeyKnocker: public QGroupBox {
    public:
        KeyKnocker(const QString& aLabel);
        void set(const std::function<void()>& aKnocker);

    private:
        QPushButton* mButton;
        QHBoxLayout* mLayout;
    };

} // namespace prop
} // namespace gui

#endif // GUI_PROP_KEYKNOCKER_H
