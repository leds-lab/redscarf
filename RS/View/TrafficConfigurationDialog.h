#ifndef __TRAFFICCONFIGURATIONDIALOG_H__
#define __TRAFFICCONFIGURATIONDIALOG_H__

#include <QDialog>
#include <QListWidgetItem>
#include <QGraphicsScene>

namespace Ui {
    class TrafficConfigurationDialog;
}

class QAbstractButton;
class SystemParameters;
class Vertice;
class Arc;
class TrafficParameters;

class TrafficConfigurationDialog : public QDialog {
    Q_OBJECT

public:
    explicit TrafficConfigurationDialog(QWidget *parent,QList<QListWidgetItem *> systemConfItems);
    ~TrafficConfigurationDialog();

protected:
    void configureGraphics(SystemParameters* sp);
    void drawRing(SystemParameters* sp);
    void draw2DMesh(SystemParameters* sp);
    void draw3DMesh(SystemParameters* sp);

    void drawFlowsSelectedVertices(QList<Vertice *> selectedVertices);

signals:
    void trafficConfigured(QList<QVariant> configuration);

private slots:
    void itemClicked(QListWidgetItem*);
    void graphicSelectionChange();
    void repaintGraphics();
    void addTrafficConfiguration();
    void applyTrafficParameters(TrafficParameters*);

    void trafficListSelectionChange();
    void trafficItemDoubleClick(QModelIndex);

    void removeSelectedTraffic();

    void apply();

    void zoomRestore();
private:

    void clearScene();
    void clearConnections();
    void loadExistentConfigurations();

    const unsigned short SPACE=80;

    Ui::TrafficConfigurationDialog *ui;
    QList<QListWidgetItem* > systemConfItems;
    QList<Vertice* > vertices;
    QList<Arc *> flowConnections;

    QGraphicsScene* scene;

    SystemParameters* currentSystemParameters;

};

#endif // __TRAFFICCONFIGURATIONDIALOG_H__
