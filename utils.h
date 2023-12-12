#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

template <typename T>
struct Node {
    Node(T value) : value(value), next(nullptr), prev(nullptr) {}
    T value;
    Node* next;
    Node* prev;
};

template <typename T>
class DoubleLinkedList {
public:
    DoubleLinkedList() : head(nullptr), tail(nullptr) {}
    void push_back(T value);
    void push_front(T value);
    void pop_back();
    void pop_front();
    void print();
    ~DoubleLinkedList();
    Node<T>* getHead() { return head; }
private:
    Node<T>* head;
    Node<T>* tail;
};

std::string static ltrim(const std::string& s);
std::string static rtrim(const std::string& s);
std::string static trim(const std::string& s);
std::vector<std::string> static split(const std::string &str, char delim);


#endif