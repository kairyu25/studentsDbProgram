/////////////////////////////////////////////////////////////////////////////
// Name:        wx/thread.h
// Purpose:     Thread API
// Author:      Guilhem Lavaux
// Modified by: Vadim Zeitlin (modifications partly inspired by omnithreads
//              package from Olivetti & Oracle Research Laboratory)
// Created:     04/13/98
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_THREAD_H_
#define _WX_THREAD_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// get the value of wxUSE_THREADS configuration flag
#include "wx/defs.h"

#if wxUSE_THREADS

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum wxMutexError
{
    wxMUTEX_NO_ERROR = 0,   // operation completed successfully
    wxMUTEX_INVALID,        // mutex hasn't been initialized
    wxMUTEX_DEAD_LOCK,      // mutex is already locked by the calling thread
    wxMUTEX_BUSY,           // mutex is already locked by another thread
    wxMUTEX_UNLOCKED,       // attempt to unlock a mutex which is not locked
    wxMUTEX_TIMEOUT,        // LockTimeout() has timed out
    wxMUTEX_MISC_ERROR      // any other error
};

enum wxCondError
{
    wxCOND_NO_ERROR = 0,
    wxCOND_INVALID,
    wxCOND_TIMEOUT,         // WaitTimeout() has timed out
    wxCOND_MISC_ERROR
};

enum wxSemaError
{
    wxSEMA_NO_ERROR = 0,
    wxSEMA_INVALID,         // semaphore hasn't been initialized successfully
    wxSEMA_BUSY,            // returned by TryWait() if Wait() would block
    wxSEMA_TIMEOUT,         // returned by WaitTimeout()
    wxSEMA_OVERFLOW,        // Post() would increase counter past the max
    wxSEMA_MISC_ERROR
};

enum wxThreadError
{
    wxTHREAD_NO_ERROR = 0,      // No error
    wxTHREAD_NO_RESOURCE,       // No resource left to create a new thread
    wxTHREAD_RUNNING,           // The thread is already running
    wxTHREAD_NOT_RUNNING,       // The thread isn't running
    wxTHREAD_KILLED,            // Thread we waited for had to be killed
    wxTHREAD_MISC_ERROR         // Some other error
};

enum wxThreadKind
{
    wxTHREAD_DETACHED,
    wxTHREAD_JOINABLE
};

enum wxThreadWait
{
    wxTHREAD_WAIT_BLOCK,
    wxTHREAD_WAIT_YIELD,       // process events while waiting; MSW only

    // For compatibility reasons we use wxTHREAD_WAIT_YIELD by default as this
    // was the default behaviour of wxMSW 2.8 but it should be avoided as it's
    // dangerous and not portable.
#if WXWIN_COMPATIBILITY_2_8
    wxTHREAD_WAIT_DEFAULT = wxTHREAD_WAIT_YIELD
#else
    wxTHREAD_WAIT_DEFAULT = wxTHREAD_WAIT_BLOCK
#endif
};

// Obsolete synonyms for wxPRIORITY_XXX for backwards compatibility-only
enum
{
    WXTHREAD_MIN_PRIORITY      = wxPRIORITY_MIN,
    WXTHREAD_DEFAULT_PRIORITY  = wxPRIORITY_DEFAULT,
    WXTHREAD_MAX_PRIORITY      = wxPRIORITY_MAX
};

// There are 2 types of mutexes: normal mutexes and recursive ones. The attempt
// to lock a normal mutex by a thread which already owns it results in
// undefined behaviour (it always works under Windows, it will almost always
// result in a deadlock under Unix). Locking a recursive mutex in such
// situation always succeeds and it must be unlocked as many times as it has
// been locked.
//
// However recursive mutexes have several important drawbacks: first, in the
// POSIX implementation, they're less efficient. Second, and more importantly,
// they CANNOT BE USED WITH CONDITION VARIABLES under Unix! Using them with
// wxCondition will work under Windows and some Unices (notably Linux) but will
// deadlock under other Unix versions (e.g. Solaris). As it might be difficult
// to ensure that a recursive mutex is not used with wxCondition, it is a good
// idea to avoid using recursive mutexes at all. Also, the last problem with
// them is that some (older) Unix versions don't support this at all -- which
// results in a configure warning when building and a deadlock when using them.
enum wxMutexType
{
    // normal mutex: try to always use this one
    wxMUTEX_DEFAULT,

