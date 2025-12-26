// Sample.c
#include <stdio.h>
#include "org_allesoft_correlator_experiment_JavaBridge.h"
#include <jni.h> // Include the JNI header

int main(int argc, char **argv);
void spi_dump_main();

JNIEXPORT void JNICALL Java_org_allesoft_correlator_experiment_JavaBridge_print(JNIEnv *env, jobject obj) {
    printf("Printing from native\\n");
    char *args[3] = {"./iceprog", "-S", "/home/wieker/Projects/electrolyser/fpga/spi_dump/top.bin"};
    main(3, args);
}

JNIEXPORT void JNICALL Java_org_allesoft_correlator_experiment_JavaBridge_spi(JNIEnv *env, jobject obj, jbyteArray data) {
    jboolean isCopy;
    jbyte *cData = (*env)->GetByteArrayElements(env, data, &isCopy);
    if (cData == NULL) {
        return; // Error handling
    }

    jsize length = (*env)->GetArrayLength(env, data);


    printf("Printing from native\\n");
    spi_dump_main();

    (*env)->ReleaseByteArrayElements(env, data, cData, JNI_COMMIT);
}

