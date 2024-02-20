#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <locale>
#include <omp.h>
#include <chrono>

//-------------------------------------SEQUENZIALE-----------------------------------------------
// Funzione per calcolare l'istogramma dei bigrammi dei caratteri in modo sequenzialmente
std::map<std::string, int> computeCharBigramHistogramSequential(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;
    for (size_t i = 0; i < text.length() - 1; ++i) {
        std::string currentBigram = text.substr(i, 2);
        if (isalpha(currentBigram[0], loc) && isalpha(currentBigram[1], loc)) {
            currentBigram[0] = tolower(currentBigram[0], loc);
            currentBigram[1] = tolower(currentBigram[1], loc);
            histogram[currentBigram]++;
        }
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei trigrammi dei caratteri in modo sequenzialmente
std::map<std::string, int> computeCharTrigramHistogramSequential(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc("it_IT.UTF-8");

    for (size_t i = 1; i < text.length() - 1; ++i) {
        std::string currentTrigram = text.substr(i - 1, 3);

        if (isalpha(currentTrigram[0], loc) &&
            isalpha(currentTrigram[1], loc) &&
            isalpha(currentTrigram[2], loc)) {
            currentTrigram[0] = tolower(currentTrigram[0], loc);
            currentTrigram[1] = tolower(currentTrigram[1], loc);
            currentTrigram[2] = tolower(currentTrigram[2], loc);
            histogram[currentTrigram]++;
        }
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei bigrammi delle parole in modo sequenzialmente
std::map<std::string, int> computeWordBigramHistogramSequential(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;
    std::istringstream iss(text);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>());

    for (size_t i = 0; i < words.size() - 1; ++i) {
        std::string currentBigram = words[i] + " " + words[i + 1];
        currentBigram[0] = std::tolower(currentBigram[0], loc);
        currentBigram[currentBigram.length() - 1] = std::tolower(currentBigram[currentBigram.length() - 1], loc);
        histogram[currentBigram]++;
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei trigrammi delle parole in modo sequenzialmente
std::map<std::string, int> computeWordTrigramHistogramSequential(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;
    std::istringstream iss(text);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>());

    for (size_t i = 0; i < words.size() - 2; ++i) {
        std::string currentTrigram = words[i] + " " + words[i + 1] + " " + words[i + 2];
        currentTrigram[0] = std::tolower(currentTrigram[0], loc);
        currentTrigram[currentTrigram.length() - 1] = std::tolower(currentTrigram[currentTrigram.length() - 1], loc);
        histogram[currentTrigram]++;
    }

    return histogram;
}

//--------------------------------------PARALLELO-----------------------------------------------
// Funzione per calcolare l'istogramma dei bigrammi dei caratteri in modo parallelo
std::map<std::string, int> computeCharBigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    std::vector<std::map<std::string, int>> localHistograms(numThreads);

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t blockSize = (textLength + numThreads - 1) / numThreads;
        size_t start = threadId * blockSize;
        size_t end = std::min(start + blockSize, textLength - 1);


        std::string currentBigram;
        for (size_t i = start; i < end; ++i) {
            currentBigram = text.substr(i, 2);
            if (std::isalpha(currentBigram[0], loc) && std::isalpha(currentBigram[1], loc)) {
                currentBigram[0] = std::tolower(currentBigram[0], loc);
                currentBigram[1] = std::tolower(currentBigram[1], loc);
                localHistograms[threadId][currentBigram]++;
            }
        }
    }

    for (const auto& localHistogram : localHistograms) {
        for (const auto& pair : localHistogram) {
#pragma omp atomic
            histogram[pair.first] += pair.second;
        }
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei trigrammi dei caratteri in modo parallelo
std::map<std::string, int> computeCharTrigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc("it_IT.UTF-8");

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    std::vector<std::map<std::string, int>> localHistograms(numThreads);

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t blockSize = (textLength + numThreads - 1) / numThreads;
        size_t start = threadId * blockSize;
        size_t end = std::min(start + blockSize, textLength - 2);

        std::string currentTrigram;
        for (size_t i = start + 1; i < end; ++i) {
            currentTrigram = text.substr(i - 1, 3);
            if (std::isalpha(currentTrigram[0], loc) && std::isalpha(currentTrigram[1], loc) &&
                std::isalpha(currentTrigram[2], loc)) {
                currentTrigram[0] = std::tolower(currentTrigram[0], loc);
                currentTrigram[1] = std::tolower(currentTrigram[1], loc);
                currentTrigram[2] = std::tolower(currentTrigram[2], loc);
                localHistograms[threadId][currentTrigram]++;
            }
        }
    }

    for (const auto& localHistogram : localHistograms) {
        for (const auto& pair : localHistogram) {
#pragma omp atomic
            histogram[pair.first] += pair.second;
        }
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei bigrammi delle parole in modo parallelo
std::map<std::string, int> computeWordBigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    std::vector<std::map<std::string, int>> localHistograms(numThreads);

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t blockSize = (textLength + numThreads - 1) / numThreads;
        size_t start = threadId * blockSize;
        size_t end = std::min(start + blockSize, textLength);

        std::istringstream iss(text.substr(start, end - start));
        std::vector<std::string> words(std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>());

        for (size_t i = 0; i < words.size() - 1; ++i) {
            std::string currentBigram = words[i] + " " + words[i + 1];
            currentBigram[0] = std::tolower(currentBigram[0], loc);
            currentBigram[currentBigram.length() - 1] = std::tolower(currentBigram[currentBigram.length() - 1], loc);
            localHistograms[threadId][currentBigram]++;
        }
    }

    for (const auto& localHistogram : localHistograms) {
        for (const auto& pair : localHistogram) {
#pragma omp atomic
            histogram[pair.first] += pair.second;
        }
    }

    return histogram;
}

// Funzione per calcolare l'istogramma dei trigrammi delle parole in modo parallelo
std::map<std::string, int> computeWordTrigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    std::vector<std::map<std::string, int>> localHistograms(numThreads);

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t blockSize = (textLength + numThreads - 1) / numThreads;
        size_t start = threadId * blockSize;
        size_t end = std::min(start + blockSize, textLength);

        std::istringstream iss(text.substr(start, end - start));
        std::vector<std::string> words(std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>());

        for (size_t i = 0; i < words.size() - 2; ++i) {
            std::string currentTrigram = words[i] + " " + words[i + 1] + " " + words[i + 2];
            currentTrigram[0] = std::tolower(currentTrigram[0], loc);
            currentTrigram[currentTrigram.length() - 1] = std::tolower(currentTrigram[currentTrigram.length() - 1], loc);
            localHistograms[threadId][currentTrigram]++;
        }
    }

    for (const auto& localHistogram : localHistograms) {
        for (const auto& pair : localHistogram) {
#pragma omp atomic
            histogram[pair.first] += pair.second;
        }
    }

    return histogram;
}



