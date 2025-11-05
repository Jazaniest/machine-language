#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Penerjemah.h"
#include "audioprocessor.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handleNoteDetected(const QString &note);
    void handleEndOfTransmission();
private:
    Ui::MainWindow *ui;
    Penerjemah *mesinPenerjemah;

    QThread* audioThread;
    AudioProcessor* audioProcessor;

    QString rangkaianNada;
};
#endif // MAINWINDOW_H
