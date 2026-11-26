#pragma once

class PRNG_Engine {
private:
	inline static std::mt19937 s_Engine;

public:
	PRNG_Engine() {
		std::random_device rd;
		s_Engine.seed(rd());
	}

	int GenerateRandom(int numOfCells) {
		std::uniform_int_distribution<int> dist(0, numOfCells - 1);
		return dist(s_Engine);
	}
};

class Timer_Engine {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	bool m_isTimeStoped = true;

public:
	Timer_Engine() {
		start = end = std::chrono::high_resolution_clock::now();
	}

	void TimeStop(bool lever) {
		m_isTimeStoped = lever;
	}

	void Reset() {
		start = end = std::chrono::high_resolution_clock::now();
	}

	float ReturnDuration() {
		if (!m_isTimeStoped)
			end = std::chrono::high_resolution_clock::now();

		duration = end - start;
		return duration.count();
	}
};