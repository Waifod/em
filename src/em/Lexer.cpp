#include "Lexer.h"

#include "utils/StringUtils.h"

namespace em {
Lexer::Lexer(std::wstring mProgram) : mProgram(std::move(mProgram)) {}

std::vector<Token> Lexer::scanTokens() {
  std::vector<Token> tokens;
  while (mPosition < mProgram.size()) {
    switch (auto c = mProgram[mPosition]) {
      case '{':
        tokens.emplace_back(TokenType::LEFT_BRACE, c, mLocation);
        break;
      case '}':
        tokens.emplace_back(TokenType::RIGHT_BRACE, c, mLocation);
        break;
      case '(':
        tokens.emplace_back(TokenType::LEFT_PAREN, c, mLocation);
        break;
      case ')':
        tokens.emplace_back(TokenType::RIGHT_PAREN, c, mLocation);
        break;
      case ',':
        tokens.emplace_back(TokenType::COMMA, c, mLocation);
        break;
      case '|':
        tokens.emplace_back(TokenType::VERTICAL_BAR, c, mLocation);
        break;
      case ':':
        if (mProgram[updatePosition()] == '=') {
          tokens.emplace_back(TokenType::ASSIGN, L":=", mLocation);
          break;
        }
        throw std::logic_error("Expected '=' after ':' at " + mLocation.str());
      case '=':
        tokens.emplace_back(TokenType::EQUAL, c, mLocation);
        break;
      case u'≠':
        tokens.emplace_back(TokenType::NOT_EQUAL, c, mLocation);
        break;
      case u'∪':
        tokens.emplace_back(TokenType::UNION, c, mLocation);
        break;
      case u'∩':
        tokens.emplace_back(TokenType::INTERSECTION, c, mLocation);
        break;
      case u'⊆':
        tokens.emplace_back(TokenType::SUBSET, c, mLocation);
        break;
      case u'⊄':
        tokens.emplace_back(TokenType::NOT_SUBSET, c, mLocation);
        break;
      case u'∈':
        tokens.emplace_back(TokenType::ELEMENT_OF, c, mLocation);
        break;
      case '\n':
        tokens.emplace_back(TokenType::LINE_BREAK, L"\\n", mLocation);
        mLocation.line++;
        mLocation.column = 0;
        break;
      default:
        if (std::isdigit(c)) {
          tokens.emplace_back(scanNumber());
        } else if (isValidForIdentifier(c)) {
          tokens.emplace_back(scanIdentifier());
        } else if (!std::isspace(c)) {
          throw std::invalid_argument("Character " +
                                      utils::string::wStringToString({c}) +
                                      " is not valid at " + mLocation.str());
        }
        break;
    }
    updatePosition();
  }
  tokens.emplace_back(TokenType::END_OF_FILE, L"🔚", mLocation);
  return tokens;
}

Token Lexer::scanNumber() {
  auto start = mPosition;
  while (mPosition < mProgram.size()) {
    if (!std::isdigit(mProgram[mPosition])) {
      break;
    }
    updatePosition();
  }
  auto len = mPosition - start;
  updatePosition(-1);
  auto substr = mProgram.substr(start, len);
  return {TokenType::NUMBER, std::wstring(substr.cbegin(), substr.cend()),
          mLocation};
}

Token Lexer::scanIdentifier() {
  auto start = mPosition;
  while (mPosition < mProgram.size()) {
    if (!isValidForIdentifier(mProgram[mPosition])) {
      break;
    }
    updatePosition();
  }
  auto len = mPosition - start;
  updatePosition(-1);
  auto substr = mProgram.substr(start, len);
  return {TokenType::IDENTIFIER, std::wstring(substr.cbegin(), substr.cend()),
          mLocation};
}

unsigned int Lexer::updatePosition(int offset) {
  mPosition += offset;
  mLocation.column += offset;
  return mPosition;
}

bool Lexer::isValidForIdentifier(wchar_t character) {
  return std::isalnum(character) || character == u'ℝ' || character == u'∅';
}

}  // namespace em