/*
 * Copyright (C) 2015 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Kyle Nitzsche <kyle.nitzsche@canonical.com>
 *
 */

#include "threadworker.h"
#include "thread_controller.h"
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QStringList>
#include <QVariant>
#include <QDebug>

ThreadWorker::~ThreadWorker() {
}

void ThreadWorker::start_me()
{
    qDebug() << "==== in ThreadWorker::start_me()";
    Thread_Controller *cont = new Thread_Controller(this, "run");
    Q_EMIT cont->operate("run");
    return;
}

void ThreadWorker::do_run()
{
    qDebug() << "==== in ThreadWorker::do_run(): ";
    //TODO better random pick, mayb 1-10 secs
    auto sleepTime = 100000L+(long)((1e5-1e4)*rand()/(RAND_MAX+1.0));  
    sleepTime = sleepTime * 30;
    qDebug() << "=== sleep time: " << QString::number(sleepTime);
    usleep(sleepTime);
    qDebug() << "=== sleep DONE: " << QString::number(sleepTime);
    Q_EMIT runDone();//the Controller for now simply connects this to Depends runDone signal, which is heard in qml
}

