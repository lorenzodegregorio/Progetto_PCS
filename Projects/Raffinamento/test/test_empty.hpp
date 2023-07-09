#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"



using namespace testing;
using namespace std;


/// T1. copio I file di input ma ignoro la prima riga delle etichette,
/// inserisco string nomefile in input (id, ascissa, ordinata)
TEST(TestImport, TestImportPoints){
    // COPIARE IL FILE INPUT points MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO PUNTO),
    // INSERIRE string nomefile IN INPUT
    // Id Marker X Y
    // 0 1 0.0000000000000000e+00 0.0000000000000000e+00

    double x = 0.0000000000000000e+00;
    double y = 0.0000000000000000e+00;
    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(x,y);

    string nomefile = "C:\\Users\\acer\\Desktop\\progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell0Ds.csv";

    ProjectLibrary::ImportPoints(nomefile,points); //
    EXPECT_EQ(punto1.x,points[0].x);
    EXPECT_EQ(punto1.y,points[0].y);
}


/// T2. copio I file di input ma ignoro la prima riga delle etichette,
/// inserisco string nomefile in input (id origine fine)
TEST(TestImport, TestImportSegments){
    // COPIARE IL FILE INPUT SEGMENTI MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO SEGMENTO),
    // INSERIRE string nomefile IN INPUT
    // Id Marker Origin End
    // 0 0 41 43

    int punto1 = 41;
    int punto2 = 43;

    vector<ProjectLibrary::Point> points;

    string nomefile_points = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    ProjectLibrary::ImportPoints(nomefile_points,points);


    vector<ProjectLibrary::Segment> edges;

    string nomefile_edges = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";

    //41 0 7.0312500000000000e-01 4.2187500000000000e-01
    //43 0 7.0521156832298137e-01 3.3011451863354035e-01

    double lunghezza = sqrt((7.0312500000000000e-01 - 7.0521156832298137e-01)*(7.0312500000000000e-01 - 7.0521156832298137e-01) +
                            (4.2187500000000000e-01 - 3.3011451863354035e-01)*(4.2187500000000000e-01 - 3.3011451863354035e-01));

    ProjectLibrary::ImportSegments(nomefile_edges,points,edges);
    EXPECT_EQ(punto1,edges[0].punto1);
    EXPECT_EQ(punto2,edges[0].punto2);
    EXPECT_EQ(edges[0].lunghezza, lunghezza);
    EXPECT_EQ(-1,edges[0].p1_midpoint);
    EXPECT_EQ(-1,edges[0].midpoint_p2);
    EXPECT_EQ(-1,edges[0].midpoint);
}


/// T3. copio I file di input ma ignoro la prima riga delle etichette,
/// inserisco string nomefile in input (id vertici e edges)
TEST(TestImport, TestImportCells){
    // COPIARE IL FILE INPUT triangles MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO TRIANGOLO),
    // INSERIRE string nomefile IN INPUT
    // Id Vertices Edges
    // 0 41 43 15 0 1 2

    vector<int> points_ = {41, 43, 15};
    vector<int> edges_ = {0, 1, 2};

    vector<ProjectLibrary::Point> points;
    string nomefile_points = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    ProjectLibrary::ImportPoints(nomefile_points,points);

    vector<ProjectLibrary::Segment> edges;
    string nomefile_edges = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
    ProjectLibrary::ImportSegments(nomefile_edges,points,edges);

    vector<ProjectLibrary::Cell> cells;
    string nomefile_cells = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv";
    ProjectLibrary::ImportCells(nomefile_cells,points,edges,cells);

    //41 0 7.0312500000000000e-01 4.2187500000000000e-01
    //43 0 7.0521156832298137e-01 3.3011451863354035e-01
    //15 0 8.7500000000000000e-01 3.7500000000000000e-01

    double area = 0.5*abs(7.0312500000000000e-01*(3.3011451863354035e-01-3.7500000000000000e-01) +
                            7.0521156832298137e-01*(3.7500000000000000e-01-4.2187500000000000e-01) +
                            8.7500000000000000e-01*(4.2187500000000000e-01-3.3011451863354035e-01));

    EXPECT_EQ(points_, cells[0].points);
    EXPECT_EQ(edges_, cells[0].edges);
    EXPECT_EQ(area, cells[0].area); //CALCOLA AREA
    EXPECT_EQ(true, cells[0].flag);
}


/// T4. aggiungo i primi due punti al vettore "points" e trovo il punto medio
TEST(TestGeometry, TestCreaPuntoMedio) //ok?
{ vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,0.5};

    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);

    points.push_back(punto1);
    points.push_back(punto2);

    int p1_id = 0;
    int p2_id = 1;

    ProjectLibrary::Segment lato(p1_id,p2_id,points);

    ProjectLibrary::CreaPuntoMedio(lato, points);

    ProjectLibrary::Point midpoint = points[points.size()-1];

    EXPECT_EQ(midpoint.x, 0.5);
    EXPECT_EQ(midpoint.y, 0.5);
    EXPECT_EQ(points.size(), 3);
}


