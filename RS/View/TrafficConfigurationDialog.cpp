#include "ui_TrafficConfigurationDialog.h"
#include "TrafficConfigurationDialog.h"

#include "TrafficEditDialog.h"

#ifndef M_PI
    #define M_PI       3.14159265358979323846
#endif

#include <QWheelEvent>
#include <QMessageBox>
#include <QAction>

#include <cmath>

#include "Model/System/SystemParameters.h"
#include "Model/Traffic/TrafficParameters.h"
#include "Model/Traffic/SpatialDistribution.h"

#include "View/Vertice.h"
#include "View/Arc.h"
#include "View/Edge.h"

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif


TrafficConfigurationDialog::TrafficConfigurationDialog(QWidget *parent, QList<QListWidgetItem *> items) :
    QDialog(parent),
    ui(new Ui::TrafficConfigurationDialog),
    systemConfItems(items),
    currentNumberElements(0),
    currentDataWidth(0)
{
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor View/TrafficConfigurationDialog" << std::endl;
#endif

    ui->setupUi(this);

    this->setWindowFlags(Qt::Window);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->scene = new QGraphicsScene();
    this->scene->setBackgroundBrush( QBrush(Qt::transparent) );
    this->ui->graphicsView->setScene(scene);
    connect(ui->buttonSelectAllNodes,SIGNAL(clicked(bool)),ui->graphicsView,SLOT(selectAllItems()));

    connect(ui->listConf,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(itemClicked(QListWidgetItem*)));
    connect(scene,SIGNAL(selectionChanged()),this,SLOT(graphicSelectionChange()));

    connect(ui->listTraffic,SIGNAL(itemSelectionChanged()),this,SLOT(trafficListSelectionChange()));

    connect(ui->buttonAddTraffic,SIGNAL(clicked(bool)),this,SLOT(addTrafficConfiguration()));

    connect(ui->zoomScale,SIGNAL(valueChanged(double)),ui->graphicsView,SLOT(setZoomFactor(double)));
    connect(ui->zoomIn,SIGNAL(clicked(bool)),ui->graphicsView,SLOT(zoomIn()));
    connect(ui->zoomOut,SIGNAL(clicked(bool)),ui->graphicsView,SLOT(zoomOut()));
    connect(ui->zoomRestore,SIGNAL(clicked(bool)),this,SLOT(zoomRestore()));

    connect(ui->listTraffic,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(trafficItemDoubleClick(QModelIndex)));
    QAction* deleteItem = new QAction(ui->listTraffic);
    deleteItem->setShortcut(Qt::Key_Delete);
    ui->listTraffic->addAction(deleteItem);
    connect(deleteItem,SIGNAL(triggered(bool)),this,SLOT(removeSelectedTraffic()));

    connect(ui->dialogButtons->button(QDialogButtonBox::Apply),SIGNAL(clicked(bool)),this,SLOT(apply()));
    connect(ui->dialogButtons,SIGNAL(rejected()),this,SLOT(reject()));

    for( int i = 0; i < items.size(); i++ ) {
        QListWidgetItem* item = items.at(i);
        QListWidgetItem* newItem = new QListWidgetItem(*item);
        ui->listConf->addItem( newItem );
    }
    this->loadExistentConfigurations();
    this->itemClicked( ui->listConf->item(0) );
    ui->listConf->item(0)->setSelected(true);
}

void TrafficConfigurationDialog::loadExistentConfigurations() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::loadExistentConfigurations" << std::endl;
#endif

    int i;
    QList<TrafficParameters> confs;
    for( i = 0; i < systemConfItems.size(); i++ ) {
        QListWidgetItem* item = systemConfItems.at(i);
        QList<QVariant> trafficConfs = item->data(Qt::UserRole+1).toList();

        for( int x = 0; x < trafficConfs.size(); x++ ) {
            TrafficParameters tp = trafficConfs.at(x).value<TrafficParameters>();
            bool alreadyAdded = false;
            for(int j = 0; j < confs.size(); j++) {
                TrafficParameters added = confs.at(j);
                if( tp.equals(&added) ) {
                    alreadyAdded = true;
                }
            }
            if( !alreadyAdded ) {
                confs.append(tp);
            }
        }
    }

    // Add to list widget
    for( i = 0; i < confs.size(); i++ ) {
        TrafficParameters tp = confs.at(i);
        QListWidgetItem* newItem = new QListWidgetItem(tp.getFormattedString(),ui->listTraffic);
        QVariant parameters;
        parameters.setValue(tp);
        newItem->setData(Qt::UserRole,parameters);
    }
}

