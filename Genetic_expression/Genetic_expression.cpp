﻿// Genetic_expression.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Tree.h"
int main()
{
	Tree function;
	function.set_expression((unsigned char*) "(((1/x)-(1/(x+2)))+y)", sizeof("(((1/x)-(1/(x+2)))+y)") - 1, (unsigned char*) "xy", 2);
	std::cout << function.view_tree() << std::endl;
	float result = 0.0f;
	for (size_t i = 1; i < 8000; i+=4) {
		float number[] = { i, result };
		result = function.calculate_tree(number, (unsigned char*)"xy", 2);
	}
	std::cout << result * 4;
	Tree* tree = new Tree[1000];
	for (size_t i = 0; i < 1000; i++) {
		tree[i].gen_random_tree(256, NULL, 0);
		std::cout << tree[i].view_tree() << std::endl;
		std::cout << tree[i].calculate_tree(0, 0, 0) << std::endl;
	}
	delete[] tree;
	while (true)
		std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
	std::cout << "Hello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.