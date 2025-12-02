# Advent of Code - C++23

Solutions for Advent of Code using C++23 and Meson build system.

## Setup

### Prerequisites
- C++23 compatible compiler (GCC 13+, Clang 16+, or MSVC 19.35+)
- Meson build system
- Ninja build backend
- Node.js (with TypeScript support via `--experimental-strip-types`)

### Configuration (Optional)

To enable automatic input downloading, create a `.env.json` file in the project root:

```json
{
  "sessionToken": "your_session_token_here",
  "userAgent": "your-email@example.com"
}
```

Get your session token from adventofcode.com cookies after logging in.

### Building

```bash
# Configure the build (output to ./dist)
meson setup dist

# Build all days
meson compile -C dist

# Build specific day
meson compile -C dist day1
```

## Running Solutions

```bash
# Run day 1 of 2025
./dist/src/2025/day1/day1 src/2025/day1/input.txt
```

## Project Structure

```
.
├── src/              # Source code root
│   ├── common/       # Common utilities
│   │   ├── utils.hpp
│   │   └── utils.cpp
│   └── YYYY/         # Solutions by year
│       └── dayXX/    # Solutions for each day
│           ├── main.cpp
│           └── input.txt
├── dist/             # Build output directory
├── meson.build       # Root build configuration
└── README.md
```

## Adding a New Day

Use the provided script to generate boilerplate for a new day:

```bash
node --experimental-strip-types scripts/new-day.ts <day> <year>

# Example: Create day 2 for 2025
node --experimental-strip-types scripts/new-day.ts 2 2025
```

Or manually:

1. Create a new directory under `src/YYYY/` (e.g., `src/2025/day02/`)
2. Copy the template from `src/2025/day01/`:
   ```bash
   cp -r src/2025/day1 src/2025/day02
   ```
3. Add the subdirectory to `src/YYYY/meson.build`:
   ```meson
   subdir('day02')
   ```
4. Reconfigure and build:
   ```bash
   meson setup --reconfigure dist
   meson compile -C dist
   ```

## Adding a New Year

1. Create a new year directory (e.g., `src/2026/`)
2. Copy the year template:
   ```bash
   cp -r src/2025 src/2026
   ```
3. Add the subdirectory to `src/meson.build`:
   ```meson
   subdir('2026')
   ```
