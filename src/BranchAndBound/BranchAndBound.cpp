#include <BranchAndBound.hpp>
#include <BranchAndBound.hpp>
#include <BranchAndBound.hpp>
#include <Matrix.hpp>
#include <algorithm>
#include <list>
#include <iterator>
#include <limits>

BranchAndBound::BranchAndBound(){
    final_result = INT_MAX;
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja zwraca tzn. lower bound, czyli dolne ograniczenie dla problemu komiwojażera
// Funkcja dodatkowo redukuje kolumny i wiersze macierzy
//--------------------------------------------------------------------------------------------------------------------------
int BranchAndBound::reduceRowColumn(Matrix& m){
    int reduce_row = 0; // koszt redukcji wybranego wiersza
    int reduce_row_total = 0; // koszt redukcji wszystkich wierszy
    int reduce_column = 0; // koszt redukcji wybranej kolumny
    int reduce_column_total = 0; // koszt redukcji wszystkich kolumn
    std::list<int> help_list;

    // redukcja wierszy
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.size(); j++)
            if (i != j && m.matrix[i][j].value != -1)
                help_list.push_front(m.matrix[i][j].value); // wpisujemy wartości z wiersza na listę pomocniczą

        reduce_row = *std::min_element(help_list.begin(), help_list.end()); // szukamy w wierszu wartości minimalnej
        reduce_row_total += reduce_row; // wynik ze wszystkich wierszy jest aktualizowany

        for(int j = 0; j < m.size(); j++)
            if (i != j && m.matrix[i][j].value != -1)
                m.matrix[i][j].value -= reduce_row; // redukujemy elementy wiersza o najmniejszą wartość

        help_list.clear();
    }

    // redukcja kolumn
    for(int j = 0; j < m.size(); j++){
        for(int i = 0; i < m.size(); i++)
            if (i != j && m.matrix[i][j].value != -1)
                help_list.push_front(m.matrix[i][j].value); // wpisujemy wartości z wiersza na listę pomocniczą
    
        reduce_column = *std::min_element(help_list.begin(), help_list.end()); // szukamy w kolumnie wartości minimalnej
        reduce_column_total += reduce_column; // wynik ze wszystkich kolumn jest aktualizowany

        for(int i = 0; i < m.size(); i++)
            if (i != j && m.matrix[i][j].value != -1)
                m.matrix[i][j].value -= reduce_column; // redukujemy elementy kolumny o najmniejszą wartość

        help_list.clear();
    }

    return (reduce_row_total + reduce_column_total); // Zwracamy ogólny wynik redukcji (LB)
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja dla danego zera w macierzy oblicza jego bottom_limit (wzrost dolnego ograniczenia w prawym poddrzewie)
//--------------------------------------------------------------------------------------------------------------------------
int BranchAndBound::bottomLimit(Matrix m, int i, int j){ // wiersz i oraz kolumna j dla danego zera

    int min_row = 0, min_col = 0, min = 0;

    std::list<int> help_list;

    for(int k = 0; k < m.size(); k++)
        if(k != j && m.matrix[i][k].value != -1 && i != k)
            help_list.push_front(m.matrix[i][k].value); // wpisujemy wartości z wiersza "i" na listę pomocniczą

    min_row = *std::min_element(help_list.begin(), help_list.end()); // szukamy minimalnej wartości w wierszu "i"

    help_list.clear();

    for(int k = 0; k < m.size(); k++)
        if(k != i && m.matrix[k][j].value != -1 && j != k)
            help_list.push_front(m.matrix[k][j].value); // wpisujemy wartości z kolumny "j" na listę pomocniczą

    min_col = *std::min_element(help_list.begin(), help_list.end()); // szukamy minimalnej wartości w kolumnie "j"
    
    min = min_col + min_row; // bottom_limit

    return min;
}

