#define _PSOC_ 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
enum list_stage { OPEN_BRACKET = 1, OPERATION = 2, NUMBER = 4, CLOSE_BRACKET = 8, SYMBOL = 16 };
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
	float parse_number(unsigned char* string);
	float* bound_check_numbers(float* ptr_numbers, float* numbers, size_t length_numbers);
	unsigned char* bound_check_expression(unsigned char* ptr_expression);
	float calculate_numbers(float number_1, float number_2, int operation);
	unsigned char* string_current_stage(unsigned char* ptr_expression, Node<float>* current_position);
public:
	void set_expression(unsigned char* expression, size_t length_expression, unsigned char* symbols, size_t number_symbols);
	void gen_random_tree(size_t max_length, unsigned char* symbols, size_t length_symbols);
	unsigned char* view_tree();
	float calculate_tree(float* value_arguments, unsigned char* arguments, size_t number_arguments);
	//float calculate_tree();
	Tree();
	~Tree();
};



