#include <jni.h>
#include <windows.h>

typedef void (WINAPI *EVERYTHING_SET_SEARCHA)(LPCSTR lpString);
typedef BOOL (WINAPI *EVERYTHING_QUERYA)(BOOL bWait);
typedef DWORD (WINAPI *EVERYTHING_GET_NUM_RESULTS)(void);
typedef LPCSTR (WINAPI *EVERYTHING_GET_RESULT_FILENAMEA)(DWORD dwIndex);
typedef LPCSTR (WINAPI *EVERYTHING_GET_RESULT_PATHA)(DWORD dwIndex);
typedef void (WINAPI *EVERYTHING_SET_MATCH_PATH)(BOOL bEnable);
typedef void (WINAPI *EVERYTHING_SET_MATCH_CASE)(BOOL bEnable);
typedef void (WINAPI *EVERYTHING_SET_MATCH_WHOLE_WORD)(BOOL bEnable);
typedef void (WINAPI *EVERYTHING_SET_REGEX)(BOOL bEnable);
typedef void (WINAPI *EVERYTHING_RESET)(void);
typedef void (WINAPI *EVERYTHING_CLEANUP)(void);
typedef LPWSTR (WINAPI *EVERYTHING_GET_RESULT_FILENAMEW)(DWORD dwIndex);
typedef LPWSTR (WINAPI *EVERYTHING_GET_RESULT_PATHW)(DWORD dwIndex);
typedef void (WINAPI *EVERYTHING_SET_SEARCHW)(LPCWSTR lpString);
typedef DWORD (WINAPI *EVERYTHING_GET_LAST_ERROR)(void);
typedef DWORD (WINAPI *EVERYTHING_GET_RESULT_FULL_PATHNAMEW)(DWORD dwIndex, LPWSTR wbuf, DWORD wbuf_size);

static EVERYTHING_SET_SEARCHA Everything_SetSearchA;
static EVERYTHING_QUERYA Everything_QueryA;
static EVERYTHING_GET_NUM_RESULTS Everything_GetNumResults;
static EVERYTHING_GET_RESULT_FILENAMEA Everything_GetResultFileNameA;
static EVERYTHING_GET_RESULT_PATHA Everything_GetResultPathA;
static EVERYTHING_SET_MATCH_PATH Everything_SetMatchPath;
static EVERYTHING_SET_MATCH_CASE Everything_SetMatchCase;
static EVERYTHING_SET_MATCH_WHOLE_WORD Everything_SetMatchWholeWord;
static EVERYTHING_SET_REGEX Everything_SetRegex;
static EVERYTHING_RESET Everything_Reset;
static EVERYTHING_CLEANUP Everything_CleanUp;
static EVERYTHING_GET_RESULT_FILENAMEW Everything_GetResultFileNameW;
static EVERYTHING_GET_RESULT_PATHW Everything_GetResultPathW;
static EVERYTHING_SET_SEARCHW Everything_SetSearchW;
static EVERYTHING_GET_LAST_ERROR Everything_GetLastError;
static EVERYTHING_GET_RESULT_FULL_PATHNAMEW Everything_GetResultFullPathNameW;

#define MAX_PATH_LENGTH 32768
static WCHAR fileNameBuffer[MAX_PATH_LENGTH];
static WCHAR pathBuffer[MAX_PATH_LENGTH];

static BOOL loadEverythingDLL() {
    HMODULE hModule = LoadLibraryA("Everything64.dll");
    if (!hModule) {
        return FALSE;
    }

    Everything_GetResultFullPathNameW = (EVERYTHING_GET_RESULT_FULL_PATHNAMEW)GetProcAddress(hModule, "Everything_GetResultFullPathNameW");
    Everything_GetLastError = (EVERYTHING_GET_LAST_ERROR)GetProcAddress(hModule, "Everything_GetLastError");
    
    Everything_SetSearchA = (EVERYTHING_SET_SEARCHA)GetProcAddress(hModule, "Everything_SetSearchA");
    Everything_QueryA = (EVERYTHING_QUERYA)GetProcAddress(hModule, "Everything_QueryA");
    Everything_GetNumResults = (EVERYTHING_GET_NUM_RESULTS)GetProcAddress(hModule, "Everything_GetNumResults");
    Everything_GetResultFileNameA = (EVERYTHING_GET_RESULT_FILENAMEA)GetProcAddress(hModule, "Everything_GetResultFileNameA");
    Everything_GetResultPathA = (EVERYTHING_GET_RESULT_PATHA)GetProcAddress(hModule, "Everything_GetResultPathA");
    Everything_SetMatchPath = (EVERYTHING_SET_MATCH_PATH)GetProcAddress(hModule, "Everything_SetMatchPath");
    Everything_SetMatchCase = (EVERYTHING_SET_MATCH_CASE)GetProcAddress(hModule, "Everything_SetMatchCase");
    Everything_SetMatchWholeWord = (EVERYTHING_SET_MATCH_WHOLE_WORD)GetProcAddress(hModule, "Everything_SetMatchWholeWord");
    Everything_SetRegex = (EVERYTHING_SET_REGEX)GetProcAddress(hModule, "Everything_SetRegex");
    Everything_Reset = (EVERYTHING_RESET)GetProcAddress(hModule, "Everything_Reset");
    Everything_CleanUp = (EVERYTHING_CLEANUP)GetProcAddress(hModule, "Everything_CleanUp");
    Everything_GetResultFileNameW = (EVERYTHING_GET_RESULT_FILENAMEW)GetProcAddress(hModule, "Everything_GetResultFileNameW");
    Everything_GetResultPathW = (EVERYTHING_GET_RESULT_PATHW)GetProcAddress(hModule, "Everything_GetResultPathW");
    Everything_SetSearchW = (EVERYTHING_SET_SEARCHW)GetProcAddress(hModule, "Everything_SetSearchW");

    return (Everything_SetSearchW && Everything_QueryA && Everything_GetNumResults && 
            Everything_GetResultFileNameW && Everything_GetResultPathW && Everything_SetMatchPath &&
            Everything_SetMatchCase && Everything_SetMatchWholeWord && Everything_SetRegex &&
            Everything_Reset && Everything_CleanUp && Everything_GetLastError &&
            Everything_GetResultFullPathNameW);
}