    // recursive mutex: don't use these ones with wxCondition
    wxMUTEX_RECURSIVE
};

// forward declarations
class WXDLLIMPEXP_FWD_BASE wxThreadHelper;
class WXDLLIMPEXP_FWD_BASE wxConditionInternal;
class WXDLLIMPEXP_FWD_BASE wxMutexInternal;
class WXDLLIMPEXP_FWD_BASE wxSemaphoreInternal;
class WXDLLIMPEXP_FWD_BASE wxThreadInternal;

// ----------------------------------------------------------------------------
// A mutex object is a synchronization object whose state is set to signaled
// when it is not owned by any thread, and nonsignaled when it is owned. Its
// name comes from its usefulness in coordinating mutually-exclusive access to
// a shared resource. Only one thread at a time can own a mutex object.
// ----------------------------------------------------------------------------

// you should consider wxMutexLocker whenever possible instead of directly
// working with wxMutex class - it is safer
class WXDLLIMPEXP_BASE wxMutex
{
public:
    // constructor & destructor
    // ------------------------

    // create either default (always safe) or recursive mutex
    wxMutex(wxMutexType mutexType = wxMUTEX_DEFAULT);

    // destroys the mutex kernel object
    ~wxMutex();

    // test if the mutex has been created successfully
    bool IsOk() const;

    // mutex operations
    // ----------------

    // Lock the mutex, blocking on it until it is unlocked by the other thread.
    // The result of locking a mutex already locked by the current thread
    // depend on the mutex type.
    //
    // The caller must call Unlock() later if Lock() returned wxMUTEX_NO_ERROR.
    wxMutexError Lock();

    // Same as Lock() but return wxMUTEX_TIMEOUT if the mutex can't be locked
    // during the given number of milliseconds
    wxMutexError LockTimeout(unsigned long ms);

    // Try to lock the mutex: if it is currently locked, return immediately
    // with an error. Otherwise the caller must call Unlock().
    wxMutexError TryLock();

    // Unlock the mutex. It is an error to unlock an already unlocked mutex
    wxMutexError Unlock();

protected:
    wxMutexInternal *m_internal;

    friend class wxConditionInternal;

    wxDECLARE_NO_COPY_CLASS(wxMutex);
};

// a helper class which locks the mutex in the ctor and unlocks it in the dtor:
// this ensures that mutex is always unlocked, even if the function returns or
// throws an exception before it reaches the end
class WXDLLIMPEXP_BASE wxMutexLocker
{
public:
    // lock the mutex in the ctor
    wxMutexLocker(wxMutex& mutex)
        : m_isOk(false), m_mutex(mutex)
        { m_isOk = ( m_mutex.Lock() == wxMUTEX_NO_ERROR ); }

    // returns true if mutex was successfully locked in ctor
    bool IsOk() const
        { return m_isOk; }

    // unlock the mutex in dtor
    ~wxMutexLocker()
        { if ( IsOk() ) m_mutex.Unlock(); }

private:
    // no assignment operator nor copy ctor
    wxMutexLocker(const wxMutexLocker&);
    wxMutexLocker& operator=(const wxMutexLocker&);

    bool     m_isOk;
    wxMutex& m_mutex;
};

// ----------------------------------------------------------------------------
// Critical section: this is the same as mutex but is only visible to the
// threads of the same process. For the platforms which don't have native
// support for critical sections, they're implemented entirely in terms of
// mutexes.
//
// NB: wxCriticalSection object does not allocate any memory in its ctor
//     which makes it possible to have static globals of this class
// ----------------------------------------------------------------------------

// in order to avoid any overhead under platforms where critical sections are
// just mutexes make all wxCriticalSection class functions inline
#if !defined(__WINDOWS__)
    #define wxCRITSECT_IS_MUTEX 1

    #define wxCRITSECT_INLINE WXEXPORT inline
#else // MSW
    #define wxCRITSECT_IS_MUTEX 0

    #define wxCRITSECT_INLINE
#endif // MSW/!MSW

enum wxCriticalSectionType
{
    // recursive critical section
    wxCRITSEC_DEFAULT,

    // non-recursive critical section
    wxCRITSEC_NON_RECURSIVE
};

