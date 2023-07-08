#include "empty_class.hpp"
using namespace ProjectLibrary;
int main()
{
    // 1) Criteri di arresto

    double theta = 0.001; // tolleranza sull'area
    int max_it = 800; // numero massimo di iterazioni

    // 2) Apertura dei file .csv: points, edges e triangles:
    // sono tutte classi definite in empty_class.hpp con i rispettivi attributi;
    // inizializzo delle variabili che tengono traccia delle dimensioni dei vettori


    std::vector<Point> points;
    points.reserve(points.size() + max_it);
    ImportPoints("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv",points);
    int dim_vect_points = points.size(); // massimo Identificatore punti = dimensione vettore punti

    std::vector<Segment> edges;
    edges.reserve(edges.size() + 4*max_it);
    ImportSegments("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv",points,edges);
    int dim_vect_edges = edges.size();

    std::vector<Cell> triangles;
    triangles.reserve(triangles.size() + 3*max_it);
    ImportCells("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv",points, edges, triangles);
    int dim_vect_cells = triangles.size();


    // 3) Stampa l'area della cella maggiore e il numero di celle PRIMA del raffinamento

    int p = TrovaIdCellaAreaMax(triangles);
    cout<<"Area Cella Maggiore (Prima di Dividere): "<<triangles[p].area<<endl;
    cout<<"Numero celle (Prima di Dividere): "<<triangles.size()<<endl;


    // 4) Raffinamento: usa TrovaIdCellaAreaMax, LatoMaggiore e RaffinaCella

    int it = 0; // iterazione in corso
    for (int i = 0; i < max_it; i++){
        int j = TrovaIdCellaAreaMax(triangles);
        if (triangles[j].area > theta){ // Controllo criterio theta per area max
            int k = LatoMaggiore(triangles[j],edges);
            RaffinaCella(j,k,triangles,edges,points,dim_vect_points,dim_vect_edges,dim_vect_cells);
            it++;
        }
        else{
            break;
        }
    }

    int j = TrovaIdCellaAreaMax(triangles);
    cout<<"Area Cella Maggiore (Dopo Divisione): "<<triangles[j].area<<endl<<endl;

    // Criteri di arresto
    if (it < max_it)
    {
        cout << "tolleranza (theta = " << theta << ") " << "raggiunta in " << it << " iterazioni" << endl;
    }
    else
    {
        cout << "raggiunto limite massimo di iterazioni (" << max_it << "), " << endl;
    }


    // 5) Stampa su file .csv di output

    std::ofstream file("C:/Users/hp/Desktop/project_PCS/Progetto_PCS/lati_raffinati.csv");
    for (unsigned int i=0;i < edges.size();i++)
    {
        file << points[edges[i].punto1].x << ";" << points[edges[i].punto1].y << ";" << points[edges[i].punto2].x << ";" << points[edges[i].punto2].y << "\n";
    }
    file.close();

    return 0;
}
