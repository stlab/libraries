#ifndef STLAB_SET_CURRENT_THREAD_NAME_HPP
#define STLAB_SET_CURRENT_THREAD_NAME_HPP

/**************************************************************************************************/
#if defined(__APPLE__)

#include <pthread.h>

/**************************************************************************************************/
#elif defined(_MSC_VER)

#include <cstring>
#include <string>

#include <processthreadsapi.h>
#include <stringapiset.h>

/**************************************************************************************************/
#elif defined(__EMSCRIPTEN__) && defined(__EMSCRIPTEN_PTHREADS__)

#include <pthread.h>

#include <emscripten/threading.h>

/**************************************************************************************************/
#elif defined(__has_include) && __has_include(<pthread.h>)

#include <pthread.h>

/**************************************************************************************************/
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/
#if defined(__APPLE__)

void set_current_thread_name(const char* name) { pthread_setname_np(name); }

/**************************************************************************************************/
#elif defined(_MSC_VER)

void set_current_thread_name(const char* name) {
    /* Should string->wstring be split out to a utility? */
    int count = MultiByteToWideChar(CP_UTF8, 0, name, (int)std::strlen(name), NULL, 0);
    if (count <= 0) return;
    std::wstring str(count, std::wchar_t{});
    int count = MultiByteToWideChar(CP_UTF8, 0, name, (int)std::strlen(name), &str[0], str.size());
    if (count <= 0) return;

    (void)SetThreadDescription(GetCurrentThread(), str.c_str());
}

/**************************************************************************************************/
#elif defined(__EMSCRIPTEN__) && defined(__EMSCRIPTEN_PTHREADS__)

void set_current_thread_name(const char* name) { emscripten_set_thread_name(pthread_self(), name); }

/**************************************************************************************************/
#elif defined(__has_include) && __has_include(<pthread.h>)

void set_current_thread_name(const char* name) { pthread_setname_np(pthread_self(), name); }

/**************************************************************************************************/
#else

void set_current_thread_name(const char*) {}

/**************************************************************************************************/
#endif

} // namespace stlab

#endif
