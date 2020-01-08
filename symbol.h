#ifndef SYMBOL_H
#define SYMBOL_H

#include "gameobject.h"

class Symbol : public GameObject
{
public:
    Symbol(int symbol_code, const  QString &image_path, qreal image_scale = 1.0, QGraphicsItem *parent = nullptr);
    int symbolCode();
    void advanceObject(int delta_time, int phase) override;
private:
    int m_symbol_code;
    unsigned int m_attribute_mask;
};

#endif // SYMBOL_H
