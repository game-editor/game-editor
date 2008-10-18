#include "llvm.h"

#include "clang/AST/TranslationUnit.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/FileManager.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "llvm/Module.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/Streams.h"
#include "llvm/Support/Timer.h"
#include "llvm/ADT/OwningPtr.h"
#include "clang/AST/CFG.h"
#include "clang/Analysis/Analyses/LiveVariables.h"
#include "clang/Analysis/LocalCheckers.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Streams.h"
#include "llvm/config/config.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "clang/AST/DeclObjC.h"
#include "clang/Sema/ParseAST.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace clang;

//===----------------------------------------------------------------------===//
/// DeclPrinter - Utility class for printing top-level decls.


namespace {
  class DeclPrinter {
  public:
    llvm::raw_ostream& Out;

    DeclPrinter(llvm::raw_ostream* out) : Out(out ? *out : llvm::errs()) {}
    DeclPrinter() : Out(llvm::errs()) {}
    virtual ~DeclPrinter();
    
    void PrintDecl(Decl *D);
    void PrintFunctionDeclStart(FunctionDecl *FD);    
    void PrintTypeDefDecl(TypedefDecl *TD);    
    void PrintLinkageSpec(LinkageSpecDecl *LS);
    void PrintObjCMethodDecl(ObjCMethodDecl *OMD);    
    void PrintObjCImplementationDecl(ObjCImplementationDecl *OID);
    void PrintObjCInterfaceDecl(ObjCInterfaceDecl *OID);
    void PrintObjCProtocolDecl(ObjCProtocolDecl *PID);  
    void PrintObjCCategoryImplDecl(ObjCCategoryImplDecl *PID);    
    void PrintObjCCategoryDecl(ObjCCategoryDecl *PID);    
    void PrintObjCCompatibleAliasDecl(ObjCCompatibleAliasDecl *AID);
    void PrintObjCPropertyDecl(ObjCPropertyDecl *PD);
    void PrintObjCPropertyImplDecl(ObjCPropertyImplDecl *PID);
  };
} // end anonymous namespace

DeclPrinter::~DeclPrinter() {
  Out.flush();
}

void DeclPrinter::PrintFunctionDeclStart(FunctionDecl *FD) {
	bool HasBody = FD->getBody();

	Out << '\n';

	switch (FD->getStorageClass()) {
  default: assert(0 && "Unknown storage class");
  case FunctionDecl::None: break;
  case FunctionDecl::Extern: Out << "extern "; break;
  case FunctionDecl::Static: Out << "static "; break;
	}

	if (FD->isInline())
		Out << "inline ";

	std::string Proto = FD->getName();
	const FunctionType *AFT = FD->getType()->getAsFunctionType();

	if (const FunctionTypeProto *FT = dyn_cast<FunctionTypeProto>(AFT)) {
		Proto += "(";
		for (unsigned i = 0, e = FD->getNumParams(); i != e; ++i) {
			if (i) Proto += ", ";
			std::string ParamStr;
			if (HasBody) ParamStr = FD->getParamDecl(i)->getName();

			FT->getArgType(i).getAsStringInternal(ParamStr);
			Proto += ParamStr;
		}

		if (FT->isVariadic()) {
			if (FD->getNumParams()) Proto += ", ";
			Proto += "...";
		}
		Proto += ")";
	} else {
		assert(isa<FunctionTypeNoProto>(AFT));
		Proto += "()";
	}

	AFT->getResultType().getAsStringInternal(Proto);
	Out << Proto;

	if (!FD->getBody())
		Out << ";\n";
	// Doesn't print the body.
}

void DeclPrinter::PrintTypeDefDecl(TypedefDecl *TD) {
	std::string S = TD->getName();
	TD->getUnderlyingType().getAsStringInternal(S);
	Out << "typedef " << S << ";\n";
}

