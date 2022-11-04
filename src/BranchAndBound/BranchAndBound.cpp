#include <BranchAndBound.hpp>
#include <Matrix.hpp>
#include <algorithm>
#include <list>

BranchAndBound::BranchAndBound(int r){
    result = r;
}

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
    result = lower_bound;

    BranchAndBound alg(result);
    alg.execution(matrix, true);
    alg.showResult();
}

void BranchAndBound::execution(Matrix& matrix, bool start_exec){

    int local_lower_bound = 0;
    if(start_exec == false)
        local_lower_bound = BranchAndBound::reduceRowColumn(matrix);
    else
        start_exec = false;

    matrix.showGraph();
    std::cout << std::endl;
    std::cout <<"local lower bound: " << local_lower_bound <<std::endl;

    result += local_lower_bound;

    int bottom_limit = 0;
    int i_del, j_del;
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
                    i_del = matrix.matrix[i][j].row_number;
                    j_del = matrix.matrix[i][j].col_number;
                    i_red = i;
                    j_red = j;
                }
            }
        }
    }

    if(j_del < matrix.size() && i_del < matrix.size())
        matrix.matrix[j_del][i_del].value = -1;
    matrix.removeColumnRow(i_red, j_red);

    std::cout << "i_red = " << i_red << std::endl;
    std::cout << "j_red = " << j_red << std::endl;

    if(bottom_limit != 0)
        BranchAndBound::execution(matrix);
        
}
void BranchAndBound::showResult(){
    std::cout << "result: " << result;
}