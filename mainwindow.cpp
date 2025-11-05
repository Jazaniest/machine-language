#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mesinPenerjemah = new Penerjemah("kamus.txt");

    audioThread = new QThread(this);
    audioProcessor = new AudioProcessor();
    audioProcessor->moveToThread(audioThread);

    connect(audioThread, &QThread::started, audioProcessor, &AudioProcessor::startListening);
    connect(audioProcessor, &AudioProcessor::noteDetected, this, &MainWindow::handleNoteDetected);
    connect(audioProcessor, &AudioProcessor::endOfTransmission, this, &MainWindow::handleEndOfTransmission);
    connect(audioThread, &QThread::finished, audioProcessor, &QObject::deleteLater);

    audioThread->start();
    ui->hasilLabel->setText("");
}

MainWindow::~MainWindow()
{
    audioThread->quit();
    audioThread->wait();
    delete mesinPenerjemah;
    delete ui;
}

// FUNGSI INI KEMBALI MENJADI PENERUS PESAN REAL-TIME
void MainWindow::handleNoteDetected(const QString &note)
{
    // 1. Setiap nada (termasuk '4') langsung diteruskan ke Penerjemah
    mesinPenerjemah->prosesInput(note.toStdString());

    // 2. Minta pratinjau real-time dari Penerjemah
    std::string hasilPratinjau = mesinPenerjemah->getKalimatSementara();

    // 3. Tampilkan pratinjau di UI
    ui->hasilLabel->setText(QString::fromStdString(hasilPratinjau));
}

// FUNGSI INI SEKARANG BERTUGAS UNTUK 'FINALISASI'
void MainWindow::handleEndOfTransmission()
{
    qDebug() << "Pusat Komando: Transmisi Selesai. Finalisasi hasil.";

    // Minta hasil final dari Penerjemah.
    // getKalimat() akan memproses sisa buffer terakhir.
    std::string hasilFinal = mesinPenerjemah->getKalimat();

    // Tampilkan hasil final di UI
    ui->hasilLabel->setText(QString::fromStdString(hasilFinal));

    // Reset penerjemah untuk transmisi berikutnya
    mesinPenerjemah->reset();
}
