#ifndef __TRAFFIC_EDIT_DIALOG_H__
#define __TRAFFIC_EDIT_DIALOG_H__

#include <QDialog>

namespace Ui {
    class TrafficEditDialog;
}

class TrafficParameters;

class TrafficEditDialog : public QDialog {
    Q_OBJECT

private:
    Ui::TrafficEditDialog *ui;

    TrafficParameters* configuredTraffic;

    short source;
    unsigned short dataWidth;
    unsigned short numElements;

    void loadSpatialsDistributions();
    void loadTrafficClasses();
    void loadTypesInjection();
    void loadSwitchingTechniques();
    void loadFunctionProbability();

    void loadDefaultValues();

    bool inputsOk();
protected:
    void changeEvent(QEvent*);

public:

    explicit TrafficEditDialog(QWidget *parent,
                               short source,
                               unsigned short numElements,
                               unsigned short dataWidth);
    void setConfiguration(TrafficParameters* tp);
    inline TrafficParameters* getConfiguredTraffic() const { return this->configuredTraffic; }
    void disableDestinationMode();
    ~TrafficEditDialog();

signals:
    void apply(TrafficParameters* configuration);

private slots:
    // Combos
    void updateSpatialDistribution(int);
    void updateTypeInjection(int);
    void updateFunctionProbability(int);

    void applyClicked();
};

#endif // __TRAFFIC_EDIT_DIALOG_H__