void TrafficConfigurationDialog::apply(){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::apply" << std::endl;
#endif

    QList<QVariant> trafficConfList;

    int trafficConfCount = ui->listTraffic->count();
    if( trafficConfCount == 0 ) {
        QMessageBox::information(this,tr("No traffic configured"),tr("None traffic defined for these systems"));
    } else {
        for( int i = 0; i < trafficConfCount; i++) {
            QListWidgetItem* item = ui->listTraffic->item(i);
            QVariant trafficVar = item->data(Qt::UserRole);
            trafficConfList.append(trafficVar);
        }
        emit trafficConfigured(trafficConfList);
    }

    this->accept();
}

void TrafficConfigurationDialog::zoomRestore() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::zoomRestore" << std::endl;
#endif

    ui->graphicsView->resetMatrix();
}

void TrafficConfigurationDialog::itemClicked(QListWidgetItem *item) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::itemClicked" << std::endl;
#endif

    QVariant data = item->data(Qt::UserRole);
    SystemParameters sp = data.value<SystemParameters>();
    currentDataWidth = sp.getDataWidth();
    currentNumberElements = sp.getNumberElements();

    this->configureGraphics(&sp);
}

TrafficConfigurationDialog::~TrafficConfigurationDialog() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor View/TrafficConfigurationDialog" << std::endl;
#endif

    this->clearScene();
    delete ui;
}

void TrafficConfigurationDialog::configureGraphics(SystemParameters *sp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::configureGraphics" << std::endl;
#endif

    this->clearScene();

    switch (sp->getTopologyType()) {
        case SystemParameters::NonOrthogonal:
            this->drawRing(sp);
            break;
        case SystemParameters::Orthogonal2D:
            this->draw2DMesh(sp);
            break;
        case SystemParameters::Orthogonal3D:
            this->draw3DMesh(sp);
            break;
        default:
            break;
    }
}

void TrafficConfigurationDialog::clearScene() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::clearScene" << std::endl;
#endif

    QList<QGraphicsItem *> graphicItems = this->scene->items();
    vertices.clear();
    flowConnections.clear();

    for( int i = 0; i < graphicItems.size(); i++ ) {
        QGraphicsItem* item = graphicItems.at(i);
        delete item;
    }

    delete scene;
    this->scene = new QGraphicsScene();
    this->scene->setBackgroundBrush( QBrush(Qt::transparent) );
    this->ui->graphicsView->setScene(scene);
    connect(scene,SIGNAL(selectionChanged()),this,SLOT(graphicSelectionChange()));
}

void TrafficConfigurationDialog::repaintGraphics() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::repaintGraphics" << std::endl;
#endif

    this->scene->invalidate();
}

void TrafficConfigurationDialog::drawRing(SystemParameters *sp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::drawRing" << std::endl;
#endif

    unsigned short numElements = sp->getNumberElements();
    float angleOffset = 360.f/(float)numElements;
    unsigned short spaceOffset = log2(numElements)-1;

    QPointF origin(0,0);

    for( unsigned int i = 0; i < numElements; i++ ) {
        Vertice* vertice = new Vertice(i,Qt::darkCyan);
        QPointF pos;
        float angleDegree = (i*angleOffset)-90.0f;
        float angleRad = angleDegree * (M_PI/180.0f);
        pos.setX( ( (SPACE * spaceOffset) * cos(angleRad) + origin.x() ) );
        pos.setY( ( (SPACE * spaceOffset) * sin(angleRad) + origin.y() ) );
        vertice->setPos(pos);

        this->vertices.insert(i,vertice);
        this->scene->addItem(vertice);
    }


    QPointF centerDraw;
    centerDraw.setX( (numElements/4.f * spaceOffset)  );
    centerDraw.setY( (numElements/4.f * spaceOffset));
    ui->graphicsView->centerOn(centerDraw);
}

