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

#ifndef JOB_H 
#define JOB_H

#include <QObject>
#include <QQuickItem>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QDebug>
#include <QThread>

class Job: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY( QString job NOTIFY jobDone )

public:
    Job(QQuickItem *parent = 0) :
        QQuickItem(parent)
    {
    }
    ~Job();
    

public slots:
    void start(QString const & name);

signals:
    void jobDone();

};

#endif // JOB_H

