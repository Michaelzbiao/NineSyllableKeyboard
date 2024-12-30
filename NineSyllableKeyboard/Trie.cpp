// Trie.cpp
#include "Trie.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

// 九键映射表
static const std::unordered_map<char, char> KEY_MAPPING = {
    {'a', '2'}, {'b', '2'}, {'c', '2'},
    {'d', '3'}, {'e', '3'}, {'f', '3'},
    {'g', '4'}, {'h', '4'}, {'i', '4'},
    {'j', '5'}, {'k', '5'}, {'l', '5'},
    {'m', '6'}, {'n', '6'}, {'o', '6'},
    {'p', '7'}, {'q', '7'}, {'r', '7'}, {'s', '7'},
    {'t', '8'}, {'u', '8'}, {'v', '8'},
    {'w', '9'}, {'x', '9'}, {'y', '9'}, {'z', '9'}
};

// 构造函数
Trie::Trie() {
    root = new TrieNode();
}

// 析构函数
Trie::~Trie() {
    free_trie(root);
}

// 递归释放内存
void Trie::free_trie(TrieNode* node) {
    if (node == nullptr) return;
    for (auto& pair : node->children) {
        free_trie(pair.second);
    }
    delete node;
}

// 插入单词
void Trie::insert(const std::string& word, int count) {
    TrieNode* node = root;
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);
    for (char ch : lower_word) {
        if (KEY_MAPPING.find(ch) == KEY_MAPPING.end()) {
            std::cout << "单词包含无效字符: " << ch << std::endl;
            return;
        }
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }
    node->is_end = true;
    node->count += count;
    std::cout << "已插入/更新单词 '" << word << "'，当前输入次数为 " << node->count << "." << std::endl;
}

// 删除单词的辅助函数
bool Trie::delete_helper(TrieNode* node, const std::string& word, int depth) {
    if (depth == word.size()) {
        if (!node->is_end) {
            return false; // 单词不存在
        }
        node->is_end = false;
        node->count = 0;
        return node->children.empty();
    }

    char ch = word[depth];
    if (node->children.find(ch) == node->children.end()) {
        return false; // 单词不存在
    }

    bool should_delete = delete_helper(node->children[ch], word, depth + 1);

    if (should_delete) {
        delete node->children[ch];
        node->children.erase(ch);
        return node->children.empty() && !node->is_end;
    }

    return false;
}

// 删除单词
void Trie::delete_word(const std::string& word) {
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);
    if (delete_helper(root, lower_word, 0)) {
        std::cout << "已删除单词 '" << word << "'." << std::endl;
    }
    else {
        std::cout << "未找到单词 '" << word << "'." << std::endl;
    }
}

// 修改单词的输入次数
void Trie::modify(const std::string& word, int new_count) {
    TrieNode* node = root;
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);
    for (char ch : lower_word) {
        if (node->children.find(ch) == node->children.end()) {
            std::cout << "未找到单词 '" << word << "'." << std::endl;
            return;
        }
        node = node->children[ch];
    }
    if (node->is_end) {
        node->count = new_count;
        std::cout << "已修改单词 '" << word << "' 的输入次数为 " << new_count << "." << std::endl;
    }
    else {
        std::cout << "未找到单词 '" << word << "'." << std::endl;
    }
}

// 深度优先搜索查询
void Trie::dfs(TrieNode* node, const std::string& digit_sequence, int depth, std::string current, std::vector<std::pair<std::string, int>>& results) {
    if (depth == digit_sequence.size()) {
        if (node->is_end) {
            results.emplace_back(current, node->count);
        }
        return;
    }

    char digit = digit_sequence[depth];
    // 获取当前数字对应的所有可能字符
    std::vector<char> possible_chars;
    for (const auto& pair : KEY_MAPPING) {
        if (pair.second == digit) {
            possible_chars.push_back(pair.first);
        }
    }

    for (char ch : possible_chars) {
        if (node->children.find(ch) != node->children.end()) {
            dfs(node->children[ch], digit_sequence, depth + 1, current + ch, results);
        }
    }
}

// 查询单词
std::vector<std::pair<std::string, int>> Trie::query(const std::string& digit_sequence) {
    std::vector<std::pair<std::string, int>> results;
    // 验证数字序列是否合法
    for (char digit : digit_sequence) {
        if (digit < '2' || digit > '9') {
            std::cout << "数字序列包含无效数字: " << digit << std::endl;
            return results;
        }
    }
    dfs(root, digit_sequence, 0, "", results);
    return results;
}

// 显示所有单词（用于调试）
void Trie::display_all(TrieNode* node, std::string current, std::vector<std::pair<std::string, int>>& all_words) {
    if (node->is_end) {
        all_words.emplace_back(current, node->count);
    }
    for (const auto& pair : node->children) {
        display_all(pair.second, current + pair.first, all_words);
    }
}

void Trie::display() {
    std::vector<std::pair<std::string, int>> all_words;
    display_all(root, "", all_words);
    std::cout << "所有单词及其输入次数：" << std::endl;
    for (const auto& pair : all_words) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

// 加载CSV数据
void Trie::load_from_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string word, count_str;
        if (!getline(ss, word, ',')) continue;
        if (!getline(ss, count_str, ',')) continue;
        try {
            int count = std::stoi(count_str);
            insert(word, count);
        }
        catch (...) {
            std::cout << "无效的计数值: " << count_str << " 在单词: " << word << std::endl;
            continue;
        }
    }

    file.close();
}
