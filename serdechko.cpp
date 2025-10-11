#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <tuple>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

void clear_terminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Получаем ширину терминала (по дефолту 80)
int get_terminal_width() {
    const char* cols = std::getenv("COLUMNS");
    if (cols) return std::atoi(cols);
    return 200;
}

// HSV -> RGB
std::tuple<int,int,int> HSVtoRGB(float H, float S, float V) {
    float C = V * S;
    float X = C * (1 - std::fabs(fmod(H / 60.0, 2) - 1));
    float m = V - C;

    float r=0, g=0, b=0;
    if (H >= 0 && H < 60)       { r=C; g=X; b=0; }
    else if (H >= 60 && H < 120){ r=X; g=C; b=0; }
    else if (H >= 120 && H < 180){ r=0; g=C; b=X; }
    else if (H >= 180 && H < 240){ r=0; g=X; b=C; }
    else if (H >= 240 && H < 300){ r=X; g=0; b=C; }
    else if (H >= 300 && H < 360){ r=C; g=0; b=X; }

    int R = static_cast<int>((r + m) * 255);
    int G = static_cast<int>((g + m) * 255);
    int B = static_cast<int>((b + m) * 255);

    return {R, G, B};
}

std::vector<std::string> split_lines(const std::string& s) {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) lines.push_back(line);
    return lines;
}

int main() {
    std::cout << "\033[?25l" << std::flush;
    const std::string love =
"ILOVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOU\n"
"ILOVEYOUILO ****** VEYOU ****** ILOVEYOUILOVEYOUILOVEYOU\n"
"ILOVEYOU *********** I *********** LOVEYOUILOVEYOUILOVEY\n"
"OUIUI *************** *************** VEYOUILOVEYOUILOVE\n"
"YOUI ********************************** LOVEYOUILOVEYOUI\n"
"IL ************************************* OVEYOUILOVEYOUI\n"
"L *************I**LOVE**YOU*!************ OVEYOUILOVEYOU\n"
"I *************************************** LOVEYOUILOVEYO\n"
"U *************************************** ILOVEYOUILOVEY\n"
"OU ************************************* ILOVEYOUILOVEYO\n"
"UIL *********************************** OVEYOUILOVEYOUIL\n"
"OVEYO ******************************* ULOVEYOUILOVEYOUIL\n"
"OVEYOUI **************************** LOVEYOUILOVEYOUILOV\n"
"EYOUILOVE *********************** YOUILOVEYOUILOVEYOUILO\n"
"VEYOUILOVEYOU ***************** ILOVEYOUILOVEYOUILOVEYOU\n"
"ILOVEYOUILOVEYO ************* LOVEYOUILOVEYOUILOVEYOUILO\n"
"UILOVEYOUILOVEYOU ********* LOVEYOUILOVEYOUILOVEYOUILOVE\n"
"LOVEYOUILOVEYOUILOV ***** ILOVEYOUILOVEYOUILOVEYOUILOVEY\n"
"EYOUILOVEYOUILOVEYOU *** YOULOVEYOUILOVEYOUILOVEYOUILOVE\n"
"VEYOUILOVEYOUILOVEYOU * VEYOUILOVEYOUILOVEYOUILOVEYOUILO\n"
"OVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOUILOVEYOUIL\n";

    const int delay_us = 20000;
    float frameOffset = 0.0f;

    auto lines = split_lines(love);
    int term_width = get_terminal_width();

    while (true) {
        clear_terminal();
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string &line = lines[i];
            int padding = (term_width - line.size()) / 2;
            if (padding > 0) std::cout << std::string(padding, ' ');

            for (size_t j = 0; j < line.size(); ++j) {
                float H = fmod((j * 8 + frameOffset), 360.0f);
                auto [r,g,b] = HSVtoRGB(H, 1.0, 1.0);
                std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << line[j] << "\033[0m";
            }
            std::cout << "\n";
        }

        frameOffset += 3.0f;
        if (frameOffset >= 360.0f) frameOffset -= 360.0f;

        usleep(delay_us);
    }
    std::cout << "\033[?25h" << std::flush;
    return 0;
}
