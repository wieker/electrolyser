// Sample.c
#include <stdio.h>
#include "org_allesoft_correlator_experiment_JavaBridge.h"
#include <jni.h> // Include the JNI header

JNIEXPORT void JNICALL Java_org_allesoft_correlator_experiment_JavaBridge_print(JNIEnv *env, jobject obj) {
    printf("Printing from native\\n");
}

