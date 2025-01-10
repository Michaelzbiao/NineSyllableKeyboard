// Trie.h
#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

struct TrieNode {
    bool is_end;
    int prefix_count; // 记录经过该节点的前缀次数
    int word_count;   // 记录以该节点为末尾的单词的输入次数
    std::unordered_map<char, TrieNode*> children;

    TrieNode() : is_end(false), prefix_count(0), word_count(0) {}
};

class Trie {

public:
    Trie();
    ~Trie();
    TrieNode* root;

    // 添加单词
    bool insert(const std::string& word, int count);

    // 删除单词
    void delete_word(const std::string& word);

    // 修改单词的输入次数
    void modify(const std::string& word, int new_count);

    // 查询单词的输入次数
    int get_count_of_word(const std::string& word);

    // 查询指定前缀的所有单词
    std::vector<std::pair<std::string, int>> get_words_with_prefix(const std::string& prefix);

    // 查询所有出现频率最高的单词
    std::vector<std::pair<std::string, int>> get_max_frequency_words();

    // 查询所有单词及输入次数
    std::vector<std::pair<std::string, int>> get_all_words();

    // 加载CSV数据
    void load_from_csv(const std::string& filename);

    // 保存Trie数据到CSV
    void save_to_csv(const std::string& filename);

    // 逐步搜索并显示结果
    void progressive_search(const std::string& digit_sequence);

    // 递归释放Trie节点
    void free_trie(TrieNode* node);

    // 正则表达式匹配的递归函数
    void regex_dfs(TrieNode* node, const std::string& pattern, int index, std::string current, std::vector<std::pair<std::string, int>>& results);

    // 查询前缀节点
    TrieNode* find_node(const std::string& prefix);

    // 收集所有单词
    void collect_all(TrieNode* node, const std::string& prefix, std::vector<std::pair<std::string, int>>& results);

    // 递归显示所有单词
    void display_all(TrieNode* node, std::string current, std::vector<std::pair<std::string, int>>& all_words);

    void progressive_dfs(TrieNode* node, const std::string& digit_sequence, int depth, std::string& current, std::vector<std::pair<std::string, int>>& results);

    // 删除单词的辅助函数
    void delete_helper(TrieNode* node, const std::string& word, int count);


};

#endif // TRIE_H
