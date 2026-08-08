#include <iostream>
#include <string>

char rotate13(char character) {
    char base = 0;
    if (character >= 'A' && character <= 'Z') {
        base = 'A';
    } else if (character >= 'a' && character <= 'z') {
        base = 'a';
    } else {
        return character;
    }

    return static_cast<char>(base + (character - base + 13) % 26);
}

std::string rot13(std::string text) {
    for (char& character : text) {
        character = rotate13(character);
    }
    return text;
}

int main() {
    const std::string message = "Cryptography From First Principles";
    const std::string encoded = rot13(message);
    std::cout << encoded << '\n' << rot13(encoded) << '\n';
}