void DeclPrinter::PrintObjCMethodDecl(ObjCMethodDecl *OMD) {
	if (OMD->isInstance())
		Out << "\n- ";
	else 
		Out << "\n+ ";
	if (!OMD->getResultType().isNull())
		Out << '(' << OMD->getResultType().getAsString() << ") ";
	// FIXME: just print original selector name!
	Out << OMD->getSelector().getName();

	for (int i = 0; i < OMD->getNumParams(); i++) {
		ParmVarDecl *PDecl = OMD->getParamDecl(i);
		// FIXME: selector is missing here!    
		Out << " :(" << PDecl->getType().getAsString() << ") " << PDecl->getName(); 
	}
}

void DeclPrinter::PrintObjCImplementationDecl(ObjCImplementationDecl *OID) {
	std::string I = OID->getName();
	ObjCInterfaceDecl *SID = OID->getSuperClass();

	if (SID)
		Out << "@implementation " << I << " : " << SID->getName();
	else
		Out << "@implementation " << I;

	for (ObjCImplementationDecl::instmeth_iterator I = OID->instmeth_begin(),
		E = OID->instmeth_end(); I != E; ++I) {
			ObjCMethodDecl *OMD = *I;
			PrintObjCMethodDecl(OMD);
			if (OMD->getBody()) {
				Out << ' ';
				OMD->getBody()->printPretty(Out);
				Out << '\n';
			}
	}

	for (ObjCImplementationDecl::classmeth_iterator I = OID->classmeth_begin(),
		E = OID->classmeth_end(); I != E; ++I) {
			ObjCMethodDecl *OMD = *I;
			PrintObjCMethodDecl(OMD);
			if (OMD->getBody()) {
				Out << ' ';
				OMD->getBody()->printPretty(Out);
				Out << '\n';
			}
	}

	Out << "@end\n";
}

/// PrintObjCPropertyDecl - print a property declaration.
///
void DeclPrinter::PrintObjCPropertyDecl(ObjCPropertyDecl *PDecl) {
  if (PDecl->getPropertyImplementation() == ObjCPropertyDecl::Required)
    Out << "@required\n";
  else if (PDecl->getPropertyImplementation() == ObjCPropertyDecl::Optional)
    Out << "@optional\n";
  
  Out << "@property";
  if (PDecl->getPropertyAttributes() != ObjCPropertyDecl::OBJC_PR_noattr) {
    bool first = true;
    Out << " (";
    if (PDecl->getPropertyAttributes() & 
        ObjCPropertyDecl::OBJC_PR_readonly) {
      Out << (first ? ' ' : ',') << "readonly";
      first = false;
  }
      
  if (PDecl->getPropertyAttributes() & ObjCPropertyDecl::OBJC_PR_getter) {
    Out << (first ? ' ' : ',') << "getter = "
    << PDecl->getGetterName().getName();
    first = false;
  }
  if (PDecl->getPropertyAttributes() & ObjCPropertyDecl::OBJC_PR_setter) {
    Out << (first ? ' ' : ',') << "setter = "
    << PDecl->getSetterName().getName();
    first = false;
  }
      
  if (PDecl->getPropertyAttributes() & ObjCPropertyDecl::OBJC_PR_assign) {
    Out << (first ? ' ' : ',') << "assign";
    first = false;
  }
      
  if (PDecl->getPropertyAttributes() &
      ObjCPropertyDecl::OBJC_PR_readwrite) {
    Out << (first ? ' ' : ',') << "readwrite";
    first = false;
  }
      
  if (PDecl->getPropertyAttributes() & ObjCPropertyDecl::OBJC_PR_retain) {
    Out << (first ? ' ' : ',') << "retain";
    first = false;
  }
      
  if (PDecl->getPropertyAttributes() & ObjCPropertyDecl::OBJC_PR_copy) {
    Out << (first ? ' ' : ',') << "copy";
    first = false;
  }
      
  if (PDecl->getPropertyAttributes() & 
      ObjCPropertyDecl::OBJC_PR_nonatomic) {
    Out << (first ? ' ' : ',') << "nonatomic";
    first = false;
  }
  Out << " )";
  }
  Out << ' ' << PDecl->getType().getAsString()
  << ' ' << PDecl->getName();
    
  Out << ";\n";
}