// you should consider wxCriticalSectionLocker whenever possible instead of
// directly working with wxCriticalSection class - it is safer
class WXDLLIMPEXP_BASE wxCriticalSection
{
public:
    // ctor & dtor
    wxCRITSECT_INLINE wxCriticalSection( wxCriticalSectionType critSecType = wxCRITSEC_DEFAULT );
    wxCRITSECT_INLINE ~wxCriticalSection();
    // enter the section (the same as locking a mutex)
    wxCRITSECT_INLINE void Enter();

    // try to enter the section (the same as trying to lock a mutex)
    wxCRITSECT_INLINE bool TryEnter();

    // leave the critical section (same as unlocking a mutex)
    wxCRITSECT_INLINE void Leave();

private:
#if wxCRITSECT_IS_MUTEX
    wxMutex m_mutex;
#elif defined(__WINDOWS__)
    // we can't allocate any memory in the ctor, so use placement new -
    // unfortunately, we have to hardcode the sizeof() here because we can't
    // include windows.h from this public header and we also have to use the
    // union to force the correct (i.e. maximal) alignment
    //
    // if CRITICAL_SECTION size changes in Windows, you'll get an assert from
    // thread.cpp and will need to increase the buffer size
#ifdef __WIN64__
    typedef char wxCritSectBuffer[40];
#else // __WIN32__
    typedef char wxCritSectBuffer[24];
#endif
    union
    {
        unsigned long m_dummy1;
        void *m_dummy2;

        wxCritSectBuffer m_buffer;
    };
#endif // Unix/Win32

    wxDECLARE_NO_COPY_CLASS(wxCriticalSection);
};

#if wxCRITSECT_IS_MUTEX
    // implement wxCriticalSection using mutexes
    inline wxCriticalSection::wxCriticalSection( wxCriticalSectionType critSecType )
       : m_mutex( critSecType == wxCRITSEC_DEFAULT ? wxMUTEX_RECURSIVE : wxMUTEX_DEFAULT )  { }
    inline wxCriticalSection::~wxCriticalSection() { }

    inline void wxCriticalSection::Enter() { (void)m_mutex.Lock(); }
    inline bool wxCriticalSection::TryEnter() { return m_mutex.TryLock() == wxMUTEX_NO_ERROR; }
    inline void wxCriticalSection::Leave() { (void)m_mutex.Unlock(); }
#endif // wxCRITSECT_IS_MUTEX

#undef wxCRITSECT_INLINE
#undef wxCRITSECT_IS_MUTEX

// wxCriticalSectionLocker is the same to critical sections as wxMutexLocker is
// to mutexes
class WXDLLIMPEXP_BASE wxCriticalSectionLocker
{
public:
    wxCriticalSectionLocker(wxCriticalSection& cs)
        : m_critsect(cs)
    {
        m_critsect.Enter();
    }

    ~wxCriticalSectionLocker()
    {
        m_critsect.Leave();
    }

private:
    wxCriticalSection& m_critsect;

    wxDECLARE_NO_COPY_CLASS(wxCriticalSectionLocker);
};

// ----------------------------------------------------------------------------
// wxCondition models a POSIX condition variable which allows one (or more)
// thread(s) to wait until some condition is fulfilled
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxCondition
{
public:
    // Each wxCondition object is associated with a (single) wxMutex object.
    // The mutex object MUST be locked before calling Wait()
    wxCondition(wxMutex& mutex);

    // dtor is not virtual, don't use this class polymorphically
    ~wxCondition();

    // return true if the condition has been created successfully
    bool IsOk() const;

    // NB: the associated mutex MUST be locked beforehand by the calling thread
    //
    // it atomically releases the lock on the associated mutex
    // and starts waiting to be woken up by a Signal()/Broadcast()
    // once its signaled, then it will wait until it can reacquire
    // the lock on the associated mutex object, before returning.
    wxCondError Wait();

    // std::condition_variable-like variant that evaluates the associated condition
    template<typename Functor>
    wxCondError Wait(const Functor& predicate)
    {
        while ( !predicate() )
        {
            wxCondError e = Wait();
            if ( e != wxCOND_NO_ERROR )
                return e;
        }
        return wxCOND_NO_ERROR;
    }

    // exactly as Wait() except that it may also return if the specified
    // timeout elapses even if the condition hasn't been signalled: in this
    // case, the return value is wxCOND_TIMEOUT, otherwise (i.e. in case of a
    // normal return) it is wxCOND_NO_ERROR.
    //
    // the timeout parameter specifies an interval that needs to be waited for
    // in milliseconds
    wxCondError WaitTimeout(unsigned long milliseconds);

    // NB: the associated mutex may or may not be locked by the calling thread
    //
    // this method unblocks one thread if any are blocking on the condition.
    // if no thread is blocking in Wait(), then the signal is NOT remembered
    // The thread which was blocking on Wait() will then reacquire the lock
    // on the associated mutex object before returning
    wxCondError Signal();

    // NB: the associated mutex may or may not be locked by the calling thread
    //
    // this method unblocks all threads if any are blocking on the condition.
    // if no thread is blocking in Wait(), then the signal is NOT remembered
    // The threads which were blocking on Wait() will then reacquire the lock
    // on the associated mutex object before returning.
    wxCondError Broadcast();

private:
    wxConditionInternal *m_internal;

    wxDECLARE_NO_COPY_CLASS(wxCondition);
};

