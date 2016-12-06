/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
* TrafficConfigurationDialog.cpp
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

#include "ui_TrafficConfigurationDialog.h"
#include "include/View/TrafficConfigurationDialog.h"

#include "include/Model/Traffic/TrafficPatternDefines.h"
#include "include/Model/Traffic/TrafficParameters.h"

#include <QMessageBox>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

/// Construtor
TrafficConfigurationDialog::TrafficConfigurationDialog(QWidget *parent,
                                                       unsigned int xSource,
                                                       unsigned int ySource,
                                                       unsigned int zSource,
                                                       unsigned int trafficNum,
                                                       unsigned int xSize,
                                                       unsigned int ySize,
                                                       unsigned int zSize,
                                                       unsigned int dataWidth)
    : QDialog(parent,Qt::Window),ui(new Ui::TrafficConfigurationDialog) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Constructor Class View/TrafficConfigurationDialog" << std::endl;
#endif
    // Setando uso do form
    ui->setupUi(this);

    // Inserindo items aos combo box da janela
    this->inserirItemsSpatialsDistributions();
    this->inserirItemsTrafficClasses();
    this->inserirItemsTypesInjection();
    this->inserirItemsSwitchingTechniques();
    this->inserirItemsFunctionProbability();

    // Título da janela
    QString title = trUtf8("Traffic Pattern %1").arg(QString::number(trafficNum));
    this->setWindowTitle(title);

    // Se não for o nodo 0x0 esconde o botão de Aplicar e Replicar da interface
    if(!(xSource == 0 && ySource == 0 && zSource == 0)){
        ui->buttonApplyAndReplicate->hide();
    }
    this->xSrc = xSource;
    this->ySrc = ySource;
    this->zSrc = zSource;
    this->trafficNum = trafficNum;
    this->dataWidth = dataWidth;

    // Formando a String do nó fonte
    QString sourceNode = QString("( %1 , %2 , %3 )").arg(xSource).arg(ySource).arg(zSource);
    ui->labelSourceNodeXY->setText(sourceNode);

    // Setando o valor máximo de x para o nodo de destino, no caso de o usuário optar na configuração de tráfico
    // por especificar o endereço de destino
    ui->spinInNodeAddressX->setMaximum(xSize - 1);
    // Setando o valor máximo de y para o nodo de destino
    ui->spinInNodeAddressY->setMaximum(ySize - 1);
    //
    ui->spinInNodeAddressZ->setMaximum(zSize - 1);

    // Conectando sinais de combos da interface com o Slot de atualização de interface combos
    updateSpatialDistribution(0);
    updateFunctionProbability(0);
    updateTypeInjection(0);

    connect(ui->comboInDestinationNode,    SIGNAL(currentIndexChanged(int)),this,SLOT(updateSpatialDistribution(int)));
    connect(ui->comboInTypeInjection,      SIGNAL(currentIndexChanged(int)),this,SLOT(updateTypeInjection(int)));
    connect(ui->comboInFunctionProbability,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFunctionProbability(int)));

    connect(ui->buttonCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->buttonApply,SIGNAL(clicked()),this,SLOT(applyClicked()));
    connect(ui->buttonApplyAndReplicate,SIGNAL(clicked()),this,SLOT(applyAndReplicateClicked()));

    loadDefaultValues();

    this->adjustSize();
}

void TrafficConfigurationDialog::loadDefaultValues() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::loadDefaultValues" << std::endl;
#endif

    TrafficParameters* tp = new TrafficParameters();
    this->setConfiguration(tp);
    delete tp;

}

void TrafficConfigurationDialog::inserirItemsSpatialsDistributions() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inserirItemsSpatialDistributions" << std::endl;
#endif
    TrafficPatternDefines* d = TrafficPatternDefines::getInstance();
    unsigned int qtd = d->sizeSpatialDistribution();
    for( unsigned int i = 0; i < qtd  ; i++) {
        std::string v = d->findSpatialDistribution(i);
        ui->comboInDestinationNode->insertItem(qint32(i),QString::fromStdString(v));
    }
}

