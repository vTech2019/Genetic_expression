#include "Header.h"

template<class T>
bool Tree<T>::flip(T number)
{
	T random = (T)rand() / RAND_MAX;
	if (random < number)
		return true;
	else
		return false;
}
size_t get_random() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis(0, UINT64_MAX);
	return dis(gen);
}
double get_random(double min, double max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(min, max);
	return dis(gen);
}
float get_random(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}
template<class T>
T Tree<T>::parse_number(unsigned char*& string, unsigned char& number) {
	T result = 0.0f;
	ptrdiff_t integer_part = 0;
	if (*string < '0' || *string > '9') {
		number = 1;
		return 0;
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
		T result_div = 0.0f;
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
template<typename T>
size_t Tree<T>::get_number_component_tree()
{
	if (number_component_tree == 0) {
		calculate_number_node();
	}
	return number_component_tree - 1;
}
template<class T>
void Tree<T>::set_expression(unsigned char* expression, size_t length_expression, unsigned char* symbols, size_t number_symbols)
{
	number_component_tree = 0;
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	unsigned char* start_expression = expression;
	unsigned char* end_expression = expression + length_expression;
	ptrdiff_t stage = OPEN_BRACKET;
	Node<T>* current_position = NULL;
	while (start_expression != 0 && *start_expression != 0) {
		if (*start_expression == '(') {
			if (stage & OPEN_BRACKET) {
				stage = NUMBER | OPEN_BRACKET;
				Node<T>* current = new Node<T>(0, OPEN_BRACKET, NULL, NULL, current_position);
				if (current_position) {
					if (!current_position->left)
						number_component_tree++,
						current_position->left = current,
						current_position->left->parent = current_position,
						current_position = current_position->left;
					else if (!current_position->right)
						number_component_tree++,
						current_position->right = current,
						current_position->right->parent = current_position,
						current_position = current_position->right;
					else {
						delete current;
					}
				}
				else
					number_component_tree++,
					current_position = current,
					root = current_position;

				start_expression++;
			}
			else
				this->~Tree(),
				start_expression = 0;
		}
		else {
			unsigned char number_or_symbol = 0;
			T number = parse_number(start_expression, number_or_symbol);
			unsigned char* find_symbol = NULL;
			for (size_t i = 0; i < number_symbols && !find_symbol; i++)
				find_symbol = symbols[i] == *start_expression ? symbols + i : NULL;

			if (!number_or_symbol || find_symbol) {
				if (stage & NUMBER) {
					stage = OPERATION | CLOSE_BRACKET;
					Node<T>* current;
					if (find_symbol) {
						current = new Node<T>(*find_symbol, SYMBOL, NULL, NULL, current_position);
					}
					else {
						current = new Node<T>(number, NUMBER, NULL, NULL, current_position);
					}
					if (!current_position->left) {
						number_component_tree++,
							current_position->left = current;
						current_position = current_position->left->parent;
					}
					else if (!current_position->right) {
						number_component_tree++,
							current_position->right = current;
						current_position = current_position->right->parent;
					}
					else if (current_position->parent)
						delete current,
						current_position = current_position->parent;
					if (number_or_symbol)
						start_expression++;
				}
				else
					this->~Tree(),
					start_expression = 0;
			}
			else {
				bool find_operator = false;
				for (size_t i = 0; i < sizeof(_list_operation) && !find_operator; i++)
					find_operator = *start_expression == _list_operation[i] ? true : false;
				if (find_operator) {
					if (stage & OPERATION) {
						stage = NUMBER | OPEN_BRACKET;
						current_position->stage = OPERATION;
						current_position->value = *start_expression;
						start_expression++;
					}
					else
						this->~Tree(),
						start_expression = 0;
				}
				else	if (*start_expression == ')') {
					if (stage & CLOSE_BRACKET) {
						stage = OPERATION | CLOSE_BRACKET;
						current_position = current_position->parent;
						start_expression++;
					}
					else
						this->~Tree(),
						start_expression = 0;
				}
			}
		}
	}
}

template<class T>
void Tree<T>::gen_random_tree(size_t max_length, unsigned char* symbols, size_t number_symbols, T min, T max)
{
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	number_component_tree = 0;
	ptrdiff_t  _list_stage = OPERATION;
	Node<T>* current_position = NULL;
	size_t length = 0;
	size_t* index_symbols = (size_t*)_malloca(number_symbols * sizeof * index_symbols);
	size_t number_index_symbols = 0;
	do {
		do {
			length++;
			if (max_length < length) {
				this->~Tree();
				_list_stage = OPERATION;
				current_position = NULL;
				length = 0;
				number_index_symbols = 0;
			}
			switch (_list_stage) {
			case OPERATION: {
				_list_stage = flip(0.5f) ? OPERATION : NUMBER;
				size_t index = (get_random() % (sizeof(_list_operation) / sizeof(*_list_operation)));
				if (current_position) {
					if (!current_position->left)
						number_component_tree++,
						current_position->left = new Node<T>((T)_list_operation[index], OPERATION, NULL, NULL, current_position),
						current_position->left->parent = current_position,
						current_position = current_position->left;
					else if (!current_position->right)
						number_component_tree++,
						current_position->right = new Node<T>((T)_list_operation[index], OPERATION, NULL, NULL, current_position),
						current_position->right->parent = current_position,
						current_position = current_position->right;
					else if (current_position->parent) {
						current_position = current_position->parent;
					}
				}
				else {
					number_component_tree++;
					current_position = new Node<T>((T)_list_operation[index], OPERATION, NULL, NULL, current_position);
					root = current_position;
				}
				break;
			}
			case NUMBER:
				_list_stage = flip(0.5f) ? OPERATION : NUMBER;
				Node<T>* current = (Node<T>*)(!current_position->left | !current_position->right);
				if (current) {
					if (flip(0.5f) && number_symbols) {
						size_t index = get_random() % number_symbols;
						current = new Node<T>(symbols[index], SYMBOL, NULL, NULL, current_position);
						bool find_symbol = false;
						for (size_t i = 0; i < number_index_symbols; i++) {
							if (index_symbols[i] == index)
								find_symbol = true,
								i = number_index_symbols;
						}
						if (!find_symbol)
							index_symbols[number_index_symbols] = index,
							number_index_symbols++;
					}
					else {
						T number = get_random(min, max); 
						current = new Node<T>(number, NUMBER, NULL, NULL, current_position);
					}
				}
				if (!current_position->left) {
					number_component_tree++;
					current_position->left = current;
					current_position = current_position->left->parent;
				}
				else if (!current_position->right) {
					number_component_tree++;
					current_position->right = current;
					current_position = current_position->right->parent;
				}
				else if (current_position->parent) {
					current_position = current_position->parent;
				}

				break;
			}
		} while (!current_position->left || !current_position->right || current_position->parent);
		if (number_index_symbols != number_symbols) {
			this->~Tree();
			_list_stage = OPERATION;
			current_position = NULL;
			length = 0;
			number_index_symbols = 0;
		}
	} while (number_index_symbols != number_symbols);
	_freea(index_symbols);
}

template<class T>
T* Tree<T>::bound_check_numbers(T* ptr_numbers, T* numbers, size_t length_numbers) {
	if (ptr_numbers == numbers + length_numbers) {
		length_numbers += 128;
		numbers = (T*)realloc(numbers, length_numbers * sizeof(T));
		ptr_numbers = numbers + length_numbers - 128;
	}
	return ptr_numbers;
}
template<class T>
unsigned char* Tree<T>::bound_check_expression(unsigned char* ptr_expression) {
	if (ptr_expression == expression + length_expression) {
		length_expression += 128;
		expression = (unsigned char*)realloc(expression, length_expression * sizeof(unsigned char));
		ptr_expression = expression + length_expression - 128;
	}
	return ptr_expression;
}
template<class T>
T Tree<T>::calculate_numbers(T number_1, T number_2, int operation) {
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
		//number_2 = number_2 == 0 ? 0.1e-32 : number_2;
		//number_1 = number_1 == 0 ? 0.1e-32 : number_1;
		return number_1 * number_2;
		break;
	}
	case DIVIDE: {
		//number_2 = number_2 == 0 ? 0.1e-32 : number_2;
		//number_1 = number_1 == 0 ? 0.1e-32 : number_1;
		return number_1 / number_2;
		break;
	}
	}
	return -0.0;
}
template<class T>
unsigned char* Tree<T>::string_current_stage(unsigned char* ptr_expression, Node<T>* current_position) {
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
template<class T>
unsigned char* Tree<T>::view_tree()
{
	Node<T>* current_position = root;
	Node<T>* previos_position = NULL;
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

template<class T>
T Tree<T>::calculate_tree(T* value_arguments, unsigned char* arguments, size_t number_arguments)
{
	Node<T>* current_position = root;
	Node<T>* previos_position = NULL;
	size_t number_positions = 128;
	Node<T>** start_positions = (Node<T> * *)malloc(number_positions * sizeof(Node<T> * *));
	Node<T>** ptr_start_positions = start_positions;
	Node<T>** ptr_end_start_positions = start_positions + number_positions;
	size_t index = 0;
	size_t number_symbol = (NUMBER | SYMBOL);
	while (current_position) {
		if (ptr_start_positions == ptr_end_start_positions) {
			number_positions += 128;
			start_positions = (Node<T> * *)realloc(start_positions, number_positions * sizeof(Node<T> * *));
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
	T* sum = (T*)malloc(number_sum * sizeof(T));
	ptr_start_positions = start_positions;
	for (size_t i = 0; i < number_sum; i++) {
		Node<T>* left = start_positions[i * 2];
		Node<T>* right = start_positions[i * 2 + 1];
		if (left->stage == NUMBER && right->stage == NUMBER)
			sum[i] = calculate_numbers(left->value, right->value, left->parent->value);
		else if (left->stage == SYMBOL && right->stage == NUMBER) {
			T value;
			for (size_t i = 0; i < number_arguments; i++)
				if (arguments[i] == left->value)
					value = value_arguments[i],
					i = number_arguments;
			sum[i] = calculate_numbers(value, right->value, left->parent->value);
		}
		else if (left->stage == NUMBER && right->stage == SYMBOL) {
			T value;
			for (size_t i = 0; i < number_arguments; i++)
				if (arguments[i] == right->value)
					value = value_arguments[i],
					i = number_arguments;
			sum[i] = calculate_numbers(left->value, value, left->parent->value);
		}
		//printf("%f\n", sum[i]);
		ptr_start_positions[i] = ptr_start_positions[i * 2]->parent;
	}
	Node<T>** stage = ptr_start_positions + number_sum;

	T result = 0;
	for (size_t i = 0; i < number_sum; i++) {
		while (stage[i] != NULL && ptr_start_positions[i]->parent) {
			Node<T>* left = ptr_start_positions[i]->parent->left;
			Node<T>* right = ptr_start_positions[i]->parent->right;
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
				T value;
				for (size_t i = 0; i < number_arguments; i++)
					if (arguments[i] == left->value)
						value = value_arguments[i],
						i = number_arguments;
				sum[i] = calculate_numbers(value, sum[i], left->parent->value);
				ptr_start_positions[i] = ptr_start_positions[i]->parent;
			}
			else if (right->stage == SYMBOL) {
				T value;
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
template<class T>
void Tree<T>::operator=(Tree<T>* tree)
{
	this->~Tree();
	number_component_tree = 0;
	size_t N = 1;
	Node<T>** memory = NULL;
	Node<T>** memory_copy = NULL;
	Node<T>** memory_next = NULL;
	Node<T>** memory_copy_next = NULL;
	if (tree->root) {
		number_component_tree++;
		memory = (Node<T> * *) realloc(memory, N * sizeof(Node<T> * *));
		memory_copy = (Node<T> * *) realloc(memory_copy, N * sizeof(Node<T> * *));
		memory_next = (Node<T> * *) realloc(memory_next, 2 * N * sizeof(Node<T> * *));
		memory_copy_next = (Node<T> * *) realloc(memory_copy_next, 2 * N * sizeof(Node<T> * *));
		memory[0] = tree->root;
		memory_copy[0] = new Node<T>(memory[0]->value, memory[0]->stage, NULL, NULL, NULL);
		root = memory_copy[0];
		size_t number = 1;
		while (number != 0) {
			number = 0;
			for (size_t i = 0; i < N; i++) {
				Node<T>* tmp_memory_copy = memory_copy[i];
				Node<T>* tmp_memory = memory[i];
				if (tmp_memory) {
					memory_next[number] = tmp_memory->left ? tmp_memory->left : NULL;
					if (memory_next[number]) {
						number_component_tree++;
						memory_copy_next[number] = new Node<T>(memory_next[number]->value, memory_next[number]->stage, NULL, NULL, tmp_memory_copy);
						tmp_memory_copy->left = memory_copy_next[number];
						number++;
					}
					memory_next[number] = tmp_memory->right ? tmp_memory->right : NULL;
					if (memory_next[number]) {
						number_component_tree++;
						memory_copy_next[number] = new Node<T>(memory_next[number]->value, memory_next[number]->stage, NULL, NULL, tmp_memory_copy);
						tmp_memory_copy->right = memory_copy_next[number];
						number++;
					}
				}
			}
			N = number * 2;
			memory = (Node<T> * *) realloc(memory, N * sizeof(Node<T> * *));
			memory_copy = (Node<T> * *) realloc(memory_copy, N * sizeof(Node<T> * *));
			memory_next = (Node<T> * *) realloc(memory_next, 2 * N * sizeof(Node<T> * *));
			memory_copy_next = (Node<T> * *) realloc(memory_copy_next, 2 * N * sizeof(Node<T> * *));
			for (size_t i = 0; i < number; i++) {
				memory[i] = memory_next[i];
				memory_copy[i] = memory_copy_next[i];
			}
			for (size_t i = number; i < N; i++)
				memory[i] = 0,
				memory_copy[i] = 0;
		}
		free(memory);
		free(memory_copy);
		free(memory_next);
		free(memory_copy_next);
	}
}
template<typename T>
void Tree<T>::operator=(size_t i)
{
	for (size_t j = 0; j < sizeof(*this); j++)
		((char*)this)[j] = i;
}
template<class T>
void Tree<T>::calculate_number_node() {
	Node<T>* current_position = root;
	Node<T>* previos_position = NULL;
	number_component_tree = 0;

	while (current_position) {
		if (previos_position == current_position->parent) {
			previos_position = current_position;
			if (current_position->left) {
				current_position = current_position->left;
			}
			else {
				number_component_tree++;
				if (current_position->right)
					current_position = current_position->right;
				else
					current_position = current_position->parent;
			}
		}
		else if (previos_position == current_position->left) {
			previos_position = current_position;
			number_component_tree++;
			if (current_position->right)
				current_position = current_position->right;
			else
				current_position = current_position->parent;
		}
		else if (previos_position == current_position->right) {
			previos_position = current_position;
			current_position = current_position->parent;
		}
	}
}
template<class T>
Node<T>* Tree<T>::find_node(size_t index) {
	Node<T>* current_position = root;
	Node<T>* previos_position = NULL;
	number_component_tree = 0;

	while (current_position || index != 0) {
		if (previos_position == current_position->parent) {
			previos_position = current_position;
			if (current_position->left) {
				current_position = current_position->left;
			}
			else {
				if (!index)
					return &(*current_position);
				index--;
				if (current_position->right)
					current_position = current_position->right;
				else
					current_position = current_position->parent;
			}
		}
		else if (previos_position == current_position->left) {
			previos_position = current_position;
			if (!index)
				return &(*current_position);
			index--;
			if (current_position->right)
				current_position = current_position->right;
			else
				current_position = current_position->parent;
		}
		else if (previos_position == current_position->right) {
			previos_position = current_position;
			current_position = current_position->parent;
		}
	}
	return &(*current_position);
}
template<typename T>
Node<T>* Tree<T>::operator[](size_t index)
{
	return find_node(index);
}
template<class T>
Node<T>* Tree<T>::get_ptr(size_t index) {
	return find_node(index);;
}
template<class T>
Tree<T>::Tree()
{
	for (size_t i = 0; i < sizeof(*this); i++)
		((char*)this)[i] = 0;
}

template<class T>
Tree<T>::~Tree()
{
	number_component_tree = 0;
	if (expression)
		free(expression);
	expression = NULL;
	Node<T>* current_position = root;
	while (current_position) {
		while (current_position->left) {
			current_position = current_position->left;
		}
		while (current_position->right) {
			current_position = current_position->right;
		}
		if (!current_position->left && !current_position->right) {
			Node<T>* delete_position = current_position;
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
	root = 0;
}

void TreeFunction()
{
	Tree<float> tree_float;
	Tree<int> tree_int;
	Tree<double> tree_double;
	tree_float.operator=(tree_float);
	tree_float.calculate_tree(NULL, NULL, NULL);
	tree_float.gen_random_tree(NULL, NULL, NULL, NULL, NULL);
	//	tree_float.set_expression(NULL, NULL, NULL, NULL);
	tree_float.view_tree();
	tree_int.calculate_tree(NULL, NULL, NULL);
	//tree_int.gen_random_tree(NULL, NULL, NULL);
	//	tree_int.set_expression(NULL, NULL, NULL, NULL);
	tree_int.view_tree();
	tree_double.calculate_tree(NULL, NULL, NULL);
	tree_double.gen_random_tree(NULL, NULL, NULL, NULL, NULL);
	tree_double.set_expression(NULL, NULL, NULL, NULL);
	tree_double.view_tree();
	tree_double.operator=(tree_double);
	tree_double.operator[](0);
	tree_double.get_ptr(0);
	tree_double.get_number_component_tree();
}