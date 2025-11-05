#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <map>
#include <vector>

struct TrieNode {
    std::map<char, TrieNode*> children;
    std::string word;
    TrieNode() = default;
};

class Trie {
public:
    Trie();
    ~Trie();
    void insert(const std::vector<std::string>& sequence, const std::string& word);
    std::string search(const std::vector<std::string>& sequence);

private:
    TrieNode* root;
    void destroy(TrieNode* node);
};

#endif // TRIE_H