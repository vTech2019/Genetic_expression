#pragma once
template<class T>
class Tree_genetic 
{
	T* steps_arguments;
	Tree<T>* main_tree;
	Tree<T>* procreation;
	Tree<T>** all_tree;
	T* arguments_function;
	T* tmp_arguments_function;
	size_t number_tree;
	size_t number_iterations;
	size_t number_symbols;
	size_t result_symbol;
	double* survival_rate;
	unsigned char* symbols_function;
	void sort_tree();
	void fitness();
	void crossing();
	void mutation();
	T calculate_function(size_t index_function);
public:
	void set_main(Tree<T>* main_tree);
	void set_arguments(T* arguments, unsigned char* symbols, size_t number_symbols, size_t number_iterations, unsigned char result_symbol, T* step_arguments);
	void push_tree(Tree<T>* ptr_tree);
	T* start_tree_genetic();
	Tree_genetic();
	~Tree_genetic();
};