void DeclPrinter::PrintObjCInterfaceDecl(ObjCInterfaceDecl *OID) {
	std::string I = OID->getName();
  ObjCInterfaceDecl *SID = OID->getSuperClass();

  if (SID)
    Out << "@interface " << I << " : " << SID->getName();
  else
    Out << "@interface " << I;
  
  // Protocols?
  const ObjCList<ObjCProtocolDecl> &Protocols = OID->getReferencedProtocols();
  if (!Protocols.empty()) {
    for (ObjCList<ObjCProtocolDecl>::iterator I = Protocols.begin(),
         E = Protocols.end(); I != E; ++I)
      Out << (I == Protocols.begin() ? '<' : ',') << (*I)->getName();
  }
  
  if (!Protocols.empty())
    Out << ">";
  Out << '\n';
  
  if (OID->ivar_size() > 0) {
    Out << '{';
    for (ObjCInterfaceDecl::ivar_iterator I = OID->ivar_begin(),
         E = OID->ivar_end(); I != E; ++I) {
      Out << '\t' << (*I)->getType().getAsString()
          << ' '  << (*I)->getName() << ";\n";      
    }
    Out << "}\n";
  }
  
  for (ObjCInterfaceDecl::classprop_iterator I = OID->classprop_begin(),
       E = OID->classprop_end(); I != E; ++I)
    PrintObjCPropertyDecl(*I);
  bool eol_needed = false;
  for (ObjCInterfaceDecl::classmeth_iterator I = OID->classmeth_begin(),
       E = OID->classmeth_end(); I != E; ++I)
    eol_needed = true, PrintObjCMethodDecl(*I);
  
  for (ObjCInterfaceDecl::instmeth_iterator I = OID->instmeth_begin(),
       E = OID->instmeth_end(); I != E; ++I)
    eol_needed = true, PrintObjCMethodDecl(*I);
  
  Out << (eol_needed ? "\n@end\n" : "@end\n");
  // FIXME: implement the rest...
}

void DeclPrinter::PrintObjCProtocolDecl(ObjCProtocolDecl *PID) {
	Out << "@protocol " << PID->getName() << '\n';
	// FIXME: implement the rest...
}

void DeclPrinter::PrintObjCCategoryImplDecl(ObjCCategoryImplDecl *PID) {
	Out << "@implementation "
		<< PID->getClassInterface()->getName()
		<< '(' << PID->getName() << ");\n";  

	// FIXME: implement the rest...
}

void DeclPrinter::PrintObjCCategoryDecl(ObjCCategoryDecl *PID) {
	Out << "@interface " 
		<< PID->getClassInterface()->getName()
		<< '(' << PID->getName() << ");\n";
	// FIXME: implement the rest...
}

void DeclPrinter::PrintObjCCompatibleAliasDecl(ObjCCompatibleAliasDecl *AID) {
	Out << "@compatibility_alias " << AID->getName() 
		<< ' ' << AID->getClassInterface()->getName() << ";\n";  
}


namespace clang {
class SourceManager;
class HeaderSearch;
class Preprocessor;

/////////////////////////////////////
//maks:teste
/*
#include "clang/AST/PrettyPrinter.h"

class StmtHelper : public PrinterHelper  {

	ASTContext *Context;
public:

	StmtHelper(ASTContext *_Context) 
	{
		Context = _Context;
	}

	virtual ~StmtHelper() {}


	virtual bool handledStmt(Stmt* S, std::ostream& OS) 
	{
		Decl *D = NULL;
		NamedDecl *ND = NULL;
		ValueDecl *VD = NULL;

		if(DeclStmt *ds = dyn_cast<DeclStmt>(S))
		{
			D = ds->getDecl();
		}
		else if (DeclRefExpr *de = dyn_cast<DeclRefExpr>(S)) 
		{
			D = de->getDecl();
		}

		if(D)
		{
			//Is an identifier

			FullSourceLoc fl(Context->getFullLoc(D->getLocation()));
			//unsigned TokOffs = Context->getSourceManager().getFullFilePos(D->getLocation());
			//Context->getSourceManager().getPhysicalLineNumber(D->getLocation())
			
			OS << "[" << D->getDeclKindName() << " L" << fl.getLineNumber() << " C" << fl.getColumnNumber();

			//Identifier name
			if(ND = dyn_cast<NamedDecl>(D))
			{
				OS << ", " << ND->getIdentifier()->getName();
			}

			//Identifier type
			if(VD = dyn_cast<ValueDecl>(D))
			{
					OS << ", " << VD->getType().getAsString();
			}

			OS << "] ";
		}
		else
		{
			int a;
			a=0;
		}


		return false;
	}
};
*/
/////////////////////////////////////

class TextDiagnostics : public DiagnosticClient {
  HeaderSearch *TheHeaderSearch;
protected:
  std::string FormatDiagnostic(Diagnostic &Diags, Diagnostic::Level Level,
                               diag::kind ID,
                               const std::string *Strs,
                               unsigned NumStrs);
public:
  TextDiagnostics() {}
  virtual ~TextDiagnostics();

