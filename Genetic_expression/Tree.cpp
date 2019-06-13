#include "Tree.h"

bool Tree::flip(float number)
{
	float random = (float)rand() / RAND_MAX;
	if (random < number)
		return true;
	else
		return false;
}
float Tree::parse_number(unsigned char* string) {
	float result = 0.0f;
	int integer_part = 0;
	if (*string < '0' || *string > '9') {
		return nanf("");
	}
	while (*string >= '0' && *string <= '9') {
		integer_part *= 10;
		integer_part += *string - '0';
		string++;
	}
	result += integer_part;
	if (*string == '.') {
		string++;
		size_t number_div = 0;
		float result_div = 0.0f;
		while (*string >= '0' && *string <= '9') {
			result_div += (*string - '0') / 10.0f;
			result_div *= 10.0f;
			string++;
			number_div++;
		}
		result += result_div / pow(10.0f, number_div);
	}
	return result;
}
void Tree::set_expression(unsigned char* expression, size_t length_expression, unsigned char* symbols, size_t number_symbols)
{
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	unsigned char* start_expression = expression;
	unsigned char* end_expression = expression + length_expression;
	ptrdiff_t stage = OPEN_BRACKET;
	Node<float>* current_position = NULL;
	while (start_expression != end_expression) {
		if (*start_expression == '(') {
			if (stage && OPEN_BRACKET) {
				stage = NUMBER | OPEN_BRACKET;
				Node<float>* current = new Node<float>(NAN, OPEN_BRACKET, NULL, NULL, current_position);
				if (current_position) {
					if (!current_position->left)
						current_position->left = current,
						current_position->left->parent = current_position,
						current_position = current_position->left;
					else if (!current_position->right)
						current_position->right = current,
						current_position->right->parent = current_position,
						current_position = current_position->right;
				}
				else
					current_position = current,
					root = current_position;

				start_expression++;
			}
			else
				this->~Tree(),
				start_expression = end_expression;
		}
		else {
			float number = parse_number(start_expression);
			unsigned char* find_symbol = NULL;
			for (size_t i = 0; i < number_symbols && !find_symbol; i++)
				find_symbol = symbols[i] == *start_expression ? symbols + i : NULL;

			if (!isnan(number) || find_symbol) {
				if (stage && NUMBER) {
					stage = OPERATION | CLOSE_BRACKET;
					Node<float>* current;
					if (find_symbol) {
						current = new Node<float>(*find_symbol, SYMBOL, NULL, NULL, current_position);
					}
					else {
						current = new Node<float>(number, NUMBER, NULL, NULL, current_position);
					}
					if (!current_position->left) {
						current_position->left = current;
						current_position = current_position->left->parent;
					}
					else if (!current_position->right) {
						current_position->right = current;
						current_position = current_position->right->parent;
					}
					else if (current_position->parent)
						current_position = current_position->parent;
					start_expression++;
				}
				else
					this->~Tree(),
					start_expression = end_expression;
			}
			else {
				bool find_operator = false;
#pragma unroll
				for (size_t i = 0; i < sizeof(_list_operation) && !find_operator; i++)
					find_operator = *start_expression == _list_operation[i] ? true : false;
				if (find_operator) {
					if (stage && OPERATION) {
						stage = NUMBER | OPEN_BRACKET;
						current_position->stage = OPERATION;
						current_position->value = *start_expression;
						start_expression++;
					}
					else
						this->~Tree(),
						start_expression = end_expression;
				}
				else	if (*start_expression == ')') {
					if (stage && CLOSE_BRACKET) {
						stage = OPERATION | CLOSE_BRACKET;
						current_position = current_position->parent;
						start_expression++;
					}
					else
						this->~Tree(),
						start_expression = end_expression;
				}
			}
		}
	}
}

void Tree::gen_random_tree(size_t max_length, unsigned char* symbols, size_t number_symbols)
{
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	ptrdiff_t  _list_stage = OPERATION;
	Node<float>* current_position = NULL;
	size_t length = 0;
	do {
		length++;
		if (max_length < length) {
			this->~Tree();
			current_position = NULL;
			_list_stage = OPERATION;
			length ^= length;
		}
		switch (_list_stage) {
		case OPERATION: {
			_list_stage = flip(0.5f) ? OPERATION : NUMBER;
			size_t index = (rand() % (sizeof(_list_operation) / sizeof(*_list_operation)));
			Node<float>* current = new Node<float>((float)_list_operation[index], OPERATION, NULL, NULL, current_position);
			if (current_position) {
				if (!current_position->left)
					current_position->left = current,
					current_position->left->parent = current_position,
					current_position = current_position->left;
				else if (!current_position->right)
					current_position->right = current,
					current_position->right->parent = current_position,
					current_position = current_position->right;
				else if (current_position->parent)
					current_position = current_position->parent;
			}
			else {
				current_position = current;
				root = current_position;
			}
			break;
		}
		case NUMBER:
			_list_stage = flip(0.5f) ? OPERATION : NUMBER;
			Node<float>* current;
			if (flip(0.5f) && number_symbols) {
				int index = rand() % number_symbols;
				current = new Node<float>(symbols[index], SYMBOL, NULL, NULL, current_position);
			}
			else {
				float number = (float)rand() / RAND_MAX;
				current = new Node<float>(number, NUMBER, NULL, NULL, current_position);
			}

			if (!current_position->left) {
				current_position->left = current;
				current_position = current_position->left->parent;
			}
			else if (!current_position->right) {
				current_position->right = current;
				current_position = current_position->right->parent;
			}
			else if (current_position->parent)
				current_position = current_position->parent;

			break;
		}
	} while (!current_position->left || !current_position->right || current_position->parent);
	//printf("%d\n", length);
}