void TrafficConfigurationDialog::draw2DMesh(SystemParameters *sp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::draw2DMesh" << std::endl;
#endif

    unsigned int id = 0;
    for(unsigned int y = 0; y < sp->getYSize(); y++) {
        for(unsigned int x = 0; x < sp->getXSize(); x++) {
            Vertice* vertice = new Vertice(id,Qt::darkCyan);
            vertice->setPos((x*SPACE), ((sp->getYSize() -1)*SPACE) - (y*SPACE));  // @param1 = X xpos, @param2 = ypos
            this->vertices.insert(id,vertice);
            this->scene->addItem(vertice);
            id++;
        }
    }
    QPointF centerDraw;
    centerDraw.setX( (float)((sp->getXSize()-1) * SPACE) / 2.f );
    centerDraw.setY( (float)((sp->getYSize()-1) * SPACE) / 2.f );
    ui->graphicsView->centerOn(centerDraw);
}

void TrafficConfigurationDialog::draw3DMesh(SystemParameters *sp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::draw3DMesh" << std::endl;
#endif

    QColor colors[4];
    colors[0] = Qt::darkCyan;
    colors[1] = Qt::blue;
    colors[2] = Qt::red;
    colors[3] = Qt::gray;

    float distanceFactor = 0.31f;

    unsigned int id = 0;
    for(unsigned int z = 0; z < sp->getZSize(); z++) {
        for( unsigned int y = 0; y < sp->getYSize(); y++ ) {
            for( unsigned int x = 0; x < sp->getXSize(); x++ ) {
                Vertice* vertice = new Vertice(id,colors[z]);
                QPointF pos;
                // XYZ
//                pos.setX( (x*SPACE*2) );
//                pos.setY( ((sp->getYSize()-1)*SPACE*2-(y*SPACE*2)) );
//                pos.setX( pos.x() + SPACE*z*0.33f);
//                pos.setY( pos.y() - SPACE*z*0.66f);
                // ZYX
//                pos.setX( (z*SPACE*2) );
//                pos.setY( ((sp->getYSize()-1)*SPACE*2-(y*SPACE*2)) );
//                pos.setX( pos.x() + SPACE*x*0.33f);
//                pos.setY( pos.y() - SPACE*x*0.66f);
                // XZY
                pos.setX( (x*SPACE*2) );
                pos.setY( (sp->getZSize()-1)*SPACE*2-(z*SPACE*2) );
                pos.setX( pos.x() + SPACE*y*distanceFactor);
                pos.setY( pos.y() - SPACE*y*distanceFactor*2);

                vertice->setPos(pos);
                this->vertices.insert(id,vertice);
                this->scene->addItem(vertice);
                id++;
            }
        }
    }
    QPointF centerDraw;
    // XYZ
//    centerDraw.setX( (float)((sp->getXSize()-1) * SPACE * 2.f + SPACE*(sp->getZSize()-1)*0.33f)/2.f );
//    centerDraw.setY( (float)((sp->getYSize()-1) * SPACE * 2.f - SPACE*(sp->getZSize()-1)*0.66f)/2.f );
    // ZYX
//    centerDraw.setX( (float)((sp->getZSize()-1) * SPACE * 2.f + SPACE*(sp->getXSize()-1)*0.33f)/2.f );
//    centerDraw.setY( (float)((sp->getYSize()-1) * SPACE * 2.f - SPACE*(sp->getXSize()-1)*0.66f)/2.f );
    // XZY
    centerDraw.setX( (float)((sp->getXSize()-1) * SPACE * 2.f + SPACE*(sp->getYSize()-1)*distanceFactor)/2.f );
    centerDraw.setY( (float)((sp->getZSize()-1) * SPACE * 2.f - SPACE*(sp->getYSize()-1)*distanceFactor*2)/2.f );
    ui->graphicsView->centerOn(centerDraw);

}

void TrafficConfigurationDialog::graphicSelectionChange() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::graphicSelectionChange" << std::endl;
#endif

    QList<QGraphicsItem *> itemsSelected = this->scene->selectedItems();

    QList<Vertice *> selectedVertices;
    for( int i = 0; i < itemsSelected.size(); i++ ) {
        Vertice* vertice = dynamic_cast<Vertice *>(itemsSelected.at(i));
        if( vertice != NULL ) {
            selectedVertices.append(vertice);
        }
    }

    this->clearConnections();
    ui->listTraffic->clearSelection();

    if( selectedVertices.size() == 0 ) {
        ui->buttonAddTraffic->setEnabled(false);
        ui->labelNodeSelected->setText(tr("None"));
    } else {
        ui->buttonAddTraffic->setEnabled(true);
        if( selectedVertices.size() == 1 ) {
            ui->labelNodeSelected->setText( QString::number( selectedVertices.at(0)->getId() ) );
        } else {
            ui->labelNodeSelected->setText( tr("**Multiple Selection**") );
        }
        this->drawFlowsSelectedVertices(selectedVertices);
    }

}