  void setHeaderSearch(HeaderSearch &HS) { TheHeaderSearch = &HS; }

  virtual bool IgnoreDiagnostic(Diagnostic::Level Level, 
                                FullSourceLoc Pos);

  virtual void HandleDiagnostic(Diagnostic &Diags, Diagnostic::Level DiagLevel,
                                FullSourceLoc Pos,
                                diag::kind ID,
                                const std::string *Strs,
                                unsigned NumStrs,
                                const SourceRange *Ranges, 
                                unsigned NumRanges) = 0;
};

class TextDiagnosticPrinter : public TextDiagnostics {
  FullSourceLoc LastWarningLoc;
public:
  TextDiagnosticPrinter() {}

  void PrintIncludeStack(FullSourceLoc Pos);

  void HighlightRange(const SourceRange &R,
                      SourceManager& SrcMgr,
                      unsigned LineNo,
                      std::string &CaratLine,
                      const std::string &SourceLine);

  virtual void HandleDiagnostic(Diagnostic &Diags,
                                Diagnostic::Level DiagLevel,
                                FullSourceLoc Pos,
                                diag::kind ID,
                                const std::string *Strs,
                                unsigned NumStrs,
                                const SourceRange *Ranges, 
                                unsigned NumRanges);
};

} // end namspace clang

TextDiagnostics:: ~TextDiagnostics() {}

std::string TextDiagnostics::FormatDiagnostic(Diagnostic &Diags,
                                              Diagnostic::Level Level,
                                              diag::kind ID,
                                              const std::string *Strs,
                                              unsigned NumStrs) {
  std::string Msg = Diags.getDescription(ID);
  
  // Replace all instances of %0 in Msg with 'Extra'.
  for (unsigned i = 0; i < Msg.size() - 1; ++i) {
    if (Msg[i] == '%' && isdigit(Msg[i + 1])) {
      unsigned StrNo = Msg[i + 1] - '0';
      Msg = std::string(Msg.begin(), Msg.begin() + i) +
            (StrNo < NumStrs ? Strs[StrNo] : "<<<INTERNAL ERROR>>>") +
            std::string(Msg.begin() + i + 2, Msg.end());
    }
  }

  return Msg;
}

bool TextDiagnostics::IgnoreDiagnostic(Diagnostic::Level Level,
                                       FullSourceLoc Pos) {
  if (Pos.isValid()) {
    // If this is a warning or note, and if it a system header, suppress the
    // diagnostic.
    if (Level == Diagnostic::Warning || Level == Diagnostic::Note) {
      if (const FileEntry *F = Pos.getFileEntryForLoc()) {
        DirectoryLookup::DirType DirInfo = TheHeaderSearch->getFileDirFlavor(F);
        if (DirInfo == DirectoryLookup::SystemHeaderDir ||
            DirInfo == DirectoryLookup::ExternCSystemHeaderDir)
          return true;
      }
    }
  }

  return false;
}

static llvm::cl::opt<bool>
NoShowColumn("fno-show-column",
             llvm::cl::desc("Do not include column number on diagnostics"));
static llvm::cl::opt<bool>
NoCaretDiagnostics("fno-caret-diagnostics",
                   llvm::cl::desc("Do not include source line and caret with"
                                  " diagnostics"));

