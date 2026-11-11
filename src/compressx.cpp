#include "algolib/compressx.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace algolib {

// ==================================================
// CUSTOM MIN-PRIORITY QUEUE FOR HUFFMAN NODES
// ==================================================

HuffmanPriorityQueue::HuffmanPriorityQueue()
    : heap(nullptr), len(0), cap(0) {}

HuffmanPriorityQueue::~HuffmanPriorityQueue() {
    delete[] heap;
    heap = nullptr;
    len = 0;
    cap = 0;
}

void HuffmanPriorityQueue::grow() {
    int newCap = (cap == 0) ? 16 : cap * 2;
    HuffmanNode** temp = new HuffmanNode*[newCap];
    for (int i = 0; i < len; ++i) {
        temp[i] = heap[i];
    }
    delete[] heap;
    heap = temp;
    cap = newCap;
}

void HuffmanPriorityQueue::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent]->freq <= heap[index]->freq)
            break;
        HuffmanNode* temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;
        index = parent;
    }
}

void HuffmanPriorityQueue::siftDown(int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < len && heap[left]->freq < heap[smallest]->freq)
            smallest = left;
        if (right < len && heap[right]->freq < heap[smallest]->freq)
            smallest = right;

        if (smallest == index)
            break;

        HuffmanNode* temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        index = smallest;
    }
}

void HuffmanPriorityQueue::push(HuffmanNode* node) {
    if (len == cap)
        grow();
    heap[len] = node;
    siftUp(len);
    ++len;
}

HuffmanNode* HuffmanPriorityQueue::pop() {
    if (len == 0)
        return nullptr;
    HuffmanNode* result = heap[0];
    --len;
    if (len > 0) {
        heap[0] = heap[len];
        siftDown(0);
    }
    return result;
}


// ==================================================
// BIT STREAM UTILITIES
// ==================================================

class BitWriter {
private:
    std::ofstream& out;
    uint8_t buffer = 0;
    int bitCount = 0;

public:
    explicit BitWriter(std::ofstream& os) : out(os) {}

    void writeBit(int bit) {
        if (bit) {
            buffer |= static_cast<uint8_t>(1 << (7 - bitCount));
        }
        ++bitCount;
        if (bitCount == 8) {
            out.put(static_cast<char>(buffer));
            buffer = 0;
            bitCount = 0;
        }
    }

    void writeCode(const char* code, int length) {
        for (int i = 0; i < length; ++i) {
            writeBit(code[i] == '1' ? 1 : 0);
        }
    }

    void flush() {
        if (bitCount > 0) {
            out.put(static_cast<char>(buffer));
            buffer = 0;
            bitCount = 0;
        }
    }
};

class BitReader {
private:
    std::ifstream& in;
    uint8_t buffer = 0;
    int bitPos = 8;

public:
    explicit BitReader(std::ifstream& is) : in(is) {}

    int readBit() {
        if (bitPos == 8) {
            int byteVal = in.get();
            if (byteVal == EOF)
                return -1;
            buffer = static_cast<uint8_t>(byteVal);
            bitPos = 0;
        }
        int bit = (buffer >> (7 - bitPos)) & 1;
        ++bitPos;
        return bit;
    }
};


// ==================================================
// HUFFMAN TREE TRAVERSAL AND CLEANUP
// ==================================================

