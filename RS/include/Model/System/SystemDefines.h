/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* SystemDefines.h
* Copyright (C) 2014 LEDS - Univali <zeferino@univali.br>
* Laboratory of Embedded and Distributed Systems
* University of Vale do Itajaí
*
* Authors: Laboratory of Embedded and Distributed Systems (LEDS)
*
* Contact: Profº Dr. Cesar Albenes Zeferino {zeferino@univali.br}
*
* RedScarf is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* RedScarf is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* ------------------------------ Reviews -------------------------------------
* Date       - Version - Author                      | Description
* ----------------------------------------------------------------------------
* 10/12/2014 - 1.0     - Eduardo Alves da Silva      | Initial release
* 20/11/2016 - 2.0     - Eduardo Alves da Silva      | Back-end change
*
*/

#ifndef __SYSTEMDEFINES_H__
#define __SYSTEMDEFINES_H__

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>

// Singleton
class SystemDefines {
public:
    class Topology {
    public:
        Topology() : topology(""){}
        inline bool isValid() { if( topology.isNull() || topology.isEmpty() ) return false; else return true; }
        inline void setTopology    (QString value) { this->topology     = value; }
        inline void setAlias       (QString value) { this->alias        = value; }
        inline void setNoCPlugin   (QString value) { this->nocPlugin    = value; }
        inline void setRouterPlugin(QString value) { this->routerPlugin = value; }
        inline QString getTopology    () const { return this->topology    ; }
        inline QString getAlias       () const { return this->alias       ; }
        inline QString getNoCPlugin   () const { return this->nocPlugin   ; }
        inline QString getRouterPlugin() const { return this->routerPlugin; }
    private:
        QString topology;
        QString alias;
        QString nocPlugin;
        QString routerPlugin;
    };

    class Routing {
    public:
        Routing() : topology(""){}
        bool isValid() const { if(topology.isNull() || topology.isEmpty()) return false; else return true; }
        inline void setTopology(QString value) { this->topology = value; }
        inline void setAlgorithms(QStringList value) { this->algorithms = value; }
        inline void setPlugins(QStringList value) { this->plugins = value; }
        inline void addRouting (QString value) { this->algorithms.push_back(value); }
        inline void addPlugin  (QString value) { this->plugins.push_back(value); }
        inline QString getTopology() const { return this->topology; }
        inline QString getRoutingAlgorithm(int index) const { return ((index >= 0 && index < algorithms.size()) ? algorithms.at(index) : ""); }
        inline QString getRoutingPlugin(int index) const { return ((index >= 0 && index < plugins.size()) ? plugins.at(index) : ""); }
        inline int getRoutingAlgorithmIndex(QString v) { return algorithms.indexOf(v); }
        inline QStringList getAlgorithms() const { return this->algorithms; }
        inline QStringList getPlugins() const { return this->plugins; }
        inline int algorithmsCount() const { return this->algorithms.size(); }
        inline bool containsAlgorithm(QString v) const { return this->algorithms.contains(v); }
    private:
        QString topology;
        QStringList algorithms;
        QStringList plugins;
    };

    class FlowControl {
    public:
        inline void    setFlowControl(QString value) { this->flowControl = value; }
        inline void    setPlugin     (QString value) { this->plugin = value; }
        inline QString getFlowControl() const { return this->flowControl; }
        inline QString getPlugin     () const { return this->plugin; }
    private:
        QString flowControl;
        QString plugin;
    };

    class PriorityGenerator {
    public:
        inline void setPG(QString value) { this->priorityGenerator = value; }
        inline void setPlugin(QString value) { this->plugin = value; }
        inline QString getPG() const { return this->priorityGenerator; }
        inline QString getPlugin() const { return this->plugin; }
    private:
        QString priorityGenerator;
        QString plugin;
    };

    // Singleton
    static SystemDefines* getInstance();

    Topology findTopology(unsigned int key) const;
    unsigned int getKeyTopology(QString topology) const;
    int sizeTopologies() const;

    Routing findRoutingAlgorithm(QString topology) const;
    QPair<int,int> getKeyRoutingAlgorithm(QString value) const;
    int sizeRoutingAlgorithms() const;
    int sizeRoutingAlgorithms(QString topology) const;

    FlowControl findFlowControl(unsigned int key) const;
    unsigned int getKeyFlowControl(QString value) const;
    int sizeFlowControls() const;

    PriorityGenerator findArbiterPG(unsigned int key) const;
    unsigned int getKeyArbiterPG(QString value) const;
    int sizeArbiterPGs() const;

    QString findVcOption(unsigned int key) const;
    unsigned int getKeyVcOption(QString value) const;
    int sizeVcOptions() const;

    virtual ~SystemDefines();

    // Flag on result not found
    static const unsigned int ERROR = 999;


protected:
    virtual void loadTopologies();
    virtual void loadRoutingAlgorithms();
    virtual void loadFlowControls();
    virtual void loadArbiterTypes();
    virtual void loadVcOptions();

private:
    static SystemDefines* def;

    QMap<unsigned int,SystemDefines::Topology>          topologies;        // Topologies available
    QMap<unsigned int,SystemDefines::Routing>           routingAlgorithms; // Routing algorithm available
    QMap<unsigned int,SystemDefines::FlowControl>       flowControls;      // Flow control available
    QMap<unsigned int,SystemDefines::PriorityGenerator> priorityGenerators;// Arbiter type available
    QMap<unsigned int,QString>                          vcOptions;         // Virtual channels options available

    // Private constructor
    SystemDefines();

    template<class T>
    T            find    (const QMap<unsigned int,T>& m, unsigned int key) const;
//    template<class T>
//    unsigned int getKey  (const std::map<unsigned int,T>& m, std::string value) const;
    template<class T>
    unsigned int size    (const QMap<unsigned int,T>& m) const;
    template<class T>
    bool         keyExist(const QMap<unsigned int,T>& m,unsigned int key) const;

};

#endif // SYSTEMDEFINES_H
