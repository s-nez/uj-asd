#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sstream>
#include "colors.h"

class AVL {
	public:
		void insert(int);
		void remove(int);
		bool exists(int);
		bool empty();
		void draw();
		std::vector<int> inorder();
		std::vector<int> preorder();
		std::vector<int> postorder();
		std::string operation_summary();
	private:
		struct Node {
			int info, height = 0;
			Node *left = nullptr, *right = nullptr;
			Node(int ninfo, int nheight = 1) : info(ninfo), height(nheight) {}
		};
		Node* root = nullptr;

		std::stringstream summary_buffer;
		bool rotation_occurred = false;
		void clear_summary();
		void summary_add_root_and_height();

		bool insert(int, Node*);
		void remove(int, Node*);
		bool remove(int, Node*, Node*);
		bool exists(int, Node*);
		Node* extract_smallest(Node*, Node*);
		Node* extract_biggest(Node*, Node*);
		Node* advance(int, Node*);

		void inorder(Node*, std::vector<int>&);
		void preorder(Node*, std::vector<int>&);
		void postorder(Node*, std::vector<int>&);

		int draw(Node*, bool, int, int, std::vector<std::string>&);
		void ascertain_size(std::string&, int);

		int tree_height(Node*);
		int tree_balance(Node*);

		void rotl(Node*, Node*);
		void rotr(Node*, Node*);

		void balance_subtree(Node*, Node*);
};

#endif
