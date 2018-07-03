#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <log/log.h>

#undef LOG_TAG
#define LOG_TAG "HOOK-LIB"

// gettid SYSCALL
static pid_t __gettid(void)
{
	return syscall(__NR_gettid);
}


typedef FILE *(*fopen_type)(const char* filepath, const char *mode);

FILE *fopen(const char* filepath, const char* mode)
{
	// Return NULL when detect one of the parameters is NULL
	if (!filepath || !mode)
	{
		ALOGV("Invalid parameters for %s", __func__);
		return NULL;
	}
	// Get handle
#if defined(__LP64__)
	void *handle = dlopen("/system/lib64/libc.so", RTLD_LAZY);
#else
	void *handle = dlopen("/system/lib/libc.so", RTLD_LAZY);
#endif /* __LP64__ */
	if (!handle)
	{
		ALOGV("Failed to get handle!");
		ALOGV("Error: %s", dlerror());
		return NULL;
	}
	// Get real fopen pointer
	fopen_type real_fopen = (fopen_type) dlsym(handle, "fopen");
	if (!real_fopen)
	{
		ALOGV("Failed to get fopen pointer!");
		ALOGV("Error: %s", dlerror());
		return NULL;
	}
	// Get some info about the call
	ALOGV("Fopen path: %s,mode %s,caller %lu", filepath, mode, __gettid());
	return real_fopen(filepath, mode);
}
