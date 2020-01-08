#include "symbol.h"
#include "attributesandstates.h"
#include "pixmapitem.h"
#include <QPropertyAnimation>

Symbol::Symbol(int symbol_code, const QString &image_path, qreal image_scale, QGraphicsItem *parent) : GameObject(parent)
{
    setRect(0, 0, 10, 10);

    m_symbol_code = symbol_code;

    PixmapItem *image = new PixmapItem(QPixmap(image_path));
    image->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    image->setScale(image_scale);
    image->setParentItem(this);

    QPropertyAnimation *animation = new QPropertyAnimation(image, "scale");
    animation->setStartValue(1.0 * image_scale);
    animation->setEndValue(0.9 * image_scale);
    animation->setLoopCount(-1);
    animation->setEasingCurve(QEasingCurve::SineCurve);
    animation->setDuration(750);
    animation->start();

    m_attribute_mask = 1<<COLLECTIBLE;
    m_databank.attributes = 0;
    m_databank.attributes = m_databank.attributes | m_attribute_mask;
}

int Symbol::symbolCode()
{
    return m_symbol_code;
}

void Symbol::advanceObject(int delta_time, int phase)
{
    if(phase == 1){
        m_databank.attributes = m_databank.attributes | m_attribute_mask;
    }
    GameObject::advanceObject(delta_time, phase);
}
