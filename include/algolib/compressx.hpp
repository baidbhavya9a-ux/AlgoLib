#ifndef ALGOLIB_COMPRESSX_HPP
#define ALGOLIB_COMPRESSX_HPP

#include <string>
#include <cstdint>
#include <cstddef>

namespace algolib {

// Statistics returned after compression or decompression operations
struct CompressionStats {
    bool success = false;
    std::string errorMessage;
    uint64_t originalBytes = 0;
    uint64_t compressedBytes = 0;
    double compressionRatio = 0.0; // Percentage reduced
    double spaceSaved = 0.0;
    int uniqueSymbols = 0;
    bool verifiedMatch = false;
};

// Huffman Tree Node for frequency-based prefix coding
struct HuffmanNode {
    int byteVal; // 0..255 for leaf byte values, -1 for internal routing nodes
    uint64_t freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(int val, uint64_t f, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr)
        : byteVal(val), freq(f), left(l), right(r) {}
};

// Custom Min-Priority Queue for Huffman tree construction (without STL)
class HuffmanPriorityQueue {
private:
    HuffmanNode** heap;
    int len;
    int cap;

    void siftUp(int index);
    void siftDown(int index);
    void grow();

public:
    HuffmanPriorityQueue();
    ~HuffmanPriorityQueue();

    void push(HuffmanNode* node);
    HuffmanNode* pop();
    int size() const { return len; }
    bool empty() const { return len == 0; }
};

// Custom Bit Code container for 256 byte values
struct HuffmanBitCode {
    char bits[256];
    int len;
};

// CompressX Huffman Coding Engine
class CompressX {
private:
    static void generateCodes(HuffmanNode* node, char* buffer, int depth, HuffmanBitCode* codeTable);
    static void destroyTree(HuffmanNode* node);

public:
    // Compress input file to output compressed file (.cmpx)
    static CompressionStats compressFile(const std::string& inputPath, const std::string& outputPath);

    // Decompress .cmpx file back to original file
    static CompressionStats decompressFile(const std::string& inputPath, const std::string& outputPath);

    // Verify bit-for-bit exact match between two files
    static bool verifyFilesMatch(const std::string& path1, const std::string& path2);

    // Get formatted frequency table and generated Huffman codes for display
    static std::string analyzeFrequenciesAndCodes(const std::string& filePath);
};

} // namespace algolib

#endif // ALGOLIB_COMPRESSX_HPP

