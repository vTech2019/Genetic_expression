#pragma once
struct Parameters {
	char* expression;
	void* step;
	void* step_parameters;
	size_t number_parameters;
	size_t number_expressions;
	size_t max_length_expression;
};
class WinApi
{
	HWND main_window;
	WNDCLASS window_class;
	HRESULT UpdateWindow(HWND hwnd);
public:
	~WinApi();
	WinApi(TCHAR* name_window, Parameters* data);
};

