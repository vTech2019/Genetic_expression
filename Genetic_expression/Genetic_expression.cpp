// Genetic_expression.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Tree.h"
#include "Tree_function.h"

int main()
{
	Tree<double> function;
	Tree<double> function2;
	function.set_expression((unsigned char*) "(((4/x)-(4/(x+2)))+y)" + 0, 0, (unsigned char*) "xy", 2);
	std::cout << function.view_tree() << std::endl;
	function2 = function;
	std::cout << function2.view_tree() << std::endl;
	float result = 0.0f;
	for (size_t i = 1; i < 80000; i += 4) {
		double number[] = { i, result };
		result = function.calculate_tree(number, (unsigned char*)"xy", 2);
	}
	std::cout << result;
	Tree<double>* tree = new Tree<double>[1000];
	Tree_genetic<double> genetic_function;
	genetic_function.set_main(&function);
	double number[] = { 1, 0 };
	double step_argument[] = { 4, 0 };
	genetic_function.set_arguments(number, (unsigned char*)"xy", 2, 1000, 'y', step_argument);
	for (size_t i = 0; i < 1000; i++)
		tree[i].gen_random_tree(64, (unsigned char*)"xy", 2),
		genetic_function.push_tree(&tree[i]);
	genetic_function.start_tree_genetic();
	delete[] tree;
	while (true)
		std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
}