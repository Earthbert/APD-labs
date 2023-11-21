package task7;

import java.util.concurrent.RecursiveTask;

import util.BinarySearchTreeNode;

public class SearchValueRecurisveTask<T extends Comparable<T>> extends RecursiveTask<Boolean> {
	private final BinarySearchTreeNode<T> binarySearchTree;
	private final T value;

	public SearchValueRecurisveTask(BinarySearchTreeNode<T> binarySearchTree, T value) {
		this.binarySearchTree = binarySearchTree;
		this.value = value;
	}

	@Override
	protected Boolean compute() {
		if (binarySearchTree != null) {
			if (value.equals(binarySearchTree.getValue())) {
				return true;
			} else if (binarySearchTree.getValue().compareTo(value) > 0) {
				SearchValueRecurisveTask<T> task = new SearchValueRecurisveTask<T>(binarySearchTree.getLeft(), value);
				task.fork();
				return task.join();
			} else {
				SearchValueRecurisveTask<T> task = new SearchValueRecurisveTask<T>(binarySearchTree.getRight(), value);
				task.fork();
				return task.join();
			}
		}
		return false;
	}
}
