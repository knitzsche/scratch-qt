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

#include <QtQml>
#include <QtQml/QQmlContext>
#include <QDebug>
#include "backend.h"
#include "depends.h"
#include "cli.h"
#include "controller.h"
#include "job.h"


void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Aptbrowser"));
    
    qDebug() << "=== register to uri: " << uri;

    qmlRegisterType<Depends>(uri, 1, 0, "Depends");
    qmlRegisterType<Cli>(uri, 1, 0, "Cli");
    qmlRegisterType<Job>(uri, 1, 0, "Job");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
