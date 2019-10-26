// Copyright 2018 Samoilescu Sebastian
#ifndef PLAYER_H_
#define PLAYER_H_
#include<string.h>
#include <iostream>
template <typename type>
class Player{
 public:
    Player(type, int, int, int);
    ~Player();
    type points;
    int id, diff;
    int height;
    Player<type> **forward;
};
template <typename type>
Player<type>::Player(type points, int id, int diff, int level) {
    this -> points = points;
    this -> id = id;
    this -> diff = diff;
    this -> height = level;
    forward = new Player*[level + 1];
    memset(forward, 0, sizeof(Player<type>*)*(level+1));
}
template <typename type>
Player<type>::~Player() {
    delete []forward;
}
#endif  // PLAYER_H_
