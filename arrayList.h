// Copyright 2018 Samoilescu Sebastian
#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_
#include <iostream>

template <typename type>
class ArrayList{
 private:
    type *elements;
    int capacity;
    int size;
 public:
    ArrayList();
    ~ArrayList();
    ArrayList(const ArrayList &other);
    ArrayList<type> &operator= (const ArrayList &other);
    void insertEl(type);
    void printList();
    int getSize();
    type getPoints();
    type getEl(int);
    void setEl(int, type);
};
template <typename type>
ArrayList<type>::~ArrayList() {
    delete []elements;
}
template <typename type>
ArrayList<type>::ArrayList() {
    capacity = 10;
    size = 0;
    elements = new type[capacity];
}
template <typename type>
ArrayList<type>::ArrayList(const ArrayList<type> &other) {
    this -> capacity = other.capacity;
    this -> size = other.size;
    this -> elements = new type[other.capacity];
    for (int i = 0; i < other.size; i++) {
       this -> elements[i] = other.elements[i];
    }
}
template <typename type>
ArrayList<type> &ArrayList<type>::operator=(const ArrayList<type> &other) {
    this -> capacity = other.capacity;
    this -> size = other.size;
    this -> elements = new type[other.capacity];
    for (int i = 0; i < other.size; i++) {
       this -> elements[i] = other.elements[i];
    }
    return *this;
}
template <typename type>
void ArrayList<type>::insertEl(type element) {
    if (size == capacity) {
        capacity *= 2;
        type *newArr = new type[capacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = elements[i];
        }
        delete elements;
        elements = newArr;
    }
    elements[size++] = element;
}
template <typename type>
void ArrayList<type>::printList() {
    for (int i = 0; i < size; i++) {
        std::cout << elements[i] << "   ";
    }
    std::cout << "\n";
}
template <typename type>
int ArrayList<type>::getSize() {
    return size;
}
template <typename type>
type ArrayList<type>::getEl(int index) {
    if (index >= size) {
        return -1;
    }
    return elements[index];
}
template <typename type>
void ArrayList<type>::setEl(int index, type element) {
    elements[index] = element;
}
#endif  // ARRAYLIST_H_
