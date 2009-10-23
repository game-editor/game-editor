/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


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