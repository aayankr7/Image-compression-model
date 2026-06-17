LeanJPEG Engine
A high-performance, multithreaded JPEG compression engine built entirely from scratch in pure C++.

This project bypasses commercial encoders and standard libraries to directly parse raw sensor data (Sony .arw) and mathematically compress it into a highly optimized, ISO-standard .jpg bitstream. By stripping out metadata bloat and aggressively optimizing spatial color math, this engine achieves significantly leaner file sizes (~18% reduction) compared to commercial out-of-the-box converters, with no perceptible loss in structural quality.

Core Features
Zero Dependencies: Does not rely on libjpeg, OpenCV, or external black-box compression libraries.

Multithreaded: Utilizes OpenMP to parallelize block processing across all available CPU cores.

Aggressive Optimization: Outputs purely functional bitstreams with zero EXIF, ICC profile, or thumbnail bloat.

Portable: Compiles to a fully static, standalone executable that runs seamlessly on any Windows machine without requiring external DLLs.

Performance Benchmarks
Commercial Converter Output: 1.30 MB

LeanJPEG Output: 1.07 MB

Result: ~18% reduction in file size achieved via mathematically optimal quantization and strictly functional bit-packing.

🧠 How It Works: The Compression Pipeline
Most tutorials and textbooks stop explaining JPEG compression after the ZigZag scanning phase, leaving the actual binary Huffman encoding as a "black box." This engine implements the entire 5-stage pipeline from scratch. Here is a detailed breakdown of the internal architecture.

Phase 1: Spatial Color Math & Subsampling
The engine extracts raw, uncompressed RGB data and converts it into the YCbCr color space (Luminance, Chroma Blue, Chroma Red). Because the human eye is highly sensitive to brightness but poor at detecting fine color detail, the engine executes aggressive 4:2:0 Chroma Subsampling. It spatially averages and drops exactly 75% of the Cb and Cr data before mathematical compression even begins, saving massive amounts of memory without degrading the visual structure.

Phase 2: Frequency Domain (FDCT & Quantization)
The image is divided into 8x8 blocks. Using a custom Forward Discrete Cosine Transform (FDCT), the engine maps physical spatial pixels into the frequency domain.
The resulting frequency matrix is then divided by an ISO-standard Quantization Table. This step cleanly wipes out microscopic, high-frequency visual noise, turning those values into literal 0s while preserving the core structural frequencies.

Phase 3: ZigZag & Run-Length Encoding (RLE)
To maximize the compression of the newly generated 0s, the engine reads the 8x8 matrix in a diagonal ZigZag pattern, clustering all the high-frequency 0s at the end of the array. The RLE shredder then isolates these sequences. Instead of writing sixty individual 0s, it counts them and outputs a single token (e.g., "End of Block"), drastically reducing the data footprint.

Phase 4: Huffman Dictionary & The "Sentinel Bit" Technique
This is where most implementations fail. JPEG compression uses Variable Length Integers (VLI) and Huffman coding to turn the RLE tokens into binary strings.

Because Huffman codes can have varying lengths and often begin with leading 0s (e.g., 0101), storing them as standard integer types in C++ is dangerous—the compiler will truncate leading zeros during math operations, corrupting the bitstream.

To solve this, this engine uses a custom Sentinel Bit & Left-Alignment Algorithm for rapid, mathematically safe bit-concatenation:

C++
struct LeftAlignedPrefix {
    uint32_t bits;           
    uint8_t  trailingZeroes; 
};

// ... inside the dictionary builder ...
uint32_t sentinelBit = static_cast<uint32_t>(1) << found.length;
uint32_t chunkWithSentinel = static_cast<uint32_t>(found.code) | sentinelBit;

uint8_t totalBits = found.length + 1;
uint8_t trailingZeros = static_cast<uint8_t>(32 - totalBits);

return { chunkWithSentinel << trailingZeros, trailingZeros };
How it works:

The Sentinel: The algorithm dynamically injects a 1 exactly at the position immediately preceding the most significant bit of the actual Huffman code.

Left-Alignment: It shifts the entire binary chunk flush to the left boundary of a 32-bit integer.

Size Calculation: It calculates the exact number of trailing zeros.

This technique allows the calculateSize function to instantly determine the exact bit-length of the payload using simple arithmetic. During the tight bit-packing loop, the engine can quickly mask and shift these left-aligned chunks into the byte accumulator without conditional branching or risking the truncation of leading zeros.

Phase 5: Byte Accumulation & Byte Stuffing
Hard drives do not write abstract 1s and 0s; they write 8-bit Bytes. The engine uses a custom JPEG_BitWriter accumulator class to catch the variable-length bits. The exact millisecond the accumulator hits 8 bits, it fires the byte into the file.

Crucially, it safely handles the JPEG Byte Stuffing Trap. In the JPEG standard, 0xFF is a reserved file marker. If the Huffman code randomly generates 11111111 (0xFF), the engine physically injects a blank 0x00 byte immediately after it (FF 00). This ensures the decoder recognizes it as image data rather than a premature End-Of-File marker, preventing corruption.

🛠️ Build and Usage Instructions
Prerequisites
A C++ compiler (GCC/MinGW, Clang, or MSVC) supporting C++17.

No external libraries required.

Compiling for Portability (Windows)
To ensure the resulting executable can run natively on any Windows machine without requiring external DLLs (like libstdc++ or libgomp), compile the engine using static linking flags.

Run the following command in your terminal:

Bash
g++ -O3 -fopenmp main.cpp -o leanjpeg -static-libgcc -static-libstdc++ -static
-O3: Aggressive runtime optimization.

-fopenmp: Enables multithreading.

-static: Packs standard C++ and OpenMP libraries directly into the executable.

Execution
Run the compiled executable from the command line, passing your raw image file as an argument (or as configured in your source):

Bash
leanjpeg input_image.arw
The engine will process the file across multiple threads and generate a fully optimized compressed.jpg in the working directory.

License
This project is open-source and available under the MIT License.
