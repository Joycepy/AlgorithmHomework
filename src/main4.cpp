#include<iostream>
#include <algorithm>
#include<map>
using namespace std;
// <<���õ�Ǯ money����ǰ���ǵ�������� index>,<�ɿ��� res���� index ��������������� count>> 
map<pair<int, int>, pair<double, int>> res_map;

/*��⵱ǰ״̬����߿ɿ���*/
pair<double, int> solve(int money, int index, int price[], double p[])
{
	// ��ʼ״̬
	if (index == -1) {
		return { 1, 0 };
	}
	if (money < price[index]) {
		// ��������һ̨���ɿ���Ϊ0
		return { 0,0 };
	}

	pair<int, int> key(money, index);
	if (res_map.find(key) != res_map.end()) {
		return res_map[key];
	}

	double res = 0;
	double pow = 1;
	int count = 0;
	int num = 0;
	while (money >= price[index])
	{
		money -= price[index];
		++num;
		pow *= p[index];
		pair<double, int> inner = solve(money, index - 1, price, p);
		double temp = (1 - pow) * inner.first;
		if (temp > res) {
			res = temp;
			count = num;
		}
	}
	res_map[key] = { res,count };
	return { res,count };
}

/*���ݲ�����ѷ���*/
int* find_path(pair<int, int> dest, int money, int n, int price[]) {
	int* count = new int[n];
	count[n - 1] = dest.second;
	for (int i = n - 2; i >= 0; --i) {
		money -= price[i + 1] * count[i + 1];
		pair<int, int> key(money, i);
		auto it = res_map.find(key);
		count[i] = it->second.second;
	}
	return count;
}

int main() {
	// -------��ʼ��-------
	int n, C;
	cout << "����������������n = ";
	cin >> n;
	cout << "�������ܷ��ã�C = ";
	cin >> C;

	int* price = new int[n];
	double* pro = new double[n];
	cout << "������ÿ�������ĵ��ۣ�" << endl;
	for (int i = 0; i < n; ++i) {
		cin >> price[i];
	}
	cout << "������ÿ��������ʧЧ���ʣ�" << endl;
	for (int i = 0; i < n; ++i) {
		cin >> pro[i];
	}
	// -------��̬�滮-------
	pair<double, int> solution = solve(C, n - 1, price, pro);
	cout << "ϵͳ���ɿ���Ϊ��" << solution.first << endl << "���򷽰�Ϊ��" << endl;
	int *count = find_path(solution, C, n, price);
	for (int i = 0; i < n; ++i) {
		cout<<i + 1 << " ��������" << count[i] << " ��" << endl;
	}
	return 0;
}