// ----------------------------------------------------------------------------
// wxSemaphore: a counter limiting the number of threads concurrently accessing
//              a shared resource
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxSemaphore
{
public:
    // specifying a maxcount of 0 actually makes wxSemaphore behave as if there
    // is no upper limit, if maxcount is 1 the semaphore behaves as a mutex
    wxSemaphore( int initialcount = 0, int maxcount = 0 );

    // dtor is not virtual, don't use this class polymorphically
    ~wxSemaphore();

    // return true if the semaphore has been created successfully
    bool IsOk() const;

    // wait indefinitely, until the semaphore count goes beyond 0
    // and then decrement it and return (this method might have been called
    // Acquire())
    wxSemaError Wait();

    // same as Wait(), but does not block, returns wxSEMA_NO_ERROR if
    // successful and wxSEMA_BUSY if the count is currently zero
    wxSemaError TryWait();

    // same as Wait(), but as a timeout limit, returns wxSEMA_NO_ERROR if the
    // semaphore was acquired and wxSEMA_TIMEOUT if the timeout has elapsed
    wxSemaError WaitTimeout(unsigned long milliseconds);

    // increments the semaphore count and signals one of the waiting threads
    wxSemaError Post();

private:
    wxSemaphoreInternal *m_internal;

    wxDECLARE_NO_COPY_CLASS(wxSemaphore);
};

// ----------------------------------------------------------------------------
// wxThread: class encapsulating a thread of execution
// ----------------------------------------------------------------------------

// there are two different kinds of threads: joinable and detached (default)
// ones. Only joinable threads can return a return code and only detached
// threads auto-delete themselves - the user should delete the joinable
// threads manually.

// NB: in the function descriptions the words "this thread" mean the thread
//     created by the wxThread object while "main thread" is the thread created
//     during the process initialization (a.k.a. the GUI thread)

// On VMS thread pointers are 64 bits (also needed for other systems???
#ifdef __VMS
   typedef unsigned long long wxThreadIdType;
#else
   typedef unsigned long wxThreadIdType;
#endif

class WXDLLIMPEXP_BASE wxThread
{
public:
    // the return type for the thread function
    typedef void *ExitCode;

    // static functions
        // Returns the wxThread object for the calling thread. NULL is returned
        // if the caller is the main thread (but it's recommended to use
        // IsMain() and only call This() for threads other than the main one
        // because NULL is also returned on error). If the thread wasn't
        // created with wxThread class, the returned value is undefined.
    static wxThread *This();

        // Returns true if current thread is the main thread.
        //
        // Notice that it also returns true if main thread id hadn't been
        // initialized yet on the assumption that it's too early in wx startup
        // process for any other threads to have been created in this case.
    static bool IsMain()
    {
        return !ms_idMainThread || GetCurrentId() == ms_idMainThread;
    }

        // Return the main thread id
    static wxThreadIdType GetMainId() { return ms_idMainThread; }

        // Release the rest of our time slice letting the other threads run
    static void Yield();

        // Sleep during the specified period of time in milliseconds
        //
        // This is the same as wxMilliSleep().
    static void Sleep(unsigned long milliseconds);

        // get the number of system CPUs - useful with SetConcurrency()
        // (the "best" value for it is usually number of CPUs + 1)
        //
        // Returns -1 if unknown, number of CPUs otherwise
    static int GetCPUCount();

