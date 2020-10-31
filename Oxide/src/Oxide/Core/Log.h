#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "Oxide/Core/Base.h"

namespace Oxide {
    class Log {
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	};
}

#define CO_CORE_TRACE(...)      Oxide::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CO_CORE_INFO(...)       Oxide::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CO_CORE_WARN(...)       Oxide::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CO_CORE_ERROR(...)      Oxide::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CO_CORE_CRITICAL(...)   Oxide::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define CO_INFO(...)            Oxide::Log::GetClientLogger()->info(__VA_ARGS__)
#define CO_WARN(...)            Oxide::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CO_TRACE(...)           Oxide::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CO_ERROR(...)           Oxide::Log::GetClientLogger()->error(__VA_ARGS__)
#define CO_CRITICAL(...)        Oxide::Log::GetClientLogger()->critical(__VA_ARGS__)