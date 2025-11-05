#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    destroy(root);
}

void Trie::destroy(TrieNode* node) {
    if (!node) return;
    for (auto const& [key, val] : node->children) {
        destroy(val);
    }
    delete node;
}

void Trie::insert(const std::vector<std::string>& sequence, const std::string& word) {
    TrieNode* current = root;
    for (const std::string& num_str : sequence) {
        char c = num_str[0];
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->word = word;
}

std::string Trie::search(const std::vector<std::string>& sequence) {
    TrieNode* current = root;
    for (const std::string& num_str : sequence) {
        char c = num_str[0];
        if (current->children.find(c) == current->children.end()) {
            return "";
        }
        current = current->children[c];
    }
    return current->word;
}