void CompressX::destroyTree(HuffmanNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

void CompressX::generateCodes(HuffmanNode* node, char* buffer, int depth, HuffmanBitCode* codeTable) {
    if (!node) return;

    // Leaf node reached
    if (!node->left && !node->right) {
        if (node->byteVal >= 0 && node->byteVal < 256) {
            if (depth == 0) {
                // Single symbol edge case: code is "0"
                buffer[0] = '0';
                buffer[1] = '\0';
                std::memcpy(codeTable[node->byteVal].bits, buffer, 2);
                codeTable[node->byteVal].len = 1;
            } else {
                buffer[depth] = '\0';
                std::memcpy(codeTable[node->byteVal].bits, buffer, depth + 1);
                codeTable[node->byteVal].len = depth;
            }
        }
        return;
    }

    if (node->left) {
        buffer[depth] = '0';
        generateCodes(node->left, buffer, depth + 1, codeTable);
    }
    if (node->right) {
        buffer[depth] = '1';
        generateCodes(node->right, buffer, depth + 1, codeTable);
    }
}


// ==================================================
// FILE COMPRESSION
// ==================================================

CompressionStats CompressX::compressFile(const std::string& inputPath, const std::string& outputPath) {
    CompressionStats stats;

    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile.is_open()) {
        stats.errorMessage = "Failed to open input file: " + inputPath;
        return stats;
    }

    // 1. Calculate file size and byte frequencies
    uint64_t frequencies[256];
    for (int i = 0; i < 256; ++i)
        frequencies[i] = 0;

    char readBuf[4096];
    uint64_t totalBytes = 0;
    while (inFile.read(readBuf, sizeof(readBuf)) || inFile.gcount() > 0) {
        std::streamsize bytesRead = inFile.gcount();
        totalBytes += static_cast<uint64_t>(bytesRead);
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            uint8_t byteVal = static_cast<uint8_t>(readBuf[i]);
            ++frequencies[byteVal];
        }
    }
    inFile.close();

    stats.originalBytes = totalBytes;

    int uniqueCount = 0;
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0)
            ++uniqueCount;
    }
    stats.uniqueSymbols = uniqueCount;

    // 2. Open output file
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        stats.errorMessage = "Failed to create compressed output file: " + outputPath;
        return stats;
    }

    // Write Magic Number: 'C' 'M' 'P' 'X'
    const char magic[4] = {'C', 'M', 'P', 'X'};
    outFile.write(magic, 4);

    // Write original file size (8 bytes)
    outFile.write(reinterpret_cast<const char*>(&totalBytes), sizeof(totalBytes));

    // Write unique character count (2 bytes)
    uint16_t uCount16 = static_cast<uint16_t>(uniqueCount);
    outFile.write(reinterpret_cast<const char*>(&uCount16), sizeof(uCount16));

    // Handle empty file edge-case
    if (totalBytes == 0 || uniqueCount == 0) {
        outFile.close();
        stats.success = true;
        stats.compressedBytes = 14; // header only
        stats.compressionRatio = 0.0;
        stats.spaceSaved = 0.0;
        return stats;
    }

    // Write frequency table entries: [1 byte char, 8 bytes frequency]
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            uint8_t ch = static_cast<uint8_t>(i);
            outFile.write(reinterpret_cast<const char*>(&ch), 1);
            outFile.write(reinterpret_cast<const char*>(&frequencies[i]), sizeof(uint64_t));
        }
    }

    // 3. Build Huffman Tree using Custom Priority Queue
    HuffmanPriorityQueue pq;
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            pq.push(new HuffmanNode(i, frequencies[i]));
        }
    }

    if (pq.size() == 1) {
        // Single unique symbol special case
        HuffmanNode* only = pq.pop();
        HuffmanNode* parent = new HuffmanNode(-1, only->freq, only, nullptr);
        pq.push(parent);
    } else {
        while (pq.size() > 1) {
            HuffmanNode* left = pq.pop();
            HuffmanNode* right = pq.pop();
            HuffmanNode* parent = new HuffmanNode(-1, left->freq + right->freq, left, right);
            pq.push(parent);
        }
    }

    HuffmanNode* root = pq.pop();

    // 4. Generate Huffman Bit Codes
    HuffmanBitCode codeTable[256];
    for (int i = 0; i < 256; ++i) {
        codeTable[i].len = 0;
        codeTable[i].bits[0] = '\0';
    }

    char codeBuf[256];
    generateCodes(root, codeBuf, 0, codeTable);

    // 5. Encode original file contents into bit stream
    std::ifstream reInFile(inputPath, std::ios::binary);
    if (!reInFile.is_open()) {
        destroyTree(root);
        stats.errorMessage = "Failed to re-read input file for encoding";
        return stats;
    }

    BitWriter bitWriter(outFile);
    while (reInFile.read(readBuf, sizeof(readBuf)) || reInFile.gcount() > 0) {
        std::streamsize bytesRead = reInFile.gcount();
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            uint8_t byteVal = static_cast<uint8_t>(readBuf[i]);
            bitWriter.writeCode(codeTable[byteVal].bits, codeTable[byteVal].len);
        }
    }
    bitWriter.flush();

    reInFile.close();
    outFile.close();
    destroyTree(root);

    // 6. Calculate statistics
    std::ifstream checkOut(outputPath, std::ios::binary | std::ios::ate);
    stats.compressedBytes = checkOut.is_open() ? static_cast<uint64_t>(checkOut.tellg()) : 0;
    checkOut.close();

    if (stats.originalBytes > 0) {
        stats.compressionRatio = (1.0 - (static_cast<double>(stats.compressedBytes) / stats.originalBytes)) * 100.0;
        stats.spaceSaved = (static_cast<double>(stats.originalBytes - stats.compressedBytes) / stats.originalBytes) * 100.0;
    } else {
        stats.compressionRatio = 0.0;
        stats.spaceSaved = 0.0;
    }

    stats.success = true;
    return stats;
}


