#pragma once

#include <filesystem>
#include <functional>
#include <iostream>

#include "../../src/em/utils/FileUtils.h"
#include "../../src/em/utils/StringUtils.h"

namespace test::utils {
[[nodiscard]] inline bool runSnapshotTests(
    const std::filesystem::path& snapshotsPath,
    const std::string& inputFilesName,
    const std::string& expectedOutputFilesName,
    const std::function<std::wstring(const std::wstring& input)>& getOutput) {
  using namespace em::utils;
  auto result = true;
  auto directories = file::getDirectories(snapshotsPath);
  for (const auto& directory : directories) {
    auto input = file::getFileContents(directory / inputFilesName);
    auto expectedOutput =
        file::getFileContents(directory / expectedOutputFilesName);
    try {
      auto output = getOutput(input);
      if (output != expectedOutput) {
        result = false;
        std::replace(expectedOutput.begin(), expectedOutput.end(), '\n', ';');
        std::replace(output.begin(), output.end(), '\n', ';');
        std::cerr << directory.c_str() << ": \""
                  << string::wStringToString(output) << "\" does not equal \""
                  << string::wStringToString(expectedOutput) << "\"\n";
      } else {
        std::cout << directory.c_str() << ": OK!\n";
      }
    } catch (const std::exception& exception) {
      result = false;
      std::cerr << directory.c_str() << ": Program failed with error: \""
                << exception.what() << "\"\n";
    }
  }
  return result;
}
}  // namespace test::utils
