#include "avl.h"

void AVL::insert (int new_element)
{
	clear_summary();
	summary_buffer << "Insert " << new_element << ", ";
	if (empty())
		root = new Node (new_element);
	else if (insert (new_element, root))
		balance_subtree (root, nullptr);
	if (!rotation_occurred)
		summary_buffer << "bez rot, ";
	summary_add_root_and_height();
}

bool AVL::insert (int new_element, Node* target)
{
	bool inserted = false;
	if (target->info > new_element) {
		if (!target->left) {
			target->left = new Node (new_element);
			return true;
		}
		inserted = insert (new_element, target->left);
		if (inserted) {
			balance_subtree (target->left, target);
		}
	} else if (target->info < new_element) {
		if (!target->right) {
			target->right = new Node (new_element);
			return true;
		}
		inserted = insert (new_element, target->right);
		if (inserted) {
			balance_subtree (target->right, target);
		}
	} else {
		return false;
	}

	return inserted;
}

bool AVL::empty()
{
	return root == nullptr;
}

void AVL::remove (int el_to_remove)
{
	clear_summary();
	summary_buffer << "Delete " << el_to_remove << ", ";
	if (!empty())
		remove (el_to_remove, root);
	if (!rotation_occurred)
		summary_buffer << "bez rot, ";
	summary_add_root_and_height();
}

void AVL::remove (int el_to_remove, Node* target)
{
	if (remove (el_to_remove, target, nullptr))
		balance_subtree (root, nullptr);
}

bool AVL::remove (int el_to_remove, Node* target, Node* parent)
{
	if (!target)
		return false;

	if (el_to_remove == target->info) {
		Node* replacement = nullptr;
		if (target->left and target->right) {
			replacement = extract_smallest (target->right, target);
			replacement->left = target->left;
			replacement->right = target->right;
		} else if (target->left) {
			replacement = target->left;
		} else if (target->right) {
			replacement = target->right;
		}
		if (replacement) {
			summary_buffer << target->info << "<->";
			summary_buffer << replacement->info << ", ";
		}
		if (parent) {
			if (target == parent->left)
				parent->left = replacement;
			else
				parent->right = replacement;
		} else {
			root = replacement;
		}
		delete target;
		return true;
	} else {
		if (remove (el_to_remove, advance (el_to_remove, target), target)) {
			balance_subtree (target, parent);
			return true;
		}
		return false;
	}
}

AVL::Node* AVL::extract_smallest (Node* current, Node* parent)
{
	Node* search_root = parent;
	while (current->left) {
		parent = current;
		current = current->left;
	}
	if (parent) {
		if (parent == search_root)
			parent->right = current->right;
		else
			parent->left = current->right;
	}
	return current;
}

AVL::Node* AVL::extract_biggest (Node* current, Node* parent)
{
	Node* search_root = parent;
	while (current->right) {
		parent = current;
		current = current->right;
	}
	if (parent) {
		if (parent == search_root)
			parent->left = current->right;
		else
			parent->right = current->left;
	}
	return current;
}

AVL::Node* AVL::advance (int key, Node* target)
{
	if (!target)
		return nullptr;
	if (key < target->info)
		return target->left;
	else
		return target->right;
}

std::vector<int> AVL::inorder()
{
	std::vector<int> result;
	inorder (root, result);
	return std::move (result);
}

void AVL::inorder (Node* target, std::vector<int>& result)
{
	if (target) {
		inorder (target->left, result);
		result.push_back (target->info);
		inorder (target->right, result);
	}
}

std::vector<int> AVL::preorder()
{
	std::vector<int> result;
	preorder (root, result);
	return std::move (result);
}

void AVL::preorder (Node* target, std::vector<int>& result)
{
	if (target) {
		result.push_back (target->info);
		preorder (target->left, result);
		preorder (target->right, result);
	}
}

std::vector<int> AVL::postorder()
{
	std::vector<int> result;
	postorder (root, result);
	return std::move (result);
}

void AVL::postorder (Node* target, std::vector<int>& result)
{
	if (target) {
		postorder (target->left, result);
		postorder (target->right, result);
		result.push_back (target->info);
	}
}

void AVL::draw()
{
	std::vector<std::string> canvas;

	draw (root, 0, 0, 0, canvas);

	for (auto it : canvas) {
		for (std::size_t i = 0; i != it.size(); ++i) {
			if (it[i] == ')') {
				std::cout << Color::Reset;
			}
			std::cout << it[i];
			if (it[i] == '(') {
				std::cout << Color::Hicolor << Color::Red;
			}
		}
		std::cout << std::endl;
	}
	//std::cout << it << std::endl;
}

