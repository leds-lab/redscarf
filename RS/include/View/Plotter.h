/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Plotter.h
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


#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMainWindow>
#include "ui_Plotter.h"

class AnalysisOptions;
class DataReport;

namespace Ui {
    class Plotter;
}

class Plotter : public QMainWindow {
    Q_OBJECT
protected:
    Ui::Plotter* ui;

public:
    explicit Plotter(QWidget *parent = 0);

    virtual void viewGraphic(QVector<QList<DataReport *>* >* data,AnalysisOptions* aop,QStringList legends) = 0;

//    virtual void exportGraphic(QImage image) = 0;

signals:
    void finished(int);
    void sendMessage(QString);

};

#endif // PLOTTER_H
