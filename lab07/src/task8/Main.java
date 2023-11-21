package task8;

import java.util.concurrent.ForkJoinPool;

import util.BSTOperations;
import util.BinarySearchTreeNode;

public class Main {
    public static void main(String[] args) {
        BinarySearchTreeNode<Integer> binarySearchTree = new BinarySearchTreeNode<>(3);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 6);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 9);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 2);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 10);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 1);

        ForkJoinPool forkJoinPool = new ForkJoinPool();
        Integer height = forkJoinPool.invoke(new MaxHeightRecursiveTask<Integer>(binarySearchTree));
        forkJoinPool.close();

        System.out.println(height);
    }
}
