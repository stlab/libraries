#ifndef STLAB_SET_CURRENT_THREAD_NAME_HPP
#define STLAB_SET_CURRENT_THREAD_NAME_HPP

#ifdef STLAB_WIN32_THREADS
  #include <cstring>
  #include <string>

  #include <windows.h>

  #include <processthreadsapi.h>
  #include <stringapiset.h>
#elif defined(STLAB_PTHREAD_THREADS)
  #include <pthread.h>
#elif defined(STLAB_PTHREAD_EMSCRIPTEN_THREADS)
  #include <pthread.h>
  #include <emscripten/threading.h>
#elif defined(STLAB_PTHREAD_APPLE_THREADS)
  #include <pthread.h>
#elif defined(STLAB_NO_THREADS)
  // Do nothing
#else
  #error "Unspecified or unknown thread mode set."
#endif

namespace stlab {

inline void set_current_thread_name(const char* name) {
#ifdef STLAB_WIN32_THREADS
    /* Should string->wstring be split out to a utility? */
    int count = MultiByteToWideChar(CP_UTF8, 0, name, static_cast<int>(std::strlen(name)), NULL, 0);
    if (count <= 0) return;
    std::wstring str(count, wchar_t{});
    count = MultiByteToWideChar(CP_UTF8, 0, name, static_cast<int>(std::strlen(name)), &str[0],
                                static_cast<int>(str.size()));
    if (count <= 0) return;

    (void)SetThreadDescription(GetCurrentThread(), str.c_str());
#elif defined(STLAB_PTHREAD_EMSCRIPTEN_THREADS)
    emscripten_set_thread_name(pthread_self(), name);
#elif defined(STLAB_PTHREAD_APPLE_THREADS)
    pthread_setname_np(name);
#elif defined(STLAB_PTHREAD_THREADS)
    pthread_setname_np(pthread_self(), name);
#elif defined(STLAB_NO_THREADS)
    // Nothing
#else
    #error "Unspecified or unknown thread mode set."
#endif
}

} // namespace stlab

#endif
