#include "empty_class.hpp"

namespace ProjectLibrary
{
void ImportPoints(string nomefile, std::vector<Point>& punti)
{   ifstream file;
    string line;
    file.open(nomefile);
    // skip titolo
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        std::istringstream conv(line);
        int id; // ID
        unsigned int marker; // Marker
        double x;
        double y;
        conv >>  id >> marker >> x >> y;
        punti.push_back(Point(x,y));
    }
    file.close();
}

void ImportSegments(string nomefile, std::vector<Point>& punti,std::vector<Segment>& lati)
{   ifstream file;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        std::istringstream conv(line);
        int id; // ID
        unsigned int marker; // Marker
        int idOrigin;
        int idEnd;
        conv >>  id >> marker >> idOrigin >> idEnd;
        lati.push_back(Segment(idOrigin,idEnd,punti));
    }
    file.close();
}

void ImportCells(string nomefile, std::vector<Point>& punti, std::vector<Segment>& lati, std::vector<Cell>& celle)
{ifstream file;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        std::istringstream conv(line);
        int id; // ID
        int idpunto1;
        int idpunto2;
        int idpunto3;
        int idlato1;
        int idlato2;
        int idlato3;
        conv >>  id >> idpunto1 >> idpunto2 >> idpunto3 >> idlato1 >> idlato2 >> idlato3;
        celle.push_back(Cell(idpunto1,idpunto2,idpunto3,idlato1,idlato2,idlato3,punti));

        // costruisco adiacenze (se il lato 1,2,3 ha cella1 o cella2 = -1
        // metti l'id di cella in cella1 o cella2)
        if(lati[idlato1].cella1 == -1){lati[idlato1].cella1 = celle.size()-1;}
        else{lati[idlato1].cella2 = celle.size()-1;}
        if(lati[idlato2].cella1 == -1){lati[idlato2].cella1 = celle.size()-1;}
        else{lati[idlato2].cella2 = celle.size()-1;}
        if(lati[idlato3].cella1 == -1){lati[idlato3].cella1 = celle.size()-1;}
        else{lati[idlato3].cella2 = celle.size()-1;}
    }
    file.close();
}

