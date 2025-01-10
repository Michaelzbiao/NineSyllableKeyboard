// Trie.cpp
#include "Trie.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>

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
bool Trie::insert(const std::string& word, int count) {
    TrieNode* node = root;
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);

    for (char ch : lower_word) {
        if (KEY_MAPPING.find(ch) == KEY_MAPPING.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符） " << std::endl;
            return false;
        }
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }

    if (node->is_end) {
        std::cout << "字符串 \"" << word << "\" 已存在，" << "次数为：" << node->word_count << std::endl;
        return false; // 插入失败，字符串已存在
    }
    else {
        node->is_end = true;
        node->word_count = count; // 设置初始输入次数
        // 重新遍历以更新 prefix_count
        node = root;
        for (char ch : lower_word) {
            node = node->children[ch];
            node->prefix_count += count; // 递增前缀计数
        }
        std::cout << "已插入单词 '" << word << "'，当前输入次数为 " << node->word_count << "." << std::endl;
        return true; // 插入成功
    }
}

// 删除单词的辅助函数
void Trie::delete_helper(TrieNode* node, const std::string& word, int count) {
    TrieNode* parent = nullptr; // 记录父节点

    for (int i = 0; i < word.size(); i++) {
        char ch = word[i];
        parent = node; // 更新父节点

        node = node->children[ch];
        node->prefix_count -= count;

        // 如果当前节点的 prefix_count 为 0，开始清理节点
        if (node->prefix_count == 0) {
            parent->children[ch] = new TrieNode; // 断开父节点和子节点的连接

            // 从当前节点向下逐层释放
            while (i < word.size()) {
                TrieNode* delete_node = node;
                node = node->children[word[++i]]; // 下一个节点
                delete delete_node;
            }
            return;
        }
    }

    // 如果未删除路径上的节点，只需要更新末尾节点的信息
    node->is_end = false;
    node->word_count = 0;
}



// 删除单词
void Trie::delete_word(const std::string& word) {
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);

    for (char ch : lower_word) {
        if (KEY_MAPPING.find(ch) == KEY_MAPPING.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符） " << std::endl;
            return;
        }
    }
    // 获取单词的输入次数，以便递减 prefix_count
    int count = get_count_of_word(lower_word);
    if (count == -1) {
        std::cout << "未找到单词 '" << word << "'." << std::endl;
        return;
    }

    // 调用删除辅助函数
    delete_helper(root, lower_word, count);

    // 再次检查单词是否存在来确认删除是否成功
    if (get_count_of_word(lower_word) == -1) {
        std::cout << "已删除单词 '" << word << "'." << std::endl;
    }
    else {
        std::cout << "删除单词 '" << word << "' 失败." << std::endl;
    }
}

// 修改单词的输入次数
void Trie::modify(const std::string& word, int new_count) {
    TrieNode* node = root;
    std::string lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);

    // 查找单词，并计算输入次数的增量
    TrieNode* target_node = nullptr;
    for (char ch : lower_word) {
        if (KEY_MAPPING.find(ch) == KEY_MAPPING.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符）" << std::endl;
            return;
        }
        if (node->children.find(ch) == node->children.end()) {
            std::cout << "未找到单词 '" << word << "'。" << std::endl;
            return;
        }
        node = node->children[ch];
    }
    target_node = node;

    if (target_node && target_node->is_end) {
        int old_count = target_node->word_count;
        int delta = new_count - old_count; // 计算输入次数的增量

        // 更新单词的输入次数
        target_node->word_count = new_count;

        // 更新前缀总次数
        node = root;
        for (char ch : lower_word) {
            node = node->children[ch];
            node->prefix_count += delta; // 更新前缀总次数
        }

        std::cout << "已修改单词 '" << word << "' 的输入次数为 " << new_count << "。" << std::endl;
    }
    else {
        std::cout << "未找到单词 '" << word << "'。" << std::endl;
    }
}

// 逐步搜索并显示结果
void Trie::progressive_search(const std::string& digit_sequence) {
    // 遍历每个前缀长度
    for (int i = 1; i <= digit_sequence.length(); i++) {
        std::string current_digits = digit_sequence.substr(0, i);
        std::cout << current_digits << " ";

        // 获取当前前缀的所有可能匹配
        std::vector<std::pair<std::string, int>> matches;
        std::string current;
        progressive_dfs(root, current_digits, 0, current, matches);

        if (matches.empty()) {
            std::cout << "no word" << std::endl;
        }
        else {
            // 按照prefix_count排序，选择最高频率的匹配
            std::sort(matches.begin(), matches.end(),
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                });
            std::cout << matches[0].first << std::endl;
        }
    }
}

