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

	double frameTimeMs = static_cast<double>(frameTime * 1000.0);
	frameTimeHistory.push_back(frameTimeMs);
	totalFrameTime += frameTimeMs;

	if (frameTimeHistory.size() > MaxSamples) {
		totalFrameTime -= frameTimeHistory.front();
		frameTimeHistory.pop_front();
	}

	averageFrameTimeMs = totalFrameTime / frameTimeHistory.size();

	if (particleSystem)
	{
		currentFrameData.activeParticleCount = particleSystem->GetActiveParticleCount();
		currentFrameData.particleGPUSizeBytes = particleSystem->GetParticleGPUSizeBytes();
		currentFrameData.totalParticlesGPUSizeBytes = particleSystem->GetTotalParticlesGPUSizeBytes();
		currentFrameData.totalDataTransferPerUpdateBytes = particleSystem->GetTotalDataTransferBytes();
		currentFrameData.drawCallsPerFrame = particleSystem->GetTotalDrawCalls();
	}

	if (recordData && particleSystem && particleSystem->activeParticleCount > 0)
	{
		data.push_back(currentFrameData);
		currentFrame++;

		if (averageFrameTimeMs >= 16.6f)
		{
			recordData = false;
			ExportDataCSV("D:/Main/University/KV6013 Computing Project/Data/benchmark_data.csv");
			data.clear();
		}
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
	ImGui::Text("Average Frametime: %.3f", averageFrameTimeMs);

	ImGui::Separator();
	ImGui::Text("Frames Measured: %u", currentFrame);
	if (ImGui::Button("Record Metrics"))
	{
		recordData = !recordData;
	}

	ImGui::End();
}

void PerformanceProfiler::ExportDataCSV(const std::string& filename)
{
	std::ofstream file(filename, std::ios::out);

	if (!file) {
		std::cerr << "Error creating/opening file: " << filename << std::endl;
		return;
	}

	file << "FrameTime,ActiveParticleCount,TotalParticlesGPUSizeBytes,TotalDataTransferPerUpdateBytes,DrawCallsPerFrame\n";

	for (const auto& metrics : data) {

		if (metrics.activeParticleCount > 0)
		{
			file << metrics.frameTime << ","
				<< metrics.activeParticleCount << ","
				<< metrics.totalParticlesGPUSizeBytes << ","
				<< metrics.totalDataTransferPerUpdateBytes << ","
				<< metrics.drawCallsPerFrame << "\n";
		}
	}

	file.close();
	std::cout << "CSV file saved successfully: " << filename << std::endl;
}