        // Get the platform specific thread ID and return as a long.  This
        // can be used to uniquely identify threads, even if they are not
        // wxThreads.  This is used by wxPython.
    static wxThreadIdType GetCurrentId();

        // sets the concurrency level: this is, roughly, the number of threads
        // the system tries to schedule to run in parallel. 0 means the
        // default value (usually acceptable, but may not yield the best
        // performance for this process)
        //
        // Returns true on success, false otherwise (if not implemented, for
        // example)
    static bool SetConcurrency(size_t level);

    // constructor only creates the C++ thread object and doesn't create (or
    // start) the real thread
    wxThread(wxThreadKind kind = wxTHREAD_DETACHED);

    // functions that change the thread state: all these can only be called
    // from _another_ thread (typically the thread that created this one, e.g.
    // the main thread), not from the thread itself

        // create a new thread and optionally set the stack size on
        // platforms that support that - call Run() to start it
    wxThreadError Create(unsigned int stackSize = 0);

        // starts execution of the thread - from the moment Run() is called
        // the execution of wxThread::Entry() may start at any moment, caller
        // shouldn't suppose that it starts after (or before) Run() returns.
    wxThreadError Run();

        // stops the thread if it's running and deletes the wxThread object if
        // this is a detached thread freeing its memory - otherwise (for
        // joinable threads) you still need to delete wxThread object
        // yourself.
        //
        // this function only works if the thread calls TestDestroy()
        // periodically - the thread will only be deleted the next time it
        // does it!
        //
        // will fill the rc pointer with the thread exit code if it's !NULL
    wxThreadError Delete(ExitCode *rc = NULL,
                         wxThreadWait waitMode = wxTHREAD_WAIT_DEFAULT);

        // waits for a joinable thread to finish and returns its exit code
        //
        // Returns (ExitCode)-1 on error (for example, if the thread is not
        // joinable)
    ExitCode Wait(wxThreadWait waitMode = wxTHREAD_WAIT_DEFAULT);

        // kills the thread without giving it any chance to clean up - should
        // not be used under normal circumstances, use Delete() instead.
        // It is a dangerous function that should only be used in the most
        // extreme cases!
        //
        // The wxThread object is deleted by Kill() if the thread is
        // detachable, but you still have to delete it manually for joinable
        // threads.
    wxThreadError Kill();

        // pause a running thread: as Delete(), this only works if the thread
        // calls TestDestroy() regularly
    wxThreadError Pause();

        // resume a paused thread
    wxThreadError Resume();

    // priority
        // Sets the priority to "prio" which must be in 0..100 range (see
        // also wxPRIORITY_XXX constants).
    void SetPriority(unsigned int prio);

        // Get the current priority.
    unsigned int GetPriority() const;

    // thread status inquiries
        // Returns true if the thread is alive: i.e. running or suspended
    bool IsAlive() const;
        // Returns true if the thread is running (not paused, not killed).
    bool IsRunning() const;
        // Returns true if the thread is suspended
    bool IsPaused() const;

        // is the thread of detached kind?
    bool IsDetached() const { return m_isDetached; }

    // Get the thread ID - a platform dependent number which uniquely
    // identifies a thread inside a process
    wxThreadIdType GetStudentId() const;

#ifdef __WINDOWS__
    // Get the internal OS handle
    WXHANDLE MSWGetHandle() const;
#endif // __WINDOWS__

    wxThreadKind GetKind() const
        { return m_isDetached ? wxTHREAD_DETACHED : wxTHREAD_JOINABLE; }

    // Returns true if the thread was asked to terminate: this function should
    // be called by the thread from time to time, otherwise the main thread
    // will be left forever in Delete()!
    virtual bool TestDestroy();

    // dtor is public, but the detached threads should never be deleted - use
    // Delete() instead (or leave the thread terminate by itself)
    virtual ~wxThread();

    // sets name to assist debugging
    static bool SetNameForCurrent(const wxString &name);

protected:
    // sets name to assist debugging
    bool SetName(const wxString &name);

    // exits from the current thread - can be called only from this thread
    void Exit(ExitCode exitcode = NULL);

    // entry point for the thread - called by Run() and executes in the context
    // of this thread.
    virtual void *Entry() = 0;

    // obsolete private function calling Entry(), do not use.
    void *CallEntry();

