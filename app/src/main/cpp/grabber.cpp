#include <jni.h>
#include <fstream>
#include <android/log.h>
#include "grabber/core/ElfReader.h"
#include "grabber/output/HeaderWriter.h"
#include "grabber/utils/StringUtils.h"
#include "log/log.h"

using namespace ELFIO;

extern "C"
JNIEXPORT jstring JNICALL
Java_antik_libgrabber_lib_dump(JNIEnv* env, jclass clazz,
                                        jstring inPath_, jstring outPath_) {
    const char* inputPath = env->GetStringUTFChars(inPath_, nullptr);
    const char* outputPath = env->GetStringUTFChars(outPath_, nullptr);

    if (!inputPath || !outputPath) {
        if (inputPath) env->ReleaseStringUTFChars(inPath_, inputPath);
        if (outputPath) env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF("invalid paths");
    }
    // Check if input file exists
    FILE* checkFile = fopen(inputPath, "r");
    if (!checkFile) {
       // LOGE("Input file does not exist or cannot be read: %s", inputPath);
        env->ReleaseStringUTFChars(inPath_, inputPath);
        env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF("Input not found");
    }
    fclose(checkFile);
    auto result = ElfReader::readSymbols(inputPath);

    FILE* testWrite = fopen(outputPath, "w");
    if (!testWrite) {
       // LOGE("Cannot write to output path: %s", outputPath);

        env->ReleaseStringUTFChars(inPath_, inputPath);
        env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF("permission denied");
    }
    fclose(testWrite);
    remove(outputPath);

    std::ofstream out(outputPath);
    if (!out.is_open()) {
        LOGE("Failed to open output file: %s", outputPath);
        env->ReleaseStringUTFChars(inPath_, inputPath);
        env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF("Failed to open output file");
    }

    HeaderWriter::write(out, result.freeFunctions, result.classes);

    out.close();
    FILE* verify = fopen(outputPath, "r");
    if (verify) {


        fseek(verify, 0, SEEK_END);
        long size = ftell(verify);
        fclose(verify);
        LOGI("written %ld bytes", size);

        std::string successMsg = "Dumped --> " + std::string(outputPath) + " --> " + std::to_string(size);
        env->ReleaseStringUTFChars(inPath_, inputPath);
        env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF(successMsg.c_str());
    } else {
        LOGE("failed ");
        env->ReleaseStringUTFChars(inPath_, inputPath);
        env->ReleaseStringUTFChars(outPath_, outputPath);
        return env->NewStringUTF("file write verification failed");
    }
}