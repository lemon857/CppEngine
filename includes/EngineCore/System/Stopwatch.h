#pragma once

#include <chrono>

class Stopwatch
{
public:
	Stopwatch();

	void start();
	// ������ ������� �� ������� ����� ������� ������� ����� � ����
	double stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock>  m_timeStart;
};