void TrafficConfigurationDialog::inserirItemsTrafficClasses() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inserirItemsTrafficClasses" << std::endl;
#endif
    TrafficPatternDefines* d = TrafficPatternDefines::getInstance();
    unsigned int qtd = d->sizeTrafficClass();
    for( unsigned int i = 0; i < qtd; i++ ) {
        std::string v = d->findTrafficClass(i);
        ui->comboInTrafficClass->insertItem(qint32(i),QString::fromStdString(v));
    }
}

void TrafficConfigurationDialog::inserirItemsTypesInjection() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inserirItemsTypesInjections" << std::endl;
#endif

    TrafficPatternDefines* d = TrafficPatternDefines::getInstance();
    unsigned int qtd = d->sizeTypeInjection();
    for( unsigned int i = 0; i < qtd; i++ ) {
        std::string v = d->findTypeInjection(i);
        ui->comboInTypeInjection->insertItem(qint32(i),QString::fromStdString(v));
    }
}

void TrafficConfigurationDialog::inserirItemsSwitchingTechniques() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inserirItemsSwitchingTechniques" << std::endl;
#endif

    TrafficPatternDefines* d = TrafficPatternDefines::getInstance();
    unsigned int qtd = d->sizeSwitchingTechnique();
    for( unsigned int i = 0; i < qtd; i++ ) {
        std::string v = d->findSwitchingTechnique(i);
        ui->comboInSwitchingTechnique->insertItem(qint32(i),QString::fromStdString(v));
    }
}

void TrafficConfigurationDialog::inserirItemsFunctionProbability() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inserirItemsFunctionProbability" << std::endl;
#endif
    TrafficPatternDefines* d = TrafficPatternDefines::getInstance();
    unsigned int qtd = d->sizeFunctionsProbability();
    for( unsigned int i = 0; i < qtd; i++ ) {
        std::string v = d->findFunctionProbability(i);
        ui->comboInFunctionProbability->insertItem(qint32(i),QString::fromStdString(v));
    }
}

void TrafficConfigurationDialog::setConfiguration(TrafficParameters *tp) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::setConfiguration" << std::endl;
#endif

    ui->doubleSpinInParetoAlfaOff->setValue( tp->getAlfaOff() );
    ui->doubleSpinInParetoAlfaOn->setValue( tp->getAlfaOn() );
    ui->spinInDeadLine->setValue( qint32(tp->getDeadline() ) );
    ui->spinInNodeAddressX->setValue( qint32(tp->getDestinationNodeX()) );
    ui->spinInNodeAddressY->setValue( qint32(tp->getDestinationNodeY()) );
    ui->spinInNodeAddressZ->setValue( qint32(tp->getDestinationNodeZ()) );
    ui->spinInIdleTime->setValue( qint32(tp->getIdleTime()) );
    ui->comboInTypeInjection->setCurrentIndex( qint32(tp->getInjectionType()) );
    ui->spinInMessageInterval->setValue( qint32(tp->getIntervalTime()) );
    ui->spinInNumberPacketsFlow->setValue( ulong(tp->getPackageToSend()) );
    ui->spinInMessageSize->setValue( qint32(tp->getMessageSize()) );
    ui->comboInFunctionProbability->setCurrentIndex( qint32(tp->getProbabilityFunction()) );
    ui->doubleSpinInRequiredBandwidth->setValue( tp->getRequiredBandwidth() );
    ui->spinInStdDeviation->setValue( qint32(tp->getRequiredBandwidthStdDeviation()) );
    ui->comboInDestinationNode->setCurrentIndex( qint32(tp->getSpatialDistribution()) );
    ui->comboInSwitchingTechnique->setCurrentIndex( qint32(tp->getSwitchingTechnique()) );
    ui->comboInTrafficClass->setCurrentIndex( qint32(tp->getTrafficClass()) );
}