    // Callbacks which may be overridden by the derived class to perform some
    // specific actions when the thread is deleted or killed. By default they
    // do nothing.

    // This one is called by Delete() before actually deleting the thread and
    // is executed in the context of the thread that called Delete().
    virtual void OnDelete() {}

    // This one is called by Kill() before killing the thread and is executed
    // in the context of the thread that called Kill().
    virtual void OnKill() {}

    // called when the thread exits - in the context of this thread
    //
    // NB: this function will not be called if the thread is Kill()ed
    virtual void OnExit() {}

private:
    // no copy ctor/assignment operator
    wxThread(const wxThread&);
    wxThread& operator=(const wxThread&);

    friend class wxThreadInternal;
    friend class wxThreadModule;


    // the main thread identifier, should be set on startup
    static wxThreadIdType ms_idMainThread;

    // the (platform-dependent) thread class implementation
    wxThreadInternal *m_internal;

    // protects access to any methods of wxThreadInternal object
    mutable wxCriticalSection m_critsect;

    // true if the thread is detached, false if it is joinable
    bool m_isDetached;
};

// wxThreadHelperThread class
// --------------------------

class wxThreadHelperThread : public wxThread
{
public:
    // constructor only creates the C++ thread object and doesn't create (or
    // start) the real thread
    wxThreadHelperThread(wxThreadHelper& owner, wxThreadKind kind)
        : wxThread(kind), m_owner(owner)
        { }

protected:
    // entry point for the thread -- calls Entry() in owner.
    virtual void *Entry() wxOVERRIDE;

private:
    // the owner of the thread
    wxThreadHelper& m_owner;

    // no copy ctor/assignment operator
    wxThreadHelperThread(const wxThreadHelperThread&);
    wxThreadHelperThread& operator=(const wxThreadHelperThread&);
};

// ----------------------------------------------------------------------------
// wxThreadHelper: this class implements the threading logic to run a
// background task in another object (such as a window).  It is a mix-in: just
// derive from it to implement a threading background task in your class.
// ----------------------------------------------------------------------------

class wxThreadHelper
{
private:
    void KillThread()
    {
        // If wxThreadHelperThread is detached and is about to finish, it will
        // set m_thread to NULL so don't delete it then.
        // But if KillThread is called before wxThreadHelperThread (in detached mode)
        // sets it to NULL, then the thread object still exists and can be killed
        wxCriticalSectionLocker locker(m_critSection);

        if ( m_thread )
        {
            m_thread->Kill();

            if ( m_kind == wxTHREAD_JOINABLE )
              delete m_thread;

            m_thread = NULL;
        }
    }

public:
    // constructor only initializes m_thread to NULL
    wxThreadHelper(wxThreadKind kind = wxTHREAD_JOINABLE)
        : m_thread(NULL), m_kind(kind) { }

    // destructor deletes m_thread
    virtual ~wxThreadHelper() { KillThread(); }

#if WXWIN_COMPATIBILITY_2_8
    wxDEPRECATED( wxThreadError Create(unsigned int stackSize = 0) );
#endif

    // create a new thread (and optionally set the stack size on platforms that
    // support/need that), call Run() to start it
    wxThreadError CreateThread(wxThreadKind kind = wxTHREAD_JOINABLE,
                               unsigned int stackSize = 0)
    {
        KillThread();

        m_kind = kind;
        m_thread = new wxThreadHelperThread(*this, m_kind);

        return m_thread->Create(stackSize);
    }

    // entry point for the thread - called by Run() and executes in the context
    // of this thread.
    virtual void *Entry() = 0;

    // returns a pointer to the thread which can be used to call Run()
    wxThread *GetThread() const
    {
        wxCriticalSectionLocker locker(m_critSection);

        wxThread* thread = m_thread;

        return thread;
    }

protected:
    wxThread *m_thread;
    wxThreadKind m_kind;
    mutable wxCriticalSection m_critSection; // To guard the m_thread variable

    friend class wxThreadHelperThread;
};

#if WXWIN_COMPATIBILITY_2_8
inline wxThreadError wxThreadHelper::Create(unsigned int stackSize)
{ return CreateThread(m_kind, stackSize); }
#endif

