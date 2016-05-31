/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* Main.cpp
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

#include <QFileOpenEvent>
#include "include/Main.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

int RedScarfApp::executeApp() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Main::executeApp" << std::endl;
#endif

//    app.setFont(fontApp);
//    QFont fontApp = app.font();
//    fontApp.setPointSize( 12 );
    ctrl->startApp();

    return exec();
}

#ifdef Q_OS_MAC
bool RedScarfApp::event(QEvent *event) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Main::event" << std::endl;
#endif

    switch(event->type()) {
        case QEvent::FileOpen: {
            QString fileToOpen = static_cast<QFileOpenEvent* >(event)->file();
            ctrl->macOpenFile(fileToOpen);
            break;
        }
        default:
            break;
    }
    return QApplication::event(event);

}
#endif

int main(int argc, char *argv[]) {

#ifdef DEBUG_POINTS_METHODS
    std::cout << "Init: main" << std::endl;
#endif

    RedScarfApp app(argc,argv);
    return app.executeApp();

}