//////// Atualizar a interface ///////////////
void TrafficConfigurationDialog::updateSpatialDistribution(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::updateSpatialDistribution" << std::endl;
#endif

    if( TrafficPatternDefines::getInstance()->findSpatialDistribution( quint32(value) ).compare("Specified Address") == 0 ){
        ui->labelNodeAddress  ->setEnabled(true);
        ui->labelNodeAddressX ->setEnabled(true);
        ui->labelNodeAddressY ->setEnabled(true);
        ui->spinInNodeAddressX->setEnabled(true);
        ui->spinInNodeAddressY->setEnabled(true);
        ui->spinInNodeAddressZ->setEnabled(true);
    } else { // Se não deve ser desabilitada
        ui->labelNodeAddress  ->setEnabled(false);
        ui->labelNodeAddressX ->setEnabled(false);
        ui->labelNodeAddressY ->setEnabled(false);
        ui->spinInNodeAddressX->setEnabled(false);
        ui->spinInNodeAddressY->setEnabled(false);
        ui->spinInNodeAddressZ->setEnabled(false);
    }
}

void TrafficConfigurationDialog::updateTypeInjection(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::updateTypeInjection" << std::endl;
#endif

    std::string v = TrafficPatternDefines::getInstance()->findTypeInjection( quint32(value) );
    if(v.compare("Constant") == 0) {
        ui->labelFunctionProbability   ->setEnabled(false);
        ui->comboInFunctionProbability ->setEnabled(false);
        ui->labelMessageSize           ->setEnabled(true );
        ui->spinInMessageSize          ->setEnabled(true );
        ui->labelIdleTime              ->setEnabled(false);
        ui->spinInIdleTime             ->setEnabled(false);
        ui->labelMessageInterval       ->setEnabled(false);
        ui->spinInMessageInterval      ->setEnabled(false);
    } else if(v.compare("Variable idle time - Fix message size") == 0) {
        ui->labelFunctionProbability   ->setEnabled(true );
        ui->comboInFunctionProbability ->setEnabled(true );
        ui->labelMessageSize           ->setEnabled(true );
        ui->spinInMessageSize          ->setEnabled(true );
        ui->labelIdleTime              ->setEnabled(false);
        ui->spinInIdleTime             ->setEnabled(false);
        ui->labelMessageInterval       ->setEnabled(false);
        ui->spinInMessageInterval      ->setEnabled(false);
    } else if(v.compare("Variable message size - Fix idle time") == 0) {
        ui->labelFunctionProbability   ->setEnabled(true );
        ui->comboInFunctionProbability ->setEnabled(true );
        ui->labelMessageSize           ->setEnabled(false);
        ui->spinInMessageSize          ->setEnabled(false);
        ui->labelIdleTime              ->setEnabled(true );
        ui->spinInIdleTime             ->setEnabled(true );
        ui->labelMessageInterval       ->setEnabled(false);
        ui->spinInMessageInterval      ->setEnabled(false);
    } else if(v.compare("Variable message size - Fix message inter-arrival") == 0) {
        ui->labelFunctionProbability   ->setEnabled(true );
        ui->comboInFunctionProbability ->setEnabled(true );
        ui->labelMessageSize           ->setEnabled(false);
        ui->spinInMessageSize          ->setEnabled(false);
        ui->labelIdleTime              ->setEnabled(false);
        ui->spinInIdleTime             ->setEnabled(false);
        ui->labelMessageInterval       ->setEnabled(true );
        ui->spinInMessageInterval      ->setEnabled(true );
    } else if(v.compare("Variable message interarrival - Fix message size") == 0) {
        ui->labelFunctionProbability   ->setEnabled(true );
        ui->comboInFunctionProbability ->setEnabled(true );
        ui->labelMessageSize           ->setEnabled(true );
        ui->spinInMessageSize          ->setEnabled(true );
        ui->labelIdleTime              ->setEnabled(false);
        ui->spinInIdleTime             ->setEnabled(false);
        ui->labelMessageInterval       ->setEnabled(false);
        ui->spinInMessageInterval      ->setEnabled(false);
    } else if(v.compare("Variable burst size - Fix message interarrival") == 0) {
        ui->labelFunctionProbability   ->setEnabled(true );
        ui->comboInFunctionProbability ->setEnabled(true );
        ui->labelMessageSize           ->setEnabled(true );
        ui->spinInMessageSize          ->setEnabled(true );
        ui->labelIdleTime              ->setEnabled(false);
        ui->spinInIdleTime             ->setEnabled(false);
        ui->labelMessageInterval       ->setEnabled(true );
        ui->spinInMessageInterval      ->setEnabled(true );
    } else {
        ui->labelFunctionProbability   ->setEnabled(true);
        ui->comboInFunctionProbability ->setEnabled(true);
        ui->labelMessageSize           ->setEnabled(true);
        ui->spinInMessageSize          ->setEnabled(true);
        ui->labelIdleTime              ->setEnabled(true);
        ui->spinInIdleTime             ->setEnabled(true);
        ui->labelMessageInterval       ->setEnabled(true);
        ui->spinInMessageInterval      ->setEnabled(true);
    }
    updateFunctionProbability(ui->comboInFunctionProbability->currentIndex());
}

