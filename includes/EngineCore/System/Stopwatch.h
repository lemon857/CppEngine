#pragma once

#include <chrono>

class Stopwatch
{
public:
	Stopwatch();

	void start();
	// ����� ������� �� ������� ����� ������� ������� ����� � ����
	double stop();

private:
	std::chrono::steady_clock::time_point m_timeStart;
};
