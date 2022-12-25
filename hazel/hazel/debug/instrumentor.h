
#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

#include "hazel/core/logger.h"

namespace Hazel
{
    struct ProfileResult
    {
        std::string Name;
        int64_t Start, End;
        std::thread::id ThreadID;  // 线程ID
    };
    struct InstrumentationSession
    {
        explicit InstrumentationSession(std::string name) : Name(std::move(name)) {}
        std::string Name;
    };
    class Instrumentor
    {
    private:
        std::mutex m_mutex;
        InstrumentationSession* m_current_session{};
        std::ofstream m_output_stream;

    public:
        Instrumentor() = default;

        void BeginSession(const std::string& name, const std::string& path = "result.json")
        {
            // m_mutex
            std::lock_guard lock((m_mutex));

            // * 当前sessionb被打开，先结束它
            if (m_current_session != nullptr)
            {
                if (Logger::GetCoreLogger())
                {  // Edge case: BeginSession() might be before Logger::Init()
                    HZ_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name,
                                  m_current_session->Name);
                }
                InternalEndSession();
            }

            //* 打开文件写入
            m_output_stream.open(path);
            if (m_output_stream.is_open())
            {
                m_current_session = new InstrumentationSession({name});
                WriteHeader();
            }
            else
            {
                if (Logger::GetCoreLogger())
                {  // Edge case: BeginSession() might be before Logger::Init()
                    HZ_CORE_ERROR("Instrumentor could not open result file '{0}'.", path);
                }
            }
        }

        void EndSession()
        {
            // m_mutex
            std::lock_guard lock(m_mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;
            json << ",{";
            json << R"("cat":"function",)";
            json << "\"dur\":" << (result.End - result.Start) << ',';
            json << R"("name":")" << result.Name << "\",";
            json << R"("ph":"X",)";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start;
            json << "}";

            // m_mutex
            std::lock_guard lock(m_mutex);
            if (m_current_session != nullptr)
            {
                m_output_stream << json.str();
                m_output_stream.flush();
            }
        }

        static Instrumentor& Get()
        {
            static Instrumentor s_instance;
            return s_instance;
        }

    private:
        void WriteHeader()
        {
            m_output_stream << R"({"otherData": {},"traceEvents":[{})";
            m_output_stream.flush();
        }

        void WriteFooter()
        {
            m_output_stream << "]}";
            m_output_stream.flush();
        }

        //! 必须是在获取lock之后执行
        void InternalEndSession()
        {
            if (m_current_session != nullptr)
            {
                WriteFooter();
                m_output_stream.close();
                delete m_current_session;
                m_current_session = nullptr;
            }
        }
    };
    class InstrumentationTimer
    {
    public:
        explicit InstrumentationTimer(const char* name) : m_name(name)
        {
            // * 开始时间
            m_start_time = std::chrono::steady_clock::now();
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
            //* 结束时间
            auto end_time = std::chrono::steady_clock::now();

            //* 获取开始时间戳 转换成ms
            auto start = std::chrono::duration_cast<std::chrono::microseconds>(m_start_time.time_since_epoch()).count();
            // * 获取结束时间戳 转换成ms
            auto end = std::chrono::duration_cast<std::chrono::microseconds>(end_time.time_since_epoch()).count();

            Instrumentor::Get().WriteProfile({m_name, start, end, std::this_thread::get_id()});
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

#define HZ_PROFILE 0
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
    #define HZ_PROFILE_SCOPE(name)                                                                    \
        constexpr auto fixedName = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
        ::Hazel::InstrumentationTimer timer##__LINE__(fixedName.Data)
    #define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(HZ_FUNC_SIG)
#else
    #define HZ_PROFILE_BEGIN_SESSION(name, path)
    #define HZ_PROFILE_END_SESSION()
    #define HZ_PROFILE_SCOPE(name)
    #define HZ_PROFILE_FUNCTION()
#endif