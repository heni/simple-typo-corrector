#pragma once
#include "words_splitter.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <string>
#include <functional>
#include <cmath>

class TLanguageModel {
    typedef std::unordered_map<std::wstring, double> TStatsDictionary;
    TStatsDictionary UniwordLogFreqs;
    TStatsDictionary BiwordLogFreqs;
    TStatsDictionary TriGramsLogFreqs;
    TStatsDictionary FourGramsLogFreqs;
    mutable std::unordered_map<std::wstring, double> NGramsCache;

    static const double DefaultLogFreq;
    static const double DefaultCharLogFreq;

    static TStatsDictionary LoadFreqs(std::wistream&& freqsReader) {
        if (!freqsReader)
            throw std::runtime_error("can't open statistics file");
        TStatsDictionary stats;
        while (freqsReader) {
            std::wstring word;
            double freq;
            if (freqsReader >> word >> freq) {
                stats[word] = freq;
            }
        }
        return std::move(stats);
    }

    template<size_t depth>
    static std::wstring CreateNGram(const std::wstring& word, int srcPos) {
        wchar_t ngram[depth];
        size_t i = 0;
        for (;i < depth && srcPos < 0; ++srcPos, ++i)
            ngram[i] = L'$';
        for (;i < depth && srcPos < word.size(); ++srcPos, ++i)
            ngram[i] = word[srcPos];
        for (;i < depth; ++i)
            ngram[i] = L'$';
        return std::wstring(ngram, depth);
    }

    template<size_t depth>
    static TStatsDictionary CreateNGramsModel(const TStatsDictionary& uniwordModel) {
        TStatsDictionary ngrams;
        for (auto entry: uniwordModel) {
            double freq = exp(entry.second);
            for (int i = 1 - static_cast<int>(depth); i + 1 < static_cast<int>(entry.first.size()); ++i)
                ngrams[CreateNGram<depth>(entry.first, i)] += freq;
        }
        for (auto& entry: ngrams) {
            entry.second = log(entry.second);
        }
        return std::move(ngrams);
    }

    static std::wstring GetBiwordKey(const std::wstring& lwh, const std::wstring& rwh) {
        return lwh + L";" + rwh;
    }

    static double GetValueOrDefault(const TStatsDictionary& stats, const std::wstring& key, double defValue) {
        auto it = stats.find(key);
        return it == stats.end() ? defValue : it->second;
    }

    static double GetValueOrDefault(const TStatsDictionary& stats, const std::wstring& key, std::function<double()> fallback) {
        auto it = stats.find(key);
        return it == stats.end() ? fallback() : it->second;
    }

public:
    TLanguageModel(const std::string& unigramsFile, const std::string& bigramsFile)
        : UniwordLogFreqs(LoadFreqs(std::wifstream(unigramsFile)))
        , BiwordLogFreqs(LoadFreqs(std::wifstream(bigramsFile)))
        , TriGramsLogFreqs(CreateNGramsModel<3>(UniwordLogFreqs))
        , FourGramsLogFreqs(CreateNGramsModel<4>(UniwordLogFreqs))
    {}

    double CalcNGramLogFrequency(const std::wstring& word) const {
        auto ngcIt = NGramsCache.find(word);
        if (ngcIt == NGramsCache.end()) {
            double logVal = GetValueOrDefault(FourGramsLogFreqs, CreateNGram<4>(word, -3), 4 * DefaultCharLogFreq);
            for (int i = -2; i + 1 < static_cast<int>(word.size()); ++i) {
                const double trgFreq = GetValueOrDefault(TriGramsLogFreqs, CreateNGram<3>(word, i), 3 * DefaultCharLogFreq);
                logVal += GetValueOrDefault(FourGramsLogFreqs, CreateNGram<4>(word, i), 4 * DefaultCharLogFreq) - trgFreq;
            }
            ngcIt = NGramsCache.insert(std::make_pair(word, logVal)).first;
        }
        return ngcIt->second;
    }

    double GetUniwordLogFrequency(const std::wstring& word) const {
        return GetValueOrDefault(UniwordLogFreqs, word, [&word, this]()->double {
            return CalcNGramLogFrequency(word);
        });
    }

    double GetBiwordLogFrequency(const std::wstring& w1, const std::wstring& w2) const {
        return GetValueOrDefault(BiwordLogFreqs, GetBiwordKey(w1, w2), [&w1, &w2, this]()->double {
            return GetUniwordLogFrequency(w1) + GetUniwordLogFrequency(w2);
        });
    }

    double CalcModel(const std::wstring& text) const {
        double modelValue = 0.0;
        TByWordsSplitIterator wIt(text);
        if (wIt) {
            std::wstring prevWord = *wIt;
            modelValue = GetUniwordLogFrequency(prevWord);
            for (++wIt; wIt; ++wIt) {
                const std::wstring curWord = *wIt;
                modelValue += GetBiwordLogFrequency(prevWord, curWord) - GetUniwordLogFrequency(prevWord);
                prevWord = curWord;
            }
        }
        return modelValue;
    }

    std::set<wchar_t> GetAllCharacters() const {
        std::set<wchar_t> characters;
        for (auto item: UniwordLogFreqs) {
            for (auto ch: item.first)
                characters.insert(ch);
        }
        return std::move(characters);
    }
};



