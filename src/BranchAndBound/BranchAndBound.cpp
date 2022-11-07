#include <BranchAndBound.hpp>
#include <Matrix.hpp>
#include <algorithm>
#include <list>
#include <iterator>
#include <limits>

BranchAndBound::BranchAndBound(){
    final_result = INT_MAX;
}

// funkcja zwraca tzn. lower bound, czyli dolne ograniczenie dla problemu komiwojażera
// funkcja dodatkowo redukuje kolumny i wiersze macierzy
int BranchAndBound::reduceRowColumn(Matrix& m){
    int reduce_row = 0; // koszt redukcji wybranego wiersza
    int reduce_row_total = 0; // koszt redukcji wszystkich wierszy
    int reduce_column = 0; // koszt redukcji wybranej kolumny
    int reduce_column_total = 0; // koszt redukcji wszystkich kolumn
    std::list<int> help_list;

    // redukcja wierszy
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.size(); j++)
            if (m.matrix[i][j].value >= 0)
                help_list.push_front(m.matrix[i][j].value);

        reduce_row = *std::min_element(help_list.begin(), help_list.end());
        reduce_row_total += reduce_row;

        for(int j = 0; j < m.size(); j++)
            if (m.matrix[i][j].value > 0)
                m.matrix[i][j].value -= reduce_row;

        help_list.clear();
    }

    // redukcja kolumn
    for(int j = 0; j < m.size(); j++){
        for(int i = 0; i < m.size(); i++)
            if (m.matrix[i][j].value >= 0)
                help_list.push_front(m.matrix[i][j].value);
    
        reduce_column = *std::min_element(help_list.begin(), help_list.end());
        reduce_column_total += reduce_column;

        for(int i = 0; i < m.size(); i++)
            if (m.matrix[i][j].value > 0)
                m.matrix[i][j].value -= reduce_column;

        help_list.clear();
    }

    return (reduce_row_total + reduce_column_total);
}

// funkcja dla danego zera w macierzy oblicza jego bottom_limit
int BranchAndBound::bottomLimit(Matrix m, int i, int j){

    int min_row = 0, min_col = 0, min = 0;

    std::list<int> help_list;

    for(int k = 0; k < m.size(); k++)
        if(k != j && m.matrix[i][k].value >= 0)
            help_list.push_front(m.matrix[i][k].value);

    min_row = *std::min_element(help_list.begin(), help_list.end());

    help_list.clear();

    for(int k = 0; k < m.size(); k++)
        if(k != i && m.matrix[k][j].value >= 0)
            help_list.push_front(m.matrix[k][j].value);

    min_col = *std::min_element(help_list.begin(), help_list.end());
    
    min = min_col + min_row;

    return min;
}

// funkcja sprawdza czy wybór określonej krawędzi na danym etapie, nie spowodowałby powstania grafu niespójnego
bool BranchAndBound::checkIfConnected(std::list<Edge> edges){
    std::list<Edge>::iterator it;
    std::list<Edge>::iterator iter;

    for(iter = edges.begin(); iter != edges.end(); iter++){
        Edge beginEdge, iteratorEdge;
        beginEdge.od_w = iter->od_w;
        beginEdge.do_w = iter->do_w;
        iteratorEdge = beginEdge;

        for (it = edges.begin(); it != edges.end(); it++){
            if(iteratorEdge.od_w != beginEdge.od_w && iteratorEdge.do_w != beginEdge.do_w){
                if(iteratorEdge.do_w == beginEdge.od_w)
                    return false;
            }
            if(iteratorEdge.do_w == it->od_w){
                iteratorEdge.od_w = it->od_w;
                iteratorEdge.do_w = it->do_w;
                it = edges.begin();
            }
        }
    }
    return true;
}

// główna funkcja klasy BranchAndBound, uruchamia drzewo wywołań, oblicza dolne ograniczenie dla danego problemu
void BranchAndBound::algorithm(Matrix matrix){
    lower_bound = BranchAndBound::reduceRowColumn(matrix); // obliczamy ograniczenie dolne dla macierzy
    int result = lower_bound;
    std::list<Edge> edges;

    result = this->executionLeft(matrix, result, edges, true); // zaczynamy łańcuch rekurencji
    final_result = result;
}

