/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Main.h
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


#ifndef MAIN_H
#define MAIN_H

#include "Control/Control.h"
#include "View/MainWindow.h"
#include <QApplication>

class RedScarfApp : public QApplication {
private:
    /*!
     * \brief tp Object that contains the mainwindow application objects
     * signals, slots and functions to control comunicate
     */
    MainWindow tp;
    /*!
     * \brief ctrl The core object of this application.
     * It contains the objects from model and the GUI application.
     * It's responsible by control all application's flow
     * It integrates a lot of methods to perform application's functions
     */
    Control* ctrl;
protected:

#ifdef Q_OS_MAC
    bool event(QEvent* );
#endif
public:

    /*!
     * \brief RedScarfApp The constructor of Application Object
     * \param argc Arguments counter of command line
     * \param argv Arguments of command line
     */
    explicit RedScarfApp(int &argc,char ** argv);

    /*!
     * \brief executeApp Start application and return
     * end state
     * \return The end state
     */
    int executeApp();
};

#endif // MAIN_H
