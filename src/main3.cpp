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

vector<Edge> edgeVec; //��Ȩ����ͼ�ߵļ���
ofstream outfile("log.txt");

//���������ļ���ȡͼ�ı���Ϣ
void getEdge(vector<Edge>& edgeVec) {
    int sideNum = 0;
    cout << "������ߵ�������" << endl;
    cin >> sideNum;
    cout << "������ÿ���ߵ���㡢�յ㡢���ۣ�" << endl;
    outfile << "������ߵ�������" << endl;
    outfile << sideNum << endl;
    outfile << "������ÿ���ߵ���㡢�յ㡢���ۣ�" << endl;
    for (int i = 0; i < sideNum; ++i) {
        Edge edgeTemp;
        cin >> edgeTemp.start >> edgeTemp.end >> edgeTemp.cost;
        outfile << edgeTemp.start << " " << edgeTemp.end << " " << edgeTemp.cost << endl;
        edgeVec.push_back(edgeTemp);
    }
}

//����ͼ�ı���Ϣ�����ڽӾ���
void getMatrix(vector<Edge>& edgeVec, int* (*&matrix), int& nodeNum) {
    set<int> nodeSet;
    for (int i = 0; i < edgeVec.size(); ++i) {
        nodeSet.insert(edgeVec[i].start);
        nodeSet.insert(edgeVec[i].end);
    }
    nodeNum = nodeSet.size();
    //��ʼ��
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
    //��ֵ
    for (auto it = edgeVec.begin(); it != edgeVec.end(); ++it) {
        matrix[it->start][it->end] = it->cost;
    }
}

void djkstra(int* (*matrix), int nodeNum) {
    //����startID�������������,��һ��int��ʾ��ǰ�ڵ��ǰһ���ڵ�
    pair<int, int>* distance = new pair<int, int>[nodeNum];
    while (true) {
        int startID = 0;
        map<int, int> shortestPath;
        cout << endl << "������Դ�㣨����-1ʱ�����������" << endl;
        cin >> startID;
        outfile << endl << "������Դ�㣨����-1ʱ�����������" << endl;
        outfile << startID << endl;
        if (startID == -1)return;
        // ���·��, startIDΪ·����㣬��������
        shortestPath.insert(make_pair(startID, -1));
        // ��¼�Ƿ���ʹ�
        bool* visited = new bool[nodeNum];
        for (int i = 0; i < nodeNum; ++i) {
            visited[i] = false;
        }
        visited[startID] = true;

        //��ʼ��startID������Y�ж���ľ���ֵ
        memset(distance, 0, nodeNum * sizeof(pair<int, int>));
        for (int i = 0; i < nodeNum; ++i) {
            if (i == startID) continue;
            distance[i] = make_pair(matrix[startID][i], startID);
        }

        //ѭ���Ĵ�����Y�����еĶ�������ȥ����㣬ֻ��nodeNum-1��
        for (int i = 0; i < nodeNum; ++i) {
            int minID = 0, minWeight = INT_MAX;
            for (int j = 0; j < nodeNum; ++j) {
                // �ҳ�δ���ʵĽڵ��о���startID�����
                if (j == startID) continue;
                if (!visited[j] && distance[j].first < minWeight) {
                    minWeight = distance[j].first;
                    minID = j;
                }
            }
            if (minWeight == INT_MAX) {
                //��startID=0������V-U��û��·��
                break;
            }

            shortestPath.insert(make_pair(minID, distance[minID].second));
            visited[minID] = true;  //����Ϊ�ѷ���

            //���¶���startIDͨ������minID������V-U�еĶ������̾���
            for (int j = 0; j < nodeNum; ++j) {
                if (!visited[j] && matrix[minID][j] != INT_MAX) {
                    // jδ�����ʹ�������ͨ��minID�ܵ���j
                    if (distance[j].first > distance[minID].first + matrix[minID][j]) {
                        distance[j].first = distance[minID].first + matrix[minID][j];
                        distance[j].second = minID;
                    }
                }
            }
        }

        // ���Դ�㵽�������е�����·��
        // shortestPath��keyΪ��ڵ㣬valueΪǰ�ڵ�
        // �����ң��ö��д洢·�����������ң���ջ�洢·��
        cout << "Դ�� " << startID << " ���������е��·�����£�" << endl;
        outfile << "Դ�� " << startID << " ���������е��·�����£�" << endl;
        
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
                cout << "δ�ҵ�·��" << endl;
                outfile << "δ�ҵ�·��" << endl;
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
                cout << "   ����Ϊ " << nodeDis << endl;
                outfile << "   ����Ϊ " << nodeDis << endl;
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