void DividiCella(int& cella,
                 int& lato,
                 std::vector<Cell>& triangoli,
                 std::vector<Segment>& lati,
                 std::vector<Point>& punti,
                 int& maxIDP,
                 int& maxIDS,
                 int& maxIDC){
    triangoli[cella].flag = false; // Uccido cella
    // controllo se lato è stato già diviso (se no lo divido)
    if (lati[lato].p1m < 0|| lati[lato].mp2 < 0 || lati[lato].pm < 0){
        // creo il punto medio
        PuntoMedio(lati[lato],punti);
        lati[lato].pm = maxIDP;
        maxIDP = maxIDP + 1;

        // creo i lati nuovi
        lati.push_back(Segment(lati[lato].punto1,lati[lato].pm,punti));
        lati.push_back(Segment(lati[lato].pm,lati[lato].punto2,punti));
        lati[lato].p1m = maxIDS;
        lati[lato].mp2 = maxIDS+1;
        maxIDS = maxIDS+2;
    }

    int verticeopp = VerticeOpposto(triangoli[cella],lati[lato]);

    // SE lato è IL LATO MAGGIORE DIVIDO CELLA IN 2
    if (lato==LatoMaggiore(triangoli[cella],lati)){
        // creo lato_bisez da punto medio di lato al vertice opposto
        lati.push_back(Segment(lati[lato].pm,verticeopp,punti));
        int lato_bisez = maxIDS;
        maxIDS = maxIDS + 1;
        // Faccio puntare l1 al lato che congiunge punto1 a VerticeOpposto e idem con l2 e punto2
        int l1; // id di lato da punto1 di 'lato' a VerticeOpposto
        int l2; // id di lato da punto2 di 'lato' a VerticeOpposto
        for(unsigned int j=0;j<3;j++){
            if(triangoli[cella].lati[j]!=lato){
                if(lati[triangoli[cella].lati[j]].punto1==lati[lato].punto1 || lati[triangoli[cella].lati[j]].punto2==lati[lato].punto1){
                    l1 = triangoli[cella].lati[j];
                }
                else{
                    l2 = triangoli[cella].lati[j];}
            }
        }
        // Creo prima cella
        triangoli.push_back(Cell(lati[lato].punto1,verticeopp,lati[lato].pm,lato_bisez,lati[lato].p1m,l1,punti));
        // riempio adiacenze della prima cella
        if(lati[lati[lato].p1m].cella1 == -1){
            lati[lati[lato].p1m].cella1 = maxIDC;
        }
        else{
            lati[lati[lato].p1m].cella2 = maxIDC;
        }
        if(lati[lato_bisez].cella1 == -1){
            lati[lato_bisez].cella1 = maxIDC;
        }else{
            lati[lato_bisez].cella2 = maxIDC;
        }
        if(lati[l1].cella1 == cella){
            lati[l1].cella1 = maxIDC;
        }else{
            lati[l1].cella2 = maxIDC;
        }
        maxIDC = maxIDC+1;

        // creo seconda cella
        triangoli.push_back(Cell(lati[lato].punto2,verticeopp,lati[lato].pm,lato_bisez,lati[lato].mp2,l2,punti));
        // riempio adiacenze della seconda cella
        if(lati[lato_bisez].cella1 == -1){
            lati[lato_bisez].cella1 = maxIDC;
        }else{
            lati[lato_bisez].cella2 = maxIDC;
        }
        if(lati[lati[lato].mp2].cella1 == -1){
            lati[lati[lato].mp2].cella1 = maxIDC;
        }else{
            lati[lati[lato].mp2].cella2 = maxIDC;
        }
        if(lati[l2].cella1 == cella){
            lati[l2].cella1 = maxIDC;
        }else{
            lati[l2].cella2 = maxIDC;
        }
        maxIDC = maxIDC+1;

    }

    // SE lato  NON è IL LATO MAGGIORE DIVIDO CELLA IN 3
    else{
        int latomax = LatoMaggiore(triangoli[cella],lati);
        // controllo che latomax non sia stato diviso se no lo divido
        if (lati[latomax].p1m < 0 || lati[latomax].mp2 < 0 || lati[latomax].pm < 0){
            PuntoMedio(lati[latomax],punti);
            lati[latomax].pm = maxIDP;
            maxIDP = maxIDP+1;
            // creo 2 nuovi lati da latomax
            lati.push_back(Segment(lati[latomax].punto1,lati[latomax].pm,punti));
            lati.push_back(Segment(lati[latomax].pm,lati[latomax].punto2,punti));
            lati[latomax].p1m = maxIDS;
            lati[latomax].mp2 = maxIDS+1;
            maxIDS = maxIDS+2;
        }

        // creo taglio dal punto medio di lato al punto medio di latomax
        lati.push_back(Segment(lati[lato].pm,lati[latomax].pm,punti));
        int taglio = maxIDS;
        maxIDS = maxIDS+1;
        int PuntoComune = VerticeComune(lati[lato],lati[latomax]);
        // punto opposto a latomax
        int puntotaglio2 = VerticeOpposto(triangoli[cella],lati[latomax]);
        // creo segmento da punto medio di latomax a al suo vertice opposto puntotaglio2
        lati.push_back(Segment (lati[latomax].pm,puntotaglio2,punti));
        int taglio2 = maxIDS;
        maxIDS = maxIDS+1;

        int latopuntocomune; // Lato generato da divisione di 'lato' dalla parte di PuntoComune
        int latopuntotaglio2; // Lato generato da divisione di 'lato' dalla parte di puntotaglio2
        // devo riempire questi id con i lati giusti:
        if (lati[lati[lato].p1m].punto1==PuntoComune || lati[lati[lato].p1m].punto2==PuntoComune){
            latopuntocomune = lati[lato].p1m;
            latopuntotaglio2 = lati[lato].mp2;
        }else{latopuntocomune = lati[lato].mp2;
            latopuntotaglio2 = lati[lato].p1m;}

        int latoLMpuntocomune; // Lato generato da divisione di 'latomax' dalla parte di PuntoComune
        int latoLMverticeopp; // Lato generato da divisione di 'latomax' dalla parte di verticeopp
        // devo riempire questi id con i lati giusti:
        if (lati[lati[latomax].p1m].punto1==PuntoComune || lati[lati[latomax].p1m].punto2==PuntoComune){
            latoLMpuntocomune = lati[latomax].p1m;
            latoLMverticeopp = lati[latomax].mp2;
        }else{latoLMpuntocomune = lati[latomax].mp2;
            latoLMverticeopp = lati[latomax].p1m;}
        int terzolato; // Individua il lato di 'cella' che non è 'latomax' nè 'lato'
        if (triangoli[cella].lati[0]!=lato && triangoli[cella].lati[0]!=latomax){
            terzolato=triangoli[cella].lati[0];
        }else{if (triangoli[cella].lati[1]!=lato && triangoli[cella].lati[1]!=latomax){
                terzolato=triangoli[cella].lati[1];
            }else{terzolato=triangoli[cella].lati[2];}
        }

        // Creo le tre nuove celle generate dalla separazione di quella più grande
        triangoli.push_back(Cell(lati[lato].pm,lati[latomax].pm,PuntoComune,latopuntocomune,latoLMpuntocomune,taglio,punti));
        // riempio adiacenze di tutti i nuovi lati
        if(lati[latopuntocomune].cella1 == -1){
            lati[latopuntocomune].cella1 = maxIDC;
        }else{
            lati[latopuntocomune].cella2 = maxIDC;
        }
        if(lati[latoLMpuntocomune].cella1 == -1){
            lati[latoLMpuntocomune].cella1 = maxIDC;
        }else{
            lati[latoLMpuntocomune].cella2 = maxIDC;
        }
        if(lati[taglio].cella1 == -1){
            lati[taglio].cella1 = maxIDC;
        }else{
            lati[taglio].cella2 = maxIDC;
        }
        maxIDC = maxIDC+1;

        triangoli.push_back(Cell(lati[lato].pm,lati[latomax].pm,puntotaglio2,latopuntotaglio2,taglio,taglio2,punti));
        // riempio adiacenze di tutti i nuovi lati
        if(lati[latopuntotaglio2].cella1 == -1){
            lati[latopuntotaglio2].cella1 = maxIDC;
        }else{
            lati[latopuntotaglio2].cella2 = maxIDC;
        }
        if(lati[taglio].cella1 == -1){
            lati[taglio].cella1 = maxIDC;
        }else{
            lati[taglio].cella2 = maxIDC;
        }
        if(lati[taglio2].cella1 == -1){
            lati[taglio2].cella1 = maxIDC;
        }else{
            lati[taglio2].cella2 = maxIDC;
        }
        maxIDC = maxIDC+1;

        triangoli.push_back(Cell(puntotaglio2,verticeopp,lati[latomax].pm,terzolato,latoLMverticeopp,taglio2,punti));
        // riempio adiacenze di tutti i nuovi lati
        if(lati[terzolato].cella1 == cella){
            lati[terzolato].cella1 = maxIDC;
        }else{
            lati[terzolato].cella2 = maxIDC;
        }
        if(lati[latoLMverticeopp].cella1 == -1){
            lati[latoLMverticeopp].cella1 = maxIDC;
        }else{
            lati[latoLMverticeopp].cella2 = maxIDC;
        }
        if(lati[taglio2].cella1 == -1){
            lati[taglio2].cella1 = maxIDC;
        }else{
            lati[taglio2].cella2 = maxIDC;
        }
        maxIDC = maxIDC+1;
    }
}

