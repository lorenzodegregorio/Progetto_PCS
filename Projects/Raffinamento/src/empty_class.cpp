#include "empty_class.hpp"

using namespace std;

namespace ProjectLibrary
{
/// 1. Importa points, segmenti e cells dai file, con le rispettive proprietà

void ImportPoints(string nomefile, std::vector<Point>& points)
{   ifstream file;
    string line;
    file.open(nomefile);

    getline(file,line); // skip etichette
    while (getline(file,line)){
        replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        int id; // ID
        unsigned int marker; // Marker
        double x, y;
        conv >>  id >> marker >> x >> y;
        points.push_back(Point(x,y));
    }
    file.close();
}

void ImportSegments(string nomefile, std::vector<Point>& points,std::vector<Segment>& edges)
{   ifstream file;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        int id; // ID
        unsigned int marker; // Marker
        int idOrigin;
        int idEnd;
        conv >>  id >> marker >> idOrigin >> idEnd;
        edges.push_back(Segment(idOrigin,idEnd,points));
    }
    file.close();
}

void ImportCells(string nomefile, std::vector<Point>& points, std::vector<Segment>& edges, std::vector<Cell>& cells)
{ifstream file;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        int id; // ID
        int idpunto1;
        int idpunto2;
        int idpunto3;
        int idlato1;
        int idlato2;
        int idlato3;
        conv >>  id >> idpunto1 >> idpunto2 >> idpunto3 >> idlato1 >> idlato2 >> idlato3;
        cells.push_back(Cell(idpunto1,idpunto2,idpunto3,idlato1,idlato2,idlato3,points));

        // costruisco adiacenze (se il lato 1,2,3 ha cella1 o cella2 = -1
        // metti l'id di cella in cella1 o cella2)
        if(edges[idlato1].cella1 == -1)
        {
            edges[idlato1].cella1 = cells.size()-1;
        }
        else
        {
            edges[idlato1].cella2 = cells.size()-1;
        }

        if(edges[idlato2].cella1 == -1)
        {
            edges[idlato2].cella1 = cells.size()-1;
        }
        else
        {
            edges[idlato2].cella2 = cells.size()-1;
        }

        if(edges[idlato3].cella1 == -1)
        {
            edges[idlato3].cella1 = cells.size()-1;
        }
        else
        {
            edges[idlato3].cella2 = cells.size()-1;
        }
    }
    file.close();
}