// call Entry() in owner, put it down here to avoid circular declarations
inline void *wxThreadHelperThread::Entry()
{
    void * const result = m_owner.Entry();

    wxCriticalSectionLocker locker(m_owner.m_critSection);

    // Detached thread will be deleted after returning, so make sure
    // wxThreadHelper::GetThread will not return an invalid pointer.
    // And that wxThreadHelper::KillThread will not try to kill
    // an already deleted thread
    if ( m_owner.m_kind == wxTHREAD_DETACHED )
        m_owner.m_thread = NULL;

    return result;
}

// ----------------------------------------------------------------------------
// Automatic initialization
// ----------------------------------------------------------------------------

// GUI mutex handling.
void WXDLLIMPEXP_BASE wxMutexGuiEnter();
void WXDLLIMPEXP_BASE wxMutexGuiLeave();

// macros for entering/leaving critical sections which may be used without
// having to take them inside "#if wxUSE_THREADS"
#define wxENTER_CRIT_SECT(cs)   (cs).Enter()
#define wxLEAVE_CRIT_SECT(cs)   (cs).Leave()
#define wxCRIT_SECT_DECLARE(cs) static wxCriticalSection cs
#define wxCRIT_SECT_DECLARE_MEMBER(cs) wxCriticalSection cs
#define wxCRIT_SECT_LOCKER(name, cs)  wxCriticalSectionLocker name(cs)

// function for checking if we're in the main thread which may be used whether
// wxUSE_THREADS is 0 or 1
inline bool wxIsMainThread() { return wxThread::IsMain(); }

#else // !wxUSE_THREADS

// no thread support
inline void wxMutexGuiEnter() { }
inline void wxMutexGuiLeave() { }

// macros for entering/leaving critical sections which may be used without
// having to take them inside "#if wxUSE_THREADS"
// (the implementation uses dummy structs to force semicolon after the macro)
#define wxENTER_CRIT_SECT(cs)            do {} while (0)
#define wxLEAVE_CRIT_SECT(cs)            do {} while (0)
#define wxCRIT_SECT_DECLARE(cs)          struct wxDummyCS##cs
#define wxCRIT_SECT_DECLARE_MEMBER(cs)   struct wxDummyCSMember##cs { }
#define wxCRIT_SECT_LOCKER(name, cs)     struct wxDummyCSLocker##name

// if there is only one thread, it is always the main one
inline bool wxIsMainThread() { return true; }

#endif // wxUSE_THREADS/!wxUSE_THREADS

// mark part of code as being a critical section: this macro declares a
// critical section with the given name and enters it immediately and leaves
// it at the end of the current scope
//
// example:
//
//      int Count()
//      {
//          static int s_counter = 0;
//
//          wxCRITICAL_SECTION(counter);
//
//          return ++s_counter;
//      }
//
// this function is MT-safe in presence of the threads but there is no
// overhead when the library is compiled without threads
#define wxCRITICAL_SECTION(name) \
    wxCRIT_SECT_DECLARE(s_cs##name);  \
    wxCRIT_SECT_LOCKER(cs##name##Locker, s_cs##name)

// automatically lock GUI mutex in ctor and unlock it in dtor
class WXDLLIMPEXP_BASE wxMutexGuiLocker
{
public:
    wxMutexGuiLocker() { wxMutexGuiEnter(); }
   ~wxMutexGuiLocker() { wxMutexGuiLeave(); }
};

// -----------------------------------------------------------------------------
// implementation only until the end of file
// -----------------------------------------------------------------------------

#if wxUSE_THREADS

#if defined(__WINDOWS__) || defined(__DARWIN__)
    // unlock GUI if there are threads waiting for and lock it back when
    // there are no more of them - should be called periodically by the main
    // thread
    extern void WXDLLIMPEXP_BASE wxMutexGuiLeaveOrEnter();

    // returns true if the main thread has GUI lock
    extern bool WXDLLIMPEXP_BASE wxGuiOwnedByMainThread();

    // wakes up the main thread if it's sleeping inside ::GetMessage()
    extern void WXDLLIMPEXP_BASE wxWakeUpMainThread();

#ifndef __DARWIN__
    // return true if the main thread is waiting for some other to terminate:
    // wxApp then should block all "dangerous" messages
    extern bool WXDLLIMPEXP_BASE wxIsWaitingForThread();
#endif
#endif // MSW, OSX

#endif // wxUSE_THREADS

#endif // _WX_THREAD_H_
