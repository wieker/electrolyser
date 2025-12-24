#include <jni.h>
#include <stdio.h>
#include "HelloWorld.h"

JNIEXPORT void JNICALL
Java_org_allesoft_correlator_experiment_JavaBridge_print(JNIEnv *env, jobject obj) {

	printf("Hello world!\n");
	return;
}
