/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* ReportDialog.h
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

#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include "include/View/PreviewDialog.h"

class QComboBox;
class DataReport;

class ReportDialog : public PreviewDialog {
    Q_OBJECT
private:

    QComboBox* combo;
    QVector<QList<DataReport *> *> *dados;

public:
    ReportDialog(QStringList legendas,
                 QVector<QList<DataReport *> *> * _dados,
                 QWidget* parent = 0);
    ~ReportDialog();

    void addNode(Node* ){}

private slots:
    void changeReport(int pos);
};

#endif // REPORTDIALOG_H
