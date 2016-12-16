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

#ifndef THREADWORKER_H 
#define THREADWORKER_H

#include <QObject>
#include <QQuickItem>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QDebug>
#include <QThread>

class ThreadWorker: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY( QString run READ run NOTIFY runDone )

public:
    Q_INVOKABLE void start_me();

    ThreadWorker(QQuickItem *parent = 0) :
        QQuickItem(parent)
    {
    }
    ~ThreadWorker();

    QString run_;
    QString run() { return run_; }
    void setRun(QString run_);

public slots:
    void do_run();

signals:
    void runDone();

};

#endif // THREAD_H

