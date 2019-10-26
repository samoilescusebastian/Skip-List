// Copyright 2018 Samoilescu Sebastian
#include <iostream>
#include <fstream>
#include "./arrayList.h"
#include "./ranking.h"
int main()
{
    // clasamentul jucatoriilor
    FILE* file_in = fopen("races.in", "r");
    std::ofstream fout;
    fout.open("races.out");
    if (file_in == NULL) {
        return -1;
    }
    int playersN, racesN, printN;
    fscanf(file_in, "%d", &playersN);
    fscanf(file_in, "%d", &racesN);
    fscanf(file_in, "%d", &printN);
    Ranking<int> ranking(3, 0.5);
    int time, readN = 1, index, printC = 0;
    char character;
    initializeRanking(ranking, playersN);
    // citim fiecare linie de timpi sau de print
    while (readN <= racesN + printN) {
        readN++;
        index = 1;
        // clasament al timpiilor
        Ranking<int>* times = new Ranking<int>(3, 0.5);
        fseek(file_in, 2, SEEK_CUR);
        // citim primul caracter de pe linie pentru a diferentia timpii de print
        fscanf(file_in, "%c", &character);
        if (character == 'p') {
            fseek(file_in, 3, SEEK_CUR);
            // daca programul e la primul print, acesta nu calculeaza
            // diferenta din clasament, in rest aceasta este calculata
            if (printC > 0) {
                ranking.updateDifference();
            }
            ranking.printRanking(fout);
            printC++;
            if (printC != printN) {
                fout << "\n";
            }
            delete times;
            continue;
        } else {
            fseek(file_in, -1, SEEK_CUR);
        }
        // citim toti timpii asociati jucatoriilor prin id - ul,index
        while (index <= playersN && fscanf(file_in, "%d", &time) != EOF) {
            // se adauga doar jucatorii care participa la cursa
            if (time != 0) {
                times->insertPl(time, index, 0);
            }
            index++;
        }
        // parcurgem list de timpi pentru a calcula cate puncte trebuie
        // sa primeasca fiecare jucator
        Player<int>* player = (times->getHeader())->forward[0];
        int length = times->getLength();
        int currPoints = -length / 2;
        bool isOdd = length % 2 == 0;
        int* points = new int[playersN + 1];
        for (int i = 0; i <= playersN; i++) {
            points[i] = 0;
        }
        while (player != NULL) {
            // folosim vectorul "ids" pentru a adauga id-urile jucatorilor ce
            // au acelasi timp intr-o anumita cursa
            ArrayList<int>* ids = new ArrayList<int>;
            int id = player->id;
            points[id] = currPoints;
            // realizam adaugarea
            while (player->forward[0] != NULL
                && player->points == player->forward[0]->points) {
                ids->insertEl(player->id);
                player = player->forward[0];
            }
            ids->insertEl(player->id);
            // parcurgem clasamentul pentru a sorta indexii din "ids"
            // in functie de locul in care acestia apar in clasament
            Player<int>* current = (ranking.getHeader())->forward[0];
            int lastPos = ids->getSize() - 1;
            while (current != 0 && lastPos > 0) {
                int id = current->id;
                for (int i = 0; i <= lastPos; i++) {
                    if (ids->getEl(i) == id) {
                        int aux = id;
                        ids->setEl(i, ids->getEl(lastPos));
                        ids->setEl(lastPos, aux);
                        lastPos--;
                    }
                }
                current = current->forward[0];
            }
            // in functie de ordinea indexiilor, atribuim acestora punctele
            // obtinute in urma cursei
            for (int i = 0; i < ids->getSize(); i++) {
                if (isOdd && currPoints == 0) {
                    currPoints++;
                }
                points[ids->getEl(i)] = currPoints;
                currPoints++;
            }
            player = player->forward[0];
            delete ids;
        }
        // se face actualizarea clasamentului in functie de numarul de puncte
        // calculat anterior
        ranking.updateRanking(points, length / 2 + 1);
        delete times;
        delete[] points;
    }
    fclose(file_in);
    fout.close();
    return 0;
}