// lewe poddrzewo
int BranchAndBound::executionLeft(Matrix& matrix, int result, std::list<Edge> edges, bool start_exec){

    int local_lower_bound = 0;
    if(start_exec == false)
        local_lower_bound = BranchAndBound::reduceRowColumn(matrix); // oblicz o ile wzrośnie dolne ograniczenie
    else // jeśli wcześniej obliczaliśmy ograniczenie dolne, nie rób tego ponownie
        start_exec = false;

    result += local_lower_bound;
    if(result >= final_result)
        return result;

    int bottom_limit = 0; // maksymalny wzrost dolnego ograniczenia dla prawego poddrzewa
    int i_red, j_red;
    int zeros_count = 0; // ile zer jest w naszej obecnej macierzy

    // szukamy bottom_limit w pętli
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(i != j && edges.size() != matrix.size() - 1){
                Edge elem;
                elem.od_w = i;
                elem.do_w = j;
                edges.push_front(elem);
                if(this->checkIfConnected(edges) == false){
                    matrix.matrix[i][j].value = -1;
                }
                edges.pop_front();
            }
            if(matrix.matrix[i][j].value == 0){ // szukamy zer w macierzy
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j); // dla danego zera obliczamy maksymalny wzrost dolnego ograniczenia dla prawego poddrzewa
                if(previous_bottom_limit > bottom_limit){ // jeśli dla danego zera bottom_limit nie jest maksymalny, to nie zmieniaj wartości dotychczasowego bottom_limit
                    bottom_limit = previous_bottom_limit;
                }
                else{ // zapisujemy wiersz i kolumnę dla nowego wyniku bottom_limit
                    i_red = i;
                    j_red = j;
                }
                zeros_count ++;
            }
        }
    }

    int resultSec = result + bottom_limit; // dolne ograniczenie dla prawego poddrzewa

    if(zeros_count > 1 && bottom_limit == 0){ // jeśli nasze dolne ograniczenie wynosi zero, ale macierz nie jest jeszcze wielkości 1x1, należy sprawdzić
                                              // która krawędź ze zbioru pozostałych ma być dodana do wyniku jako kolejna
        Matrix help_matrix(4);
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0){ // wybieramy jedno z pozostałych zer. Szukamy zera, które da najmniejszy wynik
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem); // dodajemy krawędź do zbioru wynikowego

                    help_matrix = matrix;

                    if(result <= resultSec){
                        matrix.matrix[j][i].value = -1;
                        matrix.removeColumnRow(i, j);
                        int result_temp = result;
                        result = BranchAndBound::executionLeft(matrix, result, edges);

                        if(result == -401){
                            edges.pop_front();
                            result = result_temp;
                        }
                        else if(result > final_result){ // w kolejnej iteracji pętli będziemy porównywać, które zero daje mniejszy wynik
                            return result;
                        }
                        matrix = help_matrix;
                    }
                    if(result > resultSec){ // rozpatrujemy prawe poddrzewo dla wybranego zera
                        help_matrix.matrix[i_red][j_red].value = -1;
                        if(!edges.empty())
                            edges.pop_front();
                        resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i, j, edges);
                        if(resultSec > final_result){
                            edges.pop_front();
                            return resultSec;
                        }
                    }
                }
            }
        }
    }
    else if(zeros_count > 1){ // jeśli nasza macierz ma wymiary > (1x1)

        Edge elem;
        elem.od_w = i_red;
        elem.do_w = j_red;
        edges.push_front(elem); // dodajemy krawędź do zbioru krawędzi wynikowych

        Matrix help_matrix(4);
        help_matrix = matrix;

        if(result < resultSec){ // jeśli dolne ograniczenie lewego poddrzewa jest mniejsze od prawego
            matrix.matrix[j_red][i_red].value = -1;
            matrix.removeColumnRow(i_red, j_red); // usuwamy rząd i kolumnę o wartościach takich samych jak dla wcześniej obliczonego bottom_limit
            result = BranchAndBound::executionLeft(matrix, result, edges); // rozwijamy lewe poddrzewo
        }
        if(result >= resultSec){ // jeśli okazało się, że dolne ograniczenie dla lewego poddrzewa nie jest jednak minimalne, musimy sprawdzić prawe poddrzewo
            help_matrix.matrix[i_red][j_red].value = -1;
            if(!edges.empty()) // rozwijając prawe poddrzewo nie bierzemy pod uwagę wcześniej dodanej krawędzi
                edges.pop_front();
            resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i_red, j_red, edges); // rozwijamy prawe poddrzewo
        }
    }
    else if (edges.size() == matrix.size() - 1){ // jeśli nasza macierz ma wymiary 1x1

        Edge elem;
        elem.od_w = i_red;
        elem.do_w = j_red;
        edges.push_front(elem); // dodajemy ostatnią krawędź do listy krawędzi wynikowych

        if(!finalEdges.empty()) // wcześniejsza lista krawędzi wynikowych (jeśli jest zapełniona), zostaje usunięta
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) ); // zapisujemy krawędzie wynikowe

        final_result = result; // zapisujemy otrzymany rezultat dla danego rozgałęzienia
    }
    else // jeśli nie zostało znalezione żadne zero, oznacza to, że dana krawędź nie może być dodana
        return -401; // wartość domyślna -401

    if(result > resultSec)
        return resultSec;
    else // jeśli wynik z lewego poddrzewa jest mniejszy od prawego, zapisz wynik lewego
        return result;
}

