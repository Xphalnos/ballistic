<h1 align="center">
  The Ballistic JIT Engine
</h1>

<p align="center"><em>“The world's fastest ARM recompiler"</em></p>

# Overview

This is a rewrite the dynarmic recompiler, with the goal of fixing its many flaws.

## Dynarmic Flaws

*   The JIT state structure exceeds 64 bytes (the typical size of a CPU cache line). This makes it difficult to store reliably in the cache, leading to eviction issues and inefficient access patterns.
*   The JIT state pointer is constantly read from and written to. CPU branch mispredictions regarding the next incoming store/read result in significant performance penalties.
*   The allocation strategy for compiled code blocks causes new blocks to evict older blocks, but the execution flow often jumps back to those older, now-evicted blocks, causing instruction cache misses.
*    Calls made from JIT code back to the C++ host environment result in total cache thrashing and stack clobbering, disrupting the execution pipeline.
*    The setup and teardown code for each basic block is stupidly large (approximately 128 bytes per block), wasting memory and instruction cache space
*   Unlike GCC or LLVM, the current JIT backend lacks a peephole optimizer to perform local code improvements (e.g., instruction combining or redundant instruction removal).
*   XMM (SSE/AVX) register spilling is not properly implemented, leading to potential data corruption or inefficient register usage.
*   The code relies heavily on loading absolute pointers. It is suggested to use `base[index]` addressing, which allows the CPU to use a fast `LEA` (Load Effective Address) instruction, avoiding the latency of loading 8-byte pointers (which might also be unaligned).
*    The current intrusive list implementation relies on pointer chasing, which destroys data locality.
    *   *Proposed Solution:* Switch to **dense linked lists**. This involves using a backing array of elements and an array of indices. Swapping indices is faster than swapping pointers, and keeping data contiguous improves cache coherency.
*    The Intermediate Representation layer is too heavy, creating a large memory footprint and performance hotspots.
*    The `Argument` class uses excessive memory for every instance. Since arguments are ubiquitous in the IR, this results in significant cumulative memory waste.
*   The C++ compiler fails to devirtualize critical calls, particularly terminal handlers and coprocessor logic. This adds the overhead of virtual function lookups to hot code paths.
*   The `IsImmediate()` function utilizes recursion, which constantly clobbers the micro-op (uop) cache, degrading CPU front-end performance.
*    There are "shenanigans" regarding `mmap` usage where the code attempts to outsmart the OS/Compiler but likely results in suboptimal memory management.
