package com;


import javafx.util.Pair;

import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

/**
 * 分治法求解最近点对的问题
 *
 * @author NKU-DBIS, pichunying
 * @date 2022/4/8
 */
public class NearestPair {
    private int threshold = 10;

    /**
     * 获取两点距离
     *
     * @param p1 点1
     * @param p2 点2
     * @return 距离
     */
    private double getDistance(Point p1, Point p2) {
        return Math.sqrt(Math.abs((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY())));
    }

    /**
     * 随机产生50个点，限制 x, y 值在 0~200
     */
    private ArrayList<Point> generatePoints() {
        int pointNum = 50;
        int maxLen = 200;
        ArrayList<Point> points = new ArrayList<Point>();
        for (int i = 0; i < pointNum; i++) {
            points.add(new Point((int) (Math.random() * maxLen), (int) (Math.random() * maxLen)));
        }
        return points;
    }

    private void write2File(ArrayList<Point> points) {
        try {
            String header = "X,Y\r\n";
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("points.csv"), "GBK"));
            writer.write(header);
            for (int i = 0; i < points.size(); i++) {
                StringBuilder str = new StringBuilder();
                Point point = points.get(i);
                str.append(point.getX());
                str.append(",");
                str.append(point.getY());
                str.append("\r\n");
                writer.write(str.toString());
            }
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private Pair<Point, Point> getNearestSimple(ArrayList<Point> points) {
        if (points.size() <= 1) {
            Point p1 = new Point(Integer.MIN_VALUE, Integer.MIN_VALUE);
            Point p2 = new Point(Integer.MAX_VALUE, Integer.MAX_VALUE);
            return new Pair<>(p1, p2);
        }
        double dis = Double.MAX_VALUE;
        Point p1 = null, p2 = null;
        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                double distance = getDistance(points.get(i), points.get(j));
                if (distance < dis) {
                    dis = distance;
                    p1 = points.get(i);
                    p2 = points.get(j);
                }
            }
        }
        return new Pair<>(p1, p2);
    }

    /**
     * 分治法求解最近点对的问题
     */
    private Pair<Point, Point> getNearest(ArrayList<Point> points) {
        if (points.size() <= threshold) {
            return getNearestSimple(points);
        }
        // 分治法
        // step1: 求所有点的横坐标的中位数
        int maxX = Integer.MIN_VALUE;
        int minX = Integer.MAX_VALUE;
        for (int i = 0; i < points.size(); i++) {
            minX = (int) Math.min(minX, points.get(i).getX());
            maxX = (int) Math.max(maxX, points.get(i).getX());
        }
        int midX = (minX + maxX) / 2;
        // step2: 根据X坐标划分点
        ArrayList<Point> points1 = new ArrayList<>();
        ArrayList<Point> points2 = new ArrayList<>();
        for (int i = 0; i < points.size(); i++) {
            if (points.get(i).getX() <= midX) {
                points1.add(points.get(i));
            } else {
                points2.add(points.get(i));
            }
        }
        // step3: 分治
        Pair<Point, Point> left = getNearest(points1);
        Pair<Point, Point> right = getNearest(points2);
        Pair<Point, Point> result = null;
        if (getDistance(left.getKey(), left.getValue()) <= getDistance(right.getKey(), right.getValue())) {
            result = left;
        } else {
            result = right;
        }
        double distance = getDistance(result.getKey(), result.getValue());
        // step4: 求解中间区域
        ArrayList<Point> pointsMidL = new ArrayList<>();
        ArrayList<Point> pointsMidR = new ArrayList<>();
        for (int i = 0; i < points1.size(); i++) {
            if (points1.get(i).getX() >= midX - distance) {
                pointsMidL.add(points1.get(i));
            }
        }
        for (int i = 0; i < points2.size(); i++) {
            if (points2.get(i).getX() <= midX + distance) {
                pointsMidR.add(points2.get(i));
            }
        }
        ArrayList<Point> pointsMid = new ArrayList<>(pointsMidL);
        pointsMid.addAll(pointsMidR);
        if (pointsMid.size() <= threshold) {
            Pair<Point, Point> nearestSimple = getNearestSimple(pointsMid);
            double tmp = getDistance(nearestSimple.getKey(), nearestSimple.getValue());
            if (tmp < distance) {
                result = nearestSimple;
            }
        } else {
            // 按照y坐标排序
            pointsMidL.sort((p1, p2) -> (int) (p1.getY() - p2.getY()));
            pointsMidR.sort((p1, p2) -> (int) (p1.getY() - p2.getY()));
            // 根据鸽笼定理，通过扫描X以及对于X中每个点检查Y中与其距离在dm之内的所有点(最多6个)可以完成合并；
            // 当X中的扫描指针逐次向上移动时，Y中的扫描指针可在宽为2dm的区间内移动
            for (int i = 0; i < pointsMidL.size(); i++) {
                Point point1 = pointsMidL.get(i);
                double y = point1.getY();
                for (int j = 0; j < pointsMidR.size(); j++) {
                    Point point2 = pointsMidR.get(j);
                    if (point2.getY() < y + distance) {
                        continue;
                    }
                    if (point2.getY() > y + distance) {
                        break;
                    } else {
                        double tmp = getDistance(point1, point2);
                        if (tmp < distance) {
                            distance = tmp;
                            result = new Pair<>(point1, point2);
                        }
                    }
                }
            }
        }
        return result;
    }

    public static void main(String[] args) {
        NearestPair nearestPair = new NearestPair();
        ArrayList<Point> points = nearestPair.generatePoints();
        nearestPair.write2File(points);
        long startTime = System.currentTimeMillis();
        Pair<Point, Point> nearest = nearestPair.getNearest(points);
        long endTime = System.currentTimeMillis();
        System.out.println("the nearest point pairs is: (" + nearest.getKey().getX() + ", " + nearest.getKey().getY()
                + ") -- (" + nearest.getValue().getX() + ", " + nearest.getValue().getY() + ")");
        System.out.println("their distance is: " + nearestPair.getDistance(nearest.getKey(), nearest.getValue()));
        System.out.println("time cost of ctAlgorithm is： " + (endTime - startTime) + " ms");
    }
}