void TrafficConfigurationDialog::updateFunctionProbability(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::updateFunctionProbability" << std::endl;
#endif

    if(ui->comboInFunctionProbability->isEnabled()){
        std::string v = TrafficPatternDefines::getInstance()->findFunctionProbability(quint32(value));
        if( v.compare("Normal") == 0 ) {
            ui->labelStdDeviation        ->setEnabled(true );
            ui->spinInStdDeviation       ->setEnabled(true );
            ui->labelParetoAlfaOn        ->setEnabled(false);
            ui->doubleSpinInParetoAlfaOn ->setEnabled(false);
            ui->labelParetoAlfaOff       ->setEnabled(false);
            ui->doubleSpinInParetoAlfaOff->setEnabled(false);
        } else if( v.compare("Exponential") == 0 ) {
            ui->labelStdDeviation        ->setEnabled(false);
            ui->spinInStdDeviation       ->setEnabled(false);
            ui->labelParetoAlfaOn        ->setEnabled(false);
            ui->doubleSpinInParetoAlfaOn ->setEnabled(false);
            ui->labelParetoAlfaOff       ->setEnabled(false);
            ui->doubleSpinInParetoAlfaOff->setEnabled(false);
        } else if( v.compare("Pareto") == 0 ) {
            ui->labelStdDeviation        ->setEnabled(false);
            ui->spinInStdDeviation       ->setEnabled(false);
            ui->labelParetoAlfaOn        ->setEnabled(true );
            ui->doubleSpinInParetoAlfaOn ->setEnabled(true );
            ui->labelParetoAlfaOff       ->setEnabled(true );
            ui->doubleSpinInParetoAlfaOff->setEnabled(true );
        } else {
            ui->labelStdDeviation        ->setEnabled(true);
            ui->spinInStdDeviation       ->setEnabled(true);
            ui->labelParetoAlfaOn        ->setEnabled(true);
            ui->doubleSpinInParetoAlfaOn ->setEnabled(true);
            ui->labelParetoAlfaOff       ->setEnabled(true);
            ui->doubleSpinInParetoAlfaOff->setEnabled(true);
        }
    } else {
        ui->labelStdDeviation        ->setEnabled(false);
        ui->spinInStdDeviation       ->setEnabled(false);
        ui->labelParetoAlfaOn        ->setEnabled(false);
        ui->doubleSpinInParetoAlfaOn ->setEnabled(false);
        ui->labelParetoAlfaOff       ->setEnabled(false);
        ui->doubleSpinInParetoAlfaOff->setEnabled(false);
    }

}

