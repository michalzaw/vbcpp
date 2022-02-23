#include "LoggerWindow.h"

#include "../../ImGui/imgui.h"

#include "../../Utils/Logger.h"


namespace LoggerWindow
{

    ImGuiTextBuffer buffer;
    ImVector<int> lineOffsets;
    ImVector<LogLevel> lineLogLevels;

    ImGuiTextFilter logsFilter;
    std::vector<const char*> logLevelComboItems;
    int selectedLogLevel;

    bool isAutoScroll;
    bool scrollToBottomInNextFrame;

    ImFont* font;

    void init()
    {
        lineOffsets.push_back(0);
        isAutoScroll = true;
        scrollToBottomInNextFrame = true;

        // https://opensource.com/article/17/11/how-select-open-source-programming-font
        //font = ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Inconsolata/static/Inconsolata/Inconsolata-Regular.ttf", 14.0f);
        font = ImGui::GetIO().Fonts->AddFontDefault();

        logLevelComboItems.push_back("DEBUG");
        logLevelComboItems.push_back("INFO");
        logLevelComboItems.push_back("WARNING");
        logLevelComboItems.push_back("ERROR");
        selectedLogLevel = 0;
    }

    void clearLogs()
    {
        buffer.clear();
        lineOffsets.clear();
        lineLogLevels.clear();
        lineOffsets.push_back(0);
    }

    void addLog(LogLevel level, const std::string& message)
    {
        int oldSize = buffer.size();

        buffer.appendf(message.c_str());

        for (int newSize = buffer.size(); oldSize < newSize; oldSize++)
        {
            if (buffer[oldSize] == '\n')
            {
                lineOffsets.push_back(oldSize + 1);
                lineLogLevels.push_back(level);
            }
        }

        if (isAutoScroll)
        {
            scrollToBottomInNextFrame = true;
        }
    }

    void drawWindow()
    {
        if (ImGui::Begin("Logs"))
        {
            ImGui::AlignTextToFramePadding();

            ImGui::Text("Level: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::Combo("##LogLevel", &selectedLogLevel, logLevelComboItems.data(), logLevelComboItems.size());

            ImGui::SameLine();

            ImGui::Text("Filter: ");
            ImGui::SameLine();
            logsFilter.Draw("##Filter", 0.0f);

            ImGui::SameLine();

            if (ImGui::Checkbox("Auto-scroll", &isAutoScroll))
            {
                if (isAutoScroll)
                {
                    scrollToBottomInNextFrame = true;
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Clear"))
            {
                clearLogs();
            }

            ImGui::Separator();

            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::PushFont(font);

            const char* bufferBegin = buffer.begin();
            const char* bufferEnd = buffer.end();


            if (logsFilter.IsActive() || selectedLogLevel > LL_DEBUG)
            {
                for (int lineIndex = 0; lineIndex < lineOffsets.Size; ++lineIndex)
                {
                    if (lineIndex < lineLogLevels.size() && lineLogLevels[lineIndex] < selectedLogLevel)
                    {
                        continue;
                    }

                    const char* lineStart = bufferBegin + lineOffsets[lineIndex];
                    const char* lineEnd = (lineIndex + 1 < lineOffsets.Size) ? (bufferBegin + lineOffsets[lineIndex + 1] - 1) : bufferEnd;
                    if (logsFilter.PassFilter(lineStart, lineEnd))
                    {
                        if (lineIndex < lineLogLevels.Size && lineLogLevels[lineIndex] == LL_ERROR)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
                            ImGui::TextUnformatted(lineStart, lineEnd);
                            ImGui::PopStyleColor();
                        }
                        else
                        {
                            ImGui::TextUnformatted(lineStart, lineEnd);
                        }
                    }
                }
            }
            else
            {
                ImGuiListClipper clipper;
                clipper.Begin(lineOffsets.Size);
                while (clipper.Step())
                {
                    for (int lineIndex = clipper.DisplayStart; lineIndex < clipper.DisplayEnd; ++lineIndex)
                    {
                        const char* lineStart = bufferBegin + lineOffsets[lineIndex];
                        const char* lineEnd = (lineIndex + 1 < lineOffsets.Size) ? (bufferBegin + lineOffsets[lineIndex + 1] - 1) : bufferEnd;
                        if (lineIndex < lineLogLevels.Size && lineLogLevels[lineIndex] == LL_ERROR)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
                            ImGui::TextUnformatted(lineStart, lineEnd);
                            ImGui::PopStyleColor();
                        }
                        else
                        {
                            ImGui::TextUnformatted(lineStart, lineEnd);
                        }
                    }
                }
                clipper.End();
            }

            ImGui::PopFont();
            ImGui::PopStyleVar();

            if (scrollToBottomInNextFrame)
            {
                ImGui::SetScrollHereY(1.0f);
                scrollToBottomInNextFrame = false;
            }

            ImGui::EndChild();

            ImGui::End();
        }
    }
}
