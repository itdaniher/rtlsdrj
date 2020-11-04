/*
 * Copyright (C) 2013 by Robert Schoch <r.schoch@t-online.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <jni.h>
#include "rtlsdrj.h"
#include "rtl-sdr.h"

// The ctx type for rtlsdr_read_async()

typedef struct jcb_ctx {
    rtlsdr_dev_t *dev;
    JNIEnv *env;
    jobject jcb;
} jcb_ctx_t;
typedef jcb_ctx_t *jcb_ctx_p;

// To hold classes, methods and fields as global references.

jclass illegalArgumentExceptionClass = NULL;
jclass nullPointerExceptionClass = NULL;

jclass rtlDeviceClass = NULL;
jclass rtlCallbackInterface = NULL;
jclass rtlExceptionClass = NULL;
jclass usbStringsClass = NULL;
jclass xtalFreqClass = NULL;

jfieldID rtlDevIndexField = NULL;
jfieldID rtlDevHandleField = NULL;
jmethodID rtlCallbackMethod = NULL;

jclass JNICALL createGlobalClassRef(JNIEnv *env, const char *name) {

  jclass localRef, globalRef;

  localRef = (*env)->FindClass(env, name);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;
  globalRef = (*env)->NewGlobalRef(env, localRef);
  (*env)->DeleteLocalRef(env, localRef);
  return globalRef;

}

void JNICALL throwNewRtlException(JNIEnv *env, const char *msg, ...) {

  va_list argp;
  char cb[0x80];

  memset(cb, 0, sizeof(cb));
  va_start(argp, msg);
  vsnprintf(cb, sizeof(cb)-1, msg, argp);
  va_end(argp);

  (*env)->ThrowNew(env, rtlExceptionClass, cb);

}

jlong JNICALL getDeviceHandle(JNIEnv *env, jobject rtlDeviceObject) {

  jint jdevIndex;
  jlong jdevHandle;

  jdevHandle = (*env)->GetLongField(env, rtlDeviceObject, rtlDevHandleField);

  if(jdevHandle == 0L) {

    jdevIndex = (*env)->GetIntField(env, rtlDeviceObject, rtlDevIndexField);
    throwNewRtlException(env, "There is no active device handle for index %i.", jdevIndex);

  }

  return jdevHandle;

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

  JNIEnv* env;

  if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4)!=JNI_OK) return 0;

  // Do all the class loading overhead on JNI startup.

  if((illegalArgumentExceptionClass = createGlobalClassRef(env, "java/lang/IllegalArgumentException")) == NULL) return 0;
  if((nullPointerExceptionClass = createGlobalClassRef(env, "java/lang/NullPointerException")) == NULL) return 0;

  if((rtlDeviceClass = createGlobalClassRef(env, "de/rtlsdr/RtlDevice")) == NULL) return 0;
  if((rtlCallbackInterface = createGlobalClassRef(env, "de/rtlsdr/RtlCallback")) == NULL) return 0;
  if((rtlExceptionClass = createGlobalClassRef(env, "de/rtlsdr/RtlException")) == NULL) return 0;
  if((usbStringsClass = createGlobalClassRef(env, "de/rtlsdr/UsbStrings")) == NULL) return 0;
  if((xtalFreqClass = createGlobalClassRef(env, "de/rtlsdr/XtalFreq")) == NULL) return 0;

  if((rtlDevIndexField = (*env)->GetFieldID(env, rtlDeviceClass, "devIndex", "I")) == NULL) return 0;
  if((rtlDevHandleField = (*env)->GetFieldID(env, rtlDeviceClass, "devHandle", "J")) == NULL) return 0;
  if((rtlCallbackMethod = (*env)->GetMethodID(env, rtlCallbackInterface, "rtlData",
      "(Ljava/nio/ByteBuffer;I)V")) == NULL) return 0;

  return JNI_VERSION_1_4;

}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {

  JNIEnv* env;

  if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4)!=JNI_OK) return;

  (*env)->DeleteGlobalRef(env, nullPointerExceptionClass);
  (*env)->DeleteGlobalRef(env, rtlDeviceClass);
  (*env)->DeleteGlobalRef(env, rtlCallbackInterface);
  (*env)->DeleteGlobalRef(env, rtlExceptionClass);
  (*env)->DeleteGlobalRef(env, usbStringsClass);
  (*env)->DeleteGlobalRef(env, xtalFreqClass);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getDeviceCount(
    JNIEnv *env, jclass cls) {

  return (jint)rtlsdr_get_device_count();

}

JNIEXPORT jstring JNICALL Java_de_rtlsdr_RtlDevice_getDeviceName(
    JNIEnv *env, jclass cls, jint index) {

  const char *name;

  name = rtlsdr_get_device_name((uint32_t)index);
  if(name == NULL) return NULL;
  return (*env)->NewStringUTF(env, name);

}

JNIEXPORT jobject JNICALL Java_de_rtlsdr_RtlDevice_getDeviceUsbStrings(
    JNIEnv *env, jclass cls, jint index) {

  uint32_t ret;
  char buf1[0x100], buf2[0x100], buf3[0x100];
  jstring  man, prd, ser;
  jmethodID mid;

  ret = rtlsdr_get_device_usb_strings((uint32_t)index, buf1, buf2, buf3);

  if(ret != 0) {
    throwNewRtlException(env, "rtlsdr_get_device_usb_strings returned %i", ret);
    return NULL;
  }

  man = (*env)->NewStringUTF(env, buf1);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  prd = (*env)->NewStringUTF(env, buf2);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  ser = (*env)->NewStringUTF(env, buf3);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  mid = (*env)->GetMethodID(env, usbStringsClass, "<init>",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  return (*env)->NewObject(env, usbStringsClass, mid, man, prd, ser);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getIndexBySerial(
    JNIEnv *env, jclass cls, jstring serial) {

  int ret;
  const char *ser;

  if(serial == NULL) {
    throwNewRtlException(env, "illegal null serial");
    return 0;
  }

  ser = (*env)->GetStringUTFChars(env, serial, 0);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_get_index_by_serial(ser);

  (*env)->ReleaseStringUTFChars(env, serial, ser);

  if(ret < 0) {
    throwNewRtlException(env, "rtlsdr_get_index_by_serial returned %i", ret);
    return 0;
  }

  return (jint)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_open(
    JNIEnv *env, jobject obj) {

  int ret;
  rtlsdr_dev_t *dev;
  jint jdevIndex;
  jlong jdevHandle;

  jdevIndex = (*env)->GetIntField(env, obj, rtlDevIndexField);
  jdevHandle = (*env)->GetLongField(env, obj, rtlDevHandleField);

  if(jdevHandle != 0L) {
    throwNewRtlException(env,
        "There is another active device handle for index %i.", (int)jdevIndex);
    return;
  }

  ret = rtlsdr_open(&dev, (uint32_t)jdevIndex);

  if(ret != 0) {
    throwNewRtlException(env, "rtlsdr_open returned %i", ret);
    return;
  }

  (*env)->SetLongField(env, obj, rtlDevHandleField, (jlong)dev);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_close(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_close((rtlsdr_dev_t *)jdevHandle);

  if(ret != 0) {
    throwNewRtlException(env, "rtlsdr_close returned %i", ret);
    return;
  }

  (*env)->SetLongField(env, obj, rtlDevHandleField, 0);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setXtalFreq(
    JNIEnv *env, jobject obj, jobject xtalFreq) {

  int ret;
  jlong jdevHandle;
  jmethodID rtlFreqGetter, tunerFreqGetter;
  jint rtlFreq, tunerFreq;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  rtlFreqGetter = (*env)->GetMethodID(env, xtalFreqClass, "getRtlFreq", "()I");
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  tunerFreqGetter = (*env)->GetMethodID(env, xtalFreqClass, "getTunerFreq", "()I");
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  rtlFreq = (*env)->CallIntMethod(env, xtalFreq, rtlFreqGetter);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  tunerFreq = (*env)->CallIntMethod(env, xtalFreq, tunerFreqGetter);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_xtal_freq((rtlsdr_dev_t *)jdevHandle,
      (uint32_t)rtlFreq, (uint32_t)tunerFreq);

  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_xtal_freq returned %i", ret);

}

JNIEXPORT jobject JNICALL Java_de_rtlsdr_RtlDevice_getXtalFreq(
    JNIEnv *env, jobject obj) {

  int ret;
  uint32_t rtl_freq, tuner_freq;
  jlong jdevHandle;
  jmethodID mid;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  ret = rtlsdr_get_xtal_freq((rtlsdr_dev_t *)jdevHandle, &rtl_freq, &tuner_freq);

  if(ret != 0) {
    throwNewRtlException(env, "rtlsdr_get_xtal_freq returned %i", ret);
    return NULL;
  }

  mid = (*env)->GetMethodID(env, xtalFreqClass, "<init>", "(II)V");
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  return (*env)->NewObject(env, xtalFreqClass, mid,
      (jint)rtl_freq, (jint)tuner_freq);

}

JNIEXPORT jobject JNICALL  Java_de_rtlsdr_RtlDevice_getUsbStrings(
    JNIEnv *env, jobject obj) {

  int ret;
  char buf1[0x100], buf2[0x100], buf3[0x100];
  jlong jdevHandle;
  jstring  man, prd, ser;
  jmethodID mid;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  ret = rtlsdr_get_usb_strings((rtlsdr_dev_t *)jdevHandle, buf1, buf2, buf3);

  if(ret != 0) {
    throwNewRtlException(env, "rtlsdr_get_usb_strings returned %i", ret);
    return NULL;
  }

  man = (*env)->NewStringUTF(env, buf1);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  prd = (*env)->NewStringUTF(env, buf2);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  ser = (*env)->NewStringUTF(env, buf3);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  mid = (*env)->GetMethodID(env, usbStringsClass, "<init>",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  return (*env)->NewObject(env, usbStringsClass, mid, man, prd, ser);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_writeEeprom(
    JNIEnv *env, jobject obj, jbyteArray data, jint offset, jint len) {

  int ret;
  jlong jdevHandle;
  jbyte *bytes;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;
  bytes = (*env)->GetByteArrayElements(env, data, 0);

  if(bytes == NULL) {
    throwNewRtlException(env, "GetByteArrayElements failed");
    return;
  }

  ret = rtlsdr_write_eeprom((rtlsdr_dev_t *)jdevHandle,
      (uint8_t *)bytes, (uint8_t)offset, (uint16_t)len);

  (*env)->ReleaseByteArrayElements(env, data, bytes, 0);
  if(ret < 0) throwNewRtlException(env, "rtlsdr_write_eeprom returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_readEeprom(
    JNIEnv *env, jobject obj, jbyteArray data, jint offset, jint len) {

  int ret;
  jlong jdevHandle;
  jbyte *bytes;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;
  bytes = (*env)->GetByteArrayElements(env, data, 0);

  ret = rtlsdr_read_eeprom((rtlsdr_dev_t *)jdevHandle,
      (uint8_t *)bytes, (uint8_t)offset, (uint16_t)len);

  (*env)->ReleaseByteArrayElements(env, data, bytes, 0);
  if(ret < 0) throwNewRtlException(env, "rtlsdr_read_eeprom returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setCenterFreq(
    JNIEnv *env, jobject obj, jlong freq) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_center_freq((rtlsdr_dev_t *)jdevHandle, (uint32_t)freq);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_center_freq returned %i", ret);

}

JNIEXPORT jlong JNICALL Java_de_rtlsdr_RtlDevice_getCenterFreq(
    JNIEnv *env, jobject obj) {

  uint32_t ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0L;

  ret = rtlsdr_get_center_freq((rtlsdr_dev_t *)jdevHandle);
  if(ret == 0) throwNewRtlException(env, "rtlsdr_get_center_freq returned %u", ret);
  return (jlong)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setFreqCorrection(
    JNIEnv *env, jobject obj, jint ppm) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_freq_correction((rtlsdr_dev_t *)jdevHandle, (int)ppm);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_freq_correction returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getFreqCorrection(
    JNIEnv *env, jobject obj) {

  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  return (jint)rtlsdr_get_freq_correction((rtlsdr_dev_t *)jdevHandle);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getTunerType(
    JNIEnv *env, jobject obj) {

  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  return (jint)rtlsdr_get_tuner_type((rtlsdr_dev_t *)jdevHandle);

}

JNIEXPORT jintArray JNICALL Java_de_rtlsdr_RtlDevice_getTunerGains
(JNIEnv *env, jobject obj) {

  int ret;
  int gains[0x40];
  jlong jdevHandle;
  jintArray jintGains;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  ret = rtlsdr_get_tuner_gains((rtlsdr_dev_t *)jdevHandle, gains);

  if(ret <= 0) {
    throwNewRtlException(env, "rtlsdr_get_tuner_gains returned %i", ret);
    return NULL;
  }

  jintGains = (*env)->NewIntArray(env, ret);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  (*env)->SetIntArrayRegion(env, jintGains, 0, (jsize)ret, (jint *)gains);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return NULL;

  return jintGains;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setTunerGain(
    JNIEnv *env, jobject obj, jint gain) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_tuner_gain((rtlsdr_dev_t *)jdevHandle, (int)gain);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_tuner_gain returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getTunerGain(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_get_tuner_gain((rtlsdr_dev_t *)jdevHandle);
  if(ret == 0) throwNewRtlException(env, "rtlsdr_get_tuner_gain returned %i", ret);

  return (jint)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setTunerIfGain(
    JNIEnv *env, jobject obj, jint stage, jint gain) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_tuner_if_gain((rtlsdr_dev_t *)jdevHandle, (int)stage, (int)gain);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_tuner_if_gain returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setTunerGainMode(
    JNIEnv *env, jobject obj, jint mode) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_tuner_gain_mode((rtlsdr_dev_t *)jdevHandle, (int)mode);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_tuner_gain_mode returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setSampleRate(
    JNIEnv *env, jobject obj, jint rate) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_sample_rate((rtlsdr_dev_t *)jdevHandle, (uint32_t)rate);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_sample_rate returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getSampleRate
(JNIEnv *env, jobject obj) {

  uint32_t ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_get_sample_rate((rtlsdr_dev_t *)jdevHandle);
  if(ret == 0) throwNewRtlException(env, "rtlsdr_get_sample_rate returned %i", ret);

  return (jint)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setTestMode(
    JNIEnv *env, jobject obj, jint on) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_testmode((rtlsdr_dev_t *)jdevHandle, (int)on);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_testmode returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setAgcMode(
    JNIEnv *env, jobject obj, jint on) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_agc_mode((rtlsdr_dev_t *)jdevHandle, (int)on);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_agc_mode returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setDirectSampling(
    JNIEnv *env, jobject obj, jint on) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_direct_sampling((rtlsdr_dev_t *)jdevHandle, (int)on);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_direct_sampling returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getDirectSampling(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_get_direct_sampling((rtlsdr_dev_t *)jdevHandle);
  if(ret < 0) throwNewRtlException(env, "rtlsdr_get_direct_sampling returned %i", ret);

  return (jint)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_setOffsetTuning(
    JNIEnv *env, jobject obj, jint on) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_set_offset_tuning((rtlsdr_dev_t *)jdevHandle, (int)on);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_set_offset_tuning returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_getOffsetTuning(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_get_offset_tuning((rtlsdr_dev_t *)jdevHandle);
  if(ret < 0) throwNewRtlException(env, "rtlsdr_get_offset_tuning returned %i", ret);

  return (jint)ret;

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_resetBuffer(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_reset_buffer((rtlsdr_dev_t *)jdevHandle);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_reset_buffer returned %i", ret);

}

JNIEXPORT jint JNICALL Java_de_rtlsdr_RtlDevice_readSync(
    JNIEnv *env, jobject obj, jobject buf, jint len) {

  int ret, read;
  void* addr;
  jlong jdevHandle, dbbCapacity;

  if(buf == NULL) {
    (*env)->ThrowNew(env, nullPointerExceptionClass,
        "Buffer is null.");
    return 0;
  }

  addr = (*env)->GetDirectBufferAddress(env, buf);

  if(addr == NULL) {
    (*env)->ThrowNew(env, illegalArgumentExceptionClass,
        "Cannot get the buffer address. Is this really a direct buffer?");
    return 0;
  }

  dbbCapacity = (*env)->GetDirectBufferCapacity(env, buf);

  if(len > dbbCapacity) {
    (*env)->ThrowNew(env, illegalArgumentExceptionClass,
        "The specified length exceeds the size of the buffer.");
    return 0;
  }

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return 0;

  ret = rtlsdr_read_sync((rtlsdr_dev_t *)jdevHandle, addr, (int)len, &read);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_read_sync returned %i", ret);

  return(jint)read;

}

void AsyncCallback(unsigned char *buf, uint32_t len, void *ctx) {

  jcb_ctx_p jcb_ctx = (jcb_ctx_t *)ctx;
  rtlsdr_dev_t *dev = jcb_ctx->dev;
  JNIEnv *env = jcb_ctx->env;
  jobject jcb = jcb_ctx->jcb;

  jobject dbb;

  if((*env)->ExceptionCheck(env) == JNI_TRUE) {
    rtlsdr_cancel_async(dev);
    return;
  }

  // Is this potentially expensive, although the buffers remain the same ?

  dbb = (*env)->NewDirectByteBuffer(env, buf, len);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) {
    rtlsdr_cancel_async(dev);
    return;
  }

  (*env)->CallVoidMethod(env, jcb, rtlCallbackMethod, dbb, (jint)len);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) {
    rtlsdr_cancel_async(dev);
    return;
  }

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_readAsync(
    JNIEnv *env, jobject obj, jobject jcb, jint numBuffers, jint bufferLen) {

  int ret;
  jcb_ctx_t ctx;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ctx.dev = (rtlsdr_dev_t *)jdevHandle; // the device handle
  ctx.env = env; // the current JNIEnv interface pointer
  ctx.jcb = jcb; // the Java callback instance that implements RtlCallback

  ret = rtlsdr_read_async((rtlsdr_dev_t *)jdevHandle,
      AsyncCallback, &ctx, (uint32_t)numBuffers, (uint32_t)bufferLen);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_read_async returned %i", ret);

}

JNIEXPORT void JNICALL Java_de_rtlsdr_RtlDevice_cancelAsync(
    JNIEnv *env, jobject obj) {

  int ret;
  jlong jdevHandle;

  jdevHandle = getDeviceHandle(env, obj);
  if((*env)->ExceptionCheck(env) == JNI_TRUE) return;

  ret = rtlsdr_cancel_async((rtlsdr_dev_t *)jdevHandle);
  if(ret != 0) throwNewRtlException(env, "rtlsdr_cancel_async returned %i", ret);

}
