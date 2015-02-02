#include "language_model.h"
#include "string_iterator.h"
#include "words_splitter.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <algorithm>

std::wstring to_lower(const std::wstring& src) {
    std::wstring answer(src);
    std::transform(src.begin(), src.end(), answer.begin(), std::towlower);
    return std::move(answer);
}


class TMisspellCorrector {
    typedef std::pair<std::wstring, double> TScoredCorrection;
    const TLanguageModel& LanguageModel;
    std::vector<wchar_t> ABC;

    static std::vector<wchar_t> InitABC(const std::set<wchar_t>& characterSet) {
        std::vector<wchar_t> abc {L' '};
        std::copy(characterSet.begin(), characterSet.end(), std::back_inserter(abc));
        return std::move(abc);
    }

    double GetScore(const std::wstring& suggestion) const {
        return LanguageModel.CalcModel(suggestion);
    }

    bool CheckSuggestion(const std::wstring& suggestion, TScoredCorrection& bestCorrection) const {
        double score = GetScore(suggestion);
        if (score > bestCorrection.second) {
            std::make_pair(suggestion, score).swap(bestCorrection);
            return true;
        }
        return false;
    }

    #if defined(DEBUG) 
        #define LOG_SUGGESTION(mrk, pos, suggestion) do \
            { \
                std::wcout << mrk << L"(" << pos << L"): " << suggestion << std::endl; \
            } while(0)
    #else
        #define LOG_SUGGESTION(...)
    #endif

    void TryDeleteSymbol(const std::wstring& request, TScoredCorrection& bestCorrection) const {
        for (size_t i = 0; i < request.size(); ++i) {
            const std::wstring suggestion = request.substr(0, i) + request.substr(i + 1);
            LOG_SUGGESTION(L"D", i, suggestion); 
            CheckSuggestion(suggestion, bestCorrection);
        }
    }

    void TryAddSymbol(const std::wstring& request, TScoredCorrection& bestCorrection) const {
        for (size_t i = 0; i <= request.size(); ++i) {
            std::wstring suggestion = request.substr(0, i) + L" " + request.substr(i);
            for(auto ch: ABC) { 
                suggestion[i] = ch;
                LOG_SUGGESTION(L"A", i, suggestion); 
                CheckSuggestion(suggestion, bestCorrection);
            }
        }
    }

    void TryChangeSymbol(const std::wstring& request, TScoredCorrection& bestCorrection) const {
        for (size_t i = 0; i < request.size(); ++i) {
            std::wstring suggestion = request;
            for (auto ch: ABC) {
                suggestion[i] = ch;
                LOG_SUGGESTION(L"C", i, suggestion); 
                CheckSuggestion(suggestion, bestCorrection);
            }
        }
    }

    #undef LOG_SUGGESTION

public:
    TMisspellCorrector(const TLanguageModel& lm) 
        : LanguageModel(lm)
        , ABC(InitABC(LanguageModel.GetAllCharacters()))
    {}

    std::wstring GetCorrection(std::wstring request) const {
        LanguageModel.ClearCache();
        request = to_lower(request);
        for (size_t corrected = 0; corrected < 1; ++corrected) {
            TScoredCorrection bestCorrection(request, GetScore(request));
            TryDeleteSymbol(request, bestCorrection);
            TryAddSymbol(request, bestCorrection);
            TryChangeSymbol(request, bestCorrection);
            if (bestCorrection.first == request)
                return request;
            request = bestCorrection.first;
        }
        return request;
    }


};


int main() {
    std::locale UTF_8("en_US.UTF-8");
    std::locale::global(UTF_8);
    TLanguageModel lm("unigrams.txt", "bigrams.txt");
    TMisspellCorrector mspChecker(lm);

    std::wifstream in("input.txt");
    std::wofstream out("output.txt");
    std::wstring request;
    size_t nRequests;
    in >> nRequests;
    std::getline(in, request); //read EOL
    while (std::getline(in, request)) {
        out << mspChecker.GetCorrection(request) << std::endl; 
    }
    return 0;
}
