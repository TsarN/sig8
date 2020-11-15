/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef SIG8_H
#define SIG8_H

#ifdef __cplusplus
    #define API extern "C"
#else
    #include <stdbool.h>
    #define API
#endif


//! A private function that performs various initialization tasks.
//! The function's argument is a bitmask of the following flags:
//!
//! - 0x01 - perform initialization, if 0 then only assert that
//!          not already initialized
//! - 0x02 - install atexit handler to call Deinitialize()
//! - 0x04 - initialize [window]
API void sig8_Initialize(unsigned);

API int sig8_DefaultMain(void (*setup)(void), void (*loop)(void), int argc, char **argv);

//
// Subsystem: [system]
//

#if !defined(SIG8_BUILD_LIB) && !defined(SIG8_NO_INITIALIZE)

//! Initialize the library, must be the first function called.
static inline void Initialize(void)
{
    sig8_Initialize(0);
    sig8_Initialize(
        0x01u | // initialize
        0x02u | // install atexit handler
        0x04u   // initialize [window]
    );
}

#endif

//! Deinitialize the library, should be called automatically on <code>atexit</code>.
API void Deinitialize(void);

//! Run the game with provided update function.
API void Run(void (*update)(void));

#undef API
#endif
