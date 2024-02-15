std::map<std::string, int> computeCharBigramHistogramParallel(const std::string& text) {
    std::map<std::string, int> histogram;
    std::locale loc;
    const int numThreads = omp_get_max_threads();

#pragma omp parallel for schedule(dynamic) num_threads(numThreads)
    for (int i = 0; i < text.length() - 1; ++i) {
        std::string currentBigram = text.substr(i, 2);
        if (isalpha(currentBigram[0], loc) && isalpha(currentBigram[1], loc)) {
            currentBigram[0] = tolower(currentBigram[0], loc);
            currentBigram[1] = tolower(currentBigram[1], loc);

#pragma omp critical(add_to_histogram)
            {
                histogram[currentBigram]++;
            }
        }
    }

    return histogram;
}