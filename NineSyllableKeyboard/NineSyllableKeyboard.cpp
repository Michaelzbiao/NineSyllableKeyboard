// main.cpp
#include "Trie.h"
#include <iostream>
#include <algorithm>
#include <vector>

static const std::unordered_map<char, char> KEYMAP = {
    {'a', '2'}, {'b', '2'}, {'c', '2'},
    {'d', '3'}, {'e', '3'}, {'f', '3'},
    {'g', '4'}, {'h', '4'}, {'i', '4'},
    {'j', '5'}, {'k', '5'}, {'l', '5'},
    {'m', '6'}, {'n', '6'}, {'o', '6'},
    {'p', '7'}, {'q', '7'}, {'r', '7'}, {'s', '7'},
    {'t', '8'}, {'u', '8'}, {'v', '8'},
    {'w', '9'}, {'x', '9'}, {'y', '9'}, {'z', '9'}
};
// 添加单词的菜单
void add_word_menu(Trie& trie) {
    std::string word;
    std::string count_str;
    int count;
    std::cout << "请输入要添加的单词：";
    std::getline(std::cin, word);
    std::cout << "请输入该单词的输入次数：";
    std::getline(std::cin, count_str);
    try {
        count = std::stoi(count_str); // 将字符串转换为整数, 单词次数
        if (count <= 0) throw std::invalid_argument("非正数");
        trie.insert(word, count);
    }
    catch (std::invalid_argument&) {
        std::cout << "输入次数必须是正整数。" << std::endl;
    }
    catch (...) {
        std::cout << "输入次数必须是整数。" << std::endl;
    }
}

// 删除单词的菜单
void delete_word_menu(Trie& trie) {
    std::string word;
    std::cout << "请输入要删除的单词：";
    std::getline(std::cin, word);
    trie.delete_word(word);
}

// 修改单词输入次数的菜单
void modify_word_menu(Trie& trie) {
    std::string word;
    std::string count_str;
    int new_count;
    std::cout << "请输入要修改的单词：";
    std::getline(std::cin, word);
    std::cout << "请输入新的输入次数：";
    std::getline(std::cin, count_str);
    try {
        new_count = std::stoi(count_str);
        if (new_count <= 0) throw std::invalid_argument("非正数");
        trie.modify(word, new_count);
    }
    catch (std::invalid_argument&) {
        std::cout << "输入次数必须是正整数。" << std::endl;
    }
    catch (...) {
        std::cout << "输入次数必须是整数。" << std::endl;
    }
}

void query_words_menu(Trie& trie) {
    std::string digit_sequence;
    std::cout << "请输入九键数字序列（2-9）：";
    std::getline(std::cin, digit_sequence);

    // 去除可能的空格
    digit_sequence.erase(std::remove(digit_sequence.begin(), digit_sequence.end(), ' '), digit_sequence.end());

    if (digit_sequence.empty()) {
        std::cout << "输入不能为空。" << std::endl;
        return;
    }

    // 验证输入是否合法
    for (char c : digit_sequence) {
        if (c < '2' || c > '9') {
            std::cout << "无效的输入：请只使用2-9的数字。" << std::endl;
            return;
        }
    }

    // 使用新的progressive_search函数进行逐步查询
    trie.progressive_search(digit_sequence);
}

// 正则表达式查询菜单
void regex_search_menu(Trie& trie) {
    std::cout << "请输入正则表达式（支持 ? 和 * ）: ";
    std::string pattern;
    std::getline(std::cin, pattern);

    // 验证输入的合法性
    for (char& c : pattern) {
        if (c != '?' && c != '*' && KEYMAP.find(c) == KEYMAP.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符和通配符 ? 和 *） " << std::endl;
            return;
        }
    }

    // 获取匹配结果并去重
    // 对外暴露的正则匹配接口
    std::vector<std::pair<std::string, int>> results;
    trie.regex_dfs(trie.root, pattern, 0, "", results);
    if (results.empty()) {
        std::cout << "没有找到符合该正则表达式的单词。\n";
        return;
    }

    // 使用map进行去重，同时保留最高频率
    std::map<std::string, int> unique_results;
    for (const auto& r : results) {
        auto it = unique_results.find(r.first);
        if (it != unique_results.end()) {
            // 如果单词已存在，保留较高的频率
            it->second = std::max(it->second, r.second);
        }
        else {
            // 插入新单词
            unique_results[r.first] = r.second;
        }
    }

    // 将去重后的结果转换回vector并排序
    std::vector<std::pair<std::string, int>> sorted_results(
        unique_results.begin(), unique_results.end());

    // 按输入次数降序排序
    std::sort(sorted_results.begin(), sorted_results.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) {
                return a.second > b.second; // 首先按频率降序
            }
            return a.first < b.first; // 频率相同时按字典序升序
        });

    std::cout << "符合正则表达式 \"" << pattern << "\" 的单词如下:\n";
    for (const auto& r : sorted_results) {
        std::cout << r.first << " (输入次数: " << r.second << ")\n";
    }
}