bool TrafficConfigurationDialog::inputsOk() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::inputsOk" << std::endl;
#endif

    QMessageBox* msg = new QMessageBox(this);
    msg->setIcon( QMessageBox::Warning );
    msg->setStandardButtons( QMessageBox::Ok );
    msg->setWindowTitle(trUtf8("Review Input(s)"));

    if( ui->spinInNumberPacketsFlow->value() <= 0 ) {
        msg->setText(trUtf8("Number of packets per flow \nmust be greater than 0"));
        msg->exec();
        delete msg;
        return false;
    }

    if( ui->doubleSpinInRequiredBandwidth->value() <= 0 ) {
        msg->setText(trUtf8("Required bandwidth \nmust be greater than 0"));
        msg->exec();
        delete msg;
        return false;
    }

    int typeInj = this->ui->comboInTypeInjection->currentIndex();

    if(typeInj == 0 || typeInj == 1 || typeInj == 4 || typeInj == 5) {
        unsigned int msgSizeMin = this->dataWidth * DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH;
        if( quint32(ui->spinInMessageSize->value()) < msgSizeMin ) {
            msg->setText(trUtf8("Message size must\nhave at least %1 bits\n(i.e. %2 flits)").arg(QString::number(msgSizeMin))
                         .arg(QString::number(DefaultValuesTrafficGeneration::MIN_PAYLOAD_LENGTH)));
            msg->exec();
            delete msg;
            return false;
        }
    } else if(typeInj == 2) {
        if(ui->spinInIdleTime->value() <= 0 ) {
            msg->setText(trUtf8("Idle interval must\nbe greater than 0"));
            msg->exec();
            delete msg;
            return false;
        }
    }

    if(typeInj == 3 || typeInj == 5) {
        if(ui->spinInMessageInterval->value() <= 0) {
            msg->setText(trUtf8("Message interval must\nbe greater than 0"));
            msg->exec();
            delete msg;
            return false;
        }
    }

    int distSpatial = ui->comboInDestinationNode->currentIndex();
    if( ui->comboInFunctionProbability->currentIndex() == 2 && // FunctionProb. == PARETO &&
            (distSpatial >= 7 && distSpatial <= 9) ) { // DistSpatial == NON-UNIFORM 1 OR 2 OR LOCAL
        msg->setText(trUtf8("Sorry!\nTraffic Generator not implemented for this function probability\nwith this spatial distribution (Non-Uniform 1,2 or Local)."));
        msg->exec();
        delete msg;
        return false;
    }

    delete msg;
    return true;
}

void TrafficConfigurationDialog::applyClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::applyClicked" << std::endl;
#endif

    if(! inputsOk() ) {
        return;
    }

    TrafficParameters* tp = new TrafficParameters();

    tp->setAlfaOff( (float) ui->doubleSpinInParetoAlfaOff->value() );
    tp->setAlfaOn( (float) ui->doubleSpinInParetoAlfaOn->value() );
    tp->setDeadline( ulong( ui->spinInDeadLine->value() ) );
    tp->setDestinationNodeX( quint32(ui->spinInNodeAddressX->value()) );
    tp->setDestinationNodeY( quint32(ui->spinInNodeAddressY->value()) );
    tp->setDestinationNodeZ( quint32(ui->spinInNodeAddressZ->value()) );
    tp->setIdleTime( quint32(ui->spinInIdleTime->value()) );
    tp->setInjectionType( quint32( ui->comboInTypeInjection->currentIndex() ) );
    tp->setIntervalTime( quint32( ui->spinInMessageInterval->value() ) );
