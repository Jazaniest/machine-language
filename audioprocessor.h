#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <vector>
#include "portaudio.h"
#include "kiss_fftr.h"

#define NUM_SECONDS         (0.1)
#define SAMPLE_RATE         (44100)
#define FRAMES_PER_BUFFER   (int)(NUM_SECONDS * SAMPLE_RATE)

class AudioProcessor : public QObject
{
    Q_OBJECT

public:
    explicit AudioProcessor(QObject *parent = nullptr);
    ~AudioProcessor();
    void processAudio(const float* inputBuffer); // Fungsi yang dipanggil oleh callback C

signals:
    void noteDetected(const QString &note);
    void errorOccurred(const QString &errorMessage);
    void endOfTransmission();
    void resetSilenceTimerSignal();

public slots:
    void startListening();

private slots:
    void handleSilenceTimeout();
    void resetSilenceTimerSlot();
private:
    PaStream *stream = nullptr;
    QString lastDetectedNote = "";

    // Properti untuk KissFFT
    kiss_fftr_cfg fft_cfg;
    std::vector<kiss_fft_scalar> fft_in;
    std::vector<kiss_fft_cpx> fft_out;

    QTimer *silenceTimer;
};

#endif // AUDIOPROCESSOR_H
