#ifndef GUI_RES_ITEM_H
#define GUI_RES_ITEM_H

#include "img/ResourceNode.h"
#include "util/TreePos.h"
#include <QTreeWidgetItem>

namespace gui {
namespace res {

    class Item: public QTreeWidgetItem {
    public:
        static Item* cast(QTreeWidgetItem* aItem);
        static const Item* cast(const QTreeWidgetItem* aItem);

        Item(const QTreeWidget& aTree, img::ResourceNode& aNode, const QString& aIdentifier);
        img::ResourceNode& node() {
            return mNode;
        }
        const img::ResourceNode& node() const {
            return mNode;
        }
        bool isTopNode() const {
            return !mNode.parent();
        }

        util::TreePos treePos();

    private:
        const QTreeWidget& mTree;
        img::ResourceNode& mNode;
    };

} // namespace res
} // namespace gui

#endif // GUI_RES_ITEM_H