void DividiCella(int& cella,
                 int& lato,
                 vector<Cell>& triangles,
                 vector<Segment>& edges,
                 vector<Point>& points,
                 int& dim_vect_points,
                 int& dim_vect_edges,
                 int& dim_vect_cells){
    triangles[cella].flag = false; // Uccido cella
    // controllo se lato è stato già diviso (se no lo divido)
    if (edges[lato].p1_midpoint < 0 || edges[lato].midpoint_p2 < 0 || edges[lato].midpoint < 0){
        // creo il punto medio
        CreaPuntoMedio(edges[lato],points);
        edges[lato].midpoint = dim_vect_points;
        dim_vect_points = dim_vect_points + 1;

        // creo i lati nuovi
        edges.push_back(Segment(edges[lato].punto1,edges[lato].midpoint,points));
        edges.push_back(Segment(edges[lato].midpoint,edges[lato].punto2,points));
        edges[lato].p1_midpoint = dim_vect_edges;
        edges[lato].midpoint_p2 = dim_vect_edges+1;
        dim_vect_edges = dim_vect_edges+2;
    }

    int verticeopp = TrovaVerticeOpposto(triangles[cella],edges[lato]);

    // SE lato selezionato è IL LATO MAGGIORE DIVIDO CELLA IN 2
    if (lato==TrovaLatoMaggiore(triangles[cella], edges)){
        // creo lato_bisez da punto medio di lato al vertice opposto
        edges.push_back(Segment(edges[lato].midpoint, verticeopp, points));
        int lato_bisez = dim_vect_edges;
        dim_vect_edges = dim_vect_edges + 1;

        // Faccio puntare l1 al lato che congiunge punto1 a TrovaVerticeOpposto e idem con l2 e punto2
        int l1 = -1; // id di lato da punto1 di 'lato' a TrovaVerticeOpposto
        int l2 = -1; // id di lato da punto2 di 'lato' a TrovaVerticeOpposto

        for (int j = 0; j < 3; j++)
        {
            int edgeId = triangles[cella].edges[j];

            if (edgeId != lato)
            {
                Segment& currentEdge = edges[edgeId];

                if (currentEdge.punto1 == edges[lato].punto1 || currentEdge.punto2 == edges[lato].punto1)
                {
                    l1 = edgeId;
                }
                else
                {
                    l2 = edgeId;
                }
            }
        }

        // Creo prima cella
        triangles.push_back(Cell(edges[lato].punto1,verticeopp,edges[lato].midpoint,lato_bisez,edges[lato].p1_midpoint,l1,points));
        // riempio adiacenze della prima cella
        if(edges[edges[lato].p1_midpoint].cella1 == -1){
            edges[edges[lato].p1_midpoint].cella1 = dim_vect_cells;
        }
        else{
            edges[edges[lato].p1_midpoint].cella2 = dim_vect_cells;
        }
        if(edges[lato_bisez].cella1 == -1){
            edges[lato_bisez].cella1 = dim_vect_cells;
        }else{
            edges[lato_bisez].cella2 = dim_vect_cells;
        }
        if(edges[l1].cella1 == cella){
            edges[l1].cella1 = dim_vect_cells;
        }else{
            edges[l1].cella2 = dim_vect_cells;
        }
        dim_vect_cells = dim_vect_cells+1;

        // creo seconda cella
        triangles.push_back(Cell(edges[lato].punto2,verticeopp,edges[lato].midpoint,lato_bisez,edges[lato].midpoint_p2,l2,points));
        // riempio adiacenze della seconda cella
        if(edges[lato_bisez].cella1 == -1){
            edges[lato_bisez].cella1 = dim_vect_cells;
        }else{
            edges[lato_bisez].cella2 = dim_vect_cells;
        }
        if(edges[edges[lato].midpoint_p2].cella1 == -1){
            edges[edges[lato].midpoint_p2].cella1 = dim_vect_cells;
        }else{
            edges[edges[lato].midpoint_p2].cella2 = dim_vect_cells;
        }
        if(edges[l2].cella1 == cella){
            edges[l2].cella1 = dim_vect_cells;
        }else{
            edges[l2].cella2 = dim_vect_cells;
        }
        dim_vect_cells = dim_vect_cells+1;

    }

    // SE lato  NON è IL LATO MAGGIORE DIVIDO CELLA IN 3
    else{
        int latomax = TrovaLatoMaggiore(triangles[cella],edges);
        // controllo che latomax non sia stato diviso se no lo divido
        if (edges[latomax].p1_midpoint < 0 || edges[latomax].midpoint_p2 < 0 || edges[latomax].midpoint < 0){
            CreaPuntoMedio(edges[latomax],points);
            edges[latomax].midpoint = dim_vect_points;
            dim_vect_points = dim_vect_points+1;
            // creo 2 nuovi edges da latomax
            edges.push_back(Segment(edges[latomax].punto1,edges[latomax].midpoint,points));
            edges.push_back(Segment(edges[latomax].midpoint,edges[latomax].punto2,points));
            edges[latomax].p1_midpoint = dim_vect_edges;
            edges[latomax].midpoint_p2 = dim_vect_edges+1;
            dim_vect_edges = dim_vect_edges+2;
        }

        // creo taglio dal punto medio di lato al punto medio di latomax
        edges.push_back(Segment(edges[lato].midpoint,edges[latomax].midpoint,points));
        int taglio = dim_vect_edges;
        dim_vect_edges = dim_vect_edges+1;
        int PuntoComune = TrovaVerticeComune(edges[lato],edges[latomax]);
        // punto opposto a latomax
        int puntotaglio2 = TrovaVerticeOpposto(triangles[cella],edges[latomax]);
        // creo segmento da punto medio di latomax a al suo vertice opposto puntotaglio2
        edges.push_back(Segment (edges[latomax].midpoint,puntotaglio2,points));
        int taglio2 = dim_vect_edges;
        dim_vect_edges = dim_vect_edges+1;

        int latopuntocomune; // Lato generato da divisione di 'lato' dalla parte di PuntoComune
        int latopuntotaglio2; // Lato generato da divisione di 'lato' dalla parte di puntotaglio2
        // devo riempire questi id con i edges giusti:
        if (edges[edges[lato].p1_midpoint].punto1==PuntoComune || edges[edges[lato].p1_midpoint].punto2==PuntoComune){
            latopuntocomune = edges[lato].p1_midpoint;
            latopuntotaglio2 = edges[lato].midpoint_p2;
        }else{latopuntocomune = edges[lato].midpoint_p2;
            latopuntotaglio2 = edges[lato].p1_midpoint;}

        int latolato_maxpuntocomune; // Lato generato da divisione di 'latomax' dalla parte di PuntoComune
        int latolato_maxverticeopp; // Lato generato da divisione di 'latomax' dalla parte di verticeopp
        // devo riempire questi id con i edges giusti:
        if (edges[edges[latomax].p1_midpoint].punto1==PuntoComune || edges[edges[latomax].p1_midpoint].punto2==PuntoComune){
            latolato_maxpuntocomune = edges[latomax].p1_midpoint;
            latolato_maxverticeopp = edges[latomax].midpoint_p2;
        }else{latolato_maxpuntocomune = edges[latomax].midpoint_p2;
            latolato_maxverticeopp = edges[latomax].p1_midpoint;}
        int terzolato; // Individua il lato di 'cella' che non è 'latomax' nè 'lato'
        if (triangles[cella].edges[0]!=lato && triangles[cella].edges[0]!=latomax){
            terzolato=triangles[cella].edges[0];
        }else{if (triangles[cella].edges[1]!=lato && triangles[cella].edges[1]!=latomax){
                terzolato=triangles[cella].edges[1];
            }else{terzolato=triangles[cella].edges[2];}
        }

        // Creo le tre nuove cells generate dalla separazione di quella più grande
        triangles.push_back(Cell(edges[lato].midpoint,edges[latomax].midpoint,PuntoComune,latopuntocomune,latolato_maxpuntocomune,taglio,points));
        // riempio adiacenze di tutti i nuovi edges
        if(edges[latopuntocomune].cella1 == -1){
            edges[latopuntocomune].cella1 = dim_vect_cells;
        }else{
            edges[latopuntocomune].cella2 = dim_vect_cells;
        }
        if(edges[latolato_maxpuntocomune].cella1 == -1){
            edges[latolato_maxpuntocomune].cella1 = dim_vect_cells;
        }else{
            edges[latolato_maxpuntocomune].cella2 = dim_vect_cells;
        }
        if(edges[taglio].cella1 == -1){
            edges[taglio].cella1 = dim_vect_cells;
        }else{
            edges[taglio].cella2 = dim_vect_cells;
        }
        dim_vect_cells = dim_vect_cells+1;

        triangles.push_back(Cell(edges[lato].midpoint,edges[latomax].midpoint,puntotaglio2,latopuntotaglio2,taglio,taglio2,points));
        // riempio adiacenze di tutti i nuovi edges
        if(edges[latopuntotaglio2].cella1 == -1){
            edges[latopuntotaglio2].cella1 = dim_vect_cells;
        }else{
            edges[latopuntotaglio2].cella2 = dim_vect_cells;
        }
        if(edges[taglio].cella1 == -1){
            edges[taglio].cella1 = dim_vect_cells;
        }else{
            edges[taglio].cella2 = dim_vect_cells;
        }
        if(edges[taglio2].cella1 == -1){
            edges[taglio2].cella1 = dim_vect_cells;
        }else{
            edges[taglio2].cella2 = dim_vect_cells;
        }
        dim_vect_cells = dim_vect_cells+1;

        triangles.push_back(Cell(puntotaglio2,verticeopp,edges[latomax].midpoint,terzolato,latolato_maxverticeopp,taglio2,points));
        // riempio adiacenze di tutti i nuovi edges
        if(edges[terzolato].cella1 == cella){
            edges[terzolato].cella1 = dim_vect_cells;
        }else{
            edges[terzolato].cella2 = dim_vect_cells;
        }
        if(edges[latolato_maxverticeopp].cella1 == -1){
            edges[latolato_maxverticeopp].cella1 = dim_vect_cells;
        }else{
            edges[latolato_maxverticeopp].cella2 = dim_vect_cells;
        }
        if(edges[taglio2].cella1 == -1){
            edges[taglio2].cella1 = dim_vect_cells;
        }else{
            edges[taglio2].cella2 = dim_vect_cells;
        }
        dim_vect_cells = dim_vect_cells+1;
    }
}

