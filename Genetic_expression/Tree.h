#define _PSOC_ 
#include <stdlib.h>
#include <stdio.h>
enum list_stage { OPEN_BRACKET, OPERATION, NUMBER, CLOSE_BRACKET };
enum symbols { a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z = 122 };
enum list_operation { MINUS = 45, PLUS = 43, MULTIPLY = 42, DIVIDE = 47 };


template <class T>
struct Node {
	T value;
	list_stage stage;
	Node* parent;
	Node* left;
	Node* right;

	Node(T val, list_stage stage, Node<T>* parent) {
		this->value = val;
		this->stage = stage;
		this->parent = parent;
		this->left = this->right = NULL;
	}
	Node(T val, list_stage stage, Node<T>* left, Node<T>* right, Node<T>* parent) {
		this->parent = parent;
		this->value = val;
		this->stage = stage;
		this->left = left;
		this->right = right;
	}
};

class Tree
{
	unsigned char* expression;
	Node<float>* root;
	size_t length_expression;
	bool flip(float number);
	unsigned char* bound_check_expression(unsigned char* ptr_expression);
public:
	void gen_random_tree();
	unsigned char* view_tree();
	Tree();
	~Tree();
};

