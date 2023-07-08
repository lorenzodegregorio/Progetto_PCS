#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"



using namespace testing;

TEST(TestImport, TestImportPoints){
    //COPIARE IL FILE INPUT PUNTI MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO PUNTO), INSERIRE string nomefile IN INPUT
    //Id Marker X Y
    //0 1 0.0000000000000000e+00 0.0000000000000000e+00

    double x = 0.0000000000000000e+00;
    double y = 0.0000000000000000e+00;
    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(x,y);

    string nomefile = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";

    ProjectLibrary::ImportPoints(nomefile,punti); //
    EXPECT_EQ(punto1.x,punti[0].x);
    EXPECT_EQ(punto1.y,punti[0].y);
}

TEST(TestImport, TestImportSegments){
    //COPIARE IL FILE INPUT SEGMENTI MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO SEGMENTO), INSERIRE string nomefile IN INPUT
    //Id Marker Origin End
    //0 0 41 43

    int punto1 = 41;
    int punto2 = 43;

    vector<ProjectLibrary::Point> punti;

    string nomefile_punti = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    ProjectLibrary::ImportPoints(nomefile_punti,punti);


    vector<ProjectLibrary::Segment> lati;

    string nomefile_lati = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";

    //41 0 7.0312500000000000e-01 4.2187500000000000e-01
    //43 0 7.0521156832298137e-01 3.3011451863354035e-01

    double lunghezza = sqrt((7.0312500000000000e-01 - 7.0521156832298137e-01)*(7.0312500000000000e-01 - 7.0521156832298137e-01) +
                            (4.2187500000000000e-01 - 3.3011451863354035e-01)*(4.2187500000000000e-01 - 3.3011451863354035e-01));

    ProjectLibrary::ImportSegments(nomefile_lati,punti,lati);
    EXPECT_EQ(punto1,lati[0].punto1);
    EXPECT_EQ(punto2,lati[0].punto2);
    EXPECT_EQ(lati[0].lunghezza, lunghezza);
    EXPECT_EQ(-1,lati[0].p1m);
    EXPECT_EQ(-1,lati[0].mp2);
    EXPECT_EQ(-1,lati[0].pm);
}

TEST(TestImport, TestImportCells){
    //COPIARE IL FILE INPUT TRIANGOLI MA LASCIARE SOLO LA PRIMA RIGA (IL PRIMO TRIANGOLO), INSERIRE string nomefile IN INPUT
    //Id Vertices Edges
    //0 41 43 15 0 1 2

    std::vector<int> punti_ = {41, 43, 15};
    std::vector<int> lati_ = {0, 1, 2};

    vector<ProjectLibrary::Point> punti;
    string nomefile_punti = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    ProjectLibrary::ImportPoints(nomefile_punti,punti);

    vector<ProjectLibrary::Segment> lati;
    string nomefile_lati = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
    ProjectLibrary::ImportSegments(nomefile_lati,punti,lati);

    vector<ProjectLibrary::Cell> celle;
    string nomefile_celle = "C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv";
    ProjectLibrary::ImportCells(nomefile_celle,punti,lati,celle);

    //41 0 7.0312500000000000e-01 4.2187500000000000e-01
    //43 0 7.0521156832298137e-01 3.3011451863354035e-01
    //15 0 8.7500000000000000e-01 3.7500000000000000e-01

    double area = 0.5*abs(7.0312500000000000e-01*(3.3011451863354035e-01-3.7500000000000000e-01) +
                            7.0521156832298137e-01*(3.7500000000000000e-01-4.2187500000000000e-01) +
                            8.7500000000000000e-01*(4.2187500000000000e-01-3.3011451863354035e-01));

    EXPECT_EQ(punti_, celle[0].punti);
    EXPECT_EQ(lati_, celle[0].lati);
    EXPECT_EQ(area, celle[0].area); //CALCOLA AREA
    EXPECT_EQ(true, celle[0].flag);
}


TEST(TestGeometry, TestPuntoMedio) //ok?
{ vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,0.5};

    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);

    punti.push_back(punto1);
    punti.push_back(punto2);

    int p1_id = 0;
    int p2_id = 1;

    ProjectLibrary::Segment lato(p1_id,p2_id,punti);

    ProjectLibrary::PuntoMedio(lato, punti);

    ProjectLibrary::Point pm = punti[punti.size()-1];

    EXPECT_EQ(pm.x, 0.5);
    EXPECT_EQ(pm.y, 0.5);
    EXPECT_EQ(punti.size(), 3);
}

