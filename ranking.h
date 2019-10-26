// Copyright 2018 Samoilescu Sebastian
#ifndef RANKING_H_
#define RANKING_H_
#include <stdlib.h>
#include <iostream>
#include "./arrayList.h"
#include "./player.h"
// Implementare skipList adaptat pentru problema data
template <typename type>
class Ranking {
 private:
    int maxLevels;
    int levels;
    Player<type>* header;
    double proc;
    int length;

 public:
    Ranking(int, double);
    Ranking();
    Ranking(const Ranking& other);
    Ranking& operator=(const Ranking& other);
    void updateRanking(type*, type);
    void updateDifference();
    int randomLevel();
    void insertPl(type, int, int);
    void deletePl(type, int);
    // bool isInRanking(type);
    void printRanking(std::ofstream&);
    int getLength();
    Player<type>* getHeader();
    ~Ranking();
};
template <typename type>
Ranking<type>::Ranking(int maxLevels, double proc)
{
    levels = 0;
    length = 0;
    this->maxLevels = maxLevels;
    this->proc = proc;
    header = new Player<type>(-1, 0, 0, maxLevels);
}
template <typename type>
Ranking<type>::Ranking(const Ranking& other)
{
    this->maxLevels = other.maxLevels;
    this->levels = other.levels;
    this->proc = other.proc;
    this->length = length;
    header = new Player<type>(-1, 0, 0, maxLevels);
    Player<type>* current = other.header;
    while (current->forward[0] != NULL) {
        type points = current->forward[0]->points;
        int id = current->forward[0]->id;
        this->insertPl(points, id, 0);
        current = current->forward[0];
    }
}
template <typename type>
Ranking<type>& Ranking<type>::operator=(const Ranking& other)
{
    this->maxLevels = other.maxLevels;
    this->levels = other.levels;
    this->proc = other.proc;
    this->length = length;
    header = new Player<type>(-1, 0, 0, maxLevels);
    Player<type>* current = other.header;
    while (current->forward[0] != NULL) {
        type points = current->forward[0]->points;
        int id = current->forward[0]->id;
        this->insertPl(points, id, 0);
        current = current->forward[0];
    }
    return *this;
}
template <typename type>
int Ranking<type>::randomLevel()
{
    double random = (double)rand() / RAND_MAX;
    int rLevel = 0;
    while (rLevel < maxLevels && random <= proc) {
        rLevel++;
        random = (double)rand() / RAND_MAX;
    }
    return rLevel;
}
// inserare player. Se face cautarea in functie de numarul de puncte,
// iar apoi dupa id
template <typename type>
void Ranking<type>::insertPl(type points, int id, int diff)
{
    Player<type>* current = header;
    Player<type>* newEl[levels + 1];
    memset(newEl, 0, sizeof(Player<type>*) * (levels + 1));
    for (int i = levels; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->points
                                                                 > points) {
            current = current->forward[i];
        }
        while (current->forward[i] != NULL
            && current->forward[i]->points == points
            && current->forward[i]->id < id) {
            current = current->forward[i];
        }

        newEl[i] = current;
    }
    current = current->forward[0];
    int ranLvl = randomLevel();
    if (ranLvl > levels) {
        for (int i = levels + 1; i <= ranLvl; i++) {
            newEl[i] = header;
        }
        levels = ranLvl;
    }
    Player<type>* player = new Player<type>(points, id, diff, ranLvl);
    length++;
    for (int i = 0; i <= ranLvl; i++) {
        player->forward[i] = newEl[i]->forward[i];
        newEl[i]->forward[i] = player;
    }
}
// stergere player, se face cautarea dupa puncte,
// iar apoi se sterge jucatorul cu id specificat
template <typename type>
void Ranking<type>::deletePl(type points, int id)
{
    // retinem jucatorii parcursi de la stanga la dreapta pentru a reface
    // legaturile
    Player<type>* leftPlayers[levels + 1];
    Player<type>* player = header;
    for (int i = levels; i >= 0; i--) {
        while (player->forward[i] != NULL
            && player->forward[i]->points > points) {
            player = player->forward[i];
        }
        while (player->forward[i] != NULL
            && player->forward[i]->points == points
            && player->forward[i]->id < id) {
            player = player->forward[i];
        }
        leftPlayers[i] = player;
    }
    // cazul in care nu exista jucatorul specificat prin puncte si prin id
    if (leftPlayers[0]->forward[0] == NULL || leftPlayers[0]->forward[0]->id
                                                                       != id) {
        return;
    }
    // refacere legaturi
    int i = levels;
    while (leftPlayers[i]->forward[i] == NULL) {
        i--;
    }
    while (leftPlayers[i]->forward[i] != NULL
        && leftPlayers[i]->forward[i]->points != points
        && leftPlayers[i]->forward[i]->id != id) {
        i--;
    }
    Player<type>* curr = leftPlayers[0]->forward[0];
    while (i >= 0) {
        leftPlayers[i]->forward[i] = leftPlayers[i]->forward[i]->forward[i];
        i--;
    }
    // daca in urma stergerii jucatorului rezulta un nivel gol,
    // acesta va fi sters
    while (levels > 0 && header->forward[levels] == NULL) {
        levels--;
    }
    length--;
    delete curr;
}
// in timp ce afisam clasamentul retinem in "diff" pozitia lor in
// clasamentul actual
template <typename type>
void Ranking<type>::printRanking(std::ofstream& fout)
{
    Player<type>* current = header;
    int index = 1;
    while (current->forward[0] != NULL) {
        fout << current->forward[0]->id << " " << current->forward[0]->points
        <<" " << current->forward[0]->diff << "\n";
        current->forward[0]->diff = index++;
        current = current->forward[0];
    }
}