//--------------------------------------------------------------------------------------------------------------------------
// Funkcja sprawdza czy wybór określonej krawędzi na danym etapie, nie spowodowałby powstania grafu niespójnego
//--------------------------------------------------------------------------------------------------------------------------
bool BranchAndBound::checkIfConnected(std::list<Edge> edges){
    std::list<Edge>::iterator it;
    std::list<Edge>::iterator iter;

    for(iter = edges.begin(); iter != edges.end(); iter++){

        Edge beginEdge, iteratorEdge;
        beginEdge.od_w = iter->od_w; // ścieżkę rozpoczyna krawędź początkowa
        beginEdge.do_w = iter->do_w; // ścieżkę rozpoczyna krawędź początkowa
        iteratorEdge = beginEdge; // iteratorEdge- ostatnia dodana do ścieżki krawędź
        
        bool newloop = true;
        for (it = edges.begin(); ; it++){ // pobieramy kolejne krawędzie
            if(newloop == true){
                it = edges.begin();
                newloop = false;
            }
            if(iteratorEdge.od_w != beginEdge.od_w && iteratorEdge.do_w != beginEdge.do_w){
                if(iteratorEdge.do_w == beginEdge.od_w) // jeśli wracamy do krawędzi początkowej, to oznacza, że graf jest niespójny
                    return false;
            }
            if(iteratorEdge.do_w == it->od_w){ // sprawdzamy czy dla aktualnego wierzchołka w, którym się znajdujemy, istnieje połączenie
                                               // z innym
                iteratorEdge.od_w = it->od_w; // przechodzimy do kolejnej krawędzi
                iteratorEdge.do_w = it->do_w; // przechodzimy do kolejnej krawędzi
                newloop = true; // dla wybranego iteratorEdge zaczynamy od nowa sprawdzanie możliwych połączeń
            }
            if(it == edges.end()){
                break;
            }
        }
    }
    return true; // jeśli istnieje tylko jedna ścieżka łącząca aktualne wierzchołki, to graf jest na danym etapie spójny
}

//--------------------------------------------------------------------------------------------------------------------------
// Główna funkcja klasy BranchAndBound, uruchamia drzewo wywołań, oblicza dolne ograniczenie dla danego problemu
//--------------------------------------------------------------------------------------------------------------------------
void BranchAndBound::algorithm(Matrix matrix){
    lower_bound = BranchAndBound::reduceRowColumn(matrix); // obliczamy ograniczenie dolne dla macierzy
    int result = lower_bound;
    std::list<Edge> edges;

    result = this->executionLeft(matrix, result, edges, true); // zaczynamy łańcuch rekurencji
    final_result = result; // wynik ostateczny
}

//--------------------------------------------------------------------------------------------------------------------------
// Lewe poddrzewo
//--------------------------------------------------------------------------------------------------------------------------
int BranchAndBound::executionLeft(Matrix& matrix, int result, std::list<Edge> edges, bool start_exec){

    int local_lower_bound = 0; // koszt redukcji macierzy na danym etapie
    if(start_exec == false)
        local_lower_bound = BranchAndBound::reduceRowColumn(matrix); // oblicz o ile wzrośnie dolne ograniczenie
    else // jeśli wcześniej obliczaliśmy ograniczenie dolne, nie robimy tego ponownie (w przypadku rozpoczęcia drzewa wywołań)
        start_exec = false;

    result += local_lower_bound;
    if(result >= final_result) // jeśli wynik na danej gałęzi drzewa jest większy od wcześniej obliczonego optymalnego drzewa
        return result; // to należy wyjść z gałęzi

    int bottom_limit = 0; // maksymalny wzrost dolnego ograniczenia dla prawego poddrzewa
    int i_red, j_red; // indeksy dla zera, dla którego znaleziono bottom_limit
    int zeros_count = 0; // ile zer jest w naszej obecnej macierzy

    // szukamy bottom_limit w pętli
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(i != j && edges.size() != matrix.size() - 1){ // dla każdego krawędzi, należy sprawdzić, czy dodanie jej do aktualnego wyniku
                                                             // nie utworzy grafu niespójnego
                Edge elem;
                elem.od_w = i;
                elem.do_w = j;
                edges.push_front(elem);
                if(this->checkIfConnected(edges) == false){ // sprawdzanie spójności grafu
                    matrix.matrix[i][j].value = -1; // niespójny - usuwamy krawędź z macierzy
                }
                edges.pop_front();
            }
            if(matrix.matrix[i][j].value == 0 && i != j){ // szukamy zer w macierzy
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j); // dla danego zera obliczamy wzrost dolnego ograniczenia dla prawego poddrzewa
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
                if(matrix.matrix[i][j].value == 0 && i != j){ // wybieramy jedno z pozostałych zer. Szukamy zera, które da najmniejszy wynik
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem); // dodajemy krawędź do zbioru wynikowego

                    help_matrix = matrix;

                    if(result <= resultSec){ // Jeśli na danym etapie wynik z lewego poddrzewa jest bardziej optymalny niż ten z prawego
                        matrix.matrix[j_red][i_red].value = -1; // ponieważ wybraliśmy krawędź (i,j), nie możemy przejść z "j" do "i"
                        matrix.removeColumnRow(i, j); // usuwamy wiersz "i" i kolumnę "j"
                        int result_temp = result;
                        result = BranchAndBound::executionLeft(matrix, result, edges); // rozwijamy dalej lewe poddrzewo

                        if(result == -401){ // nie zostało znalezione żadne zero w macierzy
                            edges.pop_front(); // rozważaną wcześniej krawędź nie bierzemy pod uwagę
                            result = result_temp;
                        }
                        else if(result > final_result){ // jeśli dla danego "zera" wynik jest do tej pory najbardziej optymalny ze wszystkich zer
                            return result; // zwróć najmniejszy wynik
                        }
                        matrix = help_matrix;
                    }
                    if(result > resultSec){ // po powrocie z lewego poddrzewa może się zdarzyć, że gałąż lewa daje mniej optymalny wynik niż prawa
                        help_matrix.matrix[i_red][j_red].value = -1; // nie idziemy przez krawędź (i,j)
                        if(!edges.empty())
                            edges.pop_front();
                        resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i, j, edges); // rozwijamy prawe poddrzewo
                        if(resultSec > final_result){ // jeśli po rozwinięciu prawego poddrzewa, wynik jest dalje bardziej optymalny niż ten
                                                      // z prawego
                            edges.pop_front();
                            return resultSec; // zwracamy najbardziej optymalny wynik
                        }
                    }
                }
            }
        }
    }
    else if(zeros_count > 1){ // jeśli nasza macierz ma wymiary > (1x1) i bottom_limit != 0

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
        edges.push_front(elem); // dodajemy ostatnią krawędź do listy tymczasowych krawędzi wynikowych

        if(!finalEdges.empty()) // wcześniejsza lista krawędzi wynikowych (jeśli jest zapełniona), zostaje usunięta
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) ); // zapisujemy ostateczne krawędzie wynikowe

        final_result = result; // zapisujemy otrzymany rezultat dla danego rozgałęzienia
    }
    else // jeśli nie zostało znalezione żadne zero, oznacza to, że dana krawędź nie może być dodana
        return -401; // wartość domyślna -401

    if(result > resultSec)
        return resultSec;
    else // jeśli wynik z lewego poddrzewa jest mniejszy od prawego, zwróć wynik z lewego poddrzewa
        return result;
}

