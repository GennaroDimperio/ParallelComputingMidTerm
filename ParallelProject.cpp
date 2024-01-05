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

// Funzione per calcolare l'istogramma dei bigrammi sequenzialmente
std::map<std::string, int> computeBigramHistogram(const std::string& text) {
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

// Funzione per calcolare l'istogramma dei trigrammi sequenzialmente
std::map<std::string, int> computeTrigramHistogram(const std::string& text) {
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

// Funzione per calcolare l'istogramma dei bigrammi parallelamente
std::map<std::string, int> computeBigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    const size_t blockSize = (textLength + numThreads - 1) / numThreads;

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t start = blockSize * threadId;
        size_t end = std::min(start + blockSize, textLength);

        std::map<std::string, int> localHistogram;

        for (size_t i = start; i < text.length() - 1; ++i) {
            std::string currentBigram;
            currentBigram = text.substr(i, 2);
            if (isalpha(currentBigram[0], loc) && isalpha(currentBigram[1], loc)) {
                currentBigram[0] = tolower(currentBigram[0], loc);
                currentBigram[1] = tolower(currentBigram[1], loc);
                localHistogram[currentBigram]++;
            }
        }

        // Aggiunta dei risultati locali all'istogramma globale in modo critico
#pragma omp critical(add_to_histogram)
        {
            for (const auto& pair : localHistogram) {
                histogram[pair.first] += pair.second;
            }
        }
    }

    return histogram;
}


// Funzione per calcolare l'istogramma dei trigrammi parallelamente
std::map<std::string, int> computeTrigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc("it_IT.UTF-8");

    const int numThreads = omp_get_max_threads();
    const size_t textLength = text.length();
    const size_t blockSize = (textLength + numThreads - 1) / numThreads;

#pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        size_t start = blockSize * threadId;
        size_t end = std::min(start + blockSize, textLength);

        std::map<std::string, int> localHistogram;

        for (size_t i = start + 1; i < end - 1; ++i) {
            std::string currentTrigram = text.substr(i - 1, 3);
            if (isalpha(currentTrigram[0], loc) && isalpha(currentTrigram[1], loc) &&
                isalpha(currentTrigram[2], loc)) {
                currentTrigram[0] = tolower(currentTrigram[0], loc);
                currentTrigram[1] = tolower(currentTrigram[1], loc);
                currentTrigram[2] = tolower(currentTrigram[2], loc);
                localHistogram[currentTrigram]++;
            }
        }

        // Aggiunta dei risultati locali all'istogramma globale in modo critico
#pragma omp critical(add_to_histogram)
        {
            for (const auto& pair : localHistogram) {
                histogram[pair.first] += pair.second;
            }
        }
    }

    return histogram;
}


int main() {
    // Lettura del testo da un file
    std::string path = std::filesystem::current_path().string();
    std::ifstream inputFile(path + "/MobyDick_chap1.txt");
    if (!inputFile.is_open() || !inputFile.good()) {

        std::cout << "Impossibile aprire il file o l'input non e' valido." << std::endl;
        exit(EXIT_FAILURE);

    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();

    auto startSequential = std::chrono::high_resolution_clock::now();

    std::map<std::string, int> bigramHistogram = computeBigramHistogram(text);
    std::map<std::string, int> trigramHistogram = computeTrigramHistogram(text);

    auto endSequential = std::chrono::high_resolution_clock::now();

    std::cout << "* Sequential Execution *" << std::endl;
    std::cout << "\nBigram Histogram:" << std::endl;
    for (const auto& pair : bigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nTrigram Histogram:" << std::endl;
    for (const auto& pair : trigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::chrono::duration<double> sequentialTime = endSequential - startSequential;
    std::cout << "\nSequential execution time: " << sequentialTime.count() << " seconds." << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    auto startParallel16 = std::chrono::high_resolution_clock::now();
    std::cout << "\n* Parallel Execution *" << std::endl;
    int num_threads = 16;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: "<< num_threads << std::endl;


#pragma omp parallel sections // Inizio della sezione parallela

    {
#pragma omp section
        {
            bigramHistogram = computeBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            trigramHistogram = computeTrigramHistogramParallel(text);
        }
    }

    auto endParallel16 = std::chrono::high_resolution_clock::now();

    std::cout << "\nBigram Histogram:" << std::endl;
    for (const auto& pair : bigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nTrigram Histogram:" << std::endl;
    for (const auto& pair : trigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::chrono::duration<double> parallelTime16 = endParallel16 - startParallel16;
    std::cout << "\nParallel execution time with 16 threads: " << parallelTime16.count() << " seconds." << std::endl;

    auto speedup16 = sequentialTime.count() / parallelTime16.count();

    std::cout << "\nSpeedup: " << speedup16 << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n* Parallel Execution *" << std::endl;
    auto startParallel8 = std::chrono::high_resolution_clock::now();
    num_threads = 8;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: " << num_threads << std::endl;

#pragma omp parallel sections // Inizio della sezione parallela

    {
#pragma omp section
        {
            bigramHistogram = computeBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            trigramHistogram = computeTrigramHistogramParallel(text);
        }
    }

    auto endParallel8 = std::chrono::high_resolution_clock::now();

    std::cout << "\nBigram Histogram:" << std::endl;
    for (const auto& pair : bigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nTrigram Histogram:" << std::endl;
    for (const auto& pair : trigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }


    std::chrono::duration<double> parallelTime8 = endParallel8 - startParallel8;
    std::cout << "\nParallel execution time with 8 threads: " << parallelTime8.count() << " seconds." << std::endl;

    auto speedup8 = sequentialTime.count() / parallelTime8.count();

    std::cout << "\nSpeedup: " << speedup8 << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n* Parallel Execution *" << std::endl;

    auto startParallel4 = std::chrono::high_resolution_clock::now();

    num_threads = 4;
    omp_set_num_threads(num_threads);
    std::cout << "\nNumber of threads: " << num_threads << std::endl;

#pragma omp parallel sections // Inizio della sezione parallela

    {
#pragma omp section
        {
            bigramHistogram = computeBigramHistogramParallel(text);
        }

#pragma omp section 
        {
            trigramHistogram = computeTrigramHistogramParallel(text);
        }
    }
    auto endParallel4 = std::chrono::high_resolution_clock::now();

    std::cout << "\nBigram Histogram:" << std::endl;
    for (const auto& pair : bigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nTrigram Histogram:" << std::endl;
    for (const auto& pair : trigramHistogram) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }


    std::chrono::duration<double> parallelTime4 = endParallel4 - startParallel4;
    std::cout << "\nParallel execution time with 4 threads: " << parallelTime4.count() << " seconds." << std::endl;

    auto speedup4 = sequentialTime.count() / parallelTime4.count();

    std::cout << "\nSpeedup: " << speedup4 << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\nSpeedup records:\n \n- 16 threads speedup: "<<speedup16<<"\n- 8  threads speedup: "<<speedup8<<"\n- 4  threads speedup: "<<speedup4<<"\n ";
   
    return 0;
}
