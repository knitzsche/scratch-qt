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

#ifndef DEPENDS_H
#define DEPENDS_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QDebug>
#include <QThread>
class Depends: public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString policy READ policy NOTIFY policyChanged )
    Q_PROPERTY( QString depends READ depends NOTIFY dependsChanged )
    Q_PROPERTY( QString recommends READ recommends NOTIFY recommendsChanged )
    Q_PROPERTY( QString parentsDepends READ parentsDepends NOTIFY parentsDependsChanged )
    Q_PROPERTY( QString parentsRecommends READ parentsRecommends NOTIFY parentsRecommendsChanged )

public:
    Q_INVOKABLE QString test_pass(QString);
    Q_INVOKABLE void get_dependencies(QString, int);
    Q_INVOKABLE void get_recommendations(QString, int);
    Q_INVOKABLE void get_parents_dependencies(QString, int);
    Q_INVOKABLE void get_parents_recommendations(QString, int);
    Q_INVOKABLE void get_policy(QString);
    Depends(QObject *parent = 0) :
        QObject(parent)
    {
    }
    ~Depends();

    QString pol;
    QString policy() { return pol; }
    void setPolicy(QString pkg);

    QString deps;
    QString depends() { return deps; }
    QMap<QString,QString> have_deps;//pkg to deps

    QString recs;
    QString recommends() { return recs; }
    QMap<QString,QString> have_recs;//pkg to recs

    QVariantMap parents_m;
    QVariantMap parents() { return parents_m; }
    void setParents(QVariantMap pkg);
    QMap<QString,QStringList> have_pars;//pkg to pars

    QMap<QString,QString> parChild_type;// parpkgchildpkg to dep type

    QString pars_deps;
    QString parentsDepends() { return pars_deps; }

    QString pars_recs;
    QString parentsRecommends() { return pars_recs; }

    void getParentsOfType(QString pkg, int par_idx);

public slots:
    bool updateDeps(const QString res, int dep_idx)
    {
        deps = res;
        Q_EMIT dependsChanged(dep_idx);
        qDebug() << "==== in updateDeps(), deps:" << deps;
        return true;
    }
    bool updateRecs(const QString res, int rec_idx)
    {
        recs = res;
        Q_EMIT recommendsChanged(rec_idx);
        qDebug() << "==== in updateRecs()";
        return true;
    }
    bool updateParsDeps(const QString res, int par_dep_idx)
    {
        pars_deps = res;
        Q_EMIT parentsDependsChanged(par_dep_idx);
        qDebug() << "==== in updateParsDeps()";
        return true;
    }
    bool updateParsRecs(const QString res, int par_rec_idx)
    {
        pars_recs = res;
        Q_EMIT parentsRecommendsChanged(par_rec_idx);
        qDebug() << "==== in updateParsRecs()";
        return true;
    }
    bool updatePolicy(const QString res)
    {
        pol = res;
        Q_EMIT policyChanged();
        qDebug() << "==== in updatePolicy()" << pol;
        return true;
    }
    void badPackage(const QString pkg)
    {
        Q_EMIT invalidPackageFound();
    }
    // signal to stop the Get Depends spinner
    // useful when the package is invalid
    void depsInvalid()
    {
        const QString deps__ = "";
        updateDeps(deps__, -1);
    }
    void recsInvalid()
    {
        const QString deps__ = "";
        updateRecs(deps__, -1);
    }
    void parsDepsInvalid()
    {
        const QString deps__ = "";
        updateParsDeps(deps__, -1);
    }
    void parsRecsInvalid()
    {
        const QString deps__ = "";
        updateParsRecs(deps__, -1);
    }

signals:
    void policyChanged();
    void dependsChanged(int dep_idx);
    void recommendsChanged(int rec_idx);
    void parentsChanged();
    void parentsDependsChanged(int par_dep_idx);
    void parentsRecommendsChanged(int par_rec_idx);
    void parentsDependsReady(const QString &result, int par_dep_idx);
    void parentsRecommendsReady(const QString &result, int par_rec_idx);
    void invalidPackageFound();
    void policyDone();
};

class GetDepends : public QObject
{
    Q_OBJECT
public:
    GetDepends(Depends &deps) : deps_(deps)
{
}
    Depends &deps_;

public slots:
    void get_depends(const QString &pkg, int dep_idx);
    void get_recs(const QString &pkg, int rec_idx);
    void get_parents_depends(const QString &pkg, int par_dep_idx);
    void get_parents_recommends(const QString &pkg, int par_rec_idx);
    void get_pol(const QString &pkg);
signals:
    void dependsReady(const QString &result, int dep_idx);
    void recommendsReady(const QString &result, int rec_idx);
    void policyReady(const QString &result);
    void invalidPackage(const QString &pkg);
    void depsInvalid();
    void polDone();
};



class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller(Depends *deps, QString deptype="Depends") : deps_(deps)
    {
        GetDepends *gdeps = new GetDepends(*deps_);
        gdeps->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, gdeps, &QObject::deleteLater);
        if (deptype == "Depends")
        {
            connect(this, &Controller::operate, gdeps, &GetDepends::get_depends);
            connect(gdeps, &GetDepends::dependsReady, deps_, &Depends::updateDeps);
            connect(gdeps, &GetDepends::invalidPackage, deps_, &Depends::depsInvalid);
        }
        else if (deptype == "Recommends")
        {
            connect(this, &Controller::operate, gdeps, &GetDepends::get_recs);
            connect(gdeps, &GetDepends::recommendsReady, deps_, &Depends::updateRecs);
            connect(gdeps, &GetDepends::invalidPackage, deps_, &Depends::recsInvalid);
        }
        if (deptype == "ParentsDepends")
        {
            connect(this, &Controller::operate, gdeps, &GetDepends::get_parents_depends);
            connect(deps_, &Depends::parentsDependsReady, deps_, &Depends::updateParsDeps);
            connect(gdeps, &GetDepends::invalidPackage, deps_, &Depends::parsDepsInvalid);
        }
        if (deptype == "ParentsRecommends")
        {
            connect(this, &Controller::operate, gdeps, &GetDepends::get_parents_recommends);
            connect(deps_, &Depends::parentsRecommendsReady, deps_, &Depends::updateParsRecs);
            connect(gdeps, &GetDepends::invalidPackage, deps_, &Depends::parsRecsInvalid);
        }
        else if (deptype == "policy")
        {
            connect(this, &Controller::operate, gdeps, &GetDepends::get_pol);
            connect(gdeps, &GetDepends::policyReady, deps_, &Depends::updatePolicy);
            connect(gdeps, &GetDepends::polDone, deps_, &Depends::policyDone);
        }
        connect(gdeps, &GetDepends::invalidPackage, deps_, &Depends::badPackage);

        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
    Depends *deps_;
signals:
    void operate(const QString &, int);
};


#endif // DEPENDS_H

