// Sample.c
#include <stdio.h>
#include "org_allesoft_correlator_experiment_JavaBridge.h"
#include <jni.h> // Include the JNI header

int main(int argc, char **argv);

JNIEXPORT void JNICALL Java_org_allesoft_correlator_experiment_JavaBridge_spi(JNIEnv *env, jobject obj) {
    printf("Printing from native\\n");
    char *args[3] = {"./iceprog", "-S", "/home/wieker/Projects/electrolyser/fpga/test_spi/top.bin"};
    main(1, args);
}

