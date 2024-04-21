#include <iostream>
#include <variant>
#include <vector>
#include <unordered_map>

struct OpeningBracket {};

struct ClosingBracket {};

struct Number {
    int value;
};

struct UnknownToken {
    std::string value;
};

struct MinToken {};

struct MaxToken {};

struct AbsToken {};

struct SqrToken {};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

struct Comma {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, Plus, Minus, Multiply, Modulo, Divide, MaxToken, SqrToken, Comma>;

const std::unordered_map<char, Token> kSymbol2Token{{'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}}, {'(', OpeningBracket{}}, {')', ClosingBracket{}}};


inline int ToDigit(unsigned char symbol){
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos){
    int value = 0;
    size_t size = input.size();
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)){
        value = value * 10 + ToDigit(symbol);
        if (pos == size)
        {
            return Number{value};
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{value};
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string word;
    while (pos < input.size() && std::isalpha(static_cast<unsigned char>(input[pos]))) {
        word.push_back(static_cast<char>(tolower(input[pos++])));
    }
    if (word == "max") {
        return MaxToken{};
    } else if (word == "min") {
        return MinToken{};
    } else if (word == "abs") {
        return AbsToken{};
    } else if (word == "sqr") {
        return SqrToken{};
    } else {
        return UnknownToken{word};
    }
}


std::vector<Token> Tokenize(const std::string& input)
{
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size){
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)){
            ++pos;
        } else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()){
            tokens.emplace_back(it->second);
            ++pos;
        } else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }  else if (symbol == ',') {
            tokens.emplace_back(Comma {});
            ++pos;
        } else {
            tokens.emplace_back(UnknownToken{std::string(1, symbol)});
           ++pos;
        }
    }

    return tokens;
}


int main()
{

    std::string value = "@ 2 + 3 * (3 - max(1, 2)) 222 Max";
    auto tokens = Tokenize(value);


    for (const auto& token : tokens) {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Number>) std::cout << "Number: " << arg.value << "\n";
            else if constexpr (std::is_same_v<T, UnknownToken>) std::cout << "Unknown: " << arg.value << "\n";
            else if constexpr (std::is_same_v<T, Plus>) std::cout << "Plus\n";
            else if constexpr (std::is_same_v<T, Minus>) std::cout << "Minus\n";
            else if constexpr (std::is_same_v<T, Multiply>) std::cout << "Multiply\n";
            else if constexpr (std::is_same_v<T, Divide>) std::cout << "Divide\n";
            else if constexpr (std::is_same_v<T, Modulo>) std::cout << "Modulo\n";
            else if constexpr (std::is_same_v<T, OpeningBracket>) std::cout << "Opening Bracket\n";
            else if constexpr (std::is_same_v<T, ClosingBracket>) std::cout << "Closing Bracket\n";
            else if constexpr (std::is_same_v<T, MaxToken>) std::cout << "Max\n";
            else if constexpr (std::is_same_v<T, MinToken>) std::cout << "Min\n";
            else if constexpr (std::is_same_v<T, AbsToken>) std::cout << "Abs\n";
            else if constexpr (std::is_same_v<T, SqrToken>) std::cout << "Sqr\n";
            else if constexpr (std::is_same_v<T, Comma>) std::cout << "Comma\n";
        }, token);
    }

    return 0;
}
