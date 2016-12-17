#include "jobcontroller.h"
#include "job.h"

JobController::JobController(Job *job, const QString & job_name) : job_(job)
{
    Jobs *jobs = new Jobs();
    jobs->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, jobs, &QObject::deleteLater);

    qDebug() << "=== controller job_name: " << job_name;
    if (job_name == "job1")
    {
        connect(this, &JobController::operate, jobs, &Jobs::job1);
    }
    else if (job_name == "job2")
    {
        connect(this, &JobController::operate, jobs, &Jobs::job2);
    }
    else if (job_name == "job3")
    {
        connect(this, &JobController::operate, jobs, &Jobs::job3);
    }
    else if (job_name == "job4")
    {
        connect(this, &JobController::operate, jobs, &Jobs::job4);
    }
    connect(jobs, &Jobs::jobDone, job_, &Job::jobDone);

    workerThread.start();
}

JobController::~JobController() {
    workerThread.quit();
    workerThread.wait();
}
