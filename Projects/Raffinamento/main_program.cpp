#include "empty_class.hpp"
using namespace ProjectLibrary;
int main()
{
    // 1) Criteri di arresto

    double theta = 0.001; // tolleranza sull'area
    int max_it = 800; // numero massimo di iterazioni

    // 2) Apertura dei file .csv: points, edges e triangles:
    // sono tutte classi definite in empty_class.hpp con i rispettivi attributi

    std::vector<Point> points;
    points.reserve(points.size() + max_it);
    ImportPoints("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv",points);
    std::vector<Segment> edges;
    edges.reserve(edges.size() + 4*max_it);
    ImportSegments("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv",points,edges);
    std::vector<Cell> triangles;
    triangles.reserve(triangles.size() + 3*max_it);
    ImportCells("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv",points, edges, triangles);

    // 3) Inizializzo delle variabili che tengono traccia dell'id minimo libero di ogni classe
    // in modo da fornire id validi (nuovi) per i nuovi elementi generati dal raffinamento

    int dim_vect_points = points.size(); //massimo Identificatore punti= dimensione vettore punti
    int dim_vect_edges = edges.size();
    int dim_vect_cells = triangles.size();

    // 4) Stampa L'area della cella maggiore e il numero di cells PRIMA del raffinamento

    int p = Maxcells(triangles);
    cout<<"Area Cella Maggiore (Prima di Dividere): "<<triangles[p].area<<endl;
    cout<<"Numero cells (Prima di Dividere): "<<triangles.size()<<endl;

    // Fa partire il tempo (cronometro)
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int it = 0; // iterazione in corso

    // 5) Raffinamento: usa Maxcells, LatoMaggiore e RaffinaCella

    for (int i = 0; i < max_it; i++){
        int j = Maxcells(triangles);
        if (triangles[j].area > theta){
            int k = LatoMaggiore(triangles[j],edges);
            RaffinaCella(j,k,triangles,edges,points,dim_vect_points,dim_vect_edges,dim_vect_cells);
            it++;
        }
        else{
            break;
        }
    }

    // Ferma il cronometro
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    int j = Maxcells(triangles);
    cout<<"Area Cella Maggiore (Dopo Divisione): "<<triangles[j].area<<endl<<endl;

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
    for (unsigned int i=0;i < edges.size();i++) {
        file << points[edges[i].punto1].x << ";" << points[edges[i].punto1].y << ";" << points[edges[i].punto2].x << ";" << points[edges[i].punto2].y << "\n";
    }
    file.close();

    return 0;
}
