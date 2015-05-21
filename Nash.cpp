#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

using uint = unsigned int;

using std::vector;
using std::cout;
using std::endl;


struct Profit {
    int first;
    int second;
};

int main () {
    std::random_device g;

    cout << "36 places, 10 customers, 5 goods, 2 sellers, 4 marketplaces\n";

    int ex = std::numeric_limits<int>::max(); //ex - бесконечность

    int  ways[36][36] = {};

    // делаем матрицу путей
    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 36; ++j)
            ways[i][j] = ex;
        ways[i][i] = 0;
    }

    // делаем матрицу путей
    ways[0][1] = g() % 2 + 1;
    ways[0][6] = g() % 2 + 1;
    for (int i = 1; i < 6; ++i) {
        ways[i][i - 1] = g() % 2 + 1;
        ways[i][i + 1] = g() % 2 + 1;
        ways[i][i + 6] = g() % 2 + 1;
    }
    for (int i = 6; i < 30; ++i) {
        ways[i][i - 1] = g() % 2 + 1;
        ways[i][i + 1] = g() % 2 + 1;
        ways[i][i + 6] = g() % 2 + 1;
        ways[i][i - 6] = g() % 2 + 1;
    }
    for (int i = 30; i < 35; ++i) {
        ways[i][i - 1] = g() % 2 + 1;
        ways[i][i + 1] = g() % 2 + 1;
        ways[i][i - 6] = g() % 2 + 1;
    }
    ways[35][34] = g() % 2 + 1;
    ways[35][29] = g() % 2 + 1;

    for (int i = 1; i < 6; ++i) 
        ways[6 * i][6 * i - 1] = ex;

    // симметризация
    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < i; ++j)
            ways[j][i] = ways[i][j];
    }

    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 36; ++j) {
            if (ways[i][j] == ex)
                cout << "* ";
            else
                cout << ways[i][j] << ' ';
        }
        cout << endl;
    }


    // алгоритм флойда поиска кратчайших путей 
	for(int k = 0; k < 36; ++k)
		for(int i = 0; i < 36; ++i)  
			for(int j = 0; j < 36; ++j)
				if(i != k && j != k && ways[i][k] != ex && ways[k][j] != ex)
					ways[i][j] = std::min(ways[i][j], ways[i][k] + ways[k][j]);

    // точки расположения покупателей(0-9) и продавцов(10-13)
    int points[14] = {};

    for (int i = 0; i < 14; ++i) {
        points[i] = -1;
        while (true) {
            bool flag = false;
            int temp = g() % 36;
            for (int j = 0; j < i; ++j)
                if (temp == points[j])
                    flag = true;
            if (!flag) {
                points[i] = temp;
                break;
            }
        }
    }
    std::sort(points, points + 10);
    std::sort(points + 10, points + 14);

    // векторы спроса всех покупателей
    int customers[10][10] = {};

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            customers[i][j] = g() % 5;
            std::sort(customers[i], customers[i] + 10);
        }
    }

    // цены первого продавца 10-24
    int prices1[5] = {};
    
    for (int i = 0; i < 5; ++i)
        prices1[i] = g() % 15 + 10;
    std::sort(prices1, prices1 + 5);

    // цены второго продавца 5-29
    int prices2[5] = {};
    
    for (int i = 0; i < 5; ++i)
        prices2[i] = g() % 2 ? prices1[i] + (g() % 6) : prices1[i] - (g() % 6);

    // матрица выигрышей обоих продавцов
    Profit profits[4][4] = {};

    // построение матрицы выигрышей
    for (int i = 0; i < 4; ++i) // первый продавец
        for (int j = 0; j < 4; ++j) { // второй продавец
            for (int cust = 0; cust < 10; ++cust) { // покопатели
                int point = points[cust]; // точка, где был покупатель в предыдущий момент времени

                for (int good = 0; good < 10; ++ good) { // вектор спроса
                    if (ways[point][points[10 + i]] + prices1[customers[cust][good]] < ways[point][points[10 + j]] + prices2[customers[cust][good]]) {
                        profits[i][j].first += prices1[customers[cust][good]];
                        point = points[10 + i];
                    }
                    else if (ways[point][points[10 + i]] + prices1[customers[cust][good]] > ways[point][points[10 + j]] + prices2[customers[cust][good]]) {
                        profits[i][j].second += prices2[customers[cust][good]];
                        point = points[10 + j];
                }
                    else {
                        int temp = g() % 2; // temp = 0 или 1
                        profits[i][j].first += temp * prices1[customers[cust][good]];
                        profits[i][j].second += (1 - temp) * prices2[customers[cust][good]];                   
                        point = points[10 + temp * i + (1 - temp) * j];
                    }
                }
            }
        }
    
    cout << endl;
    for (int i = 0; i < 10; ++i) {
        cout << "goods of " << i << " customer: ";
        for (int j = 0; j < 10; ++j) 
            cout << customers[i][j] << ' ';
        cout << endl;
    }

    cout << "\nprices of 1 seller: ";
    for (int i = 0; i < 5; ++i)
        cout << i << " - " << prices1[i] << "; ";
    cout << endl;

    cout << "\nprices of 2 seller: ";
    for (int i = 0; i < 5; ++i)
        cout << i << " - " << prices2[i] << "; ";
    cout << endl << endl;

    cout << "points of customers: ";
    for (int i = 0; i < 10; ++i)
        cout << points[i] << " ";
    cout << endl;

    cout << "\npoints of sellers: ";
    for (int i = 10; i < 14; ++i)
        cout << points[i] << " ";
    cout << endl << endl;

    for (int j = 0; j < 4; ++j) {
        cout << '\t' << points[10 + j] << '\t';
    }
    cout << endl;
    for (int i = 0; i < 4; ++i) {
        cout.width(2);
        cout << points[10 + i] << ' ';
        for (int j = 0; j < 4; ++j) 
            cout << "(" << profits[i][j].first << ", " << profits[i][j].second << ")\t";
        cout << endl;
    }
    
    // равновесие по Нешу
    bool flg = false;
    for (int i = 0; i < 4; ++i) {
        int max1 = -1;
        for (int j = 0; j < 4; ++j)
            if (profits[i][j].second > max1)
                max1 = profits[i][j].second;
        for (int j = 0; j < 4; ++j) {
            int max2 = -1;
            for (int k = 0; k < 4; ++k)
                if (profits[k][j].first > max2)
                    max2 = profits[k][j].first;
            if ((profits[i][j].first == max2) && (profits[i][j].second == max1)) {
                cout << "\nNash equilibrium in (" << points[10 + i] << ", " << points[10 + j] << ')' << endl;
                flg = true;
            }
        }
    }
    if (!flg)
        cout << "\nno Nash equilibrium in clear strategy\n";
}