// prawe poddrzewo (działa praktycznie analogicznie jak lewe, pomijając etap początkowy)
int BranchAndBound::executionRight(Matrix& matrix, int result, int i_r, int j_r, std::list<Edge> edges, bool start_exec){

    std::list<int> help_list;
    int min_col = 0;
    int min_row = 0;
    int min = 0;

    for(int i = 0; i < matrix.size(); i++){
        if(matrix.matrix[i][j_r].value >= 0)
            help_list.push_front(matrix.matrix[i][j_r].value);
    }
    min_col = *std::min_element(help_list.begin(), help_list.end());

    for(int i = 0; i < matrix.size(); i++)
        if (matrix.matrix[i][j_r].value > 0)
            matrix.matrix[i][j_r].value -= min_col;

    help_list.clear();

    for(int j = 0; j < matrix.size(); j++){
        if(matrix.matrix[i_r][j].value >= 0)
            help_list.push_front(matrix.matrix[i_r][j].value);
    }
    min_row = *std::min_element(help_list.begin(), help_list.end());

    for(int j = 0; j < matrix.size(); j++)
        if (matrix.matrix[i_r][j].value > 0)
            matrix.matrix[i_r][j].value -= min_row;

    help_list.clear();

    int bottom_limit = 0;
    int i_red, j_red;
    int zeros_count = 0;

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(matrix.matrix[i][j].value == 0){
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j); // dla danego zera obliczamy maksymalny wzrost dolnego ograniczenia dla prawego poddrzewa
                if(previous_bottom_limit > bottom_limit){ // jeśli dla danego zera bottom_limit nie jest maksymalny, to nie zmieniaj wartości dotychczasowego bottom_limit
                    bottom_limit = previous_bottom_limit;
                }
                else{ // zapisujemy wiersz i kolumnę dla nowego wyniku bottom_limit
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem);
                    if(this->checkIfConnected(edges) == false){
                        bottom_limit = previous_bottom_limit;
                        matrix.matrix[i][j].value = -1;
                    }
                    else{
                        i_red = i;
                        j_red = j;
                    }
                    edges.pop_front();
                }
                zeros_count ++;
            }
        }
    }

    int resultSec = result + bottom_limit;

     if(zeros_count > 1 && bottom_limit == 0){
        Matrix help_matrix(4);
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0){
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem);

                    help_matrix = matrix;

                    if(result <= resultSec){
                        matrix.matrix[j][i].value = -1;
                        matrix.removeColumnRow(i, j);
                        int result_temp = result;
                        result = BranchAndBound::executionLeft(matrix, result, edges);

                        if(result == -401){
                            edges.pop_front();
                            result = result_temp;
                        }
                        else if(result > final_result){
                            return result;
                        }
                        matrix = help_matrix;
                    }
                    if(result > resultSec){
                        help_matrix.matrix[i_red][j_red].value = -1;
                        if(!edges.empty())
                            edges.pop_front();
                        resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i, j, edges);
                        if(resultSec > final_result)
                            return resultSec;
                    }
                }
            }
        }
    }
    else if(zeros_count > 1){

        Edge elem;
        elem.od_w = i_red;
        elem.do_w = j_red;
        edges.push_front(elem);

        Matrix help_matrix(4);
        help_matrix = matrix;

        if(result < resultSec){
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
    else if (edges.size() == matrix.size() - 1){
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0){
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem);
                    break;
                }
            }
        }
        if(!finalEdges.empty())
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) );

        final_result = result;
    }
    else
        return -401;

    if(result > resultSec)
        return resultSec;
    else
        return result;

}

void BranchAndBound::showResult(){
    std::cout << std::endl << "Result (B&B): " << final_result << std::endl;

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
