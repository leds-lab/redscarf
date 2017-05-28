#include "ui_TrafficEditDialog.h"
#include "View/TrafficEditDialog.h"

#include "Model/Traffic/TrafficParameters.h"

#include <QMessageBox>

#ifdef DEBUG_POINTS_METHODS
    #include <iostream>
#endif

TrafficEditDialog::TrafficEditDialog(QWidget *parent,
                                     short source,
                                     unsigned short numElements,
                                     unsigned short dataWidth)
    : QDialog(parent,Qt::Window),
      ui(new Ui::TrafficEditDialog),
      configuredTraffic(NULL),
      source(source),
      dataWidth(dataWidth),
      numElements(numElements)
{
    // Form use (.ui)
    ui->setupUi(this);

    // Load items on the combo boxes
    this->loadSpatialsDistributions();
    this->loadTrafficClasses();
    this->loadTypesInjection();
    this->loadSwitchingTechniques();
    this->loadFunctionProbability();

    // Source node string
    QString sourceNode = QString("( %1 )").arg( (source >= 0 ? QString::number(source) : tr("**Multiple**") ));
    ui->labelSourceNode->setText(sourceNode);

    // Define the maximum accepted value for specific destination
    ui->spinInSpecificNodeAddress->setMaximum(numElements - 1);

    // Define the minimum packet size (bits) - one flit (data width)
    ui->spinInMessageSize->setMinimum( dataWidth );

    // Initializing components
    updateSpatialDistribution(0);
    updateFunctionProbability(0);
    updateTypeInjection(0);

    // Bind signals and slots
    connect(ui->comboInDestinationNode,SIGNAL(currentIndexChanged(int)),this,SLOT(updateSpatialDistribution(int)));
    connect(ui->comboInTypeInjection,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTypeInjection(int)));
    connect(ui->comboInFunctionProbability,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFunctionProbability(int)));

    connect(ui->buttonCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->buttonApply,SIGNAL(clicked()),this,SLOT(applyClicked()));

    loadDefaultValues();

//    this->adjustSize();
}

void TrafficEditDialog::loadDefaultValues() {

    TrafficParameters* tp = new TrafficParameters();
    this->setConfiguration(tp);
    delete tp;

}

void TrafficEditDialog::loadSpatialsDistributions() {

    QStringList destinarionOptions = TrafficParameters::availableSpatialDistributions();

    ui->comboInDestinationNode->insertItems(0,destinarionOptions);
}

void TrafficEditDialog::loadTrafficClasses() {

    QStringList trafficClasses = TrafficParameters::availableTrafficClasses();
    ui->comboInTrafficClass->insertItems(0,trafficClasses);
}

void TrafficEditDialog::loadTypesInjection() {

    QStringList injectionTypes = TrafficParameters::availableInjectionTypes();
    ui->comboInTypeInjection->insertItems(0,injectionTypes);
}

void TrafficEditDialog::loadSwitchingTechniques() {
    QStringList switchingTechniques = TrafficParameters::availableSwitchingTechniques();
    ui->comboInSwitchingTechnique->insertItems(0,switchingTechniques);
}

void TrafficEditDialog::loadFunctionProbability() {

    QStringList probabilityFunction = TrafficParameters::availableProbabilityFunctions();
    ui->comboInFunctionProbability->insertItems(0,probabilityFunction);
}

