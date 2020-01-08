#ifndef BARGRAPHICS_H
#define BARGRAPHICS_H

#include "objectfeature.h"
#include "pixmapitem.h"

class BarGraphics : public ObjectFeature
{
    Q_OBJECT
public:
    BarGraphics(const QString &filePath = ":/", qreal scale = 1, qreal x = 0, qreal y = 0, QObject *parent = nullptr);
protected:
    void setOwner(GameObject *owner) override;
private:
    PixmapItem *m_pixmapitem;
};

#endif // BARGRAPHICS_H
