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

#include "job.h"
#include "jobcontroller.h"
#include <QDebug>

Job::~Job() {
}

void Job::start(QString const & job_name)
{
    qDebug() << "==== in Job::start_me()";
    JobController *cont = new JobController(this, job_name);
    Q_EMIT cont->operate(job_name);
    return;
}