void TextDiagnosticPrinter::
PrintIncludeStack(FullSourceLoc Pos) {
  if (Pos.isInvalid()) return;

  Pos = Pos.getLogicalLoc();

  // Print out the other include frames first.
  PrintIncludeStack(Pos.getIncludeLoc());
  unsigned LineNo = Pos.getLineNumber();
  
  std::cerr << "In file included from " << Pos.getSourceName()
            << ":" << LineNo << ":\n";
}

/// HighlightRange - Given a SourceRange and a line number, highlight (with ~'s)
/// any characters in LineNo that intersect the SourceRange.
void TextDiagnosticPrinter::HighlightRange(const SourceRange &R,
                                          SourceManager& SourceMgr,
                                           unsigned LineNo,
                                           std::string &CaratLine,
                                           const std::string &SourceLine) {  
  assert(CaratLine.size() == SourceLine.size() &&
         "Expect a correspondence between source and carat line!");
  if (!R.isValid()) return;

  unsigned StartLineNo = SourceMgr.getLogicalLineNumber(R.getBegin());
  if (StartLineNo > LineNo) return;  // No intersection.
  
  unsigned EndLineNo = SourceMgr.getLogicalLineNumber(R.getEnd());
  if (EndLineNo < LineNo) return;  // No intersection.
  
  // Compute the column number of the start.
  unsigned StartColNo = 0;
  if (StartLineNo == LineNo) {
    StartColNo = SourceMgr.getLogicalColumnNumber(R.getBegin());
    if (StartColNo) --StartColNo;  // Zero base the col #.
  }

  // Pick the first non-whitespace column.
  while (StartColNo < SourceLine.size() &&
         (SourceLine[StartColNo] == ' ' || SourceLine[StartColNo] == '\t'))
    ++StartColNo;
  
  // Compute the column number of the end.
  unsigned EndColNo = CaratLine.size();
  if (EndLineNo == LineNo) {
    EndColNo = SourceMgr.getLogicalColumnNumber(R.getEnd());
    if (EndColNo) {
      --EndColNo;  // Zero base the col #.
      
      // Add in the length of the token, so that we cover multi-char tokens.
      EndColNo += Lexer::MeasureTokenLength(R.getEnd(), SourceMgr);
    } else {
      EndColNo = CaratLine.size();
    }
  }
  
  // Pick the last non-whitespace column.
  while (EndColNo-1 &&
         (SourceLine[EndColNo-1] == ' ' || SourceLine[EndColNo-1] == '\t'))
    --EndColNo;
  
  // Fill the range with ~'s.
  assert(StartColNo <= EndColNo && "Invalid range!");
  for (unsigned i = StartColNo; i != EndColNo; ++i)
    CaratLine[i] = '~';
}

