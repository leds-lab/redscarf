/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ThreadManager.cpp
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
*
*/

#include "ThreadManager.h"
#include <QThread>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

ThreadManager::ThreadManager(int maxThreads, QObject *parent) :
    QObject(parent),maxThreadCount(maxThreads) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class Control/ThreadManager" << std::endl;
#endif

    this->pool = new QQueue<QThread* >();
    this->inExecution = new QList<QThread* >();
    this->terminated = new QList<QThread* >();

}

void ThreadManager::addPool(QThread *thread) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ThreadManager::addPool" << std::endl;
#endif

    this->pool->enqueue(thread);

}

void ThreadManager::runThreads() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ThreadManager::runThreads" << std::endl;
#endif

    for( int i = 0; i < maxThreadCount; i++ ) {
        if( pool->isEmpty() ) {
            break;
        }
        QThread* thr = pool->dequeue();
        inExecution->append(thr);
        connect(thr,SIGNAL(finished()),this,SLOT(threadFinish()));
        thr->start();
    }

}

void ThreadManager::stopThreads() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ThreadManager::stopThreads" << std::endl;
#endif

    for( int i = 0; i < inExecution->size(); i++ ) {
        QThread* thr = inExecution->at(i);
        thr->exit();
        thr->wait();
    }

}

void ThreadManager::threadFinish() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Control/ThreadManager::threadFinish" << std::endl;
#endif

    QThread* thr = (QThread *) sender();
    this->inExecution->removeOne(thr);
    this->terminated->append( thr );

    emit threadFinished( terminated->size() );

    if( pool->isEmpty() ) {
        if( inExecution->isEmpty() ) {
            emit allFinished();
        }
    } else {
        thr = pool->dequeue();
        inExecution->append(thr);
        connect(thr,SIGNAL(finished()),this,SLOT(threadFinish()));
        thr->start();
    }

}

ThreadManager::~ThreadManager() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class Control/ThreadManager" << std::endl;
#endif

    this->stopThreads();

    for(int i = 0; i < inExecution->size(); i++) {
        QThread* thr = inExecution->at(i);
        thr->deleteLater();
    }

    for( int i = 0; i < pool->size(); i++ ) {
        QThread* thr = pool->at(i);
        thr->deleteLater();
    }

    for( int i = 0; i < terminated->size(); i++ ) {
        QThread* thr = terminated->at(i);
        thr->deleteLater();
    }

    inExecution->clear();
    delete inExecution;
    pool->clear();
    delete pool;
    terminated->clear();
    delete terminated;

}
