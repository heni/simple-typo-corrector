#include "testlib.h"
#include <iostream>

std::string Normalize(const std::string& strValue) {
    return strValue;
}

int main(int argc, const char* argv[]) {
    setName("calculate F1 measure for misspell classifier");
    registerTestlibCmd(argc, const_cast<char**>(argv));
    size_t nRequests = inf.readInt();
    inf.nextLine();

    size_t correct_misspells = 0, all_misspells = 0, need_misspells = 0;
    for (size_t i = 0; i < nRequests; ++i) {
        const std::string origRequest = Normalize(inf.readLine());
        const std::string jAns = Normalize(ans.readLine());
        const std::string tAns = Normalize(ouf.readLine());
        if (origRequest != jAns) {
            ++need_misspells;
        }
        if (origRequest != tAns) {
            ++all_misspells;
            if (tAns == jAns)
                ++correct_misspells;
        }
    }

    const double EPS = 1e-6;
    double precision = correct_misspells * 1.0 / std::max(all_misspells, 1ul);
    double recall = correct_misspells * 1.0 / std::max(need_misspells, 1ul);
    double fMeasure = (precision > EPS && recall > EPS) ? 2.0 * precision * recall / (precision + recall) : 0.0;

    std::cout << "cm = " << correct_misspells << "; am = " << all_misspells << "; nm = " << need_misspells << std::endl;
    std::cout << "precision: " << precision << std::endl;
    std::cout << "recall: " << recall << std::endl;

    quitf(_ok, "value is %.4lf\n", fMeasure);
    return 0;
}