void TextDiagnosticPrinter::HandleDiagnostic(Diagnostic &Diags,
                                             Diagnostic::Level Level, 
                                             FullSourceLoc Pos,
                                             diag::kind ID,
                                             const std::string *Strs,
                                             unsigned NumStrs,
                                             const SourceRange *Ranges,
                                             unsigned NumRanges) {
  unsigned LineNo = 0, ColNo = 0;
  const char *LineStart = 0, *LineEnd = 0;
  
  if (Pos.isValid()) {
    FullSourceLoc LPos = Pos.getLogicalLoc();
    LineNo = LPos.getLineNumber();
    
    // First, if this diagnostic is not in the main file, print out the
    // "included from" lines.
    if (LastWarningLoc != LPos.getIncludeLoc()) {
      LastWarningLoc = LPos.getIncludeLoc();
      PrintIncludeStack(LastWarningLoc);
    }
  
    // Compute the column number.  Rewind from the current position to the start
    // of the line.
    ColNo = LPos.getColumnNumber();
    const char *TokLogicalPtr = LPos.getCharacterData();
    LineStart = TokLogicalPtr-ColNo+1;  // Column # is 1-based
  
    // Compute the line end.  Scan forward from the error position to the end of
    // the line.
    const llvm::MemoryBuffer *Buffer = LPos.getBuffer();
    const char *BufEnd = Buffer->getBufferEnd();
    LineEnd = TokLogicalPtr;
    while (LineEnd != BufEnd && 
           *LineEnd != '\n' && *LineEnd != '\r')
      ++LineEnd;
  
    std::cerr << Buffer->getBufferIdentifier() 
              << ":" << LineNo << ":";
    if (ColNo && !NoShowColumn) 
      std::cerr << ColNo << ":";
    std::cerr << " ";
  }
  
  switch (Level) {
  default: assert(0 && "Unknown diagnostic type!");
  case Diagnostic::Note:    std::cerr << "note: "; break;
  case Diagnostic::Warning: std::cerr << "warning: "; break;
  case Diagnostic::Error:   std::cerr << "error: "; break;
  case Diagnostic::Fatal:   std::cerr << "fatal error: "; break;
    break;
  }
  
  std::cerr << FormatDiagnostic(Diags, Level, ID, Strs, NumStrs) << "\n";
  
  if (!NoCaretDiagnostics && Pos.isValid()) {
    // Get the line of the source file.
    std::string SourceLine(LineStart, LineEnd);
    
    // Create a line for the carat that is filled with spaces that is the same
    // length as the line of source code.
    std::string CaratLine(LineEnd-LineStart, ' ');
    
    // Highlight all of the characters covered by Ranges with ~ characters.
    for (unsigned i = 0; i != NumRanges; ++i)
      HighlightRange(Ranges[i], Pos.getManager(),LineNo, CaratLine, SourceLine);
    
    // Next, insert the carat itself.
    if (ColNo-1 < CaratLine.size())
      CaratLine[ColNo-1] = '^';
    else
      CaratLine.push_back('^');
    
    // Scan the source line, looking for tabs.  If we find any, manually expand
    // them to 8 characters and update the CaratLine to match.
    for (unsigned i = 0; i != SourceLine.size(); ++i) {
      if (SourceLine[i] != '\t') continue;
      
      // Replace this tab with at least one space.
      SourceLine[i] = ' ';
      
      // Compute the number of spaces we need to insert.
      unsigned NumSpaces = ((i+8)&~7) - (i+1);
      assert(NumSpaces < 8 && "Invalid computation of space amt");
      
      // Insert spaces into the SourceLine.
      SourceLine.insert(i+1, NumSpaces, ' ');
      
      // Insert spaces or ~'s into CaratLine.
      CaratLine.insert(i+1, NumSpaces, CaratLine[i] == '~' ? '~' : ' ');
    }
    
    // Finally, remove any blank spaces from the end of CaratLine.
    while (CaratLine[CaratLine.size()-1] == ' ')
      CaratLine.erase(CaratLine.end()-1);
    
    // Emit what we have computed.
    std::cerr << SourceLine << "\n";
    std::cerr << CaratLine << "\n";
  }
}





//===----------------------------------------------------------------------===//
/// ASTPrinter - Pretty-printer of ASTs

namespace {
  class ASTDumper : public ASTConsumer, public DeclPrinter {
    SourceManager *SM;
  public:
    ASTDumper() : DeclPrinter() {}
    
    void Initialize(ASTContext &Context) {
      SM = &Context.getSourceManager();
    }
    