void TrafficConfigurationDialog::addTrafficConfiguration() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::addTrafficConfiguration" << std::endl;
#endif

    QList<QGraphicsItem *> sourcesSelected = this->scene->selectedItems();

    short sourceAddr = -1;
    if( sourcesSelected.size() == 1 ) {
        Vertice* source = dynamic_cast<Vertice *>(sourcesSelected.at(0));
        if( source != NULL ) {
            sourceAddr = source->getId();
        }
    }

    TrafficEditDialog* ted = new TrafficEditDialog(this,sourceAddr,currentNumberElements,currentDataWidth);;

    ted->setLocale( locale() );

    connect(ted,SIGNAL(apply(TrafficParameters*)),this,SLOT(applyTrafficParameters(TrafficParameters*)));

    ted->exec();

    delete ted;

}

void TrafficConfigurationDialog::applyTrafficParameters(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::applyTrafficParameters" << std::endl;
#endif

    QList<QGraphicsItem *> sourcesSelected = this->scene->selectedItems();

    for( int i = 0; i < sourcesSelected.size(); i++ ) {
        Vertice* source = dynamic_cast<Vertice *>(sourcesSelected.at(i));
        TrafficParameters trafficConf = *tp;
        // Setting source address
        if(source == NULL) {
            continue;
        } else {
            trafficConf.setSource(source->getId());
        }
        SpatialDistribution sp(trafficConf.getSource(),currentNumberElements,trafficConf.getDestination());

        // Get destination
        switch (trafficConf.getSpatialDistribution()) {
            case SpatialDistribution::Non_Uniform:
            case SpatialDistribution::Uniform: {
                QList<int> destinations = sp.toUniform();
                for(int x = 0; x < destinations.size(); x++) {
                    unsigned short dest = destinations.at(x);
                    trafficConf.setDestination(dest);
                    // Add to list widget
                    QListWidgetItem* newItem = new QListWidgetItem(trafficConf.getFormattedString(),ui->listTraffic);
                    QVariant parameters;
                    parameters.setValue(trafficConf);
                    newItem->setData(Qt::UserRole,parameters);
                }
                break;
            }
            case SpatialDistribution::Local: {
                // TODO verificar como fazer para cada topologia!
                break;
            }
            default: {
                int dest = sp.getDestination(trafficConf.getSpatialDistribution());
                if( dest > -1 ) { // > -1 : A valid address
                    trafficConf.setDestination( dest );
                    // Add to list widget
                    QListWidgetItem* newItem = new QListWidgetItem(trafficConf.getFormattedString(),ui->listTraffic);
                    QVariant parameters;
                    parameters.setValue(trafficConf);
                    newItem->setData(Qt::UserRole,parameters);
                } else {
                    if( trafficConf.getSpatialDistribution() == SpatialDistribution::Specific_Address ) {
                        QMessageBox::information(this,tr("Invalid configuration"),tr("For this spatial distribution at node %1"
                                                                                     "\n an invalid destination was generated.\n"
                                                                                     "Not added!").arg(trafficConf.getSource()));
                    }
                }
                break;
            } // End default:
        } // End switch
    } // End for (selectedSources)

    this->graphicSelectionChange(); // Update the view with the connections to selected vertices

    delete tp;
}

void TrafficConfigurationDialog::drawFlowsSelectedVertices(QList<Vertice *> selectedVertices) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::drawFlowsSelectedVertices" << std::endl;
#endif

    unsigned int trafficConfCount = ui->listTraffic->count();

    for( unsigned int i = 0; i < trafficConfCount; i++ ) {
        QListWidgetItem* currentListItem = ui->listTraffic->item(i);
        QVariant currentData = currentListItem->data(Qt::UserRole);
        TrafficParameters trafficConf = currentData.value<TrafficParameters>();
        bool flagSelected = false;
        for( int x = 0; x < selectedVertices.size(); x++ ) {
            Vertice* source = selectedVertices.at(x);
            if( trafficConf.getSource() == source->getId() ) {
                if( trafficConf.getDestination() < vertices.size() ) {
                    Vertice* destination = vertices.at(trafficConf.getDestination());
                    if(destination != NULL) {
                        Arc* connection = new Arc(source,destination,trafficConf.getRequiredBandwidth());
                        this->flowConnections.append(connection);
                        this->scene->addItem(connection);
                        flagSelected = true;
                        break;
                    }

                }
            }
        }
        currentListItem->setSelected(flagSelected);

    }
}