template <typename type>
Ranking<type>::~Ranking()
{
    Player<type>* player = header;
    Player<type>* aux;
    while (player->forward[0] != NULL) {
        aux = player;
        player = player->forward[0];
        delete aux;
    }
    delete player;
}
template <typename type>
int Ranking<type>::getLength()
{
    return length;
}
template <typename type>
void initializeRanking(Ranking<type>& ranking, int players)
{
    for (int i = 1; i <= players; i++) {
        ranking.insertPl(0, i, 0);
    }
}
template <typename type>
Player<type>* Ranking<type>::getHeader()
{
    return header;
}
// pe baza timpurilor obtinuti intr-o cursa, actualizam clasamentul
// parcurgem clasementul, iar pentru fiecare id de player intalnit
// obtinem prin intermediul variabilei "points" numarul de puncte
// cu care va progresa sau regresa jucatorul in clasament
template <typename type>
void Ranking<type>::updateRanking(type* points, type refPoints)
{
    Player<type>* currPlayer = getHeader()->forward[0];
    while (currPlayer != NULL) {
        int id = currPlayer->id;
        int index = currPlayer->diff;
        // jucatorul care ar trebui sa primeasca "refPoints"
        // este marcat ca deja vizitat
        if (points[id] == refPoints) {
            currPlayer = currPlayer->forward[0];
            continue;
        }
        int totalPoints = currPlayer->points;
        currPlayer = currPlayer->forward[0];
        deletePl(totalPoints, id);
        totalPoints += points[id];
        // marcam jucatorul ca deja vizitat
        points[id] = refPoints;
        insertPl(totalPoints, id, index);
    }
    // printRanking();
}
// de fiecare data cand printam clasamentul, verificam ce s-a intamplat
// de la ultimul print cu pozitiile jucatoriilor in cadrul clasamentului
template <typename type>
void Ranking<type>::updateDifference()
{
    Player<type>* currPlayer = getHeader()->forward[0];
    int index = 1;
    while (currPlayer != NULL) {
        currPlayer->diff -= index;
        currPlayer = currPlayer->forward[0];
        index++;
    }
}
#endif  // RANKING_H_