//--------------------------------------------------------------------------------------------------------------------------
// prawe poddrzewo (działa praktycznie analogicznie jak lewe, pomijając etap początkowy)
//--------------------------------------------------------------------------------------------------------------------------
int BranchAndBound::executionRight(Matrix& matrix, int result, int i_r, int j_r, std::list<Edge> edges, bool start_exec){

    // (i_r, j_r) to usunięta krawędź grafu przed wejściem do prawego poddrzewa

    std::list<int> help_list;
    int min_col = 0;
    int min_row = 0;
    int min = 0;

    // jeśli usuniemy krawędź i_r oraz j_r, trzeba ponownie zredukować macierz

    for(int i = 0; i < matrix.size(); i++){
        if(matrix.matrix[i][j_r].value != -1 && i != j_r)
            help_list.push_front(matrix.matrix[i][j_r].value);
    }
    min_col = *std::min_element(help_list.begin(), help_list.end()); // obliczamy minimalną wartość dla kolumy j_r

    for(int i = 0; i < matrix.size(); i++)
        if (matrix.matrix[i][j_r].value != -1 && i != j_r)
            matrix.matrix[i][j_r].value -= min_col; // redukcja kolumny j_r

    help_list.clear();

    for(int j = 0; j < matrix.size(); j++){
        if(matrix.matrix[i_r][j].value != -1 && j != i_r)
            help_list.push_front(matrix.matrix[i_r][j].value);
    }
    min_row = *std::min_element(help_list.begin(), help_list.end()); // obliczamy minimalną wartość dla wiersza i_r

    for(int j = 0; j < matrix.size(); j++)
        if (matrix.matrix[i_r][j].value != -1 && j != i_r)
            matrix.matrix[i_r][j].value -= min_row; // redukcja wiersza i_r

    help_list.clear();

    int bottom_limit = 0;
    int i_red, j_red;
    int zeros_count = 0;

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix.size(); j++){
            if(i != j && edges.size() != matrix.size() - 1){ // dla każdego krawędzi, należy sprawdzić, czy dodanie jej do aktualnego wyniku
                                                             // nie utworzy grafu niespójnego
                Edge elem;
                elem.od_w = i;
                elem.do_w = j;
                edges.push_front(elem);
                if(this->checkIfConnected(edges) == false){ // sprawdzanie spójności grafu
                    matrix.matrix[i][j].value = -1; // niespójny - usuwamy krawędź z macierzy
                }
                edges.pop_front();
            }
            if(matrix.matrix[i][j].value == 0 && i != j){ // szukamy zer w macierzy
                int previous_bottom_limit = bottom_limit;

                bottom_limit = BranchAndBound::bottomLimit(matrix, i, j); // dla danego zera obliczamy wzrost dolnego ograniczenia dla prawego poddrzewa
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

    int resultSec = result + bottom_limit;

     if(zeros_count > 1 && bottom_limit == 0){ // jeśli nasze dolne ograniczenie wynosi zero, ale macierz nie jest jeszcze wielkości 1x1, należy sprawdzić
                                              // która krawędź ze zbioru pozostałych ma być dodana do wyniku jako kolejna
        Matrix help_matrix(4);
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix.size(); j++){
                if(matrix.matrix[i][j].value == 0 && i != j){ // wybieramy jedno z pozostałych zer. Szukamy zera, które da najmniejszy wynik
                    Edge elem;
                    elem.od_w = i;
                    elem.do_w = j;
                    edges.push_front(elem); // dodajemy krawędź do zbioru wynikowego

                    help_matrix = matrix;

                    if(result <= resultSec){ // Jeśli na danym etapie wynik z lewego poddrzewa jest bardziej optymalny niż ten z prawego
                        matrix.matrix[j_red][i_red].value = -1; // ponieważ wybraliśmy krawędź (i,j), nie możemy przejść z "j" do "i"
                        matrix.removeColumnRow(i, j); // usuwamy wiersz "i" i kolumnę "j"
                        int result_temp = result;
                        result = BranchAndBound::executionLeft(matrix, result, edges); // rozwijamy dalej lewe poddrzewo

                        if(result == -401){ // nie zostało znalezione żadne zero w macierzy
                            edges.pop_front(); // rozważaną wcześniej krawędź nie bierzemy pod uwagę
                            result = result_temp;
                        }
                        else if(result > final_result){ // jeśli dla danego "zera" wynik jest do tej pory najbardziej optymalny ze wszystkich zer
                            return result; // zwróć najmniejszy wynik
                        }
                        matrix = help_matrix;
                    }
                    if(result > resultSec){ // po powrocie z lewego poddrzewa może się zdarzyć, że gałąż lewa daje mniej optymalny wynik niż prawa
                        help_matrix.matrix[i_red][j_red].value = -1; // nie idziemy przez krawędź (i,j)
                        if(!edges.empty())
                            edges.pop_front();
                        resultSec = BranchAndBound::executionRight(help_matrix, resultSec, i, j, edges); // rozwijamy prawe poddrzewo
                        if(resultSec > final_result){ // jeśli po rozwinięciu prawego poddrzewa, wynik jest dalje bardziej optymalny niż ten
                                                      // z prawego
                            edges.pop_front();
                            return resultSec; // zwracamy najbardziej optymalny wynik
                        }
                    }
                }
            }
        }
    }
    else if(zeros_count > 1){ // jeśli nasza macierz ma wymiary > (1x1) i bottom_limit != 0

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
        edges.push_front(elem); // dodajemy ostatnią krawędź do listy tymczasowych krawędzi wynikowych

        if(!finalEdges.empty()) // wcześniejsza lista krawędzi wynikowych (jeśli jest zapełniona), zostaje usunięta
            finalEdges.clear();

        std::copy( edges.begin(), edges.end(), std::back_inserter(finalEdges) ); // zapisujemy ostateczne krawędzie wynikowe

        final_result = result; // zapisujemy otrzymany rezultat dla danego rozgałęzienia
    }
    else // jeśli nie zostało znalezione żadne zero, oznacza to, że dana krawędź nie może być dodana
        return -401; // wartość domyślna -401

    if(result > resultSec)
        return resultSec;
    else // jeśli wynik z lewego poddrzewa jest mniejszy od prawego, zwróć wynik z lewego poddrzewa
        return result;

}

//--------------------------------------------------------------------------------------------------------------------------
// funkcja wyświetlająca wynikową drogę oraz kolejne krawędzie, jakie należy przejść
//--------------------------------------------------------------------------------------------------------------------------
void BranchAndBound::showResult(){
    std::cout << std::endl << "Result (B&B): " << final_result << std::endl;

    std::cout << "Edges: " << std::endl;
    std::list<Edge>::iterator it;
    std::list<Edge>::iterator iter;
    std::list<Edge>::iterator beginiter;

    for (iter = finalEdges.begin(); iter != finalEdges.end(); iter++){
        if(iter->od_w == 0){
            std::cout << iter->od_w << " - ";
            std::cout << iter->do_w;
            it = iter;
            break;
        }
    }
    bool newloop = true;
    for (iter = finalEdges.begin(); ; iter++){
        if(newloop == true){
            iter = finalEdges.begin();
            newloop = false;
        }
        if(it->do_w == 0){
            break;
        }
        if(it->do_w == iter->od_w){
            std::cout << " - " <<iter->do_w;
            it = iter;
            newloop = true;
        }
    }
}
