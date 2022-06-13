#include<iostream>
#include <algorithm>
#include<map>
using namespace std;
// <<可用的钱 money，当前考虑的器件编号 index>,<可靠性 res，第 index 个器件购买的数量 count>> 
map<pair<int, int>, pair<double, int>> res_map;

/*求解当前状态下最高可靠度*/
pair<double, int> solve(int money, int index, int price[], double p[])
{
	// 初始状态
	if (index == -1) {
		return { 1, 0 };
	}
	if (money < price[index]) {
		// 不足以买一台，可靠性为0
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

/*回溯查找最佳方案*/
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
	// -------初始化-------
	int n, C;
	cout << "请输入器件个数：n = ";
	cin >> n;
	cout << "请输入总费用：C = ";
	cin >> C;

	int* price = new int[n];
	double* pro = new double[n];
	cout << "请输入每个部件的单价：" << endl;
	for (int i = 0; i < n; ++i) {
		cin >> price[i];
	}
	cout << "请输入每个部件的失效概率：" << endl;
	for (int i = 0; i < n; ++i) {
		cin >> pro[i];
	}
	// -------动态规划-------
	pair<double, int> solution = solve(C, n - 1, price, pro);
	cout << "系统最大可靠性为：" << solution.first << endl << "购买方案为：" << endl;
	int *count = find_path(solution, C, n, price);
	for (int i = 0; i < n; ++i) {
		cout<<i + 1 << " 号器件：" << count[i] << " 个" << endl;
	}
	return 0;
}