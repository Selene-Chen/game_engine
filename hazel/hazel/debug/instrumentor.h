#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

namespace Hazel
{
    struct ProfileResult
    {
        std::string Name;
        int64_t Start, End;
        uint32_t ThreadID;
    };
    struct InstrumentationSession
    {
        explicit InstrumentationSession(std::string name) : Name(std::move(name)) {}
        std::string Name;
    };

    class Instrumentor
    {
    public:
        Instrumentor() = default;
        void BeginSession(const std::string& name, const std::string& path = "result.json")
        {
            m_output_stream.open(path);
            WriteHeader();
            m_current_session = new InstrumentationSession(name);
        }
        void EndSession()
        {
            WriteFooter();
            m_output_stream.close();
            delete m_current_session;
            m_current_session = nullptr;
            m_profile_count = 0;
        }

        void WriteHeader()
        {
            m_output_stream << R"({"otherData": {},"traceEvents":[)";
            m_output_stream.flush();
        }
        void WriteFooter()
        {
            m_output_stream << "]}";
            m_output_stream.flush();
        }
        void WriteProfile(const ProfileResult& result)
        {
            if (m_profile_count++ > 0)
            {
                m_output_stream << ",";
            }

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_output_stream << "{";
            m_output_stream << R"("cat":"function",)";
            m_output_stream << "\"dur\":" << (result.End - result.Start) << ',';
            m_output_stream << R"("name":")" << name << "\",";
            m_output_stream << R"("ph":"X",)";
            m_output_stream << "\"pid\":0,";
            m_output_stream << "\"tid\":" << result.ThreadID << ",";
            m_output_stream << "\"ts\":" << result.Start;
            m_output_stream << "}";

            m_output_stream.flush();
        }
        static Instrumentor& Get()
        {
            static Instrumentor s_instance;
            return s_instance;
        }

    private:
        InstrumentationSession* m_current_session = {nullptr};
        std::ofstream m_output_stream;
        int m_profile_count{0};
    };

    class InstrumentationTimer
    {
    public:
        explicit InstrumentationTimer(const char* name) : m_name(name)
        {
            m_start_time = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_stopped)
            {
                Stop();
            }
        }

    private:
        void Stop()
        {
            auto end_timepoint = std::chrono::high_resolution_clock::now();

            int64_t start =
                std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time).time_since_epoch().count();
            int64_t end =
                std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();

            uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({m_name, start, end, thread_id});

            m_stopped = true;
        }
        const char* m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
        bool m_stopped{false};
    };
    namespace InstrumentorUtils
    {

        template <size_t N>
        struct ChangeResult
        {
            char Data[N];
        };

        template <size_t N, size_t K>
        constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t src_index = 0;
            size_t dst_index = 0;
            while (src_index < N)
            {
                size_t match_index = 0;
                while (match_index < K - 1 && src_index + match_index < N - 1 &&
                       expr[src_index + match_index] == remove[match_index])
                {
                    match_index++;
                }
                if (match_index == K - 1)
                {
                    src_index += match_index;
                }
                result.Data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
                src_index++;
            }
            return result;
        }
    }  // namespace InstrumentorUtils

}  // namespace Hazel

#define HZ_PROFILE 1
#if HZ_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || \
        defined(__ghs__)
        #define HZ_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define HZ_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
        #define HZ_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define HZ_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define HZ_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define HZ_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define HZ_FUNC_SIG __func__
    #else
        #define HZ_FUNC_SIG "HZ_FUNC_SIG unknown!"
    #endif

    #define HZ_PROFILE_BEGIN_SESSION(name, path) ::Hazel::Instrumentor::Get().BeginSession(name, path)
    #define HZ_PROFILE_END_SESSION()             ::Hazel::Instrumentor::Get().EndSession()
    #define HZ_PROFILE_SCOPE_LINE2(name, line)                                                              \
        constexpr auto fixedName##line = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
        ::Hazel::InstrumentationTimer timer##line(fixedName##line.Data)
    #define HZ_PROFILE_SCOPE_LINE(name, line) HZ_PROFILE_SCOPE_LINE2(name, line)
    #define HZ_PROFILE_SCOPE(name)            HZ_PROFILE_SCOPE_LINE(name, __LINE__)
    #define HZ_PROFILE_FUNCTION()             HZ_PROFILE_SCOPE(HZ_FUNC_SIG)
#else
    #define HZ_PROFILE_BEGIN_SESSION(name, path)
    #define HZ_PROFILE_END_SESSION()
    #define HZ_PROFILE_SCOPE(name)
    #define HZ_PROFILE_FUNCTION()
#endif