static void setSearch(JNIEnv* env, jobject thiz, jstring searchString) {
    Everything_Reset();
    
    const jchar* str = (*env)->GetStringChars(env, searchString, NULL);
    Everything_SetSearchW((LPCWSTR)str);
    (*env)->ReleaseStringChars(env, searchString, str);
}

static void setMatchPath(JNIEnv* env, jobject thiz, jboolean enable) {
    Everything_SetMatchPath((BOOL)enable);
}

static void setMatchCase(JNIEnv* env, jobject thiz, jboolean enable) {
    Everything_SetMatchCase((BOOL)enable);
}

static void setMatchWholeWord(JNIEnv* env, jobject thiz, jboolean enable) {
    Everything_SetMatchWholeWord((BOOL)enable);
}

static void setRegex(JNIEnv* env, jobject thiz, jboolean enable) {
    Everything_SetRegex((BOOL)enable);
}

static jboolean query(JNIEnv* env, jobject thiz) {
    BOOL result = Everything_QueryA(TRUE);
    DWORD numResults = Everything_GetNumResults();
    DWORD error = Everything_GetLastError();
           
    return (jboolean)result;
}

static jint getNumResults(JNIEnv* env, jobject thiz) {
    DWORD results = Everything_GetNumResults();
    DWORD error = Everything_GetLastError();
    
    return (jint)results;
}

static jstring getResultFileName(JNIEnv* env, jobject thiz, jint index) {
    DWORD numResults = Everything_GetNumResults();
    if ((DWORD)index >= numResults) {
        return (*env)->NewStringUTF(env, "");
    }

    DWORD result = Everything_GetResultFullPathNameW((DWORD)index, fileNameBuffer, MAX_PATH_LENGTH);
    DWORD error = Everything_GetLastError();
    
    if (result == 0) {
        return (*env)->NewStringUTF(env, "");
    }
    
    LPWSTR fileName = wcsrchr(fileNameBuffer, L'\\');
    if (fileName) {
        fileName++;
    } else {
        fileName = fileNameBuffer;
    }
    
    return (*env)->NewString(env, (const jchar*)fileName, (jsize)wcslen(fileName));
}

static jstring getResultPath(JNIEnv* env, jobject thiz, jint index) {
    DWORD numResults = Everything_GetNumResults();
    if ((DWORD)index >= numResults) {
        return (*env)->NewStringUTF(env, "");
    }

    DWORD result = Everything_GetResultFullPathNameW((DWORD)index, pathBuffer, MAX_PATH_LENGTH);
    DWORD error = Everything_GetLastError();
    
    if (result == 0) {
        return (*env)->NewStringUTF(env, "");
    }
    
    LPWSTR lastBackslash = wcsrchr(pathBuffer, L'\\');
    if (lastBackslash) {
        *lastBackslash = L'\0';
    }
    
    return (*env)->NewString(env, (const jchar*)pathBuffer, (jsize)wcslen(pathBuffer));
}

static void reset(JNIEnv* env, jobject thiz) {
    Everything_Reset();
}

static void cleanup(JNIEnv* env, jobject thiz) {
    Everything_CleanUp();
}

static JNINativeMethod methods[] = {
    {"setSearch", "(Ljava/lang/String;)V", (void*)setSearch},
    {"setMatchPath", "(Z)V", (void*)setMatchPath},
    {"setMatchCase", "(Z)V", (void*)setMatchCase},
    {"setMatchWholeWord", "(Z)V", (void*)setMatchWholeWord},
    {"setRegex", "(Z)V", (void*)setRegex},
    {"query", "()Z", (void*)query},
    {"getNumResults", "()I", (void*)getNumResults},
    {"getResultFileName", "(I)Ljava/lang/String;", (void*)getResultFileName},
    {"getResultPath", "(I)Ljava/lang/String;", (void*)getResultPath},
    {"reset", "()V", (void*)reset},
    {"cleanup", "()V", (void*)cleanup}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    if (!loadEverythingDLL()) {
        return JNI_ERR;
    }
    jclass clazz = (*env)->FindClass(env, "us/cubk/jeverything/Everything");
    if (!clazz) {
        return JNI_ERR;
    }
    if ((*env)->RegisterNatives(env, clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    Everything_CleanUp();
}