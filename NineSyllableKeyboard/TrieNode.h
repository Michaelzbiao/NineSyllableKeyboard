// TrieNode.h
#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>

// 字典树节点定义
struct TrieNode {
    std::unordered_map<char, TrieNode*> children; // 子节点
    bool is_end;                                  // 是否为单词结尾
    int count;                                    // 输入次数

    TrieNode() : is_end(false), count(0) {}
};

#endif // TRIENODE_H