//    tp->setNumberRates( ui->spin );
    tp->setPackageToSend( ulong( ui->spinInNumberPacketsFlow->value() ) );
    tp->setMessageSize( quint32( ui->spinInMessageSize->value()) );
    tp->setProbabilityFunction( quint32( ui->comboInFunctionProbability->currentIndex() ) );
    tp->setRequiredBandwidth( (float) ui->doubleSpinInRequiredBandwidth->value() );
    tp->setRequiredBandwidthStdDeviation( (float) ui->spinInStdDeviation->value() );
    tp->setSourceNodeX( this->xSrc );
    tp->setSourceNodeY( this->ySrc );
    tp->setSourceNodeZ( this->zSrc );
    tp->setSpatialDistribution( quint32( ui->comboInDestinationNode->currentIndex() ) );
    tp->setSwitchingTechnique( quint32(ui->comboInSwitchingTechnique->currentIndex()) );
    tp->setTrafficClass( quint32(ui->comboInTrafficClass->currentIndex()) );

    emit this->apply(tp,this->trafficNum);
    this->close();
}

void TrafficConfigurationDialog::applyAndReplicateClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficConfigurationDialog::applyAndReplicateClicked" << std::endl;
#endif

    if(!inputsOk()) {
        return;
    }

/// Confirmar aplicar e replicar!!!
    QMessageBox* msg =  new QMessageBox(trUtf8("Confirm Apply and Replicate"),trUtf8("Do you confirm apply and replicate this configuration for all nodes in network?\nPrevious configurations are lost!"),
                                        QMessageBox::Information,QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton,this);
    msg->setDefaultButton( QMessageBox::Yes );
    if( msg->exec() == QMessageBox::Yes ) {
        TrafficParameters* tp = new TrafficParameters();

        tp->setAlfaOff( (float) ui->doubleSpinInParetoAlfaOff->value() );
        tp->setAlfaOn( (float) ui->doubleSpinInParetoAlfaOn->value() );
        tp->setDeadline( ulong( ui->spinInDeadLine->value() ) );
        tp->setDestinationNodeX( quint32(ui->spinInNodeAddressX->value()) );
        tp->setDestinationNodeY( quint32(ui->spinInNodeAddressY->value()) );
        tp->setDestinationNodeZ( quint32(ui->spinInNodeAddressZ->value()) );
        tp->setIdleTime( quint32(ui->spinInIdleTime->value()) );
        tp->setInjectionType( quint32( ui->comboInTypeInjection->currentIndex() ) );
        tp->setIntervalTime( quint32( ui->spinInMessageInterval->value() ) );
    //    tp->setNumberRates( ui->spin );
        tp->setPackageToSend( ulong( ui->spinInNumberPacketsFlow->value() ) );
        tp->setMessageSize( quint32( ui->spinInMessageSize->value()) );
        tp->setProbabilityFunction( quint32( ui->comboInFunctionProbability->currentIndex() ) );
        tp->setRequiredBandwidth( (float) ui->doubleSpinInRequiredBandwidth->value() );
        tp->setRequiredBandwidthStdDeviation( (float) ui->spinInStdDeviation->value() );
        tp->setSourceNodeX( this->xSrc );
        tp->setSourceNodeY( this->ySrc );
        tp->setSourceNodeZ( this->zSrc );
        tp->setSpatialDistribution( quint32( ui->comboInDestinationNode->currentIndex() ) );
        tp->setSwitchingTechnique( quint32(ui->comboInSwitchingTechnique->currentIndex()) );
        tp->setTrafficClass( quint32(ui->comboInTrafficClass->currentIndex()) );

        emit this->applyAndReplicate(tp,this->trafficNum);
        this->close();
    }
}

void TrafficConfigurationDialog::changeEvent(QEvent *event) {

    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

/// Destrutor
TrafficConfigurationDialog::~TrafficConfigurationDialog(){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/TrafficConfigurationDialog" << std::endl;
#endif

    delete ui;
}

