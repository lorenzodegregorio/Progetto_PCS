#include "empty_class.hpp"
using namespace ProjectLibrary;
int main()
{
    // max iterazioni
    int n = 800;
    // tolleranza su area
    double theta = 0.001;

    std::vector<Point> punti;
    punti.reserve(punti.size() + n);
    ImportPoints("C:\\Users\\acer\\Desktop\\Progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell0Ds.csv",punti);
    std::vector<Segment> lati;
    lati.reserve(lati.size() + 4*n);
    ImportSegments("C:\\Users\\acer\\Desktop\\Progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell1Ds.csv",punti,lati);
    std::vector<Cell> triangoli;
    triangoli.reserve(triangoli.size() + 3*n);
    ImportCells("C:\\Users\\acer\\Desktop\\progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell2Ds.csv",punti, lati, triangoli);

    // Queste variabili tengono traccia dell'id minimo libero di ogni classe
    // in modo da fornire id validi (nuovi) per i nuovi elementi generati dal raffinamento
    int maxIDP = punti.size();
    int maxIDS = lati.size();
    int maxIDC = triangoli.size();

    int p = MaxCelle(triangoli);
    cout<<"Area Cella Maggiore (Prima di Dividere): "<<triangoli[p].area<<endl;
    cout<<"Numero Celle (Prima di Dividere): "<<triangoli.size()<<endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int it = 0;

    for (int i = 0; i < n; i++){
        int j = MaxCelle(triangoli);
        if (triangoli[j].area > theta){
            int k = LatoMaggiore(triangoli[j],lati);
            RaffinaCella(j,k,triangoli,lati,punti,maxIDP,maxIDS,maxIDC);
            it++;
        }
        else{
            break;
        }
    }


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    int j = MaxCelle(triangoli);
    cout<<"Area Cella Maggiore (Dopo Divisione): "<<triangoli[j].area<<endl<<endl;

    if (it < n){
        cout << "tolleranza (theta = " << theta << ") " << "raggiunta in " << it << " iterazioni" << endl;
    }
    else{
        cout << "raggiunto limite massimo di iterazioni (" << n << "), " << endl;
    }

    cout<<"Tempo Impiegato: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1.0e-06<<" s"<<endl;

    /// PROVA DI STAMPA SU FILE EXCEL:
    std::ofstream file("C:\\Users\\acer\\Desktop\\outputLati.csv");
    for (unsigned int i=0;i < lati.size();i++) {
        file << punti[lati[i].punto1].x << ";" << punti[lati[i].punto1].y << ";" << punti[lati[i].punto2].x << ";" << punti[lati[i].punto2].y << "\n";
    }
    file.close();

    /// FINE PROVA

    return 0;
}
