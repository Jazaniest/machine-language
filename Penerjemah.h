#ifndef PENERJEMAH_H
#define PENERJEMAH_H

#include <string>
#include <vector>
#include "Trie.h"

class Penerjemah {
public:
    Penerjemah(const std::string& dictionaryPath);
    void prosesInput(const std::string& input);
    std::string getKalimat();
    std::string getKalimatSementara();
    void reset();

private:
    void terjemahkanBuffer();
    
    Trie kamusTrie;
    std::string nadaSpasi;
    std::vector<std::string> bufferKataSaatIni;
    std::string kalimatHasil;
};

#endif // PENERJEMAH_H
