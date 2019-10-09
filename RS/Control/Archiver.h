#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <QObject>

class FolderCompressor;

class Archiver : public QObject {
    Q_OBJECT
public:
    explicit Archiver(QObject *parent = nullptr);

    FolderCompressor* loadSimulationResults(QString filename,QString workFolder);
    void saveSimulationResults(QStringList simulation);

signals:
    void sendMessage(QString msg);
};

#endif // ARCHIVER_H
