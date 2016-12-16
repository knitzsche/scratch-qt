#ifndef THREAD_CONTROLLER_H
#define THREAD_CONTROLLER_H

#include "threadworker.h"

class Thread_Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    Thread_Controller(ThreadWorker *thread, QString type="default");
    ~Thread_Controller();
    ThreadWorker *thread_;

signals:
    void operate(const QString &);

};

#endif // THREAD_CONTROLLER_H
