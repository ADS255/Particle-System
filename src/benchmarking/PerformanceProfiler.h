#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>

#include "PerformanceMetrics.h"
#include "ParticleEmitter.h"

class PerformanceProfiler
{
public:
	PerformanceProfiler(ParticleEmitter* particleSystem);
	~PerformanceProfiler();

	void BenchMark(double frameTime);
	void Display();

	void ExportDataCSV(const std::string& filename);

private:

	ParticleEmitter* particleSystem;
	PerformanceMetrics currentFrameData = {};
	bool recordData = false;

	std::vector<PerformanceMetrics> data = std::vector<PerformanceMetrics>(100000);
	unsigned int currentFrame = 0;

	static constexpr int MaxSamples = 100;
	std::deque<double> frameTimeHistory;
	double totalFrameTime = 0.0f;
	double averageFrameTimeMs = 0.0f;
};