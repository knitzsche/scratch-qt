/*
 * Copyright (C) 2016 Canonical Ltd
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

#include "cli.h"
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <stdio.h>
#include <stdlib.h>
#include <QStringList>
#include <QVariant>
#include <QDebug>

const QString NO_RESULT = "no_result";

QString Cli::cli(const QString &cmd) {
    FILE* pipe = popen(cmd.toStdString().c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    QString result = "";
    while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe) != NULL)
      result += QString::fromStdString(buffer);
    }
    pclose(pipe);
    Q_EMIT cliResultChanged();
    if (result.length() == 0)
        return NO_RESULT;
    else
        return result;
}
