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

#define CO_CORE_TRACE(...)      {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetCoreLogger()->trace(s);		}
#define CO_CORE_INFO(...)       {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetCoreLogger()->info(s);		}
#define CO_CORE_WARN(...)       {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetCoreLogger()->warn(s);		}
#define CO_CORE_ERROR(...)      {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetCoreLogger()->error(s);		}
#define CO_CORE_CRITICAL(...)   {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetCoreLogger()->critical(s);	}

#define CO_TRACE(...)           {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetClientLogger()->trace(s);		}
#define CO_INFO(...)            {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetClientLogger()->info(s);		}
#define CO_WARN(...)            {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetClientLogger()->warn(s);		}
#define CO_ERROR(...)           {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetClientLogger()->error(s);		}
#define CO_CRITICAL(...)        {char s[1024]; snprintf(s, 1023, __VA_ARGS__); Oxide::Log::GetClientLogger()->critical(s);	}
