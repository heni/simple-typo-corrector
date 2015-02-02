#pragma once
#include <string>

template<class TString>
class TStringIterator {
    typedef typename TString::value_type TChar;

    const TString& SourceString;
    const TChar Delimiter;
    size_t CurrentItemPosition;
    size_t CurrentItemLength;

    size_t GetNextPosition() const {
        size_t pos = SourceString.find(Delimiter, CurrentItemPosition);
        return pos == TString::npos ? SourceString.size() : pos;
    }

public:
    TStringIterator(const TString& string, TChar delimiter)
        : SourceString(string)
        , Delimiter(delimiter)
        , CurrentItemPosition(0)
        , CurrentItemLength(GetNextPosition())
    {}
    
    TStringIterator& operator++() {
        CurrentItemPosition += CurrentItemLength + 1;
        if (*this)
            CurrentItemLength = GetNextPosition() - CurrentItemPosition;
        return *this;
    }

    TString operator*() const {
        return SourceString.substr(CurrentItemPosition, CurrentItemLength);
    }

    operator bool() const {
        return CurrentItemPosition < SourceString.size();
    }

    bool operator==(const TStringIterator<TString>& other) const {
        return static_cast<bool>(*this) ? 
            (&SourceString == &other.SourceString && CurrentItemPosition == other.CurrentItemPosition && Delimiter == other.Delimiter)
            : !other;
    }
};
