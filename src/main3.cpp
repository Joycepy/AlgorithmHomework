#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

struct Edge {
    int start;
    int end;
    int cost;

    Edge() {
        this->start = 0;
        this->end = 0;
        this->cost = 0;
    }
};

vector<Edge> edgeVec; //带权有向图边的集合
ofstream outfile("log.txt");

//根据输入文件读取图的边信息
void getEdge(vector<Edge>& edgeVec) {
    int sideNum = 0;
    cout << "请输入边的总数：" << endl;
    cin >> sideNum;
    cout << "请输入每条边的起点、终点、代价：" << endl;
    outfile << "请输入边的总数：" << endl;
    outfile << sideNum << endl;
    outfile << "请输入每条边的起点、终点、代价：" << endl;
    for (int i = 0; i < sideNum; ++i) {
        Edge edgeTemp;
        cin >> edgeTemp.start >> edgeTemp.end >> edgeTemp.cost;
        outfile << edgeTemp.start << " " << edgeTemp.end << " " << edgeTemp.cost << endl;
        edgeVec.push_back(edgeTemp);
    }
}

//根据图的边信息构造邻接矩阵
void getMatrix(vector<Edge>& edgeVec, int* (*&matrix), int& nodeNum) {
    set<int> nodeSet;
    for (int i = 0; i < edgeVec.size(); ++i) {
        nodeSet.insert(edgeVec[i].start);
        nodeSet.insert(edgeVec[i].end);
    }
    nodeNum = nodeSet.size();
    //初始化
    matrix = new int* [nodeNum];
    for (int i = 0; i < nodeNum; ++i) {
        matrix[i] = new int[nodeNum];
        for (int j = 0; j < nodeNum; ++j) {
            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = INT_MAX;
        }
    }
    //赋值
    for (auto it = edgeVec.begin(); it != edgeVec.end(); ++it) {
        matrix[it->start][it->end] = it->cost;
    }
}

void djkstra(int* (*matrix), int nodeNum) {
    //顶点startID到其它定点距离,后一个int表示当前节点的前一个节点
    pair<int, int>* distance = new pair<int, int>[nodeNum];
    while (true) {
        int startID = 0;
        map<int, int> shortestPath;
        cout << endl << "请输入源点（输入-1时程序结束）：" << endl;
        cin >> startID;
        outfile << endl << "请输入源点（输入-1时程序结束）：" << endl;
        outfile << startID << endl;
        if (startID == -1)return;
        // 最短路径, startID为路径起点，进入容器
        shortestPath.insert(make_pair(startID, -1));
        // 记录是否访问过
        bool* visited = new bool[nodeNum];
        for (int i = 0; i < nodeNum; ++i) {
            visited[i] = false;
        }
        visited[startID] = true;

        //初始化startID到集合Y中顶点的距离值
        memset(distance, 0, nodeNum * sizeof(pair<int, int>));
        for (int i = 0; i < nodeNum; ++i) {
            if (i == startID) continue;
            distance[i] = make_pair(matrix[startID][i], startID);
        }

        //循环的次数是Y集合中的定点数，去除起点，只有nodeNum-1个
        for (int i = 0; i < nodeNum; ++i) {
            int minID = 0, minWeight = INT_MAX;
            for (int j = 0; j < nodeNum; ++j) {
                // 找出未访问的节点中距离startID最近的
                if (j == startID) continue;
                if (!visited[j] && distance[j].first < minWeight) {
                    minWeight = distance[j].first;
                    minID = j;
                }
            }
            if (minWeight == INT_MAX) {
                //从startID=0到集合V-U中没有路径
                break;
            }

            shortestPath.insert(make_pair(minID, distance[minID].second));
            visited[minID] = true;  //设置为已访问

            //更新顶点startID通过顶点minID到集合V-U中的定点的最短距离
            for (int j = 0; j < nodeNum; ++j) {
                if (!visited[j] && matrix[minID][j] != INT_MAX) {
                    // j未被访问过，并且通过minID能到达j
                    if (distance[j].first > distance[minID].first + matrix[minID][j]) {
                        distance[j].first = distance[minID].first + matrix[minID][j];
                        distance[j].second = minID;
                    }
                }
            }
        }

        // 输出源点到其他所有点的最短路径
        // shortestPath中key为后节点，value为前节点
        // 正着找，用队列存储路径，或逆着找，用栈存储路径
        cout << "源点 " << startID << " 到其他所有点的路径如下：" << endl;
        outfile << "源点 " << startID << " 到其他所有点的路径如下：" << endl;
        
        for (int i = 0; i < nodeNum; ++i) {
            if (i == startID)continue;
            std::stack<int> nodeS;
            map<int, int>::iterator it = shortestPath.find(i);
            while (it != shortestPath.end()) {
                nodeS.push((*it).first);
                if (it->first == startID)
                    break;
                it = shortestPath.find(it->second);
            }
            if (nodeS.top() != startID) {
                cout << "未找到路径" << endl;
                outfile << "未找到路径" << endl;
            }
            else {
                cout <<startID<< " -> " << i << ": ";
                outfile << startID << " -> " << i << ": ";
                int nodeDis = 0;
                while (!nodeS.empty()) {
                    int topEle = nodeS.top();
                    nodeS.pop();
                    cout << topEle << "  ";
                    outfile << topEle << "  ";
                    if (!nodeS.empty())
                        nodeDis += matrix[topEle][nodeS.top()];
                }
                cout << "   长度为 " << nodeDis << endl;
                outfile << "   长度为 " << nodeDis << endl;
            }
        }
    }
    delete[] distance;
}
int main() {
    vector<Edge> edgeVec;
    getEdge(edgeVec);

    int* (*matrix) = NULL;
    int nodeNum = 0;
    getMatrix(edgeVec, matrix, nodeNum);

    djkstra(matrix, nodeNum);
   
}
