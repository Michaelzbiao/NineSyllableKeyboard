// Trie.h
#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include <string>
#include <vector>
#include <utility>

class Trie {
private:
    TrieNode* root;

    // ¸¨Öúº¯ÊýÉùÃ÷
    bool delete_helper(TrieNode* node, const std::string& word, int depth);
    void dfs(TrieNode* node, const std::string& digit_sequence, int depth, std::string current, std::vector<std::pair<std::string, int>>& results);
    void display_all(TrieNode* node, std::string current, std::vector<std::pair<std::string, int>>& all_words);
    void free_trie(TrieNode* node);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word, int count);
    void delete_word(const std::string& word);
    void modify(const std::string& word, int new_count);
    std::vector<std::pair<std::string, int>> query(const std::string& digit_sequence);
    void load_from_csv(const std::string& filename);
    void display();
};

#endif // TRIE_H
