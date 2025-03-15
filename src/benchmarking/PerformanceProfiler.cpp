#include "PerformanceProfiler.h"

PerformanceProfiler::PerformanceProfiler(ParticleEmitter* particleSystem)
	:particleSystem(particleSystem)
{
}

PerformanceProfiler::~PerformanceProfiler()
{
}

void PerformanceProfiler::BenchMark(double frameTime)
{
	currentFrameData.frameTime = frameTime * 1000.0f;

	if (particleSystem)
	{
		currentFrameData.updateTime = particleSystem->GetUpdateTime();
		currentFrameData.renderTime = particleSystem->GetRenderTime();
		currentFrameData.activeParticleCount = particleSystem->GetActiveParticleCount();
		currentFrameData.particleGPUSizeBytes = particleSystem->GetParticleGPUSizeBytes();
		currentFrameData.totalParticlesGPUSizeBytes = particleSystem->GetTotalParticlesGPUSizeBytes();
		currentFrameData.totalDataTransferPerUpdateBytes = particleSystem->GetTotalDataTransferBytes();
		currentFrameData.drawCallsPerFrame = particleSystem->GetTotalDrawCalls();
	}

	if (recordData && currentFrame < maxCaptureFrames)
	{
		data[currentFrame] = currentFrameData;
		currentFrame++;
	}
}

void PerformanceProfiler::Display()
{
	ImGui::Begin("Performance Metrics");
	ImGui::Text("Frame Time: %.3f ms", currentFrameData.frameTime);
	ImGui::Text("Update Time: %.3f ms", currentFrameData.updateTime);
	ImGui::Text("Render Time: %.3f ms", currentFrameData.renderTime);
	ImGui::Text("Active Particles: %u", currentFrameData.activeParticleCount);
	ImGui::Text("Particle GPU Size: %u bytes", currentFrameData.particleGPUSizeBytes);
	ImGui::Text("Total Particles GPU Size: %u bytes", currentFrameData.totalParticlesGPUSizeBytes);
	ImGui::Text("Data Transfer Per Update: %u bytes", currentFrameData.totalDataTransferPerUpdateBytes);
	ImGui::Text("Draw Calls Per Frame: %u", currentFrameData.drawCallsPerFrame);

	ImGui::End();
}