void TrafficConfigurationDialog::trafficListSelectionChange() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::trafficListSelectionChange" << std::endl;
#endif

    if( ui->listTraffic->hasFocus() ) {
        QList<QListWidgetItem* > selectedItems = ui->listTraffic->selectedItems();
        this->clearConnections();
        this->scene->clearSelection();
        for(int i = 0; i < selectedItems.size(); i++) {
            QListWidgetItem* currentListItem = selectedItems.at(i);
            QVariant currentData = currentListItem->data(Qt::UserRole);
            TrafficParameters trafficConf = currentData.value<TrafficParameters>();
            if( trafficConf.getSource() < vertices.size() && trafficConf.getDestination() < vertices.size() ) {
                Vertice* source = vertices.at(trafficConf.getSource());
                Vertice* destination = vertices.at(trafficConf.getDestination());
                if( source != NULL && destination != NULL ) {
                    Arc* connection = new Arc(source,destination,trafficConf.getRequiredBandwidth());
                    this->flowConnections.append(connection);
                    this->scene->addItem(connection);
                }
            }
        }

    }

}

void TrafficConfigurationDialog::clearConnections() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::clearConnections" << std::endl;
#endif

    for( int i = 0; i < flowConnections.size(); i++ ) {
        Arc* arc = flowConnections.at(i);
        delete arc;
    }
    flowConnections.clear();

}

void TrafficConfigurationDialog::removeSelectedTraffic() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::removeSelectedTraffic" << std::endl;
#endif

    QList<QListWidgetItem *> items = this->ui->listTraffic->selectedItems();

    if( items.size() != 0 ) {
        QMessageBox::StandardButton reply;
        if( items.size() > 1 ) {
            reply = QMessageBox::question(this, tr("Confirm remove"),
                                          tr("Are you sure to remove the selected items?"),
                                          QMessageBox::Yes|QMessageBox::No);

        } else {
            reply = QMessageBox::Yes;
        }

        if (reply == QMessageBox::Yes) {
            for(int i = 0; i < items.size(); i++) {
                QListWidgetItem* item = items.at(i);
                delete item;
            }
            this->graphicSelectionChange();
        }
    }
}

void TrafficConfigurationDialog::trafficItemDoubleClick(QModelIndex modelIndex) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::trafficItemDoubleClick" << std::endl;
#endif

    if( currentNumberElements == 0 ) {
        QMessageBox::information(this,tr("Select a configuration"),tr("Select a topology configuration to edit"
                                                                      "\nthe traffic configuration"));
        return;
    }

    QVariant trafficParameterVar = modelIndex.data(Qt::UserRole);

    TrafficParameters tp = trafficParameterVar.value<TrafficParameters>();

    TrafficEditDialog* ted = new TrafficEditDialog(this,tp.getSource(),currentNumberElements,currentDataWidth);;
    ted->disableDestinationMode(); // Only for edit this conf, don't change de destination mode
    ted->setConfiguration(&tp);
    ted->setLocale( locale() );

    ted->exec();

    TrafficParameters* newTp = ted->getConfiguredTraffic();
    if( newTp != NULL ) {
        // For Multiple destination spatial distribution - a new set of traffic conf is generated
//        this->scene->clearSelection();
//        this->vertices.at(newTp->getSource())->setSelected(true);
//        delete ui->listTraffic->item(modelIndex.row());
//        this->applyTrafficParameters(newTp);
        // Only for this conf is applied the new configuration
        tp = *newTp;
        trafficParameterVar.setValue(tp);
        QListWidgetItem* item = ui->listTraffic->item(modelIndex.row());
        item->setData(Qt::UserRole,trafficParameterVar);
        item->setText( tp.getFormattedString() );
        delete newTp;
    }

    delete ted;

}
