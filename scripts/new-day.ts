#!/usr/bin/env node

import { mkdirSync, writeFileSync, readFileSync, existsSync } from 'fs';
import { join } from 'path';
import { parseArgs } from 'util';

interface Args {
  day: number;
  year: number;
}

interface EnvConfig {
  sessionToken: string;
  userAgent: string;
}

function getArgs(): Args {
  const now = new Date();
  const currentYear = now.getFullYear();
  const currentDay = now.getDate();

  const { values } = parseArgs({
    args: process.argv.slice(2),
    options: {
      day: {
        type: 'string',
        short: 'd',
      },
      year: {
        type: 'string',
        short: 'y',
      },
    },
  });

  const day = values.day ? parseInt(values.day, 10) : currentDay;
  const year = values.year ? parseInt(values.year, 10) : currentYear;

  if (isNaN(day) || day < 1 || day > 25) {
    console.error('Error: Day must be a number between 1 and 25');
    process.exit(1);
  }

  if (isNaN(year) || year < 2015) {
    console.error('Error: Year must be a number >= 2015');
    process.exit(1);
  }

  return { day, year };
}

function loadEnvConfig(): EnvConfig | null {
  const envPath = '.env.json';
  if (!existsSync(envPath)) {
    return null;
  }

  try {
    const envContent = readFileSync(envPath, 'utf-8');
    const config = JSON.parse(envContent);
    
    if (!config.sessionToken || !config.userAgent) {
      console.warn('Warning: .env.json missing sessionToken or userAgent');
      return null;
    }
    
    return config as EnvConfig;
  } catch (error) {
    console.warn('Warning: Failed to parse .env.json:', error);
    return null;
  }
}

async function downloadInput(day: number, year: number, config: EnvConfig): Promise<string | null> {
  const url = `https://adventofcode.com/${year}/day/${day}/input`;
  
  try {
    const response = await fetch(url, {
      headers: {
        'Cookie': `session=${config.sessionToken}`,
        'User-Agent': config.userAgent,
      },
    });

    if (!response.ok) {
      if (response.status === 404) {
        console.warn(`Warning: Input not available yet (404)`);
      } else if (response.status === 400) {
        console.warn(`Warning: Invalid session token or input not available`);
      } else {
        console.warn(`Warning: Failed to download input (HTTP ${response.status})`);
      }
      return null;
    }

    const input = await response.text();
    return input;
  } catch (error) {
    console.warn('Warning: Failed to download input:', error);
    return null;
  }
}

async function createDayBoilerplate(day: number, year: number): Promise<void> {
  const dayStr = day;
  const yearDir = join('src', year.toString());
  const dayDir = join(yearDir, `day${dayStr}`);

  // Check if day already exists
  if (existsSync(dayDir)) {
    console.error(`Error: ${dayDir} already exists`);
    process.exit(1);
  }

  // Load env config for downloading input
  const envConfig = loadEnvConfig();

  // Create year directory if it doesn't exist
  if (!existsSync(yearDir)) {
    mkdirSync(yearDir, { recursive: true });
    console.log(`Created year directory: ${yearDir}`);
    
    // Create year CMakeLists.txt
    const yearCMakeContent = `# Advent of Code ${year}\nadd_subdirectory(day${dayStr})\n`;
    writeFileSync(join(yearDir, 'CMakeLists.txt'), yearCMakeContent);
    console.log(`Created ${yearDir}/CMakeLists.txt`);
    
    // Add year to main CMakeLists.txt
    const mainCMakePath = join('src', 'CMakeLists.txt');
    const mainCMakeContent = readFileSync(mainCMakePath, 'utf-8');
    if (!mainCMakeContent.includes(`add_subdirectory(${year})`)) {
      const updatedContent = mainCMakeContent.replace(
        /# Year solutions\n/,
        `# Year solutions\nadd_subdirectory(${year})\n`
      );
      writeFileSync(mainCMakePath, updatedContent);
      console.log(`Added year ${year} to src/CMakeLists.txt`);
    }
  } else {
    // Update year CMakeLists.txt to include new day
    const yearCMakePath = join(yearDir, 'CMakeLists.txt');
    const yearCMakeContent = readFileSync(yearCMakePath, 'utf-8');
    if (!yearCMakeContent.includes(`add_subdirectory(day${dayStr})`)) {
      const updatedContent = yearCMakeContent + `add_subdirectory(day${dayStr})\n`;
      writeFileSync(yearCMakePath, updatedContent);
      console.log(`Added day${dayStr} to ${yearDir}/CMakeLists.txt`);
    }
  }

  // Create day directory
  mkdirSync(dayDir, { recursive: true });
  console.log(`Created day directory: ${dayDir}`);

  // Create main.cpp
  const mainCppContent = `#include <iostream>
#include "common/Base.hpp"

class Day${dayStr} : public aoc::Base {
  public:
    Day${dayStr}() = default;
    
    [[nodiscard]] int day() const override {
       return ${dayStr}; 
    }
    
    [[nodiscard]] int year() const override {
       return ${year % 100}; 
    }

    [[nodiscard]] std::optional<i64> part1Expected() const override {
        return std::nullopt;
    }
    
    [[nodiscard]] std::optional<i64> part2Expected() const override {
        return std::nullopt;
    }

    [[nodiscard]] i64 solvePart1() const override {
        // TODO: Implement part 1
        return 0;
    }

    [[nodiscard]] i64 solvePart2() const override {
        // TODO: Implement part 2
        return 0;
    }
};

int main() {
    Day${dayStr} solution;
    
    solution.run();
    
    return 0;
}
`;
  writeFileSync(join(dayDir, 'main.cpp'), mainCppContent);
  console.log(`Created ${dayDir}/main.cpp`);

  // Create CMakeLists.txt
  const cmakeBuildContent = `add_executable(day${dayStr}
  main.cpp
)

target_link_libraries(day${dayStr} PRIVATE common fmt::fmt)

set_target_properties(day${dayStr} PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
`;
  writeFileSync(join(dayDir, 'CMakeLists.txt'), cmakeBuildContent);
  console.log(`Created ${dayDir}/CMakeLists.txt`);

  // Create input.txt - try to download if config is available
  let inputContent = `# Place your input here\n`;
  
  if (envConfig) {
    console.log('Downloading input from adventofcode.com...');
    const downloadedInput = await downloadInput(day, year, envConfig);
    if (downloadedInput) {
      inputContent = downloadedInput;
      console.log('✓ Input downloaded successfully');
    } else {
      console.log('Using placeholder input file');
    }
  } else {
    console.log('No .env.json found, using placeholder input file');
  }
  
  writeFileSync(join(dayDir, 'input.txt'), inputContent);
  console.log(`Created ${dayDir}/input.txt`);

  console.log(`\n✓ Successfully created boilerplate for Day ${day}, ${year}`);
  console.log(`\nNext steps:`);
  console.log(`  1. Add your puzzle input to ${dayDir}/input.txt`);
  console.log(`  2. Implement part1() and part2() in ${dayDir}/main.cpp`);
  console.log(`  3. Build: cmake --build build`);
  console.log(`  4. Run: ./build/src/${year}/day${dayStr}/day${dayStr} ${dayDir}/input.txt`);
}

const { day, year } = getArgs();
await createDayBoilerplate(day, year);
