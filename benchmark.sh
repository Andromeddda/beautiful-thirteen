#!/usr/bin/env bash
set -euo pipefail

# Configuration 
BUILD_DIR="build"
BINARY="${BUILD_DIR}/beautiful-thirteen"
RUNS=100
CMAKE_BUILD_TYPE="Release"

# Colors 
bold="\033[1m"
dim="\033[2m"
reset="\033[0m"

printf "${bold}===== Beautiful Numbers Benchmark ====${reset}\n\n"

# Build 
printf "${dim}Build type:${reset} %s\n\n" "$CMAKE_BUILD_TYPE"

printf "Configuring & building... "
compile_start=$(date +%s%N)
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" --fresh > /dev/null 2>&1
cmake --build "$BUILD_DIR" --clean-first -j "$(nproc)" > /dev/null 2>&1
compile_end=$(date +%s%N)
compile_ms=$(( (compile_end - compile_start) / 1000000 ))
printf "done in ${bold}%d ms${reset}\n" "$compile_ms"

# Detect compiler from CMake cache
compiler=$(cmake -LA -N "$BUILD_DIR" 2>/dev/null \
    | grep 'CMAKE_CXX_COMPILER:' \
    | cut -d= -f2)
compiler_version=$("$compiler" --version 2>/dev/null | head -1 || echo "unknown")
cxx_flags=$(cmake -LA -N "$BUILD_DIR" 2>/dev/null \
    | grep 'CMAKE_CXX_FLAGS_RELEASE:' \
    | cut -d= -f2)

printf "${dim}Compiler:${reset}   %s\n" "$compiler_version"
printf "${dim}Flags:${reset}      %s\n\n" "$cxx_flags"

#  2. Correctness check 
result=$("$BINARY")
printf "Result:     ${bold}%s${reset}\n\n" "$result"

#  3. Execution time (averaged over $RUNS runs) 
printf "Running %d iterations... " "$RUNS"

total_us=0
for ((i = 1; i <= RUNS; i++)); do
    iter_start=$(date +%s%N)
    "$BINARY" > /dev/null
    iter_end=$(date +%s%N)
    total_us=$(( total_us + (iter_end - iter_start) / 1000 ))
done

avg_us=$(( total_us / RUNS ))
printf "done\n\n"

# Binary size 
binary_size=$(wc -c < "$BINARY")
binary_size_kb=$(awk "BEGIN { printf \"%.1f\", $binary_size / 1024 }")

# Report 
divider="================================"
printf "${bold}%s${reset}\n" "$divider"
printf "  Build time       :  %d ms\n"       "$compile_ms"
printf "  Avg execution    :  %d µs  (%d runs)\n" "$avg_us" "$RUNS"
printf "  Binary size      :  %s KiB\n"      "$binary_size_kb"
printf "  Answer           :  %s\n"           "$result"
printf "${bold}%s${reset}\n\n" "$divider"

# Markdown table
REPORT="BENCHMARK.md"
cat > "$REPORT" <<EOF
## Benchmark

| Metric | Value |
|-|-|
| Compiler | \`${compiler_version}\` |
| Release flags | \`${cxx_flags}\` |
| Build time | ${compile_ms} ms  |
| Avg execution time | ${avg_us} us (averaged over ${RUNS} runs) |
| Binary size | ${binary_size_kb} KiB  |
| **Answer** | **${result}** |

> Measured on \`$(uname -srm)\`, $(nproc) cores, $(date -u +"%Y-%m-%d %H:%M UTC").
EOF

printf "Markdown report saved to ${bold}%s${reset}\n" "$REPORT"