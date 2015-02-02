#pragma once
#include <string>
#include <tuple>
#include <cwctype>

class TByWordsSplitIterator {
    const std::wstring& Text;
    size_t CurrentWordBegin;
    size_t CurrentItemLength;

    bool IsAlpha(wchar_t sym) {
        return iswalpha(sym) || iswdigit(sym);
    }

    std::pair<size_t, size_t> NextWord() {
        size_t i = CurrentWordBegin + CurrentItemLength;
        while (i < Text.size() && !IsAlpha(Text[i]))
            ++i;
        const size_t stPos = i;
        while (i < Text.size() && IsAlpha(Text[i]))
            ++i;
        return std::make_pair(stPos, i - stPos);
    }

public:
    TByWordsSplitIterator(const std::wstring& text)
        : Text(text)
        , CurrentWordBegin(0)
        , CurrentItemLength(0)
    {
        std::tie(CurrentWordBegin, CurrentItemLength) = NextWord();
    }

    operator bool() const {
        return CurrentItemLength > 0;
    }

    std::wstring operator*() const {
        return Text.substr(CurrentWordBegin, CurrentItemLength);
    }

    TByWordsSplitIterator& operator++() {
        std::tie(CurrentWordBegin, CurrentItemLength) = NextWord();
        return *this;
    }
};



