#include <pthread.h>
#include <unistd.h>

// #define DEMO_DEBUG 1

template <class T>
class Singleton {
private:
  static T* mInstance;
  static T* mDebugInstance;
  static pthread_mutex_t mMutex;
  Singleton(const Singleton& arg1) {};
  Singleton& operator=(const Singleton& arg1) {};

protected:
  Singleton() {};

public:
  static T* getInstance();
#ifdef DEMO_DEBUG
  static T* debugGetInstance(int waitTime);
#endif
};

template<class T>
T* Singleton<T>::getInstance() {
  if (mInstance == nullptr) {
    pthread_mutex_lock(&mMutex);
    if (mInstance == nullptr) {
      T* tmp = new T();
      mInstance = tmp;
    }
    pthread_mutex_unlock(&mMutex);
  }
  return mInstance;
};

#ifdef DEMO_DEBUG
template<class T>
T* Singleton<T>::debugGetInstance(int waitTime) {
  if (mDebugInstance == nullptr) {
    sleep(waitTime);
    T* tmp = new T();
    mDebugInstance = tmp;
  }
  return mDebugInstance;
};
#endif

template<class T>
T* Singleton<T>::mInstance = nullptr;

#ifdef DEMO_DEBUG
template<class T>
T* Singleton<T>::mDebugInstance = nullptr;
#endif

template<class T>
pthread_mutex_t Singleton<T>::mMutex = PTHREAD_MUTEX_INITIALIZER;