void RaffinaCella(int& cella,
                  int& lato,
                  std::vector<Cell>& triangoli,
                  std::vector<Segment>& lati,
                  std::vector<Point>& punti,
                  int& maxIDP,
                  int& maxIDS,
                  int& maxIDC){
    int latomax = LatoMaggiore(triangoli[cella],lati);
    int CellaDopo;
    if (cella!=lati[latomax].cella1) {CellaDopo = lati[latomax].cella1;}
    else {CellaDopo = lati[latomax].cella2;}
    DividiCella(cella,lato,triangoli,lati,punti,maxIDP,maxIDS,maxIDC);
    if (CellaDopo > -1){
        if (latomax==(LatoMaggiore(triangoli[CellaDopo],lati))){
            DividiCella(CellaDopo,latomax,triangoli,lati,punti,maxIDP,maxIDS,maxIDC);
        }
        else{
            RaffinaCella(CellaDopo,latomax,triangoli,lati,punti,maxIDP,maxIDS,maxIDC);
        }}
}

int MaxCelle(std::vector<Cell>& triangoli){
    unsigned int n = triangoli.size();
    int max = -1;
    double areamax = 0;
    for(unsigned int i = 0;i<n;i++){
        if (triangoli[i].flag && triangoli[i].area > areamax ){max=i; areamax = triangoli[i].area;}
    }
    return max;
}
// Fine File
}
