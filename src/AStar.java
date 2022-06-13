package com;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;

class Node {
    int[][] state;
    int diff;
    int depth;
    // 上一次移动的反方向
    int nd = -1;
}

/**
 * A*算法求解八数码问题
 *
 * @author NKU-DBIS, pichunying
 * @date 2022/4/9
 */
public class AStar {
    /**
     * 移动方向 左0 右1 下2 上3
     */
    private final int[] moveX = {-1, 1, 0, 0};
    private final int[] moveY = {0, 0, -1, 1};
    private final int[] verseD = {1, 0, 3, 2};

    ArrayList<Node> open = new ArrayList<>();
    ArrayList<Node> close = new ArrayList<>();
    /**
     * 初始状态
     */
    private final int[][] initState = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
    /**
     * 目标状态
     */
    private final int[][] gaolState = {{2, 8, 3}, {1, 4, 5}, {7, 6, 0}};
    /**
     * 目标状态：值-位置
     */
    private final HashMap<Integer, int[]> goalDic = new HashMap<Integer, int[]>() {
        {
            put(2, new int[]{0, 0});
            put(8, new int[]{0, 1});
            put(3, new int[]{0, 2});
            put(1, new int[]{1, 0});
            put(4, new int[]{1, 1});
            put(5, new int[]{1, 2});
            put(7, new int[]{2, 0});
            put(6, new int[]{2, 1});
            put(0, new int[]{2, 2});
        }
    };

    /**
     * 计算曼哈顿距离
     *
     * @param node 源
     */
    private void getDistance(Node node) {
        int[][] src = node.state;
        int dis = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (src[i][j] != 0) {
                    int[] pos = goalDic.get(src[i][j]);
                    dis += Math.abs(pos[0] - i) + Math.abs(pos[1] - j);
                }
            }
        }
        node.diff = dis;
    }

    private int[] getZeroPos(Node node) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (node.state[i][j] == 0) {
                    return new int[]{i, j};
                }
            }
        }
        return null;
    }

    private int[][] cloneArray(int[][] state) {
        int[][] res = new int[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                res[i][j] = state[i][j];
            }
        }
        return res;
    }

    private void expand(Node node) {
        int[] zeroPos = getZeroPos(node);
        assert zeroPos != null;
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
            Node tmp = new Node();
            tmp.state = cloneArray(node.state);
            tmp.nd = verseD[d];
            tmp.state[newX][newY] = 0;
            tmp.state[x][y] = node.state[newX][newY];
            getDistance(tmp);
            tmp.depth = node.depth + 1;
            // 检查open中是否已存在
            boolean exist = false;
            for (Node node1 : open) {
                int[][] state = node1.state;
                if (Arrays.deepEquals(state, tmp.state)) {
                    if (tmp.depth + tmp.diff < node1.depth + node1.diff) {
                        // 出现代价更小的路径，移除之前的
                        open.remove(node1);
                    } else {
                        exist = true;
                    }
                }
            }
            if (!exist) {
                open.add(tmp);
            }
            // 排序
            open.sort(Comparator.comparingInt(p -> (p.diff + p.depth)));
        }
    }

    private Node initNode() {
        Node node = new Node();
        node.depth = 0;
        node.state = initState;
        getDistance(node);
        return node;
    }

    private void print(Node node) {
        int[][] state = node.state;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                System.out.print(state[i][j] + " ");
            }
            System.out.println();
        }
        System.out.printf("g(x) = %s, h*(x) = %s\n\n", node.depth, node.diff);
    }

    public void solution() {
        Node node = initNode();
        open.add(node);
        while (!open.isEmpty()) {
            Node tmp = open.get(0);
            close.add(tmp);
            open.remove(0);
            if (Arrays.deepEquals(tmp.state, gaolState)) {
                for (Node value : close) {
                    print(value);
                }
                return;
            }
            expand(tmp);
        }
    }


    public static void main(String[] args) {
        AStar aStar = new AStar();
        aStar.solution();
    }
}
