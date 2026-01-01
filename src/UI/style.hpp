#ifndef STYLE_HPP
#define STYLE_HPP
#include <string>

using namespace std;

namespace UI {
    enum TCol {
        /**
     * @brief Text Colors.
     *
     * Provides ANSI color codes for text (`TCol`).
     * Example usage:
     * @code
     * Style("Text Here", TCol::Red, BgCol::Black);
     *
     */
        Black = 30,
        Red = 31,
        Green = 32,
        Yellow = 33,
        Blue = 34,
        Magenta = 35,
        Cyan = 36,
        White = 37,
        Default = 39
    };

    enum BgCol {
        /**
     * @brief Text Colors.
     *
     * Provides ANSI color codes for text (`BgCol`).
     * Example usage:
     * @code
     * Style("Text Here", TCol::Red, BgCol::Black);
     *
     */
        Black = 40,
        Red = 41,
        Green = 42,
        Yellow = 43,
        Blue = 44,
        Magenta = 45,
        Cyan = 46,
        White = 47,
        Default = 49
    };

    enum StCode {
        /**
     * @brief Text Styles.
     *
     * Provides ANSI style codes for text (`StCode`).
     * Example usage:
     * @code
     * Style("Text Here", TCol::Red, BgCol::Black, StCode::Bold);
     *
     */
        Reset = 0,
        Bold = 1,
        Dim = 2,
        Italic = 3,
        Underline = 4,
        Blink = 5,
        Reverse = 7,
        Hidden = 8,
        Strikethrough = 9
    };

    template<typename... Args>
    string Style(string &text, Args... args) {
        /**
         * @brief Style Text.
         *
         * Styles the given text using ANSI escape codes based on the provided
         * text color, background color, and style codes.
         *
         * @param text The text to be styled.
         * @param args Variable number of style arguments (TCol, BgCol, StCode).
         * @return A string containing the styled text with ANSI escape codes.
         *
         * Example usage:
         * @code
         * string myText = "Hello, World!";
         * string styledText = Style(myText, UI::TCol::Red, UI::BgCol::Black, UI::StCode::Bold);
         * cout << styledText << endl;
         * @endcode
         */
        string styleCode = "\033[";
        ((styleCode += to_string(static_cast<int>(args)) + ";"), ...);
        styleCode.back() = 'm'; // Replace last ';' with 'm'
        string resetCode = "\033[0m";
        return styleCode + text + resetCode;
    }
}


#endif
