//LLVM

#ifdef WIN32

#ifdef _DEBUG

#pragma comment(lib, "../../llvm/win32/bin/win32/debug/bitcode.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/debug/support.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/debug/system.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/debug/vmcore.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/debug/target.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clanganalysis.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangast.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangsema.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangcodegen.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangbasic.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangparse.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clangrewrite.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/debug/clanglex.lib")

#else

#pragma comment(lib, "../../llvm/win32/bin/win32/release/bitcode.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/release/support.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/release/system.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/release/vmcore.lib")
#pragma comment(lib, "../../llvm/win32/bin/win32/release/target.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clanganalysis.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangast.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangsema.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangcodegen.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangbasic.lib")
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangparse.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clangrewrite.lib") 
#pragma comment(lib, "../../llvm/tools/clang/win32/release/clanglex.lib")

#endif

#endif