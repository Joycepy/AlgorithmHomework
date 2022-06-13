// Nearestpair.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<vector>
#include<string>
#include<utility>
#include<cmath>
#include<fstream>
#include<algorithm>
#include <time.h>
#include<ctime>
using namespace std;
class Point {
private:
    int X;
    int Y;
public:
    Point() : X(0), Y(0) {}
    Point(int x, int y) : X(x), Y(y) {}
    int getX() {
        return X;
    }
    int getY() {
        return Y;
    }
};

/**
 * 分治法求解最近点对的问题
 *
 * @author NKU-DBIS, pichunying
 * @date 2022/4/8
 */
    int threshold = 10;

/**
* 获取两点距离
*
* @param p1 点1
* @param p2 点2
* @return 距离
*/
double getDistance(Point p1, Point p2) {
    return sqrt(abs((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY())));
}

bool comp(Point p1,Point p2) {
    return p1.getY() < p2.getY();
}

/**
* 随机产生50个点，限制 x, y 值在 0~200
*/

int maxLen = 200;
vector<Point> generatePoints() {
    int pointNum = 50;
    vector<Point> points;
    srand((unsigned)time(NULL));
    for (int i = 0; i < pointNum; i++) {
        Point p( rand() % maxLen, rand() % maxLen);
        points.push_back(p);
    }
    return points;
}

void write2File(vector<Point> points) {
    try {
        string header = "X,Y\r\n";
        ofstream out("points.csv");
        if (out.is_open()) {
            for (auto it = points.begin();it!=points.end(); it++) {
                string str = "";
                str.append(to_string(it->getX()));
                str.append(",");
                str.append(to_string(it->getY()));
                str.append("\n");
                out<<str;
            }
            out.close();
        }
    }
    catch (exception e) {
        cout << "writeFile Error.";
    }
}

pair<Point, Point> getNearestSimple(const vector<Point> points) {
    if (points.size() <= 1) {
        Point p1(0, 0);
        Point p2(maxLen, maxLen);
        return pair<Point,Point>(p1, p2);
    }
    double dis = DBL_MAX;
    Point p1, p2;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            double distance = getDistance(points[i], points[j]);
            if (distance < dis) {
                dis = distance;
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    return pair<Point, Point>(p1, p2);
}

/**
    * 分治法求解最近点对的问题
    */
pair<Point, Point> getNearest(vector<Point> points) {
    if (points.size() <= threshold) {
        return getNearestSimple(points);
    }
    // 分治法
    // step1: 求所有点的横坐标的中位数
    int maxX = INT_MIN;
    int minX = INT_MAX;
    for (int i = 0; i < points.size(); i++) {
        minX = (int)min(minX, points[i].getX());
        maxX = (int)max(maxX, points[i].getX());
    }
    int midX = (minX + maxX) / 2;
    // step2: 根据X坐标划分点
    vector<Point> points1;
    vector<Point> points2;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].getX() <= midX) {
            points1.push_back(points[i]);
        }
        else {
            points2.push_back(points[i]);
        }
    }
    // step3: 分治
    pair<Point, Point> left = getNearest(points1);
    pair<Point, Point> right = getNearest(points2);
    pair<Point, Point> result;
    if (getDistance(left.first, left.second) <= getDistance(right.first, right.second)) {
        result = left;
    }
    else {
        result = right;
    }
    double distance = getDistance(result.first, result.second);
    // step4: 求解中间区域
    vector<Point> pointsMidL;
    vector<Point> pointsMidR;
    for (int i = 0; i < points1.size(); i++) {
        if (points1[i].getX() >= midX - distance) {
            pointsMidL.push_back(points1[i]);
        }
    }
    for (int i = 0; i < points2.size(); i++) {
        if (points2[i].getX() <= midX + distance) {
            pointsMidR.push_back(points2[i]);
        }
    }
    vector<Point> pointsMid;
    pointsMid.insert(pointsMid.end(),pointsMidL.begin(), pointsMidL.end());
    pointsMid.insert(pointsMid.end(), pointsMidR.begin(), pointsMidR.end());
    if (pointsMid.size() <= threshold) {
        pair<Point, Point> nearestSimple = getNearestSimple(pointsMid);
        double tmp = getDistance(nearestSimple.first, nearestSimple.second);
        if (tmp < distance) {
            result = nearestSimple;
        }
    }
    else {
        // 按照y坐标排序
        sort(pointsMidL.begin(), pointsMidL.end(), comp);
        sort(pointsMidR.begin(), pointsMidR.end(), comp);

        // 根据鸽笼定理，通过扫描X以及对于X中每个点检查Y中与其距离在dm之内的所有点(最多6个)可以完成合并；
        // 当X中的扫描指针逐次向上移动时，Y中的扫描指针可在宽为2dm的区间内移动
        for (int i = 0; i < pointsMidL.size(); i++) {
            Point point1 = pointsMidL[i];
            double y = point1.getY();
            for (int j = 0; j < pointsMidR.size(); j++) {
                Point point2 = pointsMidR[j];
                if (point2.getY() < y + distance) {
                    continue;
                }
                if (point2.getY() > y + distance) {
                    break;
                }
                else {
                    double tmp = getDistance(point1, point2);
                    if (tmp < distance) {
                        distance = tmp;
                        result = pair<Point, Point>(point1, point2);
                    }
                }
            }
        }
    }
    return result;
}
int main()
{
    vector<Point> points = generatePoints();
    write2File(points);
    clock_t start, end;
    start = clock();
    pair<Point, Point> nearest = getNearest(points);
    end = clock();

    cout << "the nearest point pairs is: (" << nearest.first.getX() << ", " << nearest.first.getY()
        << ") -- (" << nearest.second.getX() << ", " << nearest.second.getY() << ")" << endl;
    cout<<"their distance is: " << getDistance(nearest.first, nearest.second)<<endl;
    
    cout << "time cost of ctAlgorithm is： " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;

}
