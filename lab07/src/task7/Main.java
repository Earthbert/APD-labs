package task7;

import java.util.Scanner;
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

        Scanner scanner = new Scanner(System.in);
        Integer value = scanner.nextInt();
        scanner.close();

        ForkJoinPool forkJoinPool = new ForkJoinPool();
        Boolean found = forkJoinPool.invoke(new SearchValueRecurisveTask<Integer>(binarySearchTree, value));
        forkJoinPool.close();
        System.out.println(found);
    }
}
