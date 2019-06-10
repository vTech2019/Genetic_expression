#include "Tree.h"

bool Tree::flip(float number)
{
	if (rand() < number)
		return true;
	else
		return false;
}

void Tree::gen_random_tree()
{
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	bool find_open_bracket = false;
	ptrdiff_t  _list_stage = OPERATION;
	Node<float>* current_position = NULL;
	do {
		switch (_list_stage) {
		case OPERATION: {
			_list_stage = flip(0.5f) ? OPERATION : NUMBER;
			size_t index = 4 * rand() % (sizeof(_list_operation) / sizeof(*_list_operation));
			Node<float>* current = new Node<float>((float)_list_operation[index], OPERATION, NULL, NULL, current_position);
			if (current_position) {
				if (!current_position->left)
					current_position->left = current,
					current_position->left->parent = current_position,
					current_position = current_position->left;
				else if (!current_position->right)
					current_position->right = current,
					current_position->right->parent = current_position;
				current_position = current_position->right;
			}
			else {
				current_position = current;
				root = current_position;
			}
			break;
		}
		case NUMBER:
			_list_stage = flip(0.5f) ? OPERATION : NUMBER;
			float number = rand() % RAND_MAX;
			Node<float>* current = new Node<float>(number, NUMBER, NULL, NULL, current_position);
			if (!current_position->left)
				current_position->left = current,
				current_position->left->parent = current_position;
			else if (!current_position->right)
				current_position->right = current,
				current_position->left->parent = current_position;
			break;
		}
	} while (!current_position->left || !current_position->right);
}

unsigned char* Tree::bound_check_expression(unsigned char* ptr_expression) {
	if (ptr_expression == expression + length_expression) {
		ptr_expression = expression + length_expression;
		length_expression += 128;
		expression = (unsigned char*)realloc(expression, length_expression * sizeof(unsigned char));
	}
	return ptr_expression;
}

unsigned char* Tree::view_tree()
{
	Node<float>* current_position = root;
	length_expression = 128;
	expression = (unsigned char*)malloc(length_expression * sizeof(unsigned char));
	unsigned char* ptr_expression = expression;
	unsigned char* ptr_end_expression = expression + length_expression;
	size_t left = 0;
	size_t right = 0;
	size_t stage = current_position->stage;
	while (current_position) {
		if (ptr_expression == ptr_end_expression) {
			length_expression += 128;
			expression = (unsigned char*)realloc(expression, length_expression * sizeof(unsigned char));
			ptr_expression = expression + length_expression - 128;
			ptr_end_expression = expression + length_expression;
		}
		switch (stage) {
		case OPEN_BRACKET: {
			*ptr_expression = current_position->value;
			ptr_expression++;
			current_position = current_position->left;
			stage = current_position->stage;

			break;
		}
		case OPERATION: {
			*ptr_expression = '(';
			ptr_expression++;
			stage = OPEN_BRACKET;

			break;
		}
		case NUMBER: {
			char number[32] = { 0 };
			char* ptr_number = number;
			sprintf_s(number, "%f", current_position->value);
			while (*ptr_number != 0) {
				*ptr_expression++ = *ptr_number++;
				ptr_expression = bound_check_expression(ptr_expression);
			}
			if (current_position->parent->left == current_position) {
				current_position = current_position->parent->right;
				stage = current_position->stage;
			}
			else if (current_position->parent->right == current_position) {
				if (current_position->parent->parent)
					current_position = current_position->parent->parent->right;
				else {
					ptr_expression = bound_check_expression(ptr_expression);
					*ptr_expression++ = ')';
					ptr_expression = bound_check_expression(ptr_expression);
					*ptr_expression = 0;
					current_position = NULL;
				}
				stage = CLOSE_BRACKET;
			}
			break;
		}
		case CLOSE_BRACKET: {
			*ptr_expression = ')';
			ptr_expression++;
			stage = current_position->stage;
			break;
		}
		}
	}
	return expression;
}

Tree::Tree()
{
}

Tree::~Tree()
{
	if (expression)
		free(expression);
	expression = NULL;
	Node<float>* current_position = root;
	while (current_position) {
		while (current_position->left) {
			current_position = current_position->left;
		}
		while (current_position->right) {
			current_position = current_position->right;
		}
		if (!current_position->left && !current_position->right) {
			Node<float>* delete_position = current_position;
			if (current_position->parent) {
				current_position->parent->left = current_position == current_position->parent->left ? NULL : current_position->parent->left;
				current_position->parent->right = current_position == current_position->parent->right ? NULL : current_position->parent->right;
				current_position = current_position->parent;
			}
			else
				current_position = NULL;
			delete delete_position;
		}
	}
}