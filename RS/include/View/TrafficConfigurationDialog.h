/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficConfigurationDialog.h
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

#ifndef TRAFFICCONFIGURATIONDIALOG_H
#define TRAFFICCONFIGURATIONDIALOG_H

#include <QDialog>

namespace Ui {
    class TrafficConfigurationDialog;
}

class TrafficParameters;

class TrafficConfigurationDialog : public QDialog {
    Q_OBJECT

private:
    Ui::TrafficConfigurationDialog *ui;

    unsigned int xSrc;
    unsigned int ySrc;
    unsigned int dataWidth;
    unsigned int trafficNum;

    void inserirItemsSpatialsDistributions();
    void inserirItemsTrafficClasses();
    void inserirItemsTypesInjection();
    void inserirItemsSwitchingTechniques();
    void inserirItemsFunctionProbability();

    void loadDefaultValues();

    bool inputsOk();
protected:
    void changeEvent(QEvent*);

public:

    explicit TrafficConfigurationDialog(QWidget *parent,unsigned int xSource,unsigned int ySource,unsigned int trafficNum,
                                  unsigned int xTam,unsigned int yTam,unsigned int dataWidth);
    void setConfiguration(TrafficParameters* tp);
    ~TrafficConfigurationDialog();

signals:
    void apply(TrafficParameters* configuration,unsigned int trafficNum);
    void applyAndReplicate(TrafficParameters* configuration,unsigned int trafficNum);

private slots:
    // Combos
    void updateSpatialDistribution(int);
    void updateTypeInjection(int);
    void updateFunctionProbability(int);

    void applyClicked();
    void applyAndReplicateClicked();
};

#endif // TRAFFICCONFIGURATIONDIALOG_H
