#include "CodeEmitter.h"


XS_DEFINE_IO_HANDLER(CodeEmitter, xsCodeEmitter)

bool xsCodeEmitter::bGetCode = false;
wxString xsCodeEmitter::code;

void xsCodeEmitter::Start()
{	
	bGetCode = true;
	code = "";
}

void xsCodeEmitter::Stop()
{
	bGetCode = false;
}

wxString xsCodeEmitter::ToString(CodeEmitter value)
{
	if(bGetCode) code += value.data;
	return value.data;
}


CodeEmitter xsCodeEmitter::FromString(const wxString& value)
{	
	CodeEmitter out;
	out.data = value;
	return out;
}