float* Tree::bound_check_numbers(float* ptr_numbers, float* numbers, size_t length_numbers) {
	if (ptr_numbers == numbers + length_numbers) {
		length_numbers += 128;
		numbers = (float*)realloc(numbers, length_numbers * sizeof(float));
		ptr_numbers = numbers + length_numbers - 128;
	}
	return ptr_numbers;
}
unsigned char* Tree::bound_check_expression(unsigned char* ptr_expression) {
	if (ptr_expression == expression + length_expression) {
		length_expression += 128;
		expression = (unsigned char*)realloc(expression, length_expression * sizeof(unsigned char));
		ptr_expression = expression + length_expression - 128;
	}
	return ptr_expression;
}
float Tree::calculate_numbers(float number_1, float number_2, int operation) {
	switch (operation) {
	case MINUS: {
		return number_1 - number_2;
		break;
	}
	case PLUS: {
		return number_1 + number_2;
		break;
	}
	case MULTIPLY: {
		return number_1 * number_2;
		break;
	}
	case DIVIDE: {
		return number_1 / number_2;
		break;
	}
	}
	return -0.0f;
}
unsigned char* Tree::string_current_stage(unsigned char* ptr_expression, Node<float>* current_position) {
	switch (current_position->stage) {
	case OPERATION: {
		*ptr_expression = current_position->value;
		ptr_expression++;
		break;
	}
	case NUMBER: {
		char number[32] = { 0 };
		char* ptr_number = number;
		sprintf_s(number, 32, "%f", current_position->value);
		while (*ptr_number != 0) {
			*ptr_expression++ = *ptr_number++;
			ptr_expression = bound_check_expression(ptr_expression);
		}
		break;
	}
	case SYMBOL: {
		*ptr_expression = current_position->value;
		ptr_expression++;
		break;
	}
	}
	return ptr_expression;
}
unsigned char* Tree::view_tree()
{
	Node<float>* current_position = root;
	Node<float>* previos_position = NULL;
	length_expression = 128;
	expression = (unsigned char*)malloc(length_expression * sizeof(unsigned char));
	unsigned char* ptr_expression = expression;
	unsigned char* ptr_end_expression = expression + length_expression;

	while (current_position) {
		if (ptr_expression == ptr_end_expression) {
			length_expression += 128;
			expression = (unsigned char*)realloc(expression, length_expression * sizeof(unsigned char));
			ptr_expression = expression + length_expression - 128;
			ptr_end_expression = expression + length_expression;
		}
		if (previos_position == current_position->parent) {
			previos_position = current_position;
			if (current_position->left) {
				*ptr_expression++ = '(';
				current_position = current_position->left;
			}
			else {
				ptr_expression = string_current_stage(ptr_expression, current_position);
				ptr_end_expression = expression + length_expression;
				if (current_position->right)
					current_position = current_position->right;
				else
					current_position = current_position->parent;
			}
		}
		else if (previos_position == current_position->left) {
			previos_position = current_position;
			ptr_expression = string_current_stage(ptr_expression, current_position);
			ptr_end_expression = expression + length_expression;
			if (current_position->right)
				current_position = current_position->right;
			else
				current_position = current_position->parent;
		}
		else if (previos_position == current_position->right) {
			previos_position = current_position;
			current_position = current_position->parent;
			*ptr_expression++ = ')';
		}
	}
	*ptr_expression = 0;
	return expression;
}
float Tree::calculate_tree(float* value_arguments, unsigned char* arguments, size_t number_arguments)
{
	Node<float>* current_position = root;
	Node<float>* previos_position = NULL;
	size_t number_positions = 128;
	Node<float>** start_positions = (Node<float> * *)malloc(number_positions * sizeof(Node<float> * *));
	Node<float>** ptr_start_positions = start_positions;
	Node<float>** ptr_end_start_positions = start_positions + number_positions;
	size_t index = 0;
	size_t number_symbol = (NUMBER | SYMBOL);
	while (current_position) {
		if (ptr_start_positions == ptr_end_start_positions) {
			number_positions += 128;
			start_positions = (Node<float> * *)realloc(start_positions, number_positions * sizeof(Node<float> * *));
			ptr_start_positions = start_positions + number_positions - 128;
			ptr_end_start_positions = start_positions + number_positions;
		}
		if (previos_position == current_position->parent) {
			previos_position = current_position;
			if (current_position->left) {
				current_position = current_position->left;
			}
			else {
				if (current_position->right)
					current_position = current_position->right;
				else {
					if (current_position->parent->left->stage & number_symbol && current_position->parent->right->stage & number_symbol) {
						*(ptr_start_positions + index) = current_position;
						index++;
					}
					current_position = current_position->parent;
				}
			}
		}
		else if (previos_position == current_position->left) {
			previos_position = current_position;
			if (current_position->right)
				current_position = current_position->right;
			else {
				current_position = current_position->parent;
			}
		}
		else if (previos_position == current_position->right) {
			previos_position = current_position;
			current_position = current_position->parent;
		}
	}
	size_t number_sum = (index >> 1);
	float* sum = (float*)malloc(number_sum * sizeof(float));
	ptr_start_positions = start_positions;
	for (size_t i = 0; i < number_sum; i++) {
		Node<float>* left = start_positions[i * 2];
		Node<float>* right = start_positions[i * 2 + 1];
		if (left->stage == NUMBER && right->stage == NUMBER)
			sum[i] = calculate_numbers(left->value, right->value, left->parent->value);
		else if (left->stage == SYMBOL && right->stage == NUMBER) {
			float value;
			for (size_t i = 0; i < number_arguments; i++)
				if (arguments[i] == left->value)
					value = value_arguments[i],
					i = number_arguments;
			sum[i] = calculate_numbers(value, right->value, left->parent->value);
		}
		else if (left->stage == NUMBER && right->stage == SYMBOL) {
			float value;
			for (size_t i = 0; i < number_arguments; i++)
				if (arguments[i] == right->value)
					value = value_arguments[i],
					i = number_arguments;
			sum[i] = calculate_numbers(left->value, value, left->parent->value);
		}
		//printf("%f\n", sum[i]);
		ptr_start_positions[i] = ptr_start_positions[i * 2]->parent;
	}
	Node<float>** stage = ptr_start_positions + number_sum;

	float result;
	for (size_t i = 0; i < number_sum; i++) {
		while (stage[i] != NULL && ptr_start_positions[i]->parent) {
			Node<float>* left = ptr_start_positions[i]->parent->left;
			Node<float>* right = ptr_start_positions[i]->parent->right;
			if (left->stage == NUMBER) {
				//printf("%f %f\n", left->value, sum[i]),
				sum[i] = calculate_numbers(left->value, sum[i], left->parent->value);
				ptr_start_positions[i] = ptr_start_positions[i]->parent;
			}
			else if (right->stage == NUMBER) {
				//printf("%f %f\n", sum[i], right->value),
				sum[i] = calculate_numbers(sum[i], right->value, right->parent->value);
				ptr_start_positions[i] = ptr_start_positions[i]->parent;
			}
			if (left->stage == SYMBOL) {
				float value;
				for (size_t i = 0; i < number_arguments; i++)
					if (arguments[i] == left->value)
						value = value_arguments[i],
						i = number_arguments;
				sum[i] = calculate_numbers(value, sum[i], left->parent->value);
				ptr_start_positions[i] = ptr_start_positions[i]->parent;
			}
			else if (right->stage == SYMBOL) {
				float value;
				for (size_t i = 0; i < number_arguments; i++)
					if (arguments[i] == right->value)
						value = value_arguments[i],
						i = number_arguments;
				sum[i] = calculate_numbers(sum[i], value, right->parent->value);
				ptr_start_positions[i] = ptr_start_positions[i]->parent;
			}
			else {
				if (i > 0) {
					if (ptr_start_positions[i - 1]->parent == ptr_start_positions[i]->parent) {
						if (ptr_start_positions[i]->parent->left == ptr_start_positions[i])
							//printf("%f %f\n", sum[i], sum[i - 1]),
							sum[i] = calculate_numbers(sum[i], sum[i - 1], ptr_start_positions[i]->parent->value);
						else
							//printf("%f %f\n", sum[i - 1], sum[i]),
							sum[i] = calculate_numbers(sum[i - 1], sum[i], ptr_start_positions[i]->parent->value);
						ptr_start_positions[i] = ptr_start_positions[i]->parent;
					}
					else
						stage[i] = NULL;
				}
				else
					stage[i] = NULL;
			}
		}
		result = sum[i];
	}
	free(sum);
	free(start_positions);
	return result;
}
Tree::Tree()
{
	for (size_t i = 0; i < sizeof(*this); i++)
		((char*)this)[i] = 0;
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