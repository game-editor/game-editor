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