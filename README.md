# fadfs-core

> [!CAUTION]
> **Project Status: Suspended (Paused)**

## Overview
`fadfs-core` is a custom file system implementation written in C. It was designed as a lightweight, block-based file system for educational and experimental purposes.

## Why this project is suspended
Development on this project has been paused indefinitely due to two primary critical issues:

1.  **Persistent Data Corruption:** The file system suffers from recurring corruption for reasons that remain elusive. Despite various attempts to stabilize the block management and inode allocation, data integrity cannot be guaranteed.
2.  **Flawed Error Handling Design:** The initial design decision for error handling was suboptimal. The heavy reliance on C macros and manual error propagation has made the codebase nearly impossible to debug or maintain. Catching and tracing bugs in this environment has become an exercise in frustration.

## Future Plans
There is a possibility that this project will be completely rewritten in **Rust** or **C++** in the future to leverage better memory safety, modern error handling patterns (like `Result<T, E>`), and more robust abstractions. However, there are no immediate plans to start this rewrite.

## Core Concepts
The `fadfs` (File AS Disk File System) is built around several standard file system primitives:

-   **Superblock:** Contains global metadata about the file system, including block size, inode counts, and offsets to various tables.
-   **Inodes:** Represents file system objects. Each inode stores metadata (size, type) and pointers to data blocks.
-   **Block-Based Storage:** Data is organized into fixed-size blocks (4096 bytes).
-   **Direct & Indirect Blocks:** Inodes use 12 direct block pointers for small files and an indirect block pointer for larger files.
-   **Bitmaps:** Used for tracking the allocation status of data blocks and inodes.
-   **Path Resolution:** A basic path-to-inode resolution system for navigating directories.

## Technical Details
-   **Magic Number:** `FDFS`
-   **Block Size:** 4096 bytes
-   **Inode Size:** 128 bytes
-   **Language:** C
-   **Build System:** CMake
