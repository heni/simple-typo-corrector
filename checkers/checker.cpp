#include "utf8.h"
#if !defined(HEADERS_GENERATION_GUARD)
#include "testlib.h"
#include <cwctype>
#include <iostream>
#include <locale>
#endif

enum ESymbolType {
   EST_DIGIT, EST_ALPHA, EST_WHITESPACE
};

inline ESymbolType GetSymbolType(wchar_t sym) {
    if (iswdigit(sym))
        return EST_DIGIT;
    if (iswalpha(sym))
        return EST_ALPHA;
    return EST_WHITESPACE;
}

std::wstring Normalize(const std::string& strValue) {
    std::wstring normValue;

    ESymbolType prevType = EST_WHITESPACE;
    for (std::string::const_iterator it = strValue.begin(); it != strValue.end(); ) {
        const wchar_t sym = std::towlower(utf8::next(it, strValue.end()));
        const ESymbolType curType = GetSymbolType(sym);
        switch (curType) {
            case EST_DIGIT:
                if (prevType == EST_ALPHA)
                    normValue.push_back(L' ');
                normValue.push_back(sym);
                break;
            case EST_ALPHA:
                if (prevType == EST_DIGIT)
                    normValue.push_back(L' ');
                normValue.push_back(sym);
                break;
            case EST_WHITESPACE:
                if (prevType != EST_WHITESPACE)
                    normValue.push_back(L' ');
                break;
        }
        prevType = curType;
    }
    if (prevType == EST_WHITESPACE && normValue.size())
        normValue.erase(normValue.size() - 1);
    return normValue;
}

#if defined(DEBUG)
    #define R(request) (L"[" + (request) + L"]")
    #define LOG_STAGE1(a) do { std::wcout << (a) << std::endl; } while (0)
    #define LOG_STAGE2(a, b) do { std::wcout << (a) << L"\t" << (b) << std::endl; } while (0)
    #define LOG_STAGE3(a, b, c) do { std::wcout << (a) << L"\t" << (b) << L"\t" << (c) << std::endl; } while (0)
#else 
    #define R(x)
    #define LOG_STAGE1(a)
    #define LOG_STAGE2(a,b)
    #define LOG_STAGE3(a,b,c)
#endif

int main(int argc, const char* argv[]) {
    std::locale UTF_8("en_US.UTF-8");
    std::locale::global(UTF_8);
    setName("calculate F1 measure for misspell classifier");
    registerTestlibCmd(argc, const_cast<char**>(argv));

    size_t nRequests = inf.readInt();
    inf.nextLine();

    size_t correct_misspells = 0, all_misspells = 0, need_misspells = 0;
    for (size_t i = 0; i < nRequests; ++i) {
        const std::wstring origRequest = Normalize(inf.readLine());
        const std::wstring jAns = Normalize(ans.readLine());
        const std::wstring tAns = Normalize(ouf.readLine());

        if (origRequest != jAns) {
            ++need_misspells;
        }

        LOG_STAGE2(L"REQUEST", R(origRequest));
        if (origRequest != tAns) {
            ++all_misspells;
            if (tAns == jAns)
                ++correct_misspells;
            LOG_STAGE3((tAns == jAns ? L"+" : L"-"), R(tAns), R(jAns));
        }
    }

    const double EPS = 1e-6;
    double precision = correct_misspells * 1.0 / std::max(all_misspells, 1ul);
    double recall = correct_misspells * 1.0 / std::max(need_misspells, 1ul);
    double fMeasure = (precision > EPS && recall > EPS) ? 2.0 * precision * recall / (precision + recall) : 0.0;

    std::wcout << L"precision: " << precision << std::endl;
    std::wcout << L"recall: " << recall << std::endl;
    std::wcout << L"F1: " << fMeasure << std::endl;

    quitf(_ok, "value is %.4lf\n", fMeasure);
    return 0;
}
