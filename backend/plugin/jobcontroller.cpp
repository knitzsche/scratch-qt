#include "jobcontroller.h"
#include "job.h"

JobController::JobController(Job *job, const QString & job_name) : job_(job)
{
    Jobs *jobs = new Jobs();
    jobs->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, jobs, &QObject::deleteLater);

    if (job_name == "job1")
    {
        connect(this, &JobController::operate, jobs, &Jobs::job1);
        //connect(gdeps, &GetDepends::runReady, deps_, &Depends::updateRun);
        connect(jobs, &Jobs::job1Done, job_, &Job::jobDone);
    }
/*
    if (deptype == "policy")
    {
        connect(this, &Job_Controller::operate, gdeps, &GetDepends::get_pol);
        connect(gdeps, &GetDepends::policyReady, deps_, &Depends::updatePolicy);
        connect(gdeps, &GetDepends::polDone, deps_, &Depends::policyDone);
    }
    else if (deptype == "run")
    {
        connect(this, &Job_Controller::operate, gdeps, &GetDepends::run);
        //connect(gdeps, &GetDepends::runReady, deps_, &Depends::updateRun);
        connect(gdeps, &GetDepends::runDone, deps_, &Depends::runDone);
    }
*/

    workerThread.start();
}

JobController::~JobController() {
    workerThread.quit();
    workerThread.wait();
}
