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

#ifndef JOBS_H 
#define JOBS_H

#include <QObject>
#include <QQuickItem>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QDebug>
#include <QThread>

class Jobs: public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString run READ run NOTIFY jobDone )

public:
    Jobs(QObject *parent = 0) :
        QObject(parent)
    {
    }
    ~Jobs();
    QString run_;
    QString run() { return run_; }
    void setRun(QString run_);

public slots:
    void job1();
    void job2();
    void job3();
    void job4();

signals:
    void jobDone();

};

#endif // JOBS_H

