#pragma once
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <queue>


#include "imgui.h"


//todo collapse function

namespace Engine
{
    struct Logger
    {
        ImGuiTextBuffer Buffer; // just add this buffer....
        ImGuiTextFilter Filter;
        ImVector<int> LineOffsets;
        bool AutoScroll;

        std::ofstream logFile;
        std::string logFilePath;
        std::string logDir;
        std::string sessionCountFile;
        int sessionCount;

        Logger() : AutoScroll(true)
        {
            Clear();
            sessionCountFile = GetSessionCountFilePath();
            logDir = "../Engine/Logs/";
            CreateLogDirectory();
            LoadSessionCount();
            OpenLogFile();
        }

        void CreateLogDirectory()
        {
            std::filesystem::create_directory(logDir);
        }

        ~Logger()
        {
            CloseLogFile();
        }

        void Clear()
        {
            Buffer.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        std::string GetSessionCountFilePath() //
        {
            std::filesystem::path dataDir = std::filesystem::temp_directory_path();
            const char* envDataDir = std::getenv("APPDATA");

            if (envDataDir)
                dataDir = std::filesystem::path(envDataDir);
            else
            {
                const char* envHomeDir = std::getenv("HOME"); //todo: not thread safe
                if (envHomeDir)
                    dataDir = std::filesystem::path(envHomeDir) / ".local" / "share";
            }

            // Append a subdirectory and filename for the session count file
            std::filesystem::path sessionCountFilePath = dataDir / ".your_app_name" / "session_count.txt";
            return sessionCountFilePath.string();
        }

        void LoadSessionCount()
        {
            std::ifstream countFile("session_count.txt");
            if (countFile)
            {
                countFile >> sessionCount;
                countFile.close();
            }
        }

        void OpenLogFile() // Should be called when entering playmode?
        {
            if (logFile.is_open())
                logFile.close();

            std::stringstream filename;
            filename << "Log_" << GetCurrentTimestamp() << ".txt";
            logFilePath = logDir + filename.str();
            logFile.open(logFilePath);
           // sessionCount++;
        }

        void CloseLogFile() //todo: should be called when exiting playmode?
        {
            if (logFile.is_open()) logFile.close();
            SaveSessionCount();
        }

        std::string GetCurrentTimestamp() const
        {
            std::time_t now = std::time(nullptr);
            std::tm* timeInfo = std::localtime(&now); //todo: not thread safe

            std::stringstream ss;
            ss << std::put_time(timeInfo, "%Y-%m-%d_%H%M%S");
            return ss.str();
        }

        void RotateLogFile()
        {
            if (sessionCount >= 50) sessionCount = 0;

            OpenLogFile();
        }

        void SaveSessionCount()
        {
            std::ofstream countFile(sessionCountFile);
            if (countFile)
            {
                countFile << sessionCount;
                countFile.close();
            }
        }

        void AddLog(const char* fmt, ...) IM_FMTARGS(2)
        {
            int old_size = Buffer.size();
            va_list args;
            va_start(args, fmt);

            std::string formattedString = std::string(fmt) + "\n";

            Buffer.appendfv(formattedString.c_str(), args);

            if (logFile.is_open()) logFile << formattedString;

            va_end(args);
            for (int new_size = Buffer.size(); old_size < new_size; old_size++)
                if (Buffer[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);

        }

        void Draw(const char* title, bool* p_open = nullptr)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            Filter.Draw("Filter", -100.0f);

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clear) Clear();
                if (copy) ImGui::LogToClipboard();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                const char* buf = Buffer.begin();
                const char* buf_end = Buffer.end();
                if (Filter.IsActive())
                {
                    // In this example we don't use the clipper when Filter is enabled.
                    // This is because we don't have random access to the result of our filter.
                    // A real application processing logs with ten of thousands of entries may want to store the result of
                    // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                    for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        if (Filter.PassFilter(line_start, line_end))
                            ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                else
                {
                    // The simplest and easy way to display the entire buffer:
                    //   ImGui::TextUnformatted(buf_begin, buf_end);
                    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                    // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                    // within the visible area.
                    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                    // on your side is recommended. Using ImGuiListClipper requires
                    // - A) random access into your data
                    // - B) items all being the  same height,
                    // both of which we can handle since we have an array pointing to the beginning of each line of text.
                    // When using the filter (in the block of code above) we don't have random access into the data to display
                    // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                    // it possible (and would be recommended if you want to search through tens of thousands of entries).
                    ImGuiListClipper clipper;
                    clipper.Begin(LineOffsets.Size);
                    while (clipper.Step())
                    {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                        {
                            const char* line_start = buf + LineOffsets[line_no];
                            const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                            ImGui::TextUnformatted(line_start, line_end);
                        }
                    }
                    clipper.End();
                }
                ImGui::PopStyleVar();

                if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();
            ImGui::End();
        }

    public:

        static void SubmitText(const std::string_view message);

        static bool GetText(std::string_view& textToPrint);
    private:

        static std::queue<std::string_view>  Texts;
    };
}
