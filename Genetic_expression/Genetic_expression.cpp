﻿// Genetic_expression.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Header.h"
#include <iostream>

int main()
{
	//Parameters parameters_expression = { 0 };

	//WinApi main_window((TCHAR*)TEXT("Genetic expression"), &parameters_expression);
	double number[] = { 1, 0 };
	double step_argument[] = { 4, 0 };
	size_t max_length_expression = 64;
	size_t number_expression = 2;
	size_t number_iterations = 160;
	Tree<double> function;
	function.set_expression((unsigned char*) "(((1/x)-(1/(x+2)))+y)" + 0, 0, (unsigned char*) "xy", 2);
	std::cout << function.view_tree() << std::endl;
	float result = 0.0f;
	{
		double number[] = { 1, result };
		for (size_t i = 1; i < number_iterations; i++) {
			result = function.calculate_tree(number, (unsigned char*)"xy", 2);
			*number = i * 4 + 1;
			*(number + 1) = result;
		}
	}
	std::cout << 4 * result << "   " << 3.1415 << std::endl;
	Tree<double>* tree = new Tree<double>[number_expression];
	Tree_genetic<double> genetic_function;
	genetic_function.set_main(&function);
	genetic_function.set_arguments(number, (unsigned char*)"xy", 2, number_iterations, 'y', step_argument);
	for (size_t i = 0; i < number_expression; i++)
		tree[i].gen_random_tree(max_length_expression, (unsigned char*)"xy", 2, -999.999, 999.999),
		genetic_function.push_tree(&tree[i]);
	genetic_function.start_tree_genetic();
	delete[] tree;
}