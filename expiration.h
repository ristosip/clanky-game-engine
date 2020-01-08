#ifndef EXPIRATION_H
#define EXPIRATION_H

#include "objectfeature.h"

class Expiration : public ObjectFeature
{
    Q_OBJECT
public:
    Expiration(int time_ms, bool start = true, bool loop = false, bool remove_object_when_expired = false, QObject *parent = nullptr);
    void update() override;
public slots:
    void start();
signals:
    void expired();
private:
    bool m_remove_object_when_expired;
    int m_time_ms;
    int m_time_passed;
    bool m_done;
    bool m_running;
    bool m_loop;
};

#endif // EXPIRATION_H
