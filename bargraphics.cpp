#include "bargraphics.h"
#include "pixmapitem.h"

BarGraphics::BarGraphics(const QString &filePath, qreal scale, qreal x, qreal y, QObject *parent) : ObjectFeature(parent)
{
    m_pixmapitem = new PixmapItem(QPixmap(filePath));
    m_pixmapitem->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    m_pixmapitem->setScale(scale);
    m_pixmapitem->setPos(x, y);
}

void BarGraphics::setOwner(GameObject *owner)
{
    m_owner = owner;
    m_pixmapitem->setParentItem(m_owner);
}
