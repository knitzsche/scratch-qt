#include "thread_controller.h"

Thread_Controller::Thread_Controller(ThreadWorker *thread, QString type) : thread_(thread)
{
    thread->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, thread_, &QObject::deleteLater);

    if (type == "run")
    {
        connect(this, &Thread_Controller::operate, thread, &ThreadWorker::do_run);
        //connect(gdeps, &GetDepends::runReady, deps_, &Depends::updateRun);
        //connect(thread, &MThead::runDone, d, &Depends::runDone);
    }
/*
    if (deptype == "policy")
    {
        connect(this, &Thread_Controller::operate, gdeps, &GetDepends::get_pol);
        connect(gdeps, &GetDepends::policyReady, deps_, &Depends::updatePolicy);
        connect(gdeps, &GetDepends::polDone, deps_, &Depends::policyDone);
    }
    else if (deptype == "run")
    {
        connect(this, &Thread_Controller::operate, gdeps, &GetDepends::run);
        //connect(gdeps, &GetDepends::runReady, deps_, &Depends::updateRun);
        connect(gdeps, &GetDepends::runDone, deps_, &Depends::runDone);
    }
*/

    workerThread.start();
}

Thread_Controller::~Thread_Controller() {
    workerThread.quit();
    workerThread.wait();
}