int AVL::draw (Node* tree, bool is_left, int offset,
               int depth, std::vector<std::string>& canvas)
{
	std::string b;

	if (!tree) return 0;

	b = "(" + std::to_string (tree->info) + ")";
	int width = b.size();

	int left  = draw (tree->left,  true,
	                  offset, depth + 1, canvas);
	int right = draw (tree->right, false,
	                  offset + left + width, depth + 1, canvas);

	if (depth >= canvas.size())
		canvas.resize (depth + 1);

	ascertain_size (canvas[depth], offset + left + width);
	canvas[depth].replace (offset + left, b.size(), b);

	if (depth && is_left) {
		int insert_offset = offset + left + width / 2;
		int insert_width = width + right;
		int link_position = insert_offset;

		ascertain_size (canvas[depth - 1], insert_offset + insert_width);
		canvas[depth - 1].insert (insert_offset, insert_width, '-');
		canvas[depth - 1][link_position] = '.';
	} else if (depth && !is_left) {
		int insert_offset = offset - width / 2;
		int insert_width = left + width;
		int link_position = offset + left + width / 2;

		ascertain_size (canvas[depth - 1], insert_offset + insert_width);
		canvas[depth - 1].insert (insert_offset, insert_width, '-');
		canvas[depth - 1][link_position] = '.';
	}

	return left + width + right;
}

int AVL::tree_height (Node* tree)
{
	if (!tree)
		return 0;
	return 1 + std::max (tree_height (tree->left), tree_height (tree->right));
}

int AVL::tree_balance (Node* tree)
{
	if (!tree) {
		return 0;
	}
	return tree_height (tree->left) - tree_height (tree->right);
}

void AVL::rotr (Node* target, Node* parent)
{
	summary_buffer << "R";
	rotation_occurred = true;
	if (!parent) {
		// Rotating the root
		Node* old_root = root;
		root = root->left;
		old_root->left = root->right;
		root->right = old_root;
	} else {
		if (parent->left == target) {
			parent->left = target->left;
			target->left = target->left->right;
			parent->left->right = target;
		} else {
			parent->right = target->left;
			target->left = target->left->right;
			parent->right->right = target;
		}
	}
}

void AVL::rotl (Node* target, Node* parent)
{
	summary_buffer << "L";
	rotation_occurred = true;
	if (!parent) {
		// Rotating the root
		Node* old_root = root;
		root = root->right;
		old_root->right = root->left;
		root->left = old_root;
	} else {
		if (parent->left == target) {
			parent->left = target->right;
			target->right = target->right->left;
			parent->left->left = target;
		} else {
			parent->right = target->right;
			target->right = target->right->left;
			parent->right->left = target;
		}
	}
}

bool AVL::exists (int element)
{
	return exists (element, root);
}

bool AVL::exists (int element, Node* target)
{
	if (!target)
		return false;
	if (element == target->info)
		return true;
	return exists (element, advance (element, target));
}

void AVL::balance_subtree (Node* target, Node* parent)
{
	int balance = tree_balance (target);
	if (balance > 1) {
		summary_buffer << "rot";
		if (tree_balance (target->left) < 0)
			rotl (target->left, target);
		if (tree_balance (target->left) >= 0)
			rotr (target, parent);
		summary_buffer << "(" << target->info << "), ";
	} else if (balance < -1) {
		summary_buffer << "rot";
		if (tree_balance (target->right) > 0)
			rotr (target->right, target);
		if (tree_balance (target->right) <= 0)
			rotl (target, parent);
		summary_buffer << "(" << target->info << "), ";
	}
}

void AVL::clear_summary()
{
	summary_buffer.str (std::string());
	summary_buffer.clear();
	rotation_occurred = false;
}

std::string AVL::operation_summary()
{
	return summary_buffer.str();
}

void AVL::summary_add_root_and_height()
{
	summary_buffer << "r=";
	if (root)
		summary_buffer << root->info;
	else
		summary_buffer << "NULL";
	summary_buffer << ", h=";
	summary_buffer << tree_height (root) - 1;
}

void AVL::ascertain_size (std::string& target, int size)
{
	if (target.size() < size)
		target.resize (size, ' ');
}


