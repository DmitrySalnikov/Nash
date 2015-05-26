#include <random>
#include <iostream>
#include <algorithm>
#include <limits>
#include <utility>

using std::cout;
using std::cin;
using std::endl;
using std::pair;

int main () {
    std::random_device g;

    char ans;
	do {
		cout << "36 places, 10 customers, 5 goods, 2 sellers, 4 marketplaces\n";

		int inf = std::numeric_limits<int>::max(); //inf - �������������

		// ������� �����{0, inf}
		int  ways[36][36] = {};
		for (int i = 0; i < 36; ++i) {
			for (int j = 0; j < 36; ++j)
				ways[i][j] = inf;
			ways[i][i] = 0;
		}

		// ������ ������� �����{0, 1, 2, inf}
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
			ways[6 * i][6 * i - 1] = inf;

		// ������������� ������� �����
		for (int i = 0; i < 36; ++i) {
			for (int j = 0; j < i; ++j)
				ways[j][i] = ways[i][j];
		}

		// ���� ������� �����
		cout << "\nmatrix of ways:\n";
		for (int i = 0; i < 36; ++i) {
			for (int j = 0; j < 36; ++j) {
				if (ways[i][j] == inf)
					cout << "* ";
				else
					cout << ways[i][j] << ' ';
			}
			cout << endl;
		}

		// �������� ������ ������ ���������� ����� 
		for(int k = 0; k < 36; ++k)
			for(int i = 0; i < 36; ++i)  
				for(int j = 0; j < 36; ++j)
					if(i != k && j != k && ways[i][k] != inf && ways[k][j] != inf)
						ways[i][j] = std::min(ways[i][j], ways[i][k] + ways[k][j]);

		// ����� ������������ �����������{0..9} � ���������{10..13}
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

		// ������� ������ ���� �����������
		int customers[10][10] = {};
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j)
				customers[i][j] = g() % 5;
			std::sort(customers[i], customers[i] + 10);
		}

		// ���� ������� ��������{10..24}
		int prices1[5] = {};
		for (int i = 0; i < 5; ++i)
			prices1[i] = g() % 15 + 10;
		std::sort(prices1, prices1 + 5);

		// ���� ������� ��������{8..26}
		int prices2[5] = {};
		for (int i = 0; i < 5; ++i)
			prices2[i] = g() % 2 ? prices1[i] + (g() % 2) : prices1[i] - (g() % 2);

		// ����� ������������� ������
		for (int i = 0; i < 9; ++i) {
			cout << "\ngoods of " << i + 1 << " customer:  ";
			for (int j = 0; j < 10; ++j) 
				cout << customers[i][j] + 1 << ' ';
		}
		cout << "\ngoods of 10 customer: ";
		for (int j = 0; j < 10; ++j) 
			cout << customers[9][j] + 1 << ' ';
		cout << endl;

		cout << "\nprices of 1 seller: ";
		for (int i = 0; i < 5; ++i)
			cout << prices1[i] << ' ';

		cout << "\nprices of 2 seller: ";
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

		// ������� ������������ �����������
		cout << "matrix of choises:\n      ";
		for (int cust = 0; cust < 10; ++cust) {
			cout.width(3);
			cout << points[cust] + 1;
		}
		cout << endl;
		bool choises[16][10] = {};
        // ������� ���������
        pair<int, int> profits[4][4] = {};
		int n = 0;
		for (int i = 0; i < 4; ++i) // ������ ��������
			for (int j = 0; j < 4; ++j) { // ������ ��������
				cout.width(2);
				cout << points[10 + i] + 1  << ", ";
				cout.width(2);
				cout << points[10 + j] + 1 << ": ";
				int cust1 = 0;
				int cust2 = 0;
				for (int cust = 0; cust < 10; ++cust) { // ����������
					int cost1 = ways[points[cust]][points[10 + i]]; // ������� �� 1 ��������
					int cost2 = ways[points[cust]][points[10 + j]]; // ������� �� 2 ��������
					for (int good = 0; good < 10; ++good) { // ������ ������
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

            cout << "\nmatrix of wins:\n";
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
    
            // ���������� �� ����
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
                cout << "\nno Nash equilibrium in clear strategy\n";

			cout << "\ngenerate new model?[y/n]: ";
            ans = cin.get();
            cin.get();
		} while (ans == 'y' || ans == 'Y');
}