/// T5. aggiungo i primi tre punti e i primi tre lati ai rispettivi vettori,
/// dopodiché trovo vertice opposto
TEST(TestGeometry, TestTrovaVerticeOpposto){
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};
    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    points.push_back(punto1);
    points.push_back(punto2);
    points.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,points);
    ProjectLibrary::Segment lato2(p2_id,p3_id,points);
    ProjectLibrary::Segment lato3(p1_id,p3_id,points);

    vector<ProjectLibrary::Segment> edges;
    edges.push_back(lato1);
    edges.push_back(lato2);
    edges.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,points);

    int vertopp = TrovaVerticeOpposto(triangolo,lato1);

    EXPECT_EQ(vertopp, 2);
}


/// T6. aggiungo i primi tre punti e i primi tre lati ai rispettivi vettori,
/// dopodiché trovo lato maggiore
TEST(TestGeometry, TestTrovaLatoMaggiore){
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};
    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    points.push_back(punto1);
    points.push_back(punto2);
    points.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,points);
    ProjectLibrary::Segment lato2(p2_id,p3_id,points);
    ProjectLibrary::Segment lato3(p1_id,p3_id,points);

    vector<ProjectLibrary::Segment> edges;
    edges.push_back(lato1);
    edges.push_back(lato2);
    edges.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,points);

    int latomax = ProjectLibrary::TrovaLatoMaggiore(triangolo,edges);

    EXPECT_EQ(latomax, l1_id);
}


/// T7. aggiungo i primi tre punti al vettore "points",
/// trovo punto comune tra i due lati
TEST(TestGeometry, TestTrovaVerticeComune){
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);
    vector<ProjectLibrary::Point> points;

    points.push_back(punto1);
    points.push_back(punto2);
    points.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p3_id,points);
    ProjectLibrary::Segment lato2(p2_id,p3_id,points);
    int puntocomune = ProjectLibrary::TrovaVerticeComune(lato1,lato2);
    EXPECT_EQ(puntocomune, 2);
}


/// T8. aggiungo i primi tre punti, i primi tre segmenti e le prime tre celle
/// ai rispettivi vettori;
/// trovo lato maggiore e divido cella
TEST(Testcells, TestDividiCellaPerTrovaLatoMaggiore){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    points.push_back(punto1);
    points.push_back(punto2);
    points.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,points);
    ProjectLibrary::Segment lato2(p2_id,p3_id,points);
    ProjectLibrary::Segment lato3(p1_id,p3_id,points);

    vector<ProjectLibrary::Segment> edges;
    edges.push_back(lato1);
    edges.push_back(lato2);
    edges.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,points);
    vector<ProjectLibrary::Cell> triangles;
    triangles.push_back(triangolo);
    int c1_id = 0;

    int IDP = 3;
    int IDE = 3;
    int IDC = 1;

    int latomax = ProjectLibrary::TrovaLatoMaggiore(triangolo,edges);
    ProjectLibrary::DividiCella(c1_id,latomax,triangles,edges,points,IDP,IDE,IDC);
    //
    vector<unsigned int> resultID = {0, 1, 2};
    vector<double> resultAREA = {0.25, 0.125, 0.125};
    for(unsigned int i=0;i<2;i++){
        int it = i;
        if(triangles[i].flag==true){
            EXPECT_EQ(it, resultID[i]);
            EXPECT_EQ(triangles[it].area, resultAREA[i]);
        }
    }
}


/// T9. aggiungo i primi tre points, i primi tre segmenti e le prime tre cells
/// ai vettori dei points, dei segmenti e delle cells;
/// divido cella
TEST(Testcells, TestDividiCellaPerLatoQualsiasi){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    vector<ProjectLibrary::Point> points;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    points.push_back(punto1);
    points.push_back(punto2);
    points.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,points);
    ProjectLibrary::Segment lato2(p2_id,p3_id,points);
    ProjectLibrary::Segment lato3(p1_id,p3_id,points);

    vector<ProjectLibrary::Segment> edges;
    edges.push_back(lato1);
    edges.push_back(lato2);
    edges.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,points);
    vector<ProjectLibrary::Cell> triangles;
    triangles.push_back(triangolo);
    int c1_id = 0;

    int IDP = 3;
    int IDE = 3;
    int IDC = 1;
    ProjectLibrary::DividiCella(c1_id,l2_id,triangles,edges,points,IDP,IDE,IDC);
    //
    vector<unsigned int> resultID = {0, 1, 2, 3};
    vector<double> resultAREA = {0.25 ,0.0625, 0.0625,0.125};
    for(unsigned int i=0;i<3;i++){
        int it = i;
        if(triangles[i].flag==true){
            EXPECT_EQ(it, resultID[i]);
            EXPECT_EQ(triangles[it].area, resultAREA[i]);
        }
    }
}
#endif // __TEST_EMPTY_H
