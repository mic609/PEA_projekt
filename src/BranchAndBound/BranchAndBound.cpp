#include <BranchAndBound.hpp>
#include <Matrix.hpp>
#include <algorithm>

// funkcja zwraca tzn. lower bound, czyli dolne ograniczenie dla problemu komiwojażera
// funkcja dodatkowo redukuje kolumny i wiersze macierzy
int BranchAndBound::reduceRowColumn(Matrix m){
    int reduce_row = 0;
    int reduce_row_total = 0;
    int reduce_column = 0;
    int reduce_column_total = 0;
    int *help_tab = new int[m.size() - 1];

    // redukcja wierszy
    for(int i = 0; i < m.size(); i++){
        int h_j = 0;
        for(int j = 0; j < m.size(); j++){
            if (m.matrix[i][j].value >= 0){
                help_tab[h_j] = m.matrix[i][j].value;
                h_j ++;
            }
        }
        reduce_row = *std::min_element(help_tab, help_tab + (m.size() - 1));
        reduce_row_total += reduce_row;

        for(int j = 0; j < m.size(); j++)
            if (m.matrix[i][j].value >= 0)
                m.matrix[i][j].value -= reduce_row;
    }

    // redukcja kolumn
    for(int j = 0; j < m.size(); j++){
        int h_i = 0;
        for(int i = 0; i < m.size(); i++){
            if (m.matrix[i][j].value >= 0){
                help_tab[h_i] = m.matrix[i][j].value;
                h_i ++;
            }
        }
        reduce_column = *std::min_element(help_tab, help_tab + (m.size() - 1));
        reduce_column_total += reduce_column;

        for(int i = 0; i < m.size(); i++)
            if (m.matrix[i][j].value >= 0)
                m.matrix[i][j].value -= reduce_column;
    }

    delete [] help_tab;

    return (reduce_row_total + reduce_column_total);
}

void BranchAndBound::algorithm(Matrix matrix){
    int lower_bound;
    lower_bound = BranchAndBound::reduceRowColumn(matrix);

    std::cout << lower_bound << std::endl;
}
