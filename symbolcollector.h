#ifndef SYMBOLCOLLECTOR_H
#define SYMBOLCOLLECTOR_H

#include "objectfeature.h"

class SymbolCollector : public ObjectFeature
{
    Q_OBJECT
public:
    void update() override;
};

#endif // SYMBOLCOLLECTOR_H