void TrafficEditDialog::setConfiguration(TrafficParameters *tp) {

    ui->doubleSpinInParetoAlfaOff->setValue( tp->getAlfaOff() );
    ui->doubleSpinInParetoAlfaOn->setValue( tp->getAlfaOn() );
    ui->spinInDeadLine->setValue( qint32(tp->getDeadline() ) );
    ui->spinInSpecificNodeAddress->setValue( qint32(tp->getDestination()) );
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
void TrafficEditDialog::updateSpatialDistribution(int value) {

    if( value == 0 ){ // 0 == Specified Address
        ui->labelNodeAddress   ->setEnabled(true);
        ui->spinInSpecificNodeAddress->setEnabled(true);
    } else { // Se não deve ser desabilitada
        ui->labelNodeAddress   ->setEnabled(false);
        ui->spinInSpecificNodeAddress->setEnabled(false);
    }
}

void TrafficEditDialog::updateTypeInjection(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficEditDialog::updateTypeInjection" << std::endl;
#endif

    switch(value) {
        case 0: // Constant
            ui->labelFunctionProbability   ->setEnabled(false);
            ui->comboInFunctionProbability ->setEnabled(false);
            ui->labelMessageSize           ->setEnabled(true );
            ui->spinInMessageSize          ->setEnabled(true );
            ui->labelIdleTime              ->setEnabled(false);
            ui->spinInIdleTime             ->setEnabled(false);
            ui->labelMessageInterval       ->setEnabled(false);
            ui->spinInMessageInterval      ->setEnabled(false);
            break;
        case 1: // Variable idle time - Fix message size
            ui->labelFunctionProbability   ->setEnabled(true );
            ui->comboInFunctionProbability ->setEnabled(true );
            ui->labelMessageSize           ->setEnabled(true );
            ui->spinInMessageSize          ->setEnabled(true );
            ui->labelIdleTime              ->setEnabled(false);
            ui->spinInIdleTime             ->setEnabled(false);
            ui->labelMessageInterval       ->setEnabled(false);
            ui->spinInMessageInterval      ->setEnabled(false);
            break;
        case 2: // Variable message size - Fix idle time
            ui->labelFunctionProbability   ->setEnabled(true );
            ui->comboInFunctionProbability ->setEnabled(true );
            ui->labelMessageSize           ->setEnabled(false);
            ui->spinInMessageSize          ->setEnabled(false);
            ui->labelIdleTime              ->setEnabled(true );
            ui->spinInIdleTime             ->setEnabled(true );
            ui->labelMessageInterval       ->setEnabled(false);
            ui->spinInMessageInterval      ->setEnabled(false);
            break;
        case 3: // Variable message size - Fix message inteval
            ui->labelFunctionProbability   ->setEnabled(true );
            ui->comboInFunctionProbability ->setEnabled(true );
            ui->labelMessageSize           ->setEnabled(false);
            ui->spinInMessageSize          ->setEnabled(false);
            ui->labelIdleTime              ->setEnabled(false);
            ui->spinInIdleTime             ->setEnabled(false);
            ui->labelMessageInterval       ->setEnabled(true );
            ui->spinInMessageInterval      ->setEnabled(true );
            break;
        case 4: // Variable message interval - Fix message size
            ui->labelFunctionProbability   ->setEnabled(true );
            ui->comboInFunctionProbability ->setEnabled(true );
            ui->labelMessageSize           ->setEnabled(true );
            ui->spinInMessageSize          ->setEnabled(true );
            ui->labelIdleTime              ->setEnabled(false);
            ui->spinInIdleTime             ->setEnabled(false);
            ui->labelMessageInterval       ->setEnabled(false);
            ui->spinInMessageInterval      ->setEnabled(false);
            break;
        case 5: // Variable burst size - Fix message interval
            ui->labelFunctionProbability   ->setEnabled(true );
            ui->comboInFunctionProbability ->setEnabled(true );
            ui->labelMessageSize           ->setEnabled(true );
            ui->spinInMessageSize          ->setEnabled(true );
            ui->labelIdleTime              ->setEnabled(false);
            ui->spinInIdleTime             ->setEnabled(false);
            ui->labelMessageInterval       ->setEnabled(true );
            ui->spinInMessageInterval      ->setEnabled(true );
            break;
        default:
            ui->labelFunctionProbability   ->setEnabled(true);
            ui->comboInFunctionProbability ->setEnabled(true);
            ui->labelMessageSize           ->setEnabled(true);
            ui->spinInMessageSize          ->setEnabled(true);
            ui->labelIdleTime              ->setEnabled(true);
            ui->spinInIdleTime             ->setEnabled(true);
            ui->labelMessageInterval       ->setEnabled(true);
            ui->spinInMessageInterval      ->setEnabled(true);
            break;
    }

    updateFunctionProbability(ui->comboInFunctionProbability->currentIndex());
}

void TrafficEditDialog::updateFunctionProbability(int value) {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficEditDialog::updateFunctionProbability" << std::endl;
#endif

    if(ui->comboInFunctionProbability->isEnabled()){
        switch (value) {
            case 0: // Normal
                ui->labelStdDeviation        ->setEnabled(true );
                ui->spinInStdDeviation       ->setEnabled(true );
                ui->labelParetoAlfaOn        ->setEnabled(false);
                ui->doubleSpinInParetoAlfaOn ->setEnabled(false);
                ui->labelParetoAlfaOff       ->setEnabled(false);
                ui->doubleSpinInParetoAlfaOff->setEnabled(false);
                break;
            case 1: // Exponential
                ui->labelStdDeviation        ->setEnabled(false);
                ui->spinInStdDeviation       ->setEnabled(false);
                ui->labelParetoAlfaOn        ->setEnabled(false);
                ui->doubleSpinInParetoAlfaOn ->setEnabled(false);
                ui->labelParetoAlfaOff       ->setEnabled(false);
                ui->doubleSpinInParetoAlfaOff->setEnabled(false);
                break;
            case 2: // Pareto
                ui->labelStdDeviation        ->setEnabled(false);
                ui->spinInStdDeviation       ->setEnabled(false);
                ui->labelParetoAlfaOn        ->setEnabled(true );
                ui->doubleSpinInParetoAlfaOn ->setEnabled(true );
                ui->labelParetoAlfaOff       ->setEnabled(true );
                ui->doubleSpinInParetoAlfaOff->setEnabled(true );
                break;
            default:
                ui->labelStdDeviation        ->setEnabled(true);
                ui->spinInStdDeviation       ->setEnabled(true);
                ui->labelParetoAlfaOn        ->setEnabled(true);
                ui->doubleSpinInParetoAlfaOn ->setEnabled(true);
                ui->labelParetoAlfaOff       ->setEnabled(true);
                ui->doubleSpinInParetoAlfaOff->setEnabled(true);
                break;
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

bool TrafficEditDialog::inputsOk() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficEditDialog::inputsOk" << std::endl;
#endif

    QMessageBox msg(this);// = QMessageBox(this);
    msg.setIcon( QMessageBox::Warning );
    msg.setStandardButtons( QMessageBox::Ok );
    msg.setWindowTitle(trUtf8("Review Input(s)"));

    if( ui->comboInDestinationNode->currentIndex() == 0 ) { // Specified address
        if( ui->spinInSpecificNodeAddress->value() == source ) {
            msg.setText( tr("The source and destination are the same!") );
            msg.exec();
            return false;
        }
    }

    if( ui->spinInNumberPacketsFlow->value() <= 0 ) {
        msg.setText(trUtf8("Number of packets per flow \nmust be greater than 0"));
        msg.exec();
        return false;
    }

    if( ui->doubleSpinInRequiredBandwidth->value() <= 0 ) {
        msg.setText(trUtf8("Required bandwidth \nmust be greater than 0"));
        msg.exec();
        return false;
    }

    int typeInj = this->ui->comboInTypeInjection->currentIndex();

    if(typeInj == 0 || typeInj == 1 || typeInj == 4 || typeInj == 5) {
        unsigned int msgSizeMin = this->dataWidth;
        if( quint32(ui->spinInMessageSize->value()) < msgSizeMin ) {
            msg.setText(trUtf8("Message size must\nhave at least %1 bits\n(i.e. %2 flit)")
                         .arg(QString::number(msgSizeMin))
                         .arg(QString::number(1)));
            msg.exec();
            return false;
        }
    } else if(typeInj == 2) {
        if(ui->spinInIdleTime->value() <= 0 ) {
            msg.setText(trUtf8("Idle interval must\nbe greater than 0"));
            msg.exec();
            return false;
        }
    }

    if(typeInj == 3 || typeInj == 5) {
        if(ui->spinInMessageInterval->value() <= 0) {
            msg.setText(trUtf8("Message interval must\nbe greater than 0"));
            msg.exec();
            return false;
        }
    }

    int distSpatial = ui->comboInDestinationNode->currentIndex();
    if( ui->comboInFunctionProbability->currentIndex() == 2 && // FunctionProb. == PARETO &&
            (distSpatial >= 7 && distSpatial <= 9) ) { // DistSpatial == NON-UNIFORM 1 OR 2 OR LOCAL
        msg.setText(trUtf8("Sorry!\nTraffic Generator not implemented for this function probability\nwith this spatial distribution (Non-Uniform 1,2 or Local)."));
        msg.exec();
        return false;
    }

    return true;
}

void TrafficEditDialog::applyClicked() {
#ifdef DEBUG_POINTS_METHODS
    std::cout << "View/TrafficEditDialog::applyClicked" << std::endl;
#endif

    if(! inputsOk() ) {
        return;
    }

    TrafficParameters* tp = new TrafficParameters();

    tp->setAlfaOff( (float) ui->doubleSpinInParetoAlfaOff->value() );
    tp->setAlfaOn( (float) ui->doubleSpinInParetoAlfaOn->value() );
    tp->setDeadline( ulong( ui->spinInDeadLine->value() ) );
    tp->setDestination( quint32(ui->spinInSpecificNodeAddress->value()) );
    tp->setIdleTime( quint32(ui->spinInIdleTime->value()) );
    tp->setInjectionType( quint32( ui->comboInTypeInjection->currentIndex() ) );
    tp->setIntervalTime( quint32( ui->spinInMessageInterval->value() ) );
    tp->setPackageToSend( ulong( ui->spinInNumberPacketsFlow->value() ) );
    tp->setMessageSize( quint32( ui->spinInMessageSize->value()) );
    tp->setProbabilityFunction( quint32( ui->comboInFunctionProbability->currentIndex() ) );
    tp->setRequiredBandwidth( (float) ui->doubleSpinInRequiredBandwidth->value() );
    tp->setRequiredBandwidthStdDeviation( (float) ui->spinInStdDeviation->value() );
    tp->setSource( this->source );
    tp->setSpatialDistribution(  static_cast<SpatialDistribution::Distribution>( ui->comboInDestinationNode->currentIndex() ) );
    tp->setSwitchingTechnique( quint32(ui->comboInSwitchingTechnique->currentIndex()) );
    tp->setTrafficClass( quint32(ui->comboInTrafficClass->currentIndex()) );

    emit this->apply(tp);
    this->configuredTraffic = tp;
    this->close();
}

void TrafficEditDialog::disableDestinationMode() {

    ui->labelDestinationNode->setEnabled(false);
    ui->comboInDestinationNode->setEnabled(false);

}

void TrafficEditDialog::changeEvent(QEvent *event) {

    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }

    QWidget::changeEvent(event);

}

/// Destrutor
TrafficEditDialog::~TrafficEditDialog(){
#ifdef DEBUG_POINTS_METHODS
    std::cout << "Destructor Class View/TrafficEditDialog" << std::endl;
#endif

    delete ui;
}

