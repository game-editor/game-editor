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


#ifndef _CODE_EMITTER_PROPERTY_IO
#define _CODE_EMITTER_PROPERTY_IO

#include <wx/wx.h>
#include <wx/wxxmlserializer/XmlSerializer.h>
#include <wx/wfstream.h>

class CodeEmitter
{
public:
	wxString data;

	CodeEmitter() {}

	CodeEmitter( const CodeEmitter& copy ) 
	{
		data = copy.data;
	}

	void operator=( const CodeEmitter& copy ) 
	{
		data = copy.data;
	}
};


class xsCodeEmitter : public xsPropertyIO 
{ 
public: 
	DECLARE_DYNAMIC_CLASS(xsCodeEmitter); 
	xsCodeEmitter(){ bGetCode = false;} 
	virtual ~xsCodeEmitter(){;} 

	virtual void Read(xsProperty *property, wxXmlNode *source); 
	virtual void Write(xsProperty *property, wxXmlNode *target); 
	virtual wxString GetValueStr(xsProperty *property); 
	static wxString ToString(CodeEmitter value); 
	static CodeEmitter FromString(const wxString& value); 

	static void Start();
	static void Stop();

	static wxString GetCode() {return code;}

private:
	static bool bGetCode;
	static wxString code;
}; 

#define XS_SERIALIZE_CODEEMITTER(x, xsCodeEmitter) XS_SERIALIZE_PROPERTY(x, wxT("emit"), xsCodeEmitter);

#endif