// ==================================================
// FILE DECOMPRESSION
// ==================================================

CompressionStats CompressX::decompressFile(const std::string& inputPath, const std::string& outputPath) {
    CompressionStats stats;

    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile.is_open()) {
        stats.errorMessage = "Failed to open compressed file: " + inputPath;
        return stats;
    }

    // 1. Read and verify Magic Header
    char magic[4];
    inFile.read(magic, 4);
    if (inFile.gcount() < 4 || std::memcmp(magic, "CMPX", 4) != 0) {
        stats.errorMessage = "Invalid file format: not a valid CompressX (.cmpx) archive.";
        return stats;
    }

    // 2. Read original file size
    uint64_t originalSize = 0;
    inFile.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));
    stats.originalBytes = originalSize;

    // 3. Read unique character count
    uint16_t uniqueCount = 0;
    inFile.read(reinterpret_cast<char*>(&uniqueCount), sizeof(uniqueCount));
    stats.uniqueSymbols = uniqueCount;

    // Open destination output file
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        stats.errorMessage = "Failed to create decompressed output file: " + outputPath;
        return stats;
    }

    // Handle empty file
    if (originalSize == 0 || uniqueCount == 0) {
        outFile.close();
        inFile.close();
        stats.success = true;
        stats.compressedBytes = 14;
        return stats;
    }

    // 4. Read frequency table
    uint64_t frequencies[256];
    for (int i = 0; i < 256; ++i)
        frequencies[i] = 0;

    for (int i = 0; i < uniqueCount; ++i) {
        uint8_t ch = 0;
        uint64_t freq = 0;
        inFile.read(reinterpret_cast<char*>(&ch), 1);
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(uint64_t));
        frequencies[ch] = freq;
    }

    // 5. Reconstruct exact Huffman Tree
    HuffmanPriorityQueue pq;
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            pq.push(new HuffmanNode(i, frequencies[i]));
        }
    }

    if (pq.size() == 1) {
        HuffmanNode* only = pq.pop();
        HuffmanNode* parent = new HuffmanNode(-1, only->freq, only, nullptr);
        pq.push(parent);
    } else {
        while (pq.size() > 1) {
            HuffmanNode* left = pq.pop();
            HuffmanNode* right = pq.pop();
            HuffmanNode* parent = new HuffmanNode(-1, left->freq + right->freq, left, right);
            pq.push(parent);
        }
    }

    HuffmanNode* root = pq.pop();

    // 6. Decode bit stream using the Huffman tree
    BitReader bitReader(inFile);
    HuffmanNode* current = root;
    uint64_t decodedBytes = 0;

    while (decodedBytes < originalSize) {
        int bit = bitReader.readBit();
        if (bit == -1) {
            stats.errorMessage = "Unexpected end of file while decoding compressed bit stream.";
            destroyTree(root);
            outFile.close();
            inFile.close();
            return stats;
        }

        current = (bit == 0) ? current->left : current->right;
        if (!current) {
            stats.errorMessage = "Corrupt bit sequence encountered during Huffman tree decoding.";
            destroyTree(root);
            outFile.close();
            inFile.close();
            return stats;
        }

        // If leaf reached
        if (!current->left && !current->right) {
            outFile.put(static_cast<char>(current->byteVal));
            ++decodedBytes;
            current = root; // Reset to tree root for next symbol
        }
    }

    outFile.close();
    inFile.close();
    destroyTree(root);

    std::ifstream checkIn(inputPath, std::ios::binary | std::ios::ate);
    stats.compressedBytes = checkIn.is_open() ? static_cast<uint64_t>(checkIn.tellg()) : 0;
    checkIn.close();

    stats.success = true;
    return stats;
}


