#include "Token.h"

#include "utils/StringUtils.h"

namespace em {
Token::Token(TokenType type, std::wstring text, Location location)
    : mType(type), mText(std::move(text)), mLocation(location) {}

Token::Token(TokenType type, wchar_t text, Location location)
    : mType(type), mText(std::wstring{text}), mLocation(location) {}

TokenType Token::type() const { return mType; }

std::wstring Token::text() const { return mText; }

Token::Location Token::location() const { return mLocation; }

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << "(" << TokenTypeToString(token.mType) << ", '"
     << utils::string::wStringToString(token.mText) << "', ' "
     << token.mLocation.str().c_str() << "')";
  return os;
}
}  // namespace em