    virtual void HandleTopLevelDecl(Decl *D) {
      if (FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
        PrintFunctionDeclStart(FD);
        
        if (FD->getBody()) {
          Out << '\n';
          // FIXME: convert dumper to use std::ostream?
          FD->getBody()->dumpAll(*SM);
          Out << '\n';
        }
      } else if (TypedefDecl *TD = dyn_cast<TypedefDecl>(D)) {
        PrintTypeDefDecl(TD);
      } else if (ScopedDecl *SD = dyn_cast<ScopedDecl>(D)) {
        Out << "Read top-level variable decl: '" << SD->getName() << "'\n";
      } else if (ObjCInterfaceDecl *OID = dyn_cast<ObjCInterfaceDecl>(D)) {
        Out << "Read objc interface '" << OID->getName() << "'\n";
      } else if (ObjCProtocolDecl *OPD = dyn_cast<ObjCProtocolDecl>(D)) {
        Out << "Read objc protocol '" << OPD->getName() << "'\n";
      } else if (ObjCCategoryDecl *OCD = dyn_cast<ObjCCategoryDecl>(D)) {
        Out << "Read objc category '" << OCD->getName() << "'\n";
      } else if (isa<ObjCForwardProtocolDecl>(D)) {
        Out << "Read objc fwd protocol decl\n";
      } else if (isa<ObjCClassDecl>(D)) {
        Out << "Read objc fwd class decl\n";
      } else if (isa<FileScopeAsmDecl>(D)) {
        Out << "Read file scope asm decl\n";
      } else if (ObjCMethodDecl* MD = dyn_cast<ObjCMethodDecl>(D)) {
        Out << "Read objc method decl: '" << MD->getSelector().getName()
            << "'\n";
        if (MD->getBody()) {
          // FIXME: convert dumper to use std::ostream?
          MD->getBody()->dumpAll(*SM);
          Out << '\n';
        }
      } else if (isa<ObjCImplementationDecl>(D)) {
        Out << "Read objc implementation decl\n";
      }
      else {
        assert(0 && "Unknown decl type!");
      }
    }
  };
}

namespace {
  class TripleProcessor {
    llvm::StringMap<char> TriplesProcessed;
    std::vector<std::string>& triples;
  public:
    TripleProcessor(std::vector<std::string>& t) :  triples(t) {}
    
    void addTriple(const std::string& t) {
      if (TriplesProcessed.find(t.c_str(),t.c_str()+t.size()) ==
          TriplesProcessed.end()) {
        triples.push_back(t);
        TriplesProcessed.GetOrCreateValue(t.c_str(),t.c_str()+t.size());
      }
    }
  };
}


static llvm::cl::opt<bool>
VerifyDiagnostics("verify",
                  llvm::cl::desc("Verify emitted diagnostics and warnings"));

static std::string CreateTargetTriple() {
  // Initialize base triple.  If a -triple option has been specified, use
  // that triple.  Otherwise, default to the host triple.
  std::string Triple = LLVM_HOSTTRIPLE;
  
  // If -arch foo was specified, remove the architecture from the triple we have
  // so far and replace it with the specified one.
  //if (Arch.empty())
    return Triple;
    
  // Decompose the base triple into "arch" and suffix.
  std::string::size_type FirstDashIdx = Triple.find("-");
  
  if (FirstDashIdx == std::string::npos) {
    fprintf(stderr, 
            "Malformed target triple: \"%s\" ('-' could not be found).\n",
            Triple.c_str());
    exit(1);
  }
  
  //return Arch + std::string(Triple.begin()+FirstDashIdx, Triple.end());
}

void testClang()
{
	return; //maks:teste
	std::string InFile = "E:/GAMEDEVELOPMENT/output/dots.c";

	// Create a file manager object to provide access to and cache the filesystem.
	FileManager FileMgr;
	char *PredefineBuffer = "";


	ASTConsumer *consumer = new ASTDumper();	

	// Create the diagnostic client for reporting errors or for
  // implementing -verify.
  DiagnosticClient* TextDiagClient = 0;
  
  TextDiagClient = new TextDiagnosticPrinter();
  
	

	llvm::OwningPtr<DiagnosticClient> DiagClient(TextDiagClient);
	Diagnostic Diags(DiagClient.get());

	LangOptions LangInfo;
	SourceManager SourceMgr;
	HeaderSearch HeaderInfo(FileMgr);
	TargetInfo *Target;

	
	std::string Triple = CreateTargetTriple();	
	//llvm::OwningPtr<TargetInfo> Target(TargetInfo::CreateTargetInfo(Triple));
	Target = TargetInfo::CreateTargetInfo(Triple);
	
	

	// Set up the preprocessor with these options.
	Preprocessor PP(Diags, LangInfo, *Target, SourceMgr, HeaderInfo);

	PP.setPredefines(PredefineBuffer);

	const FileEntry *File = FileMgr.getFile(InFile);
	if (File) SourceMgr.createMainFileID(File, SourceLocation());

	ParseAST(PP, consumer, false);


}