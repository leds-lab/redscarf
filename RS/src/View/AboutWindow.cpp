/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* AboutWindow.cpp
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

#include "include/View/AboutWindow.h"
#include "ui_AboutWindow.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/AboutWindow" << std::endl;
#endif
    this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    this->addAuthors();

    delete ui->widgetAuthors->layout();
    QVBoxLayout* layout = new QVBoxLayout(ui->widgetAuthors);
    ui->widgetAuthors->setLayout(layout);

    for(int i = 0; i < authors.size(); i++) {
        QStringList autor = authors.at(i);
        QLabel* lab = new QLabel(ui->scrollAuthors);
        lab->setWordWrap(true);
        lab->setText("");
        for(int x = 0; x < autor.size(); x++) {
            lab->setText( lab->text()+"<br />"+autor.at(x));
        }
        layout->addWidget(lab);
    }
}

void AboutWindow::addAuthors() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/AboutWindow::addAuthors" << std::endl;
#endif

    QStringList autor1;
    autor1 << trUtf8("<b>Maintener and Developer</b>") << "Cesar Albenes Zeferino" << "zeferino@univali.br";

    QStringList autor2;
    autor2 << trUtf8("<b>Developer</b>") << "Eduardo Alves da Silva" << "eduardoalves@edu.univali.br";


    authors.insert(0,autor1);
    authors.insert(1,autor2);

}

AboutWindow::~AboutWindow() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/AboutWindow" << std::endl;
#endif

    delete ui;
}