// ==================================================
// FILE VERIFICATION
// ==================================================

bool CompressX::verifyFilesMatch(const std::string& path1, const std::string& path2) {
    std::ifstream f1(path1, std::ios::binary);
    std::ifstream f2(path2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open())
        return false;

    // Check sizes
    f1.seekg(0, std::ios::end);
    f2.seekg(0, std::ios::end);
    if (f1.tellg() != f2.tellg())
        return false;

    f1.seekg(0, std::ios::beg);
    f2.seekg(0, std::ios::beg);

    char buf1[4096];
    char buf2[4096];

    while (f1.good() && f2.good()) {
        f1.read(buf1, sizeof(buf1));
        f2.read(buf2, sizeof(buf2));

        if (f1.gcount() != f2.gcount())
            return false;

        if (std::memcmp(buf1, buf2, f1.gcount()) != 0)
            return false;
    }

    return true;
}


// ==================================================
// FREQUENCY ANALYSIS & CODES GENERATION (FOR UI)
// ==================================================

std::string CompressX::analyzeFrequenciesAndCodes(const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open())
        return "Unable to open file for analysis.";

    uint64_t frequencies[256];
    for (int i = 0; i < 256; ++i)
        frequencies[i] = 0;

    char readBuf[4096];
    uint64_t totalBytes = 0;
    while (inFile.read(readBuf, sizeof(readBuf)) || inFile.gcount() > 0) {
        std::streamsize bytesRead = inFile.gcount();
        totalBytes += static_cast<uint64_t>(bytesRead);
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            uint8_t byteVal = static_cast<uint8_t>(readBuf[i]);
            ++frequencies[byteVal];
        }
    }
    inFile.close();

    if (totalBytes == 0)
        return "File is empty (0 bytes). No character frequencies to display.";

    // Build tree
    HuffmanPriorityQueue pq;
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0)
            pq.push(new HuffmanNode(i, frequencies[i]));
    }

    if (pq.size() == 1) {
        HuffmanNode* only = pq.pop();
        HuffmanNode* parent = new HuffmanNode(-1, only->freq, only, nullptr);
        pq.push(parent);
    } else {
        while (pq.size() > 1) {
            HuffmanNode* left = pq.pop();
            HuffmanNode* right = pq.pop();
            HuffmanNode* parent = new HuffmanNode(-1, left->freq + right->freq, left, right);
            pq.push(parent);
        }
    }

    HuffmanNode* root = pq.pop();

    HuffmanBitCode codeTable[256];
    for (int i = 0; i < 256; ++i) {
        codeTable[i].len = 0;
        codeTable[i].bits[0] = '\0';
    }

    char codeBuf[256];
    generateCodes(root, codeBuf, 0, codeTable);
    destroyTree(root);

    std::ostringstream ss;
    ss << "=== CHARACTER FREQUENCY & HUFFMAN CODES ===\n";
    ss << "Total File Size: " << totalBytes << " bytes\n\n";
    ss << std::left << std::setw(8) << "Byte"
       << std::setw(12) << "Char"
       << std::setw(14) << "Frequency"
       << std::setw(12) << "Percentage"
       << "Huffman Code\n";
    ss << std::string(62, '-') << "\n";

    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            double pct = (static_cast<double>(frequencies[i]) / totalBytes) * 100.0;
            std::string charDesc;
            if (i >= 33 && i <= 126) {
                charDesc = "'";
                charDesc += static_cast<char>(i);
                charDesc += "'";
            } else if (i == 32) {
                charDesc = "'SPACE'";
            } else if (i == 10) {
                charDesc = "'\\n'";
            } else if (i == 13) {
                charDesc = "'\\r'";
            } else if (i == 9) {
                charDesc = "'\\t'";
            } else {
                std::ostringstream hexStr;
                hexStr << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << i;
                charDesc = hexStr.str();
            }

            ss << std::left << std::setw(8) << i
               << std::setw(12) << charDesc
               << std::setw(14) << frequencies[i]
               << std::fixed << std::setprecision(2) << std::setw(10) << pct << "%  "
               << codeTable[i].bits << "\n";
        }
    }

    return ss.str();
}

} // namespace algolib

