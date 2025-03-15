#pragma once

struct PerformanceMetrics
{
	double frameTime;
	double updateTime;
	double renderTime;
	unsigned int activeParticleCount;
	unsigned int particleGPUSizeBytes;
	unsigned int totalParticlesGPUSizeBytes;
	unsigned int totalDataTransferPerUpdateBytes;
	unsigned int drawCallsPerFrame;
};