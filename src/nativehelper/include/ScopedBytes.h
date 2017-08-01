/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCOPED_BYTES_H_included
#define SCOPED_BYTES_H_included

#include "Common.h"
#include "ScopedLocalRef.h"

jclass byteArrayClass;

static jclass findClass(JNIEnv *env, const char *name) {
    ScopedLocalRef<jclass> localClass(env, env->FindClass(name));
    jclass result = reinterpret_cast<jclass>(env->NewGlobalRef(localClass.get()));
    if (result == NULL) {
        INNER_LOG("failed to find class '%s'", name);
        abort();
    }
    return result;
}

static bool initByteArrayClass(JNIEnv *env) {
    if (byteArrayClass == NULL) {
        byteArrayClass = findClass(env, "[B");
    }
    return true;
}

/**
 * ScopedBytesRO and ScopedBytesRW attempt to paper over the differences between byte[]s and
 * ByteBuffers. This in turn helps paper over the differences between non-direct ByteBuffers backed
 * by byte[]s, direct ByteBuffers backed by bytes[]s, and direct ByteBuffers not backed by byte[]s.
 * (On Android, this last group only contains MappedByteBuffers.)
 */
template<bool readOnly>
class ScopedBytes {
public:
    ScopedBytes(JNIEnv *env, jobject object)
            : mEnv(env), mObject(object), mByteArray(NULL), mPtr(NULL) {
        if (mObject == NULL) {
            jniThrowNullPointerException(mEnv, NULL);
        } else if (initByteArrayClass(env) &&
                   mEnv->IsInstanceOf(mObject, byteArrayClass)) {
            mByteArray = reinterpret_cast<jbyteArray>(mObject);
            mPtr = mEnv->GetByteArrayElements(mByteArray, NULL);
            mSize = mEnv->GetArrayLength(mByteArray);
        } else {
            mPtr = reinterpret_cast<jbyte *>(mEnv->GetDirectBufferAddress(mObject));
            mSize = mEnv->GetDirectBufferCapacity(mObject);
        }
    }

    ~ScopedBytes() {
        if (mByteArray != NULL) {
            mEnv->ReleaseByteArrayElements(mByteArray, mPtr, readOnly ? JNI_ABORT : 0);
        }

    }

    size_t size() const {
        return mSize;
    }

private:
    JNIEnv *const mEnv;
    const jobject mObject;
    jbyteArray mByteArray;
    size_t mSize;

protected:
    jbyte *mPtr;

private:
    DISALLOW_COPY_AND_ASSIGN(ScopedBytes);
};

class ScopedBytesRO : public ScopedBytes<true> {
public:
    ScopedBytesRO(JNIEnv *env, jobject object) : ScopedBytes<true>(env, object) {}

    const jbyte *get() const {
        return mPtr;
    }
};

class ScopedBytesRW : public ScopedBytes<false> {
public:
    ScopedBytesRW(JNIEnv *env, jobject object) : ScopedBytes<false>(env, object) {}

    jbyte *get() {
        return mPtr;
    }


};

#endif  // SCOPED_BYTES_H_included
