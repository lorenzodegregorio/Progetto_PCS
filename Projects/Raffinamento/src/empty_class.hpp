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

    Point(double& asc, double& ord )
    {
        x=asc;
        y=ord;
    }
};

// 1c. La classe Segment ha per attributi l'ID dei punti estremi, la lunghezza,
// il punto medio e le due celle adiacenti
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
            std::vector<Point>& points_vect)
    {
        punto1=punto1_;
        punto2=punto2_;
        lunghezza =sqrt((abs(points_vect[punto1_].x-points_vect[punto2_].x))*(abs(points_vect[punto1_].x-points_vect[punto2_].x))+(abs(points_vect[punto1_].y-points_vect[punto2_].y))*(abs(points_vect[punto1_].y-points_vect[punto2_].y)));
    }

    friend bool operator>(const Segment& lato1, const Segment& lato2) {
        return (lato1.lunghezza > lato2.lunghezza);
        // funzione booleana che restitisce TRUE se lato1 > lato2
    }
};

// 1c. La classe Cell ha per attributi i suoi punti, i suoi lati, l'area e
// un booleano per distruggere la cella
class Cell{
public:
    vector<int> points; // essendo vettori potrebbero essere un problema
    vector<int> edges;
    double area;
    bool flag; // distrugge cella


    /// 2. chiamo la funzione Cell:
        /// 2a. aggiungo i tre punti al vettore points e i tre lati al vettore edges,
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



/// 3. METODI: operazioni sul lato più lungo della cella

/// 3a. Trova lato più lungo (confronta i 3 edges con if/else)

inline int TrovaLatoMaggiore(Cell& cella, std::vector<Segment>& univedges) {

    int lato_max = 0;

    if (univedges[cella.edges[0]] > univedges[cella.edges[1]] && univedges[cella.edges[0]] > univedges[cella.edges[2]]) {
        lato_max = cella.edges[0];
    }

    else if (univedges[cella.edges[1]] > univedges[cella.edges[2]]) {
        lato_max = cella.edges[1];
    }

    else {
        lato_max = cella.edges[2];
    }

    return lato_max;
}

/// 3b. Crea punto medio del lato massimo trovato in (3a.)

inline void CreaPuntoMedio(Segment& lato,std::vector<Point>& points)
{
    // crea nuovo punto
    double asc_mid=(points[lato.punto1].x+points[lato.punto2].x)*0.5;
    double ord_mid=(points[lato.punto1].y+points[lato.punto2].y)*0.5;
    // aggiungi nuovo punto al vettore points
    points.push_back(Point(asc_mid,ord_mid));

}

/// 3c. Trova vertice opposto al lato massimo trovato in (3a.)

inline int TrovaVerticeOpposto(const Cell& cella, const Segment& lato) {
    for (int i = 0; i < 3; i++)
    {
        int punto = cella.points[i];
        if (punto != lato.punto1 && punto != lato.punto2)
        {
            return punto;
        }
    }
    return -1; // Se non trova il vertice
}

/// 3d. Trova puntatore a vertice comune tra due lati, dando per scontato che esista

inline int TrovaVerticeComune(Segment& lato1,Segment& lato2){
    int punto;
    if (lato1.punto1==lato2.punto1 || lato1.punto1==lato2.punto2){
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
