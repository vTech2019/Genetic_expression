
#include "Header.h"

template<class T>
void Tree_genetic<T>::sort_tree()
{
	unsigned char info[50][2 * 100] = { 0 };
	for (size_t i = 0; i <2 *  number_tree; i++) {
		for (size_t j = i; j < 2 * number_tree; j++) {
			if (survival_rate[i] < survival_rate[j]) {
				Tree<T>* tmp_tree = all_tree[i];
				all_tree[i] = all_tree[j];
				all_tree[j] = tmp_tree;
				double tmp_rate = survival_rate[i];
				survival_rate[i] = survival_rate[j];
				survival_rate[j] = tmp_rate;
			}
		}
	}
	for (size_t i = 0; i < 2 * number_tree; i++) {
		for (size_t j = 0; j < survival_rate[i] * 50; j++) {
			info[j][i] = 46;
		}
		printf("%s\n", all_tree[i]->view_tree());
	}
	for (size_t i = 0; i < 50; i++)
		if (info[0][i])
			printf("%s\n", info[i]);
	for (size_t i = 0; i < number_iterations; i++) {
		tmp_arguments_function[result_symbol] = all_tree[0]->calculate_tree(tmp_arguments_function, symbols_function, number_symbols);
		for (size_t j = 0; j < number_symbols; j++)
			tmp_arguments_function[j] += steps_arguments[j];
	}
	double value = tmp_arguments_function[result_symbol];
	for (size_t i = 0; i < number_symbols; i++)
		tmp_arguments_function[i] = arguments_function[i];
	printf("value function %f\n", value);
	//printf("%s\n", all_tree[0]->view_tree());
}

template<class T>
void Tree_genetic<T>::set_arguments(T* arguments, unsigned char* symbols, size_t number_symbols, size_t number_iterations, unsigned char result_symbol, T* step_arguments)
{
	tmp_arguments_function = (T*)realloc(tmp_arguments_function, number_symbols * sizeof(*tmp_arguments_function));
	arguments_function = (T*)realloc(arguments_function, number_symbols * sizeof(*arguments_function));
	steps_arguments = (T*)realloc(steps_arguments, number_symbols * sizeof(*steps_arguments));
	symbols_function = (unsigned char*)realloc(symbols_function, number_symbols * sizeof(unsigned char));
	for (size_t i = 0; i < number_symbols; i++)
		arguments_function[i] = arguments[i],
		tmp_arguments_function[i] = arguments[i];
	for (size_t i = 0; i < number_symbols; i++) {
		if (symbols[i] == result_symbol)
			this->result_symbol = i;
		symbols_function[i] = symbols[i];
		steps_arguments[i] = step_arguments[i];
	}
	this->number_symbols = number_symbols;
	this->number_iterations = number_iterations;
}

template<class T>
void Tree_genetic<T>::push_tree(Tree<T>* ptr_tree)
{
	survival_rate = (double*)realloc(survival_rate, (number_tree + 1) * sizeof(double));
	all_tree = (Tree<T> * *)realloc(all_tree, (number_tree + 1) * sizeof(Tree<T> * *));
	all_tree[number_tree] = ptr_tree;
	number_tree++;
}

template<class T>
void Tree_genetic<T>::fitness()
{
	T value = 0;
	if (main_tree) {
		for (size_t i = 0; i < number_iterations; i++) {
			tmp_arguments_function[result_symbol] = main_tree->calculate_tree(tmp_arguments_function, symbols_function, number_symbols);
			for (size_t j = 0; j < number_symbols; j++)
				tmp_arguments_function[j] += steps_arguments[j];
		}
		value = tmp_arguments_function[result_symbol];
	}

	for (size_t i = 0; i < number_symbols; i++)
		tmp_arguments_function[i] = arguments_function[i];
	if (arguments_function) {
		double div = 0.0f;
		for (size_t i = 0; i < 2 * number_tree; i++) {
			for (size_t j = 0; j < number_iterations; j++) {
				tmp_arguments_function[result_symbol] = all_tree[i]->calculate_tree(tmp_arguments_function, symbols_function, number_symbols);
				for (size_t j = 0; j < number_symbols; j++)
					tmp_arguments_function[j] += steps_arguments[j];
			}
			if (!isnan((double)tmp_arguments_function[result_symbol])) {
				survival_rate[i] = (double)value - tmp_arguments_function[result_symbol];
				if (survival_rate[i] < 0)
					survival_rate[i] = -survival_rate[i];
				div += 1.0 / survival_rate[i];
			}
			else {
				survival_rate[i] = 0xffffffffffffffff;
			}
			for (size_t i = 0; i < number_symbols; i++)
				tmp_arguments_function[i] = arguments_function[i];
		}
		for (size_t i = 0; i < 2 * number_tree; i++) {
			survival_rate[i] = (1 / survival_rate[i]) / div;
			//printf("survival rate %f\n", survival_rate[i]);
			//for (size_t k = 0; k < number_iterations; k++) {
			//	tmp_arguments_function[result_symbol] = all_tree[i]->calculate_tree(tmp_arguments_function, symbols_function, number_symbols);
			//	for (size_t j = 0; j < number_symbols; j++)
			//		tmp_arguments_function[j] += steps_arguments[j];
			//}
			//double value = tmp_arguments_function[result_symbol];
			//for (size_t i = 0; i < number_symbols; i++)
			//	tmp_arguments_function[i] = arguments_function[i];
			printf("%f\n", survival_rate[i]);
		}
	}
}

