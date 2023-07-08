#include "empty_class.hpp"
using namespace ProjectLibrary;
int main()
{
    // 1) Criteri di arresto

    int max_it = 800; // numero massimo di iterazioni
    double theta = 0.001; // tolleranza sull'area

    // 2) Apertura dei file .csv: punti, lati e triangoli:
    // sono tutte classi definite in empty_class con i rispettivi attributi

    std::vector<Point> punti;
    punti.reserve(punti.size() + max_it);
    ImportPoints("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv",punti);
    std::vector<Segment> lati;
    lati.reserve(lati.size() + 4*max_it);
    ImportSegments("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv",punti,lati);
    std::vector<Cell> triangoli;
    triangoli.reserve(triangoli.size() + 3*max_it);
    ImportCells("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv",punti, lati, triangoli);

    // 3) Inizializzo delle variabili che tengono traccia dell'id minimo libero di ogni classe
    // in modo da fornire id validi (nuovi) per i nuovi elementi generati dal raffinamento

    int maxIDP = punti.size();
    int maxIDS = lati.size();
    int maxIDC = triangoli.size();

    // 4) Stampa L'area della cella maggiore e il numero di celle PRIMA del raffinamento

    int p = MaxCelle(triangoli);
    cout<<"Area Cella Maggiore (Prima di Dividere): "<<triangoli[p].area<<endl;
    cout<<"Numero Celle (Prima di Dividere): "<<triangoli.size()<<endl;

    // Fa partire il tempo (cronometro)
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int it = 0; // iterazione in corso

    // 5) Raffinamento: usa MaxCelle, LatoMaggiore e RaffinaCella

    for (int i = 0; i < max_it; i++){
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

    // Ferma il cronometro
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    int j = MaxCelle(triangoli);
    cout<<"Area Cella Maggiore (Dopo Divisione): "<<triangoli[j].area<<endl<<endl;

    // Criteri di arresto
    if (it < max_it){
        cout << "tolleranza (theta = " << theta << ") " << "raggiunta in " << it << " iterazioni" << endl;
    }
    else{
        cout << "raggiunto limite massimo di iterazioni (" << max_it << "), " << endl;
    }

    cout<<"Tempo Impiegato: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1.0e-06<<" s"<<endl;


    // 6) stampa su file .csv di output

    std::ofstream file("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/output_edges.csv");
    for (unsigned int i=0;i < lati.size();i++) {
        file << punti[lati[i].punto1].x << ";" << punti[lati[i].punto1].y << ";" << punti[lati[i].punto2].x << ";" << punti[lati[i].punto2].y << "\n";
    }
    file.close();

    return 0;
}
