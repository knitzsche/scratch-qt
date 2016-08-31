#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "depends.h"


class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller(Depends *deps, QString deptype="Depends");
    ~Controller();
    Depends *deps_;
signals:
    void operate(const QString &, int);
};

#endif // CONTROLLER_H

