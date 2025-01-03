// TrieNode.h
#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>

// 字典树节点定义
struct TrieNode {
    bool is_end;
    int prefix_count; // 记录经过该节点的前缀次数
    int word_count;   // 记录以该节点为末尾的单词的输入次数
    std::unordered_map<char, TrieNode*> children;

    TrieNode() : is_end(false), prefix_count(0), word_count(0) {}
};

#endif // TRIENODE_H
