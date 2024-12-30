// main.cpp
#include "Trie.h"
#include <iostream>
#include <algorithm>
#include <vector>

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
        count = std::stoi(count_str);
        trie.insert(word, count);
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
        trie.modify(word, new_count);
    }
    catch (...) {
        std::cout << "输入次数必须是整数。" << std::endl;
    }
}

// 查询单词的菜单
void query_words_menu(Trie& trie) {
    std::string digit_sequence;
    std::cout << "请输入九键数字序列（不包括1）：";
    std::getline(std::cin, digit_sequence);
    // 去除可能的空格
    digit_sequence.erase(std::remove(digit_sequence.begin(), digit_sequence.end(), ' '), digit_sequence.end());
    std::vector<std::pair<std::string, int>> results = trie.query(digit_sequence);
    if (!results.empty()) {
        // 按输入次数降序排序
        std::sort(results.begin(), results.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return b.second < a.second;
            });
        std::cout << "查询结果：" << std::endl;
        for (const auto& pair : results) {
            std::cout << pair.first << " (输入次数: " << pair.second << ")" << std::endl;
        }
    }
    else {
        std::cout << "没有找到匹配的单词。" << std::endl;
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
        std::cout << "4. 查询单词" << std::endl;
        std::cout << "5. 显示所有单词（调试用）" << std::endl;
        std::cout << "0. 退出" << std::endl;
        std::cout << "请选择操作（0-5）：";

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
            // 查询单词
            query_words_menu(trie);
        }
        else if (choice == "5") {
            // 显示所有单词
            trie.display();
        }
        else if (choice == "0") {
            // 退出
            std::cout << "退出系统。" << std::endl;
            break;
        }
        else {
            std::cout << "无效的选择，请重新输入。" << std::endl;
        }
    }

    return 0;
}
