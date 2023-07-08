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
/// 1. Dichiarazione delle classi Point, Segment e Cell

// 1a. La classe Point ha per attributi le coordinate x e y
class Point{
public:
    double x;
    double y;

    Point(double& x_,
          double& y_){x=x_;y=y_;}
};

// 1c. La classe Segment ha per attributi l'ID dei points estremi, la lunghezza,
// il punto medio e le due cells adiacenti
class Segment{
public:
    int punto1;
    int punto2;
    double lunghezza;
    int p1_midpoint = -1; // Puntatore al segmento da p1 a punto medio
    int midpoint_p2 = -1; // Puntatore al segmento da punto medio a p2
    int midpoint = -1; // Puntatore al punto medio
    int cella1 = -1; // Prima Cella Adiacente
    int cella2 = -1; // Seconda Cella Adiacente

    // Calcolo della lunghezza
    Segment(int& punto1_,
            int& punto2_,
            std::vector<Point>& univpoints){ punto1=punto1_;
        punto2=punto2_;
        lunghezza =sqrt((abs(univpoints[punto1_].x-univpoints[punto2_].x))*(abs(univpoints[punto1_].x-univpoints[punto2_].x))+(abs(univpoints[punto1_].y-univpoints[punto2_].y))*(abs(univpoints[punto1_].y-univpoints[punto2_].y)));}

    friend bool operator>(const Segment& lato1, const Segment& lato2) {
        return (lato1.lunghezza - lato2.lunghezza > 0.0);
    }
};

// 1c. La classe Cell ha per attributi i suoi points, i suoi edges, l'area e
// un booleano per distruggere la cella
class Cell{
public:
    vector<int> points; // essendo vettori potrebbero essere un problema
    vector<int> edges;
    double area;
    bool flag; // distrugge cells


    /// 2. chiamo la funzione Cell:
        /// 2a. aggiungo i tre points al vettore points e i tre edges al vettore edges,
        /// 2b. pongo l’indicatore booleano = vero e calcolo l’area del triangolo

    Cell(int& punto1,int& punto2,int& punto3,int& lato1,int& lato2,int& lato3,
         std::vector<Point>& upoints){
        points.push_back(punto1);
        points.push_back(punto2);
        points.push_back(punto3);
        edges.push_back(lato1);
        edges.push_back(lato2);
        edges.push_back(lato3);
        flag = true;
        area= 0.5*abs(upoints[punto1].x*(upoints[punto2].y-upoints[punto3].y) + upoints[punto2].x*(upoints[punto3].y-upoints[punto1].y) + upoints[punto3].x*(upoints[punto1].y-upoints[punto2].y));
    }
};



/// 3. METODI: operazioni sul lato più lòungo della cella

/// 3a. Trova lato più lungo (confronta i 3 edges con if/else)

inline int LatoMaggiore(Cell& cella, std::vector<Segment>& univedges) {
    if (univedges[cella.edges[0]] > univedges[cella.edges[1]] && univedges[cella.edges[0]] > univedges[cella.edges[2]]) {
        int lm = cella.edges[0];
        return lm;
    }
    else if (univedges[cella.edges[1]] > univedges[cella.edges[2]]) {
        int lm = cella.edges[1];
        return lm;
    }
    else {
        int lm = cella.edges[2];
        return lm;
    }
}

/// 3b. Crea punto medio del lato massimo trovato in (3a.)

inline void PuntoMedio(Segment& lato,std::vector<Point>& points){
    double x_med=(points[lato.punto1].x+points[lato.punto2].x)*0.5;
    double y_med=(points[lato.punto1].y+points[lato.punto2].y)*0.5;
    // crea nuovo punto
    points.push_back(Point(x_med,y_med));
    ;}

/// 3c. Trova vertice opposto al lato massimo trovato in (3a.)

inline int VerticeOpposto(Cell& cella,Segment& lato){
    int i = 0;
    int punto = cella.points[i];
    while(punto==lato.punto1 || punto==lato.punto2){
        i = i +1;
        punto = cella.points[i];
    }
    return punto;
}

/// 3d. Trova puntatore a vertice comune tra due edges, dando per scontato che ci sia

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


/// 4. Importo dai file .csv i vettori di points, segmenti e cells

// points
void ImportPoints(string nomefile, std::vector<Point>& points);
// segmenti
void ImportSegments(string nomefile, std::vector<Point>& points, std::vector<Segment>& edges);
// cells
void ImportCells(string nomefile,std::vector<Point>& points, std::vector<Segment>& edges, std::vector<Cell>& cells);


/// 5. Funzioni sulla CELLA, dichiarate in "empty_class.cpp"

// 5a. Divide cella dimezzando lato, se poi lato non è il lato maggiore
// divide ancora creando un lato tra il punto medio di lato e del lato maggiore
void DividiCella(int& cella,
                 int& lato,
                 std::vector<Cell> &triangles,
                 std::vector<Segment>& edges,
                 std::vector<Point> &points,
                 int& dim_vect_points,
                 int& dim_vect_edges,
                 int& dim_vect_cells);

// 5b. data una cella e un lato, divide questa e poi quella adiacente al lato
// dimezzato, a cascata fino a che non raggiunge il bordo della mesh
// oppure la divisione della cella non crea inammissibilità nella mesh
void RaffinaCella(int& cella,
                  int& lato,
                  std::vector<Cell>& triangles,
                  std::vector<Segment>& edges,
                  std::vector<Point>& points,
                  int& dim_vect_points,
                  int& dim_vect_edges,
                  int& dim_vect_cells);

int TrovaIdCellaAreaMax(vector<Cell>& triangles);
}
#endif // __EMPTY_H
