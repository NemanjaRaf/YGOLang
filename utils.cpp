#include "utils.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

template <typename T>
void DoubleLinkedList<T>::push_back(T value) {
    Node<T>* node = new Node<T>(value);
    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
}

template <typename T>
void DoubleLinkedList<T>::push_front(T value) {
    Node<T>* node = new Node<T>(value);
    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        head->prev = node;
        node->next = head;
        head = node;
    }
}

template <typename T>
void DoubleLinkedList<T>::pop_back() {
    if (tail == nullptr) {
        return;
    }
    Node<T>* node = tail;
    tail = tail->prev;
    if (tail != nullptr) {
        tail->next = nullptr;
    }
    delete node;
    node = nullptr; // Set node to nullptr after deleting
}

template <typename T>
void DoubleLinkedList<T>::pop_front() {
    if (head == nullptr) {
        return;
    }
    Node<T>* node = head;
    head = head->next;
    if (head != nullptr) {
        head->prev = nullptr;
    }
    delete node;
    node = nullptr; // Set node to nullptr after deleting
}

template <typename T>
void DoubleLinkedList<T>::print() {
    Node<T>* node = head;
    std::cout << "Node: " << node << std::endl;
    std::cout << "Node 2: " << node->next << std::endl;
    while (node != nullptr) {
        
        if (node->value != nullptr) {
            std::cout << *(node->value) << std::endl;
        } else {
            std::cout << "Null token" << std::endl;
        }
        node = node->next;
    }
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList() {
    Node<T>* node = head;
    while (node != nullptr) {
        Node<T>* next = node->next;
        delete node;
        node = next;
    }
}

std::string static ltrim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string static rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string static trim(const std::string& s) {
    return rtrim(ltrim(s));
}

std::vector<std::string> static split(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}