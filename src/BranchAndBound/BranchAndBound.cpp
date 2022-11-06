#include <BranchAndBound.hpp>
#include <Matrix.hpp>
#include <algorithm>
#include <list>
#include <iterator>
#include <limits>

// funkcja zwraca tzn. lower bound, czyli dolne ograniczenie dla problemu komiwojażera
// funkcja dodatkowo redukuje kolumny i wiersze macierzy
int BranchAndBound::reduceRowColumn(Matrix& m){
    int reduce_row = 0;
    int reduce_row_total = 0;
    int reduce_column = 0;
    int reduce_column_total = 0;
    std::list<int> help_list;

    // redukcja wierszy
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.size(); j++){
            if (m.matrix[i][j].value >= 0){
                help_list.push_back(m.matrix[i][j].value);
                // for (auto const &w: help_list) {
                //     std::cout << w << std::endl;
                // }
                // std::cout << std::endl;
            }
        }
        reduce_row = *std::min_element(help_list.begin(), help_list.end());
        reduce_row_total += reduce_row;

        for(int j = 0; j < m.size(); j++)
            if (m.matrix[i][j].value > 0)
                m.matrix[i][j].value -= reduce_row;

        help_list.clear();
    }

    // redukcja kolumn
    for(int j = 0; j < m.size(); j++){
        for(int i = 0; i < m.size(); i++){
            if (m.matrix[i][j].value >= 0){
                help_list.push_back(m.matrix[i][j].value);
            }
        }
        reduce_column = *std::min_element(help_list.begin(), help_list.end());
        reduce_column_total += reduce_column;

        for(int i = 0; i < m.size(); i++)
            if (m.matrix[i][j].value > 0)
                m.matrix[i][j].value -= reduce_column;

        help_list.clear();
    }

    return (reduce_row_total + reduce_column_total);
}

int BranchAndBound::bottomLimit(Matrix m, int i, int j){

    int min_row = 0, min_col = 0, min = 0;

    std::list<int> help_list;

    for(int k = 0; k < m.size(); k++)
        if(k != j && m.matrix[i][k].value >= 0)
            help_list.push_back(m.matrix[i][k].value);

    min_row = *std::min_element(help_list.begin(), help_list.end());

    help_list.clear();

    for(int k = 0; k < m.size(); k++)
        if(k != i && m.matrix[k][j].value >= 0)
            help_list.push_back(m.matrix[k][j].value);

    min_col = *std::min_element(help_list.begin(), help_list.end());
    
    min = min_col + min_row;

    return min;
}

void BranchAndBound::algorithm(Matrix matrix){
    lower_bound = BranchAndBound::reduceRowColumn(matrix);
    int result = lower_bound;
    std::list<Edge> edges;
    final_result = INT_MAX;

    BranchAndBound alg;
    result = alg.executionLeft(matrix, result, edges, true);
    std::cout << "result: " << result << std::endl;

    std::cout << "FINALLY: " << std::endl;
    for (auto v : alg.returnEdges()){
        std::cout << v.od_w;
        std::cout << " " << v.do_w << "\n";
    }
}

