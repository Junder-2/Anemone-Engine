//Based of https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h

#pragma once

#include "ANE/Core/Logging.h"
#include <fstream>

namespace Engine
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filename = "results.json")
		{
			std::lock_guard lock(_mutex);
			if (_currentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Logging::GetEngineLogger())
				{
					ANE_ELOG_WARN("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, _currentSession->Name);
				}
				InternalEndSession();
			}
			_outputStream.open("../Profiling/" + filename);

			if (_outputStream.is_open())
			{
				_currentSession = new InstrumentationSession({name});
				WriteHeader();
			}
			else
			{
				if (Logging::GetEngineLogger())
				{
					ANE_ELOG_WARN("Instrumentor could not open results file '{0}'.", filename);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(_mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << R"("cat":"function",)";
			json << R"("dur":)" << (result.ElapsedTime.count()) << ',';
			json << R"("name":")" << result.Name << "\",";
			json << R"("ph":"X",)";
			json << R"("pid":0,)";
			json << R"("tid":)" << result.ThreadID << ",";
			json << R"("ts":)" << result.Start.count();
			json << "}";

			std::lock_guard lock(_mutex);
			if (_currentSession)
			{
				_outputStream << json.str();
				_outputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: _currentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			_outputStream << R"({"otherData": {},"traceEvents":[{})";
			_outputStream.flush();
		}

		void WriteFooter()
		{
			_outputStream << "]}";
			_outputStream.flush();
		}

		// Note: you must already own lock on _mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (_currentSession)
			{
				WriteFooter();
				_outputStream.close();
				delete _currentSession;
				_currentSession = nullptr;
			}
		}
	private:
		std::mutex _mutex;
		InstrumentationSession* _currentSession;
		std::ofstream _outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: _name(name), _stopped(false)
		{
			_startTimePoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!_stopped)
				Stop();
		}

		void Stop()
		{
            const auto endTimePoint = std::chrono::steady_clock::now();
            const auto highResStart = FloatingPointMicroseconds{ _startTimePoint.time_since_epoch() };
            const auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ _name, highResStart, elapsedTime, std::this_thread::get_id() });

			_stopped = true;
		}
	private:
		const char* _name;
		std::chrono::time_point<std::chrono::steady_clock> _startTimePoint;
		bool _stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

// Set the level of profiling
#define ANE_ACTIVE_PROFILE_LEVEL PROFILE_LEVEL_OFF

#define PROFILE_LEVEL_OFF 0
#define PROFILE_LEVEL_SHALLOW 1
#define PROFILE_LEVEL_DEEP 2

#if ANE_ACTIVE_PROFILE_LEVEL <= PROFILE_LEVEL_OFF
#define ANE_PROFILE_BEGIN_SESSION(name, filepath)
#define ANE_PROFILE_END_SESSION()

#define ANE_PROFILE_SCOPE(name)
#define ANE_PROFILE_FUNCTION()
#endif

#if ANE_ACTIVE_PROFILE_LEVEL >= PROFILE_LEVEL_SHALLOW
    // Resolve which function signature macro will be used. Note that this only
    // is resolved when the (pre)compiler starts, so the syntax highlighting
    // could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define ANE_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define ANE_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
    #define ANE_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define ANE_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define ANE_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define ANE_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define ANE_FUNC_SIG __func__
    #else
    #define ANE_FUNC_SIG "ANE_FUNC_SIG unknown!"
    #endif

    #define ANE_PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::Get().BeginSession(name, filepath)
    #define ANE_PROFILE_END_SESSION() ::Engine::Instrumentor::Get().EndSession()
    #define ANE_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Engine::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
                                                                                    ::Engine::InstrumentationTimer timer##line(fixedName##line.Data)
    #define ANE_PROFILE_SCOPE_LINE(name, line) ANE_PROFILE_SCOPE_LINE2(name, line)
    #define ANE_PROFILE_SCOPE(name) ANE_PROFILE_SCOPE_LINE(name, __LINE__)
    #define ANE_PROFILE_FUNCTION() ANE_PROFILE_SCOPE(ANE_FUNC_SIG)
#endif

#if ANE_ACTIVE_PROFILE_LEVEL >= PROFILE_LEVEL_DEEP
    #define ANE_DEEP_PROFILE_SCOPE(name) ANE_PROFILE_SCOPE_LINE(name, __LINE__)
    #define ANE_DEEP_PROFILE_FUNCTION() ANE_PROFILE_SCOPE(ANE_FUNC_SIG)
#else
    #define ANE_DEEP_PROFILE_SCOPE(name)
    #define ANE_DEEP_PROFILE_FUNCTION()
#endif