// 查询单词输入次数的菜单
void get_count_menu(Trie& trie) {
    std::cout << "请输入要查询的单词: ";
    std::string word;
    std::getline(std::cin, word);
    for (char& c : word) {
        if (KEYMAP.find(c) == KEYMAP.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符） " << std::endl;
            return;
        }
    }
    int count = trie.get_count_of_word(word);
    if (count < 0) {
        std::cout << "单词 \"" << word << "\" 不存在于输入法中。\n";
    }
    else {
        std::cout << "单词 \"" << word << "\" 的输入次数为: " << count << "\n";
    }
}

// 查询指定前缀的所有单词的菜单
void get_prefix_words_menu(Trie& trie) {
    std::cout << "请输入前缀字符串: ";
    std::string prefix;
    std::getline(std::cin, prefix);
    for (char& c : prefix) {
        if (KEYMAP.find(c) == KEYMAP.end()) {
            std::cerr << "请重新输入合法字符串（仅包含英文字符） " << std::endl;
            return;
        }
    }
    auto results = trie.get_words_with_prefix(prefix);
    if (results.empty()) {
        std::cout << "没有找到前缀为 \"" << prefix << "\" 的单词。\n";
        return;
    }

    // 按输入次数降序排序
    std::sort(results.begin(), results.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return b.second < a.second;
        });

    std::cout << "以下是前缀 \"" << prefix << "\" 的所有单词及其输入次数:\n";
    for (const auto& p : results) {
        std::cout << p.first << " (次数: " << p.second << ")\n";
    }
}

// 查询出现频率最高的单词的菜单
// 查询出现频率最高的单词的菜单
void get_max_freq_menu(Trie& trie) {
    auto max_words = trie.get_max_frequency_words();
    if (max_words.empty()) {
        std::cout << "输入法中没有任何单词。\n";
    }
    else {
        std::cout << "出现频率最高的单词如下：" << std::endl;
        for (const auto& word : max_words) {
            std::cout << "\"" << word.first << "\": " << word.second << std::endl;
        }
    }
}

// 查询所有单词及输入次数的菜单
void get_all_words_menu(Trie& trie) {
    auto all_words = trie.get_all_words();
    if (all_words.empty()) {
        std::cout << "输入法中没有任何单词。\n";
        return;
    }

    // 按输入次数降序排序
    std::sort(all_words.begin(), all_words.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return b.second < a.second;
        });

    std::cout << "所有单词及其输入次数:\n";
    for (const auto& w : all_words) {
        std::cout << w.first << " (次数: " << w.second << ")\n";
    }
}

int main() {
    Trie trie;

    // 加载CSV数据
    std::string filename = "test.csv";
    trie.load_from_csv(filename);

    while (true) {
        std::cout << "\n九键输入系统菜单:" << std::endl;
        std::cout << "1. 添加单词" << std::endl;
        std::cout << "2. 删除单词" << std::endl;
        std::cout << "3. 修改单词输入次数" << std::endl;
        std::cout << "4. 九键查询单词" << std::endl;
        std::cout << "5. 正则表达式查询" << std::endl;
        std::cout << "6. 查询单词输入次数" << std::endl;
        std::cout << "7. 查询指定前缀的所有单词" << std::endl;
        std::cout << "8. 查询出现频率最高的单词" << std::endl;
        std::cout << "9. 查询所有单词及输入次数" << std::endl;
        std::cout << "0. 退出" << std::endl;
        std::cout << "请选择操作（0-10）：";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            // 添加单词
            add_word_menu(trie);
        }
        else if (choice == "2") {
            // 删除单词
            delete_word_menu(trie);
        }
        else if (choice == "3") {
            // 修改单词输入次数
            modify_word_menu(trie);
        }
        else if (choice == "4") {
            // 查询单词（逐步查询）
            query_words_menu(trie);
        }
        else if (choice == "5") {
            // 正则表达式查询
            regex_search_menu(trie);
        }
        else if (choice == "6") {
            // 查询单词输入次数
            get_count_menu(trie);
        }
        else if (choice == "7") {
            // 查询指定前缀的所有单词
            get_prefix_words_menu(trie);
        }
        else if (choice == "8") {
            // 查询出现频率最高的单词
            get_max_freq_menu(trie);
        }
        else if (choice == "9") {
            // 查询所有单词及输入次数
            get_all_words_menu(trie);
        }
        else if (choice == "0") {
            // 退出前保存数据
            trie.save_to_csv(filename);
            std::cout << "退出系统。" << std::endl;
            break;
        }
        else {
            std::cout << "无效的选择，请重新输入。" << std::endl;
        }
    }

    return 0;
}