int BranchAndBound::executionLeft(Matrix& matrix, int result, std::list<Edge> edges, bool start_exec){

    std::cout << "Left Wing: ";
    std::cout<< result << std::endl;

    int local_lower_bound = 0;
    if(start_exec == false)
        local_lower_bound = BranchAndBound::reduceRowColumn(matrix);
    else
        start_exec = false;

    result += local_lower_bound;
    if(result >= final_result)
        return result;

    int bottom_limit = 0;
    int i_red, j_red;

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(matrix.matrix[i][j].value == 0){
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j);
                if(previous_bottom_limit > bottom_limit){
                    bottom_limit = previous_bottom_limit;
                }
                else{
                    i_red = i;
                    j_red = j;
                }
            }
        }
    }

    int resultSec = result + bottom_limit;

    if(bottom_limit != 0){

        Edge elem;
        elem.od_w = i_red;
        elem.do_w = j_red;
        edges.push_back(elem);

        for (auto v : edges){
            std::cout << v.od_w;
            std::cout << " " << v.do_w << "\n";
        }

        Matrix help_matrix(4);
        help_matrix = matrix;

        if(result < resultSec){
            if(j_red < matrix.size() && i_red < matrix.size())
                matrix.matrix[j_red][i_red].value = -1;
            matrix.removeColumnRow(i_red, j_red);

            result = BranchAndBound::executionLeft(matrix, result, edges);
        }
        if(result >= resultSec){
            help_matrix.matrix[i_red][j_red].value = -1;
            if(!edges.empty())
                edges.pop_front();
            resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i_red, j_red, edges);
        }
    }
    else{
        int j_temp = -1;
        int i_temp = -1;
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0 && j != j_temp && i != i_temp){
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    i_temp = i;
                    j_temp = j;
                    edges.push_front(elem);
                }
            }
        }
        while(edges.size() > matrix.size())
            edges.pop_front();
        
        if(!finalEdges.empty())
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) );

        final_result = result;

        std::cout << "FINALLY: " << std::endl;
        for (auto v : finalEdges){
            std::cout << v.od_w;
            std::cout << " " << v.do_w << "\n";
        }
    }

    if(result > resultSec)
        return resultSec;
    else
        return result;
}
int BranchAndBound::executionRight(Matrix& matrix, int result, int i_r, int j_r, std::list<Edge> edges, bool start_exec){

    std::cout << "Right Wing: ";
    std::cout<< result << std::endl;

    std::list<int> help_list;
    int min_col = 0;
    int min_row = 0;
    int min = 0;

    for(int i = 0; i < matrix.size(); i++){
        if(matrix.matrix[i][j_r].value >= 0)
            help_list.push_back(matrix.matrix[i][j_r].value);
    }
    min_col = *std::min_element(help_list.begin(), help_list.end());

    for(int i = 0; i < matrix.size(); i++)
        if (matrix.matrix[i][j_r].value > 0)
            matrix.matrix[i][j_r].value -= min_col;

    help_list.clear();

    for(int j = 0; j < matrix.size(); j++){
        if(matrix.matrix[i_r][j].value >= 0)
            help_list.push_back(matrix.matrix[i_r][j].value);
    }
    min_row = *std::min_element(help_list.begin(), help_list.end());

    for(int j = 0; j < matrix.size(); j++)
        if (matrix.matrix[i_r][j].value > 0)
            matrix.matrix[i_r][j].value -= min_row;

    help_list.clear();

    min = min_col + min_row;
    result += min;
    if(result >= final_result)
        return result;

    int bottom_limit = 0;
    int i_red, j_red;

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(matrix.matrix[i][j].value == 0){
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j);
                if(previous_bottom_limit > bottom_limit){
                    bottom_limit = previous_bottom_limit;
                }
                else{
                    i_red = i;
                    j_red = j;
                }
            }
        }
    }

    int resultSec = result + bottom_limit;

    if(bottom_limit != 0){

        Edge elem;
        elem.od_w = i_red;
        elem.do_w = j_red;
        edges.push_back(elem);

        Matrix help_matrix(4);
        help_matrix = matrix;

        for (auto v : edges){
            std::cout << v.od_w;
            std::cout << " " << v.do_w << "\n";
        }

        if(result < resultSec){
            //std::cout << "left wing: " << std::endl;
            //matrix.showGraph();
            if(j_red < matrix.size() && i_red < matrix.size())
                matrix.matrix[j_red][i_red].value = -1;
            matrix.removeColumnRow(i_red, j_red);
            result = BranchAndBound::executionLeft(matrix, result, edges);
        }
        if(result >= resultSec){
            //std::cout << "right wing: " << std::endl;
            //help_matrix.showGraph();
            help_matrix.matrix[i_red][j_red].value = -1;
            if(!edges.empty())
                edges.pop_front();
            resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i_red, j_red, edges);
        }
    }
    else{
        int j_temp = -1;
        int i_temp = -1;
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0 && j != j_temp && i != i_temp){
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    i_temp = -1;
                    j_temp = j;
                    edges.push_front(elem);
                }
            }
        }
        while(edges.size() > matrix.size())
            edges.pop_front();

        if(!finalEdges.empty())
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) );

        final_result = result;
    }

    if(result > resultSec)
        return resultSec;
    else
        return result;

}

void BranchAndBound::bruteForce(Matrix matrix){
    int *tab = new int[matrix.size()];
    int result = -1;
    for(int i = 0; i < matrix.size(); i++)
        tab[i] = i;
    
    do {
        int local_result;

        for(int i = 0; i < matrix.size(); i++){
            if(i == matrix.size() - 1)
                local_result += matrix.matrix[tab[matrix.size() - 1]][tab[0]].value;
            else
                local_result += matrix.matrix[tab[i]][tab[i + 1]].value;
        }

        if(local_result <= result || result == -1)
            result = local_result;
        
        local_result = 0;

  } while ( std::next_permutation(tab, tab + matrix.size()) );

  delete [] tab;

  std::cout << "brute force result: " << result << std::endl;
}

std::list<BranchAndBound::Edge> BranchAndBound::returnEdges(){
    return finalEdges;
} 