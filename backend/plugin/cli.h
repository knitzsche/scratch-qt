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

#ifndef CLI_H
#define CLI_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QDebug>
#include <QThread>

class Cli: public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString cliResult NOTIFY cliResultChanged )

public:
    Q_INVOKABLE QString cli(QString const&);

    Cli(QObject *parent = 0) :
        QObject(parent)
    {}
    

signals:
    void cliResultChanged();
};

#endif // CLI_H

