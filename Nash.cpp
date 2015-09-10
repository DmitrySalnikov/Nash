#include <random>
#include <iostream>
#include <algorithm>
#include <limits>
#include <utility>

using std::cout;
using std::cin;
using std::endl;
using std::pair;

std::random_device g;

int way() {
  return g() % 2 + 1;
}

int main () {
  int const sz = 45;

  char ans;
	do {
		cout << "9x5 places, 10 customers, 5 goods, 2 sellers, 4 marketplaces\n";

		int inf = std::numeric_limits<int>::max(); //infinity

		// matrix ways{0, inf}
		int ways[sz][sz] = {};
		for (int i = 0; i < sz; ++i) {
			for (int j = 0; j < sz; ++j)
				ways[i][j] = inf;
			ways[i][i] = 0;
		}

		// do matrix ways{0, 1, 2, inf}
		ways[0][1] = g() % 2 + 1;
		ways[0][5] = g() % 2 + 1;
		for (int i = 1; i < 4; ++i) {
			ways[i][i - 1] = g() % 2 + 1;
			ways[i][i + 1] = g() % 2 + 1;
			ways[i][i + 5] = g() % 2 + 1;
		}
    ways[4][3] = way();
    ways[4][9] = way();
		for (int i = 5; i < 40; ++i) {
			ways[i][i - 1] = g() % 2 + 1;
			ways[i][i + 1] = g() % 2 + 1;
			ways[i][i + 5] = g() % 2 + 1;
			ways[i][i - 5] = g() % 2 + 1;
		}
		for (int i = 1; i < 8; ++i) {
			ways[5 * i][5 * i - 1] = inf;
			ways[5 * (i + 1) - 1][5 * (i + 1)] = inf;
    }
    ways[40][35] = way();
    ways[40][41] = way();
		for (int i = 41; i < 44; ++i) {
			ways[i][i - 1] = g() % 2 + 1;
			ways[i][i + 1] = g() % 2 + 1;
			ways[i][i - 5] = g() % 2 + 1;
		}
		ways[44][43] = g() % 2 + 1;
		ways[44][39] = g() % 2 + 1;

		// symmetrization matrix ways
		for (int i = 1; i < sz; ++i) {
			for (int j = 0; j < i; ++j)
				ways[j][i] = ways[i][j];
		}

		// output matrix ways
		cout << "\nmatrix ways:\n";
		for (int i = 0; i < sz; ++i) {
			for (int j = 0; j < sz; ++j) {
				if (ways[i][j] == inf)
					cout << "* ";
				else
					cout << ways[i][j] << ' ';
			}
			cout << endl;
		}

		// Floyd's algorithm 
		for(int k = 0; k < sz; ++k)
			for(int i = 0; i < sz; ++i)  
				for(int j = 0; j < sz; ++j)
					if(i != k && j != k && ways[i][k] != inf && ways[k][j] != inf)
						ways[i][j] = std::min(ways[i][j], ways[i][k] + ways[k][j]);

		// points of customers{0..9} and sellers{10..13}
		int points[14] = {};
		for (int i = 0; i < 14; ++i) {
			points[i] = -1;
			while (true) {
				bool flag = false;
				int temp = g() % sz;
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

		// vectors demands
		int customers[10][10] = {};
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j)
				customers[i][j] = g() % 5;
			std::sort(customers[i], customers[i] + 10);
		}

		// prices of first seller{10..24}
		int prices1[5] = {};
		for (int i = 0; i < 5; ++i)
			prices1[i] = g() % 15 + 10;
		std::sort(prices1, prices1 + 5);

		// prices of second seller{8..26}
		int prices2[5] = {};
		for (int i = 0; i < 5; ++i)
			prices2[i] = g() % 2 ? prices1[i] + (g() % 2) : prices1[i] - (g() % 2);

		// output characteristics of model
		for (int i = 0; i < 9; ++i) {
			cout << "\ngoods of " << i + 1 << " customer:  ";
			for (int j = 0; j < 10; ++j) 
				cout << customers[i][j] + 1 << ' ';
		}
		cout << "\ngoods of 10 customer: ";
		for (int j = 0; j < 10; ++j) 
			cout << customers[9][j] + 1 << ' ';
		cout << endl;

		cout << "\nprices of first seller: ";
		for (int i = 0; i < 5; ++i)
			cout << prices1[i] << ' ';

		cout << "\nprices of second seller: ";
		for (int i = 0; i < 5; ++i) {
			cout.width(2);
			cout << prices2[i] << ' ';
		}
		cout << endl;

		cout << "\npoints of customers: ";
		for (int i = 0; i < 10; ++i)
			cout << points[i]  + 1 << " ";

		cout << "\npoints of sellers: ";
		for (int i = 10; i < 14; ++i)
			cout << points[i] + 1 << " ";
		cout << endl << endl;

		// matrix preferences
		cout << "matrix preferences:\n      ";
		for (int cust = 0; cust < 10; ++cust) {
			cout.width(3);
			cout << points[cust] + 1;
		}
		cout << endl;
		bool choises[16][10] = {};
        // matrix wins
        pair<int, int> profits[4][4] = {};
		int n = 0;
		for (int i = 0; i < 4; ++i) // first seller
			for (int j = 0; j < 4; ++j) { // second seller
				cout.width(2);
				cout << points[10 + i] + 1  << ", ";
				cout.width(2);
				cout << points[10 + j] + 1 << ": ";
				int cust1 = 0;
				int cust2 = 0;
				for (int cust = 0; cust < 10; ++cust) { // customers
					int cost1 = ways[points[cust]][points[10 + i]]; // costs by first seller
					int cost2 = ways[points[cust]][points[10 + j]]; // costs by second seller
					for (int good = 0; good < 10; ++good) { // vector demands
						cost1 += prices1[customers[cust][good]];
						cost2 += prices2[customers[cust][good]];
					}
					if (cost1 < cost2) {
						choises[n][cust] = 0;
						++cust1;
                        profits[i][j].first += cost1 - ways[points[cust]][points[10 + i]];
					}
					else if (cost1 > cost2) {
						choises[n][cust] = 1;
						++cust2;
                        profits[i][j].second += cost2 - ways[points[cust]][points[10 + j]];
					}
					else {
						if (g() % 2) { 
							choises[n][cust] = 1;
							++cust2;
                            profits[i][j].second += cost2 - ways[points[cust]][points[10 + j]];
						}
						else {
							choises[n][cust] = 0;
							++cust1;
                            profits[i][j].first += cost1 - ways[points[cust]][points[10 + i]];
						}

					}
					cout << choises[n][cust] + 1 << "  ";
				}
				cout << cust1 << ':' << cust2 << endl;
			n += 1;
			}

            cout << "\nmatrix wins:\n";
            for (int j = 0; j < 4; ++j) {
                cout << "        ";
                cout.width(2);
                cout << points[10 + j] + 1 << "    ";
            }
            cout << endl;
            for (int i = 0; i < 4; ++i) {
                cout.width(2);
                cout << points[10 + i] + 1 << "  ";
                for (int j = 0; j < 4; ++j) {
                    cout << "(";
                    cout.width(4);
                    cout << profits[i][j].first << ", "; 
                    cout.width(4);
                    cout << profits[i][j].second << ")  ";
                }
                cout << endl;
            }
    
            // Nash equilibrium
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
                        cout << "\nNash equilibrium in (" << points[10 + i] + 1 << ", " << points[10 + j] + 1 << ')' << endl;
                        flg = true;
                    }
                }
            }
            if (!flg)
                cout << "\nno Nash equilibrium in clear strategies\n";

			cout << "\ngenerate new model?[y/n]: ";
            ans = cin.get();
            cin.get();
		} while (ans == 'y' || ans == 'Y');
}

