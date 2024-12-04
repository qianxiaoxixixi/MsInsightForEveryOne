/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "BinFileGenerator.h"

namespace Dic::Module::Source::Test {
void BinFileGenerator::Generate(const std::string& outputPath)
{
    this->filePath = outputPath;
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        return;
    }
    for (const auto &item: dataBlocks) {
        item->Write2File(outFile);
    }
    outFile.close();
}
bool BinFileGenerator::RemoveFile(const std::string& path)
{
    if (std::remove(path.c_str()) != 0) {
        std::cout << "remove bin file failed: " + path << std::endl;
        return false;
    } else {
        std::cout << "remove bin file successfully: " + path << std::endl;
        return true;
    }
}

bool BinFileGenerator::RemoveFile()
{
    return RemoveFile(filePath);
}
}

using namespace Dic::Module::Source::Test;