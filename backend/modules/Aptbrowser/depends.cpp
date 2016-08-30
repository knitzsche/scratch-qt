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

#include "depends.h"
#include "controller.h"
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

/*
 * run this command and return result as string
 */
QString result(const QString &cmd) {
    FILE* pipe = popen(cmd.toStdString().c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    QString result = "";
    while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe) != NULL)
      result += QString::fromStdString(buffer);
    }
    pclose(pipe);
    if (result.length() == 0)
        return NO_RESULT;
    else
        return result;
}

/*
 * Return string of packages this pkg depends on delimited by newlines
 */
QString depends_(const QString pkg, QString mode="Depends") {
    QString res = result(QString("apt-cache depends --no-suggests --no-enhances %1").arg(pkg));
    QStringList deps;
    QStringList lines = res.split("\n");
    for (int n=0; n<lines.count(); n++)
    {
        if (lines[n].trimmed().startsWith(mode))
        {
            QString dep = lines[n].trimmed();
            dep = dep.replace(mode + ": ", "");
            if (deps.contains(dep))
                continue;
            deps.append(dep);
        }
        else if (res == NO_RESULT)
        {
            return "INVALID_PACKAGE";
        }
    }
    return deps.join('\n');
}

/*
 * Return list of rdepends packages for the passed pkg
 */
QStringList rdepends(const QString pkg) {
    qDebug() << "==== in redepends()";
    QString res = result(QString("apt-cache rdepends --no-suggests --no-enhances %1").arg(pkg));
    QStringList rdeps;
    QStringList lines = res.split("\n");
    for (int n=0; n<lines.count(); n++)
    {
        QString line = lines[n].trimmed();
        if (n < 1) continue; //discard fist two lines
        if (line.startsWith("|"))
            continue;
        if (line.startsWith("Reverse"))
            continue;
        QString rdep = line;
        if (!rdep.isEmpty())
        {
            rdeps.append(rdep);
            qDebug() << "==== rdep: " << rdep;
        }
    }
    return  rdeps;
}

/*
 * For the given parent package and child package, return the type of
 * dependency, that is, how the parent depends on the child
 */
QString getRdepType(QString parent, QString child)
{
    qDebug() << "==== in getRdepType(). parent: " << parent << " child: " <<child;
    QString res = result(QString("apt-cache depends %1").arg(parent));
    QString type;
    QStringList lines = res.split("\n");
    for (int n=0; n<lines.count(); n++) {
        if (n == lines.count() -1) break;
        QString line = lines[n].trimmed();
        //qDebug() << "==== line: " << line;
        if (line.isEmpty())
            continue;
        if (line.startsWith("E:"))
            continue;
        if (line.endsWith(child, Qt::CaseInsensitive)) {
            type = line.split(":")[0];
            break;
        }
    }
    //qDebug() << "==== in getRdepType(). type: " << type;
    return type;
}

Depends::~Depends() {
}


void Depends::get_dependencies(QString pkg, int dep_idx)
{
    qDebug() << "==== in Depends::get_dependencies()";
    Controller *cont = new Controller(this);
    Q_EMIT cont->operate(pkg.toLower(), dep_idx);
    return;
}

void GetDepends::get_depends(const QString &pkg, int dep_idx)
{
    qDebug() << "====== in get_depends, pkg:" << pkg;

    if (deps_.have_deps.contains(pkg))
    {
        Q_EMIT dependsReady(deps_.have_deps[pkg], dep_idx);
        return;
    }

    QString res = depends_(pkg);
    qDebug() << "==== in GetDepends::get_depends() deps:\n" << res;
    if (res == "INVALID_PACKAGE")
    {
        qDebug() << "PACKAGE is INVALID" << pkg;
        Q_EMIT invalidPackage(pkg);
        return;
    }
    deps_.have_deps[pkg] = res;
    Q_EMIT dependsReady(res, dep_idx);
}

void Depends::get_recommendations(QString pkg, int rec_idx)
{
    qDebug() << "==== in get_recommendations()";
    Controller *cont = new Controller(this, "Recommends");
    Q_EMIT cont->operate(pkg.toLower(), rec_idx);
    return;
}

