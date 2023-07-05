#ifndef __EMPTY_H
#define __EMPTY_H
#include <Eigen/Eigen>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;

namespace ProjectLibrary
{
class Point{
public:
    double x; // ascissa
    double y; // ordinata

    Point(double& x_,
          double& y_){x=x_;y=y_;}
};

class Segment{
public:
    int punto1;
    int punto2;
    double lunghezza;
    int p1m = -1; // Puntatore al segmento da p1 a pmedio
    int mp2 = -1; // Puntatore al segmento da pmedio a p2
    int pm = -1; // Puntatore al punto medio
    int cella1 = -1; // Prima Cella Adiacente
    int cella2 = -1; // Seconda Cella Adiacente

    Segment(int& punto1_,
            int& punto2_,
            std::vector<Point>& univpunti){ punto1=punto1_;
        punto2=punto2_;
        lunghezza =sqrt((abs(univpunti[punto1_].x-univpunti[punto2_].x))*(abs(univpunti[punto1_].x-univpunti[punto2_].x))+(abs(univpunti[punto1_].y-univpunti[punto2_].y))*(abs(univpunti[punto1_].y-univpunti[punto2_].y)));}

    friend bool operator>(const Segment& lato1, const Segment& lato2) {
        return (lato1.lunghezza - lato2.lunghezza > 0.0);
    }
};

class Cell{
public:
    vector<int> punti; // essendo vettori potrebbero essere un problema
    vector<int> lati;
    double area;
    bool flag; // Ausiliario per distruggere celle

    Cell(int& punto1,int& punto2,int& punto3,int& lato1,int& lato2,int& lato3,
         std::vector<Point>& upunti){
        punti.push_back(punto1);
        punti.push_back(punto2);
        punti.push_back(punto3);
        lati.push_back(lato1);
        lati.push_back(lato2);
        lati.push_back(lato3);
        flag = true;
        area= 0.5*abs(upunti[punto1].x*(upunti[punto2].y-upunti[punto3].y) + upunti[punto2].x*(upunti[punto3].y-upunti[punto1].y) + upunti[punto3].x*(upunti[punto1].y-upunti[punto2].y));
    }
};

// Metodo: Trova lato più lungo
inline int LatoMaggiore(Cell& cella, std::vector<Segment>& univlati) {
    if (univlati[cella.lati[0]] > univlati[cella.lati[1]] && univlati[cella.lati[0]] > univlati[cella.lati[2]]) {
        int lm = cella.lati[0];
        return lm;
    }
    else if (univlati[cella.lati[1]] > univlati[cella.lati[2]]) {
        int lm = cella.lati[1];
        return lm;
    }
    else {
        int lm = cella.lati[2];
        return lm;
    }
}

inline void PuntoMedio(Segment& lato,std::vector<Point>& punti){
    double x_med=(punti[lato.punto1].x+punti[lato.punto2].x)*0.5;
    double y_med=(punti[lato.punto1].y+punti[lato.punto2].y)*0.5;
    // crea nuovo punto
    punti.push_back(Point(x_med,y_med));
    ;}

// Metodo: Dato lato trovare vertice opposto
inline int VerticeOpposto(Cell& cella,Segment& lato){
    int i = 0;
    int punto = cella.punti[i];
    while(punto==lato.punto1 || punto==lato.punto2){
        i = i +1;
        punto = cella.punti[i];
    }
    return punto;
}

// Trova puntatore a vertice comune tra due lati, dando per scontato che ci sia
inline int VerticeComune(Segment& lato1,Segment& lato2){
    int punto;
    if (lato1.punto1==lato2.punto1){
        punto=lato1.punto1;
    }
    else if (lato1.punto1==lato2.punto2){
        punto=lato1.punto1;
    }
    else{
        punto=lato1.punto2;
    }
    return punto;
}

// Importa punti da file CSV in vettore di punti
void ImportPoints(string nomefile, std::vector<Point>& punti);

// Importa segmenti da file CSV in vettore di segmenti
void ImportSegments(string nomefile, std::vector<Point>& punti, std::vector<Segment>& lati);

// Importa celle da file CSV in vettore di celle
void ImportCells(string nomefile,std::vector<Point>& punti, std::vector<Segment>& lati, std::vector<Cell>& celle);

// Divide cella dimezzando lato, se poi lato non è il lato maggiore
// divide ancora creando un lato tra il punto medio di lato e del lato maggiore
void DividiCella(int& cella,
                 int& lato,
                 std::vector<Cell> &triangoli,
                 std::vector<Segment>& lati,
                 std::vector<Point> &punti,
                 int& maxIDP,
                 int& maxIDS,
                 int& maxIDC);

// data una cella e un lato, divide questa e poi quella adiacente al lato
// dimezzato, a cascata fino a che non raggiunge il bordo della mesh
// oppure la divisione della cella non crea inammissibilità nella mesh
void RaffinaCella(int& cella,
                  int& lato,
                  std::vector<Cell>& triangoli,
                  std::vector<Segment>& lati,
                  std::vector<Point>& punti,
                  int& maxIDP,
                  int& maxIDS,
                  int& maxIDC);

int MaxCelle(vector<Cell>& triangoli);
}
#endif // __EMPTY_H