TEST(TestGeometry, TestVerticeOpposto){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};
    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    punti.push_back(punto1);
    punti.push_back(punto2);
    punti.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,punti);
    ProjectLibrary::Segment lato2(p2_id,p3_id,punti);
    ProjectLibrary::Segment lato3(p1_id,p3_id,punti);

    vector<ProjectLibrary::Segment> lati;
    lati.push_back(lato1);
    lati.push_back(lato2);
    lati.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,punti);

    int vertopp = VerticeOpposto(triangolo,lato1);

    EXPECT_EQ(vertopp, 2);
}

TEST(TestGeometry, TestLatoMaggiore){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};
    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    punti.push_back(punto1);
    punti.push_back(punto2);
    punti.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,punti);
    ProjectLibrary::Segment lato2(p2_id,p3_id,punti);
    ProjectLibrary::Segment lato3(p1_id,p3_id,punti);

    vector<ProjectLibrary::Segment> lati;
    lati.push_back(lato1);
    lati.push_back(lato2);
    lati.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,punti);

    int latomax = ProjectLibrary::LatoMaggiore(triangolo,lati);

    EXPECT_EQ(latomax, l1_id);
}

TEST(TestGeometry, TestVerticeComune){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);
    vector<ProjectLibrary::Point> punti;

    punti.push_back(punto1);
    punti.push_back(punto2);
    punti.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p3_id,punti);
    ProjectLibrary::Segment lato2(p2_id,p3_id,punti);
    int puntocomune = ProjectLibrary::VerticeComune(lato1,lato2);
    EXPECT_EQ(puntocomune, 2);
}

TEST(TestCelle, TestDividiCellaPerLatoMaggiore){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    punti.push_back(punto1);
    punti.push_back(punto2);
    punti.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,punti);
    ProjectLibrary::Segment lato2(p2_id,p3_id,punti);
    ProjectLibrary::Segment lato3(p1_id,p3_id,punti);

    vector<ProjectLibrary::Segment> lati;
    lati.push_back(lato1);
    lati.push_back(lato2);
    lati.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,punti);
    vector<ProjectLibrary::Cell> triangoli;
    triangoli.push_back(triangolo);
    int c1_id = 0;

    int IDP = 3;
    int IDS = 3;
    int IDC = 1;

    int latomax = ProjectLibrary::LatoMaggiore(triangolo,lati);
    ProjectLibrary::DividiCella(c1_id,latomax,triangoli,lati,punti,IDP,IDS,IDC);
    //
    std::vector<unsigned int> resultID = {0, 1, 2};
    std::vector<double> resultAREA = {0.25, 0.125, 0.125};
    for(unsigned int i=0;i<2;i++){
        int it = i;
        if(triangoli[i].flag==true){
            EXPECT_EQ(it, resultID[i]);
            EXPECT_EQ(triangoli[it].area, resultAREA[i]);
        }
    }
}

TEST(TestCelle, TestDividiCellaPerLatoQualsiasi){ //ok?
    vector<double> ascisse ={0.0,1.0,0.5};
    vector<double> ordinate ={0.0,1.0,1.0};

    vector<ProjectLibrary::Point> punti;
    ProjectLibrary::Point punto1(ascisse[0],ordinate[0]);
    ProjectLibrary::Point punto2(ascisse[1],ordinate[1]);
    ProjectLibrary::Point punto3(ascisse[2],ordinate[2]);

    punti.push_back(punto1);
    punti.push_back(punto2);
    punti.push_back(punto3);

    int p1_id = 0;
    int p2_id = 1;
    int p3_id = 2;

    ProjectLibrary::Segment lato1(p1_id,p2_id,punti);
    ProjectLibrary::Segment lato2(p2_id,p3_id,punti);
    ProjectLibrary::Segment lato3(p1_id,p3_id,punti);

    vector<ProjectLibrary::Segment> lati;
    lati.push_back(lato1);
    lati.push_back(lato2);
    lati.push_back(lato3);

    int l1_id = 0;
    int l2_id = 1;
    int l3_id = 2;

    ProjectLibrary::Cell triangolo(p1_id,p2_id,p3_id,l1_id,l2_id,l3_id,punti);
    vector<ProjectLibrary::Cell> triangoli;
    triangoli.push_back(triangolo);
    int c1_id = 0;

    int IDP = 3;
    int IDS = 3;
    int IDC = 1;
    ProjectLibrary::DividiCella(c1_id,l2_id,triangoli,lati,punti,IDP,IDS,IDC);
    //
    std::vector<unsigned int> resultID = {0, 1, 2, 3};
    std::vector<double> resultAREA = {0.25 ,0.0625, 0.0625,0.125};
    for(unsigned int i=0;i<3;i++){
        int it = i;
        if(triangoli[i].flag==true){
            EXPECT_EQ(it, resultID[i]);
            EXPECT_EQ(triangoli[it].area, resultAREA[i]);
        }
    }
}
#endif // __TEST_EMPTY_H
