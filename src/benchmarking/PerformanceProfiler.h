#pragma once

#include "PerformanceMetrics.h"
#include "ParticleEmitter.h"

class PerformanceProfiler
{
public:
	PerformanceProfiler(ParticleEmitter* particleSystem);
	~PerformanceProfiler();

	void BenchMark(double frameTime);
	void Display();

private:

	ParticleEmitter* particleSystem;
	PerformanceMetrics currentFrameData = {};
	bool recordData = false;

	const unsigned int maxCaptureFrames = 1000;
	std::vector<PerformanceMetrics> data = std::vector<PerformanceMetrics>(maxCaptureFrames);
	unsigned int currentFrame = 0;
};