#include "audioprocessor.h"
#include <QDebug>
#include <QThread>
#include <cmath>
#include <numeric>
#include <map>

// -- KONFIGURASI NADA --
const float TOLERANCE = 15.0f;
const std::map<QString, float> NOTE_FREQUENCIES = {
    {"1", 2000.00f}, {"2", 2050.00f}, {"3", 2100.00f}, {"4", 2150.01f},
    {"5", 2200.00f}, {"6", 2250.00f}, {"7", 2300.00f}
    // Nada '4' tidak dideteksi, ia adalah sinyal internal dari keheningan
};
const float AMPLITUDE_THRESHOLD = 0.005f;


// Callback function C-style untuk PortAudio
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    AudioProcessor *processor = static_cast<AudioProcessor*>(userData);
    if (processor) {
        processor->processAudio(static_cast<const float*>(inputBuffer));
    }
    return paContinue;
}


AudioProcessor::AudioProcessor(QObject *parent) : QObject(parent) {
    fft_cfg = kiss_fftr_alloc(FRAMES_PER_BUFFER, 0, nullptr, nullptr);
    fft_in.resize(FRAMES_PER_BUFFER);
    fft_out.resize(FRAMES_PER_BUFFER / 2 + 1);

    silenceTimer = new QTimer(this);
    silenceTimer->setInterval(1500);
    silenceTimer->setSingleShot(true);

    connect(silenceTimer, &QTimer::timeout, this, &AudioProcessor::handleSilenceTimeout);
    connect(this, &AudioProcessor::resetSilenceTimerSignal, this, &AudioProcessor::resetSilenceTimerSlot);
}

AudioProcessor::~AudioProcessor() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
    kiss_fftr_free(fft_cfg);
}

void AudioProcessor::startListening() {
    qDebug() << "AudioProcessor: Memulai proses mendengarkan di thread:" << QThread::currentThreadId();
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        emit errorOccurred("PortAudio error: " + QString(Pa_GetErrorText(err)));
        return;
    }
    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, this);
    if (err != paNoError) {
        emit errorOccurred("PortAudio error: " + QString(Pa_GetErrorText(err)));
        Pa_Terminate();
        return;
    }
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        emit errorOccurred("PortAudio error: " + QString(Pa_GetErrorText(err)));
        Pa_CloseStream(stream);
        Pa_Terminate();
    }
}

// FUNGSI INI DENGAN LOGIKA YANG SUDAH DIBERSIHKAN
void AudioProcessor::processAudio(const float* inputBuffer) {
    float max_amplitude = 0.0f;
    for (int i = 0; i < FRAMES_PER_BUFFER; ++i) {
        if (std::abs(inputBuffer[i]) > max_amplitude) {
            max_amplitude = std::abs(inputBuffer[i]);
        }
    }

    if (max_amplitude < AMPLITUDE_THRESHOLD) return;

    std::copy(inputBuffer, inputBuffer + FRAMES_PER_BUFFER, fft_in.begin());
    kiss_fftr(fft_cfg, fft_in.data(), fft_out.data());

    float max_magnitude = -1.0f;
    int peak_index = 0;
    for (int i = 1; i < (FRAMES_PER_BUFFER / 2 + 1); ++i) {
        float magnitude = fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i;
        if (magnitude > max_magnitude) {
            max_magnitude = magnitude;
            peak_index = i;
        }
    }

    float peak_frequency = (float)peak_index * SAMPLE_RATE / FRAMES_PER_BUFFER;

    // HANYA SATU PERULANGAN YANG BERSIH
    for (auto const& [note, freq] : NOTE_FREQUENCIES) {
        if (std::abs(peak_frequency - freq) <= TOLERANCE) {

            emit resetSilenceTimerSignal(); // <-- KIRIM PESAN KE DIRI SENDIRI

            if (note != lastDetectedNote) {
                lastDetectedNote = note;
                emit noteDetected(note);
            }
            return;
        }
    }
    lastDetectedNote = "";
}

void AudioProcessor::resetSilenceTimerSlot()
{
    silenceTimer->start();
}

void AudioProcessor::handleSilenceTimeout()
{
    qDebug() << "Timer Keheningan: Transmisi dianggap selesai.";
    emit endOfTransmission();
}