void GetDepends::get_recs(const QString &pkg, int rec_idx)
{
    qDebug() << "===== in get_recs, pkg:" << pkg;

    if (deps_.have_recs.contains(pkg))
    {
        Q_EMIT recommendsReady(deps_.have_recs[pkg], rec_idx);
        return;
    }

    QString res = depends_(pkg, "Recommends");
    if (res == "INVALID_PACKAGE")
    {
        qDebug() << "PACKAGE is INVALID" << pkg;
        Q_EMIT invalidPackage(pkg);
        return;
    }
    deps_.have_recs[pkg] = res;
    Q_EMIT recommendsReady(res, rec_idx);
}

void Depends::get_parents_dependencies(QString pkg, int par_dep_idx)
{
    qDebug() << "==== in Depends::get_parents_dependencies()";
    Controller *cont = new Controller(this, "ParentsDepends");
    Q_EMIT cont->operate(pkg.toLower(), par_dep_idx);
    return;
}

void Depends::get_parents_recommendations(QString pkg, int par_rec_idx)
{
    qDebug() << "==== in Depends::get_parents_recommendations()";
    Controller *cont = new Controller(this, "ParentsRecommends");
    Q_EMIT cont->operate(pkg.toLower(), par_rec_idx);
    return;
}

void GetDepends::get_parents_depends(const QString &pkg_, int par_dep_idx)
{
    QString pkg = pkg_.toLower();
    qDebug() << "==== in GetDepends::get_parents_depends(): pkg: " << pkg;
    QVariantMap m;
    m.insert("package",pkg);
    deps_.setParents(m);
    deps_.getParentsOfType("Depends", par_dep_idx);
}

void GetDepends::get_parents_recommends(const QString &pkg, int par_rec_idx)
{
    qDebug() << "==== in GetDepends::get_parents_recommends(): pkg: " << pkg;
    QVariantMap m;
    m.insert("package",pkg);
    deps_.setParents(m);
    deps_.getParentsOfType("Recommends", par_rec_idx);
}

/*
 * returns the current parents whose depends are of specified type
 */
void Depends::getParentsOfType(QString type, int par_idx)
{
    QStringList parents_of_type;

    QMap<QString, QVariant>::const_iterator i = parents_m.constBegin();
    while (i != parents_m.constEnd()) {
        if (i.value() == type)
            parents_of_type.append(i.key());
        ++i;
    }
    //return parents_of_type.join('\n');
    QString p_of_t = parents_of_type.join('\n');
    qDebug() << "==== parents of type:\n" << p_of_t;
    if (type.endsWith("Depends"))
        Q_EMIT parentsDependsReady(p_of_t, par_idx);
    else if (type.endsWith("Recommends"))
        Q_EMIT parentsRecommendsReady(p_of_t, par_idx);
}

void Depends::setParents(QVariantMap pkg)
{
    QStringList rdeps;
    QVariant pkg_v = pkg.value("package");
    if (have_pars.contains(pkg_v.toString()))
    {
        qDebug() << "==== already have these rdeps";
        rdeps = have_pars[pkg_v.toString()];
    }
    else
    {
        rdeps = rdepends(pkg_v.toString());
        have_pars[pkg_v.toString()] = rdeps;
    }
    parents_m.clear();
    QVariantMap parents_m_;
    QString child = pkg_v.toString();
    for (QString parent : rdeps)
    {
        QString key = QString("%1%2").arg(parent, child);
        if (parents_m.contains(parent))
            continue;
        if (parChild_type.contains(key))
        {
            qDebug() << "==== we KNOW this parchild dep type";
            parents_m_[parent] = parChild_type[key];
        }
        else
        {
            qDebug() << "==== we DONT know this parchild dep type";
            parents_m_[parent] = getRdepType(parent, pkg_v.toString());
            parChild_type[key] = parents_m_[parent].toString();
        }
        //qDebug() << "==== parents_m_[parent]: " << parents_m_[parent];
    }
    parents_m = parents_m_;
    Q_EMIT parentsChanged();
}

void Depends::get_policy(QString pkg)
{
    qDebug() << "==== in get_policy(). pkg: " << pkg;
    Controller *cont = new Controller(this, "policy");
    Q_EMIT cont->operate(pkg, -1);
    return;
}
void GetDepends::get_pol(const QString &pkg)
{
    QString pol = result("apt-cache policy " + pkg);
    qDebug() << "==== in get_pol(). pol: " << pol ;
    Q_EMIT policyReady(pol);
}

