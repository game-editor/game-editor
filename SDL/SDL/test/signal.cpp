#include<libc\signal.h>
#include<e32std.h>

_sig_func_ptr signal(int /*aCode*/, _sig_func_ptr)
    {
    //User::Leave(aCode);
    return 0;
    }
int raise(int) {
    return 0;
    }