int main() {
    // Lettura del testo da un file
    std::string path = std::filesystem::current_path().string();
    std::ifstream inputFile(path + "/generic_books.txt");
    if (!inputFile.is_open() || !inputFile.good()) {

        std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
        exit(EXIT_FAILURE);

    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();

    auto startSequential = std::chrono::high_resolution_clock::now();

    std::map<std::string, int> charBigramHistogram = computeCharBigramHistogramSequential(text);
    std::map<std::string, int> charTrigramHistogram = computeCharTrigramHistogramSequential(text);
    std::map<std::string, int> wordBigramHistogram = computeWordBigramHistogramSequential(text);
    std::map<std::string, int> wordTrigramHistogram = computeWordTrigramHistogramSequential(text);

    auto endSequential = std::chrono::high_resolution_clock::now();

    std::cout << "* Sequential Execution *" << std::endl;

    //std::cout << "\nCharacters Bigram Histogram:" << std::endl;
    //for (const auto& pair : charBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nCharacters Trigram Histogram:" << std::endl;
    //for (const auto& pair : charTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}
    //std::cout << "Words Bigram Histogram:" << std::endl;
    //for (const auto& entry : wordBigramHistogram) {
    //    std::cout << entry.first << ": " << entry.second << std::endl;
    //}

    //std::cout << "Words Trigram Histogram:" << std::endl;
    //for (const auto& entry : wordTrigramHistogram) {
    //    std::cout << entry.first << ": " << entry.second << std::endl;
    //}

    std::chrono::duration<double> sequentialTime = endSequential - startSequential;
    std::cout << "\nSequential execution time: " << sequentialTime.count() << " seconds." << std::endl;

    std::cout << "\n---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n* Parallel Execution *" << std::endl;
    int num_threads = 16;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: " << num_threads << std::endl;

    auto startParallel16 = std::chrono::high_resolution_clock::now();

#pragma omp parallel sections num_threads(num_threads) // Sezione parallela

    {
#pragma omp section
        {
            charBigramHistogram = computeCharBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            charTrigramHistogram = computeCharTrigramHistogramParallel(text);
        }
#pragma omp section
        {
            wordBigramHistogram = computeWordBigramHistogramParallel(text);
        }

#pragma omp section
        {
            wordTrigramHistogram = computeWordTrigramHistogramParallel(text);
        }
    }

    auto endParallel16 = std::chrono::high_resolution_clock::now();

    //std::cout << "\Characters Bigram Histogram:" << std::endl;
    //for (const auto& pair : charBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nCharacters Trigram Histogram:" << std::endl;
    //for (const auto& pair : charTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Bigram Histogram:" << std::endl;
    //for (const auto& pair : wordBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Trigram Histogram:" << std::endl;
    //for (const auto& pair : wordTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    std::chrono::duration<double> parallelTime16 = endParallel16 - startParallel16;
    std::cout << "\nParallel execution time with 16 threads: " << parallelTime16.count() << " seconds." << std::endl;

    auto speedup16 = sequentialTime.count() / parallelTime16.count();

    std::cout << "\nSpeedup: " << speedup16 << std::endl;
    std::cout << "\n---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n* Parallel Execution *" << std::endl;
    num_threads = 8;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: " << num_threads << std::endl;
    auto startParallel8 = std::chrono::high_resolution_clock::now();

#pragma omp parallel sections num_threads(num_threads) // Sezione parallela

    {
#pragma omp section
        {
            charBigramHistogram = computeCharBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            charTrigramHistogram = computeCharTrigramHistogramParallel(text);
        }
#pragma omp section
        {
            wordBigramHistogram = computeWordBigramHistogramParallel(text);
        }

#pragma omp section
        {
            wordTrigramHistogram = computeWordTrigramHistogramParallel(text);
        }
    }

    auto endParallel8 = std::chrono::high_resolution_clock::now();

    //std::cout << "\nCharacters Bigram Histogram:" << std::endl;
    //for (const auto& pair : charBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nCharacters Trigram Histogram:" << std::endl;
    //for (const auto& pair : charTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Bigram Histogram:" << std::endl;
    //for (const auto& pair : wordBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Trigram Histogram:" << std::endl;
    //for (const auto& pair : wordTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    std::chrono::duration<double> parallelTime8 = endParallel8 - startParallel8;
    std::cout << "\nParallel execution time with 8 threads: " << parallelTime8.count() << " seconds." << std::endl;

    auto speedup8 = sequentialTime.count() / parallelTime8.count();

    std::cout << "\nSpeedup: " << speedup8 << std::endl;
    std::cout << "\n---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n* Parallel Execution *" << std::endl;
    num_threads = 4;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: " << num_threads << std::endl;
    auto startParallel4 = std::chrono::high_resolution_clock::now();

#pragma omp parallel sections num_threads(num_threads) // Sezione parallela

    {
#pragma omp section
        {
            charBigramHistogram = computeCharBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            charTrigramHistogram = computeCharTrigramHistogramParallel(text);
        }
#pragma omp section
        {
            wordBigramHistogram = computeWordBigramHistogramParallel(text);
        }

#pragma omp section
        {
            wordTrigramHistogram = computeWordTrigramHistogramParallel(text);
        }
    }
    auto endParallel4 = std::chrono::high_resolution_clock::now();

    //std::cout << "\nCharacters Bigram Histogram:" << std::endl;
    //for (const auto& pair : charBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nCharacters Trigram Histogram:" << std::endl;
    //for (const auto& pair : charTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Bigram Histogram:" << std::endl;
    //for (const auto& pair : wordBigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    //std::cout << "\nWords Trigram Histogram:" << std::endl;
    //for (const auto& pair : wordTrigramHistogram) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}

    std::chrono::duration<double> parallelTime4 = endParallel4 - startParallel4;
    std::cout << "\nParallel execution time with 4 threads: " << parallelTime4.count() << " seconds." << std::endl;

    auto speedup4 = sequentialTime.count() / parallelTime4.count();

    std::cout << "\nSpeedup: " << speedup4 << std::endl;
    std::cout << "\n---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\nSpeedup records:\n \n- 16 threads speedup: " << speedup16 << "\n- 8  threads speedup: " << speedup8 << "\n- 4  threads speedup: " << speedup4 << "\n ";

    return 0;
}
