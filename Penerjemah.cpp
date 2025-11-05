#include "Penerjemah.h"
#include <iostream>
#include <fstream>
#include <sstream>

Penerjemah::Penerjemah(const std::string& dictionaryPath) {
    nadaSpasi = "4";

    std::ifstream fileKamus(dictionaryPath);
    if (!fileKamus.is_open()) {
        std::cerr << "ERROR: Tidak bisa membuka file kamus di: " << dictionaryPath << std::endl;
        return;
    }

    std::string baris;
    while (std::getline(fileKamus, baris)) {
        std::stringstream ss(baris);
        std::string kode, kata;

        if (std::getline(ss, kode, ',') && std::getline(ss, kata)) {
            std::vector<std::string> urutanKode;
            for (char c : kode) {
                urutanKode.push_back(std::string(1, c));
            }
            if (!urutanKode.empty() && !kata.empty()) {
                kamusTrie.insert(urutanKode, kata);
            }
        }
    }
    std::cout << "Kamus berhasil dimuat dari " << dictionaryPath << std::endl;
}

void Penerjemah::prosesInput(const std::string& input) {
    if (input == nadaSpasi) {
        terjemahkanBuffer();
    } else {
        bufferKataSaatIni.push_back(input);
    }
}

void Penerjemah::terjemahkanBuffer() {
    if (bufferKataSaatIni.empty()) return;

    std::string hasil = kamusTrie.search(bufferKataSaatIni);
    if (!hasil.empty()) {
        kalimatHasil += hasil + " ";
    } else {
        kalimatHasil += "[?] ";
    }
    bufferKataSaatIni.clear();
}

std::string Penerjemah::getKalimat() {
    terjemahkanBuffer();
    return kalimatHasil;
}

void Penerjemah::reset() {
    bufferKataSaatIni.clear();
    kalimatHasil.clear();
}

std::string Penerjemah::getKalimatSementara() {
    // Jika buffer kosong (misalnya setelah spasi), cukup tampilkan kalimat yang sudah jadi.
    if (bufferKataSaatIni.empty()) {
        return kalimatHasil;
    }

    // Coba cari padanan kata di Trie dengan isi buffer saat ini.
    std::string hasilPencarian = kamusTrie.search(bufferKataSaatIni);

    if (!hasilPencarian.empty()) {
        // JIKA KATA DITEMUKAN (misal: buffer berisi "123" dan itu berarti "hotel")
        // Tampilkan pratinjau kata yang sudah jadi.
        return kalimatHasil + hasilPencarian;
    } else {
        // JIKA KATA BELUM LENGKAP (misal: buffer berisi "12")
        // Tampilkan angka mentah agar pengguna tahu inputnya masuk.
        std::string buffer_str;
        for(const auto& nada : bufferKataSaatIni) {
            buffer_str += nada;
        }
        return kalimatHasil + buffer_str;
    }
}
