package task8;

import java.util.concurrent.RecursiveTask;

import util.BinarySearchTreeNode;

/**
 * MaxHeightRecurrsiveTask
 */
public class MaxHeightRecursiveTask<T extends Comparable<T>> extends RecursiveTask<Integer> {
	private final BinarySearchTreeNode<Integer> binarySearchTree;

	public MaxHeightRecursiveTask(BinarySearchTreeNode<Integer> binarySearchTree) {
		this.binarySearchTree = binarySearchTree;
	}

	@Override
	protected Integer compute() {
		if (binarySearchTree == null) {
			return 0;
		}

		MaxHeightRecursiveTask<T> leftTask = new MaxHeightRecursiveTask<T>(binarySearchTree.getLeft());
		MaxHeightRecursiveTask<T> rightTask = new MaxHeightRecursiveTask<T>(binarySearchTree.getRight());

		leftTask.fork();
		rightTask.fork();

		Integer leftHeight = leftTask.join();
		Integer rightHeight = rightTask.join();

		return Math.max(leftHeight, rightHeight) + 1;
	}

}