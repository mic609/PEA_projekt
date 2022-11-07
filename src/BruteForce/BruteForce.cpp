#include <BruteForce.hpp>
#include <Matrix.hpp>
#include <algorithm>

void BruteForce::algorithm(Matrix matrix){
    int *tab = new int[matrix.size()];
    int result = -1;
    std::list<Edge> bruteEdges;
    for(int i = 0; i < matrix.size(); i++)
        tab[i] = i;
    
    do {
        int local_result;

        for(int i = 0; i < matrix.size(); i++){
            if(i == matrix.size() - 1){
                local_result += matrix.matrix[tab[matrix.size() - 1]][tab[0]].value;
            }
            else{
                local_result += matrix.matrix[tab[i]][tab[i + 1]].value;
            }
        }

        if(local_result <= result || result == -1){
            if(!bruteEdges.empty())
                bruteEdges.clear();
            for(int i = 0; i < matrix.size(); i++){
                Edge elem;
                if(i == matrix.size() - 1){
                    elem.od_w = tab[matrix.size() - 1];
                    elem.do_w = tab[0];
                    bruteEdges.push_front(elem);
                }
                else{
                    elem.od_w = tab[i];
                    elem.do_w = tab[i + 1];
                    bruteEdges.push_front(elem);
                }
            }
            result = local_result;
        }

        local_result = 0;

  } while ( std::next_permutation(tab, tab + matrix.size()) );

  delete [] tab;

  final_result = result;
  finalEdges = bruteEdges;
}

void BruteForce::showResult(){
    std::cout << std::endl << "Result (Brute Force): " << final_result << std::endl;

    std::cout << "Edges: " << std::endl;
    std::list<Edge>::iterator it = finalEdges.begin();
    std::list<Edge>::iterator beginit = finalEdges.begin();
    std::list<Edge>::iterator iter;
    std::cout << "("<< it->od_w << ", ";
    std::cout << it->do_w << ")" << std::endl;

    for (iter = finalEdges.begin(); iter != finalEdges.end(); iter++){
        if(it != iter){
            if(it->do_w == beginit->od_w){
                break;
            }
            if(it->do_w == iter->od_w){
                std::cout << "("<< iter->od_w << ", ";
                std::cout << iter->do_w << ")" << std::endl;
                it = iter;
                iter = finalEdges.begin();
            }
        }
    }
}
