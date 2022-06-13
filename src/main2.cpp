#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;

struct Node {
	int** state;
	int diff;
	int depth;
	// 上一次移动的反方向
	int nd = -1;
};

/**
 * A*算法求解八数码问题
 *
 * @author NKU-DBIS, pichunying
 * @date 2022/4/9
 */

 /**
  * 移动方向 左0 右1 下2 上3
  */
const int moveX[4] = { -1, 1, 0, 0 };
const int moveY[4] = { 0, 0, -1, 1 };
const int verseD[4] = { 1, 0, 3, 2 };

vector<Node> open;
vector<Node> close;
/**
 * 初始状态
 */
const int initState[3][3] = { {2, 8, 3}, {1, 6, 4}, {7, 0, 5} };
/**
 * 目标状态
 */
const int goalState[3][3] = { {2, 8, 3}, {1, 4, 5}, {7, 6, 0} };

/**
* 目标状态：值-位置
*/

const map<int, vector<int>> goalDic{
	 {2,  {0, 0}},
	 {8,{0, 1}},
	{3,{0, 2}},
	{1,{1, 0}},
	{4,{1, 1}},
	{5,{1, 2}},
	{7,{2, 0}},
	{6,{2, 1}},
	{0,{2, 2}}

};

/**
 * 计算曼哈顿距离
 *
 * @param node 源
 */
void getDistance(Node& node) {
	int dis = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (node.state[i][j] != 0) {
				auto it = goalDic.find(node.state[i][j]);
				dis += abs(it->second[0] - i) + abs(it->second[1] - j);
			}
		}
	}
	node.diff = dis;
}

int* getZeroPos(Node node) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (node.state[i][j] == 0) {
				return new int[] {i, j};
			}
		}
	}
	return nullptr;
}

int** cloneArray(int** state) {
	int **res = new int*[3];
	for (int i = 0;i < 3;i++) {
		res[i] = new int[3];
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i][j] = state[i][j];
		}
	}
	return res;
}
bool deepEqual(Node const& node1, Node const& tmp) {
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;i < 3;j++) {
			if (node1.state[i][j] != tmp.state[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
bool cmp(Node const& a, Node const& b) {
	return a.depth + a.diff > b.depth + b.diff;
}
void expand(Node node) {
	int* zeroPos = getZeroPos(node);
	int x = zeroPos[0];
	int y = zeroPos[1];
	for (int d = 0; d < 4; d++) {
		if (node.nd == d) {
			continue;
		}
		int newX = x + moveX[d];
		int newY = y + moveY[d];
		if (newX < 0 || newX >= 3 || newY < 0 || newY >= 3) {
			continue;
		}
		Node tmp;
		tmp.state = cloneArray(node.state);
		tmp.nd = verseD[d];
		tmp.state[newX][newY] = 0;
		tmp.state[x][y] = node.state[newX][newY];
		getDistance(tmp);
		tmp.depth = node.depth + 1;
		// 检查open中是否已存在
		bool exist = false;
		for (auto it = open.begin();it != open.end();it++) {
			bool equal = deepEqual(*it, tmp);
			if (equal) {
				if (tmp.depth + tmp.diff < it->depth + it->diff) {
					// 出现代价更小的路径，移除之前的
					open.erase(it);
				}
				else {
					exist = true;
				}
			}
		}
		if (!exist) {
			open.push_back(tmp);
		}
		// 排序
		sort(open.begin(), open.end(), cmp);
	}
}

Node initNode() {
	Node node;
	node.depth = 0;
	node.state = new int*[3];
	for (int i = 0;i < 3;i++) {
		node.state[i] = new int[3];
		for (int j = 0;j < 3;j++) {
			node.state[i][j] = initState[i][j];
		}
	}
	getDistance(node);
	return node;
}

void print(Node node) {
	int** state = node.state;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << state[i][j] << " ";
		}
		cout << endl;
	}
	cout << "g(x) = " << node.depth << ", h * (x) = " << node.diff << endl << endl;
}

void solution() {
	Node node = initNode();
	open.push_back(node);
	while (!open.empty()) {
		Node tmp = open.back();
		close.push_back(tmp);
		open.pop_back();
		bool equal = true;
		for (int i = 0;i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				if (tmp.state[i][j] != goalState[i][j]) {
					equal = false;
					break;
				}
			}
		}
		if (equal){
			for (Node value : close) {
				print(value);
			}
			return;
		}
		expand(tmp);
	}
}

void main() {
	solution();
}