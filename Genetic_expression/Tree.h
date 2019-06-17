#pragma once
#define _PSOC_ 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
enum list_stage { OPEN_BRACKET = 1, OPERATION = 2, NUMBER = 4, CLOSE_BRACKET = 8, SYMBOL = 16 };
enum symbols { a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z = 122 };
enum list_operation { MINUS = 45, PLUS = 43, MULTIPLY = 42, DIVIDE = 47 };
size_t get_random();
double get_random(double min, double max);
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
template<typename T>
class Tree
{
	Node<T>* root;
	unsigned char* expression;
	size_t length_expression;
	size_t number_component_tree;
	bool flip(T number);
	T parse_number(unsigned char* string, unsigned char& number);
	T* bound_check_numbers(T* ptr_numbers, T* numbers, size_t length_numbers);
	unsigned char* bound_check_expression(unsigned char* ptr_expression);
	T calculate_numbers(T number_1, T number_2, int operation);
	unsigned char* string_current_stage(unsigned char* ptr_expression, Node<T>* current_position);
public:
	size_t get_number_component_tree();
	void set_expression(unsigned char* expression, size_t length_expression, unsigned char* symbols, size_t number_symbols);
	void gen_random_tree(size_t max_length, unsigned char* symbols, size_t length_symbols);
	unsigned char* view_tree();
	T calculate_tree(T* value_arguments, unsigned char* arguments, size_t number_arguments);
	void operator=(Tree<T>& tree);
	Node<T>* operator[](size_t index);
	//float calculate_tree();
	Tree();
	~Tree();
};