/// 3. Inizializzo lato maggiore con funzione TrovaLatoMaggiore,
/// inizializzo la cella successiva e gli assegno il valore corretto

void RaffinaCella(int& cella,
                  int& lato,
                  vector<Cell>& triangles,
                  vector<Segment>& edges,
                  vector<Point>& points,
                  int& dim_vect_points,
                  int& dim_vect_edges,
                  int& dim_vect_cells){
    int latomax = TrovaLatoMaggiore(triangles[cella],edges);
    int CellaDopo;
    if (cella!=edges[latomax].cella1) {CellaDopo = edges[latomax].cella1;}
    else {CellaDopo = edges[latomax].cella2;}
    DividiCella(cella,lato,triangles,edges,points,dim_vect_points,dim_vect_edges,dim_vect_cells);
    if (CellaDopo > -1){
        if (latomax==(TrovaLatoMaggiore(triangles[CellaDopo],edges))){
            DividiCella(CellaDopo,latomax,triangles,edges,points,dim_vect_points,dim_vect_edges,dim_vect_cells);
        }
        else{
            RaffinaCella(CellaDopo,latomax,triangles,edges,points,dim_vect_points,dim_vect_edges,dim_vect_cells);
        }}
}

/// 4. 	Inizializzo contatore per la size dei triangles (cells)

int TrovaIdCellaAreaMax(vector<Cell>& triangles){
    unsigned int contatore = triangles.size();
    int max = -1;
    double areamax = 0;
    for(unsigned int i = 0;i<contatore;i++){
        if (triangles[i].flag && triangles[i].area > areamax )
        {
            // se flag booleano=true e se l’area del triangolo è maggiore dell’attuale area massima --->
            // ---> allora:
            max = i;  // assegno l'indice all'indice di area massima
            areamax = triangles[i].area;  // l'area del triangolo all'area massima
        }
    }
    return max;
}
// Fine File
}