// 用于progressive search的DFS函数
void Trie::progressive_dfs(TrieNode* node, const std::string& digit_sequence, int depth,
    std::string& current, std::vector<std::pair<std::string, int>>& results) {

    // 如果当前字符串长度等于目标长度，检查是否匹配
    if (current.length() == digit_sequence.length()) {
        bool matches = true;
        for (size_t i = 0; i < current.length(); i++) {
            if (KEY_MAPPING.at(current[i]) != digit_sequence[i]) {
                matches = false;
                break;
            }
        }
        if (matches) {
            results.emplace_back(current, node->prefix_count);
        }
        return;
    }

    if (depth >= digit_sequence.length()) {
        return;
    }

    char digit = digit_sequence[depth];
    for (const auto& pair : KEY_MAPPING) {
        if (pair.second == digit) {
            char ch = pair.first;
            if (node->children.find(ch) != node->children.end()) {
                current.push_back(ch);
                progressive_dfs(node->children[ch], digit_sequence, depth + 1, current, results);
                current.pop_back();
            }
        }
    }
}

// 正则表达式匹配的递归函数
void Trie::regex_dfs(TrieNode* node, const std::string& pattern, int index, std::string current, std::vector<std::pair<std::string, int>>& results) {
    if (!node) return;

    // 如果到达模式末尾，则判断当前节点是否是一个单词
    if (index == (int)pattern.size()) {
        if (node->is_end) {
            results.emplace_back(current, node->word_count);
        }
        return;
    }

    char ch = pattern[index];
    if (ch != '?' && ch != '*') {
        // 普通字符
        char c = ::tolower(ch);
        if (node->children.find(c) != node->children.end()) {
            regex_dfs(node->children[c], pattern, index + 1, current + c, results);
        }
    }
    else if (ch == '?') {
        // '?' 可以匹配任意单个字符
        for (auto& child : node->children) {
            regex_dfs(child.second, pattern, index + 1, current + child.first, results);
        }
    }
    else if (ch == '*') {
        // '*' 可以匹配任意多个字符（包括 0 个）

        // 1) 先考虑匹配 0 个字符的情况：跳过 '*'
        regex_dfs(node, pattern, index + 1, current, results);

        // 2) 考虑匹配 >=1 个字符的情况：对所有子节点进行尝试，但 pattern 的 index 不变
        for (auto& child : node->children) {
            regex_dfs(child.second, pattern, index, current + child.first, results);
        }
    }
}

// 查询前缀节点
TrieNode* Trie::find_node(const std::string& prefix) {
    TrieNode* node = root;
    for (char ch : prefix) {
        char c = ::tolower(ch);
        if (node->children.find(c) == node->children.end()) {
            return nullptr;
        }
        node = node->children[c];
    }
    return node;
}

// 获取单词的输入次数
int Trie::get_count_of_word(const std::string& word) {

    TrieNode* node = find_node(word);
    if (!node || !node->is_end) {
        return -1; // 表示未找到
    }
    return node->word_count;
}

// 从给定node出发，收集它所有子孙节点对应的单词
void Trie::collect_all(TrieNode* node, const std::string& prefix, std::vector<std::pair<std::string, int>>& results) {
    if (!node) return;
    if (node->is_end) {
        results.emplace_back(prefix, node->word_count);
    }
    for (auto& p : node->children) {
        collect_all(p.second, prefix + p.first, results);
    }
}

// 获取指定前缀的所有单词及输入次数
std::vector<std::pair<std::string, int>> Trie::get_words_with_prefix(const std::string& prefix) {
    std::vector<std::pair<std::string, int>> results;
    TrieNode* node = find_node(prefix);
    if (!node) {
        return results; // 返回空
    }
    // node 对应前缀的最后一个字符的节点，从此节点收集所有后续单词
    collect_all(node, prefix, results);
    return results;
}

// 获取所有出现频率最高的单词
std::vector<std::pair<std::string, int>> Trie::get_max_frequency_words() {
    // 获取所有单词
    std::vector<std::pair<std::string, int>> all_words;
    std::vector<std::pair<std::string, int>> result;
    display_all(root, "", all_words);

    // 如果没有任何单词，返回空
    if (all_words.empty()) {
        return result;
    }

    // 找到最大频率
    int max_frequency = all_words[0].second;
    for (const auto& p : all_words) {
        if (p.second > max_frequency) {
            max_frequency = p.second;
        }
    }

    // 收集所有具有最大频率的单词
    for (const auto& p : all_words) {
        if (p.second == max_frequency) {
            result.push_back(p);
        }
    }

    // 按字典序排序
    std::sort(result.begin(), result.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

    return result;
}

// 显示所有单词的辅助函数
void Trie::display_all(TrieNode* node, std::string current, std::vector<std::pair<std::string, int>>& all_words) {
    if (node->is_end) {
        all_words.emplace_back(current, node->word_count);
    }
    for (const auto& pair : node->children) {
        display_all(pair.second, current + pair.first, all_words);
    }
}

// 获取所有单词及输入次数
std::vector<std::pair<std::string, int>> Trie::get_all_words() {
    std::vector<std::pair<std::string, int>> all_words;
    display_all(root, "", all_words);
    return all_words;
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

// 保存Trie数据到CSV
void Trie::save_to_csv(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件进行保存: " << filename << std::endl;
        return;
    }

    std::vector<std::pair<std::string, int>> all_words = get_all_words();
    for (const auto& pair : all_words) {
        file << pair.first << "," << pair.second << "\n";
    }

    file.close();
    std::cout << "数据已保存到 " << filename << "。" << std::endl;
}