template<class T>
void Tree_genetic<T>::set_main(Tree<T>* main_tree)
{
	this->main_tree = main_tree;
}

template<class T>
void Tree_genetic<T>::crossing()
{
	for (size_t i = number_tree; i < 2 * number_tree; i += 2) {
		//printf("%s\n", all_tree[i]->view_tree());
		//printf("%s\n", all_tree[i+1]->view_tree());
		size_t index_1 = get_random() % all_tree[i]->get_number_component_tree();
		size_t index_2 = get_random() % all_tree[i + 1]->get_number_component_tree();
		Node<T>* node_1 = (*all_tree[i])[index_1];
		Node<T>* node_2 = (*all_tree[i + 1])[index_2];
		if (node_1->parent && node_2->parent) {
			Node<T>* tmp_node_1 = node_1->parent;
			Node<T>* tmp_node_2 = node_2->parent;
			if (node_1->parent->left == node_1)
				node_1->parent->left = node_2,
				tmp_node_1->left->parent = tmp_node_1;
			else
				node_1->parent->right = node_2,
				tmp_node_1->right->parent = tmp_node_1;

			if (tmp_node_2->left == node_2)
				tmp_node_2->left = node_1,
				tmp_node_2->left->parent = tmp_node_2;
			else
				tmp_node_2->right = node_1,
				tmp_node_2->right->parent = tmp_node_2;
			//printf("%s\n", all_tree[i]->view_tree());
			//printf("%s\n", all_tree[i + 1]->view_tree());
		}
	}
}

template<class T>
void Tree_genetic<T>::mutation()
{
	const unsigned char _list_operation[] = { MINUS, PLUS, MULTIPLY, DIVIDE };
	for (size_t i = number_tree; i < 2 * number_tree; i++) {
		//printf("%s\n", all_tree[i]->view_tree());
		size_t index_1 = get_random() % all_tree[i]->get_number_component_tree();
		Node<T>* node = (*all_tree[i])[index_1];
		if (node->stage == OPERATION) {
			size_t index = (get_random() % 4);
			while (_list_operation[index] == node->value)
				index = (get_random() % 4);
			node->value = _list_operation[index];
		}
		else if (node->stage == NUMBER) {
			T random_number = (T)get_random(-99999.999999, 99999.999999);
			while (random_number == node->value)
				random_number = (T)get_random(-99999.999999, 99999.999999);
			node->value = random_number;
		}
		//printf("%s\n", all_tree[i]->view_tree());
	}
}

template<class T>
T* Tree_genetic<T>::start_tree_genetic()
{
	if (arguments_function && symbols_function && all_tree) {
		number_tree;
		survival_rate = (double*)realloc(survival_rate, (number_tree * 2) * sizeof(double));
		all_tree = (Tree<T> * *)realloc(all_tree, (number_tree * 2) * sizeof(Tree<T> * *));
		procreation = new Tree<T>[number_tree];
		size_t y = 0;
		while (y != 1000) {
			//all_tree[0]->set_expression((unsigned char*) "((21471099638640898048.000000+(((207958440897685.687500*(((x*((y+x)/249547080859045.218750))-160114481265378.531250)/y))*(207958440897685.687500*(((x*(x/249547080859045.218750))-160114481265378.531250)/y)))+((163006364045689.437500*x)/(46485023228176580608.000000-281082064356422.187500))))+224637166321610.656250)" + 0, 0, (unsigned char*) "xy", 2);
			for (size_t i = 0; i < number_tree; i++) {
				printf("   %s\n", all_tree[i]->view_tree());
				procreation[i] = *all_tree[i];
			}
			for (size_t i = 0; i < number_tree; i++) {
				all_tree[i + number_tree] = &procreation[i];
				printf(" %s\n", all_tree[i + number_tree]->view_tree());
			}
			crossing();
			mutation();
			fitness();
			sort_tree();
			y++;
		}
	}
	return nullptr;
}

template<class T>
Tree_genetic<T>::Tree_genetic()
{
	for (size_t i = 0; i < sizeof(*this); i++)
		((char*)this)[i] = 0;
}

template<class T>
Tree_genetic<T>::~Tree_genetic()
{
	if (survival_rate)
		free(survival_rate);
	if (tmp_arguments_function)
		free(tmp_arguments_function);
	if (procreation)
		delete[] procreation;
	if (all_tree)
		free(all_tree);
	if (arguments_function)
		arguments_function;
	if (symbols_function)
		free(symbols_function);
}

void GeneticFunction()
{
	Tree_genetic<float> tree_float;
	Tree_genetic<int> tree_int;
	Tree_genetic<double> tree_double;
	tree_double.push_tree(NULL);
	tree_double.set_arguments(NULL, NULL, NULL, NULL, NULL, NULL);
	tree_double.start_tree_genetic();
	tree_double.set_main(NULL);
}