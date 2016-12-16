#ifndef JOBCONTROLLER_H
#define JOBCONTROLLER_H

#include "job.h"
#include "jobs.h"

class JobController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    JobController(Job *job, const QString & job_name);
    ~JobController();

signals:
    void operate(const QString &);

private:
    Job * job_;
};

#endif // JOB_CONTROLLER_H
