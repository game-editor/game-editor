/* emitter.c
 *
 *	(C) Copyright Apr 15 1995, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "MachSet.h"
#include "global.h"


static void showjmptab(InsT_t * inst, int i, int spaces)
{
  struct {
    int n;
    val_t *loc;
  } *p;
  int j, k;
  p = inst->val.p.p;
  printf("\n");
  for (k = 0; k < spaces; k++)
    printf(" ");
  printf("     default  %d\n", i + p->loc[0].ival);
  for (j = 1; j < p->n; j += 2) {
    for (k = 0; k < spaces; k++)
      printf(" ");

    printf("     case %d: %d\n", p->loc[j].ival,
	   i + p->loc[j + 1].ival);
  }
}



#ifdef _DEBUG

void showcode(FILE *fp, InsT_t * inst,int i)
{
  static char str[30], *s;

  s = NULL;
  switch (inst->opcode) {
      case __eiclongjmp: s = "__longjmp"; break; //maks
      case __eicsetjmp: s = "__setjmp"; break; //maks
    case jmptab: s = "jmptab"; break;
    case lval: sprintf(str, "lval   %d %d",
		       inst->val.ival,
		       inst->ext); break;

    case ixa: sprintf(str, "ixa      %d  %d",inst->val.ival, inst->ext);
      break;
    case lda: sprintf(str, "lda      %d  %d",inst->val.ival, inst->ext);
      break;

    case refchar: s = "refchar"; break;
    case drefuchar: s = "drefuchar"; break;  
    case drefchar: s = "drefchar"; break;
    case rvaluchar: sprintf(str, "rvaluchar  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case rvalchar: sprintf(str, "rvalchar  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case stochar: sprintf(str, "stochar %d %d",
			   inst->val.ival,
			   inst->ext); break;

    case refshort: s = "refshort"; break;
    case drefushort: s = "drefushort"; break;  
    case drefshort: s = "drefshort"; break;
    case rvalushort: sprintf(str, "rvalushort  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case rvalshort: sprintf(str, "rvalshort  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case stoshort: sprintf(str, "stoshort %d %d",
			   inst->val.ival,
			   inst->ext); break;
      

    case reffloat: s = "reffloat"; break;
    case dreffloat: s = "dreffloat"; break;
    case rvalfloat: sprintf(str, "rvalfloat  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case stofloat: sprintf(str, "stofloat %d %d",
			   inst->val.ival,
			   inst->ext); break;
      
    case jmpFint: sprintf(str, "jmpFint  %d", i + inst->val.ival); break;
    case jmpFlng: sprintf(str, "jmpFlng  %d", i + inst->val.ival); break;
    case jmpFllng: sprintf(str, "jmpFllng  %d", i + inst->val.ival); break;
    case jmpFdbl: sprintf(str, "jmpFdbl  %d", i + inst->val.ival); break;
    case jmpFptr: sprintf(str, "jmpFptr  %d", i + inst->val.ival); break;
      
    case jmpTint: sprintf(str, "jmpTint  %d", i + inst->val.ival); break;
    case jmpTlng: sprintf(str, "jmpTlng  %d", i + inst->val.ival); break;
    case jmpTllng: sprintf(str, "jmpTllng  %d", i + inst->val.ival); break;
    case jmpTdbl: sprintf(str, "jmpTdbl  %d", i + inst->val.ival); break;
    case jmpTptr: sprintf(str, "jmpTptr  %d", i + inst->val.ival); break;

    case jmpu: sprintf(str, "jmpU     %d", i + inst->val.ival); break;
    case halt: s = "halt"; break;
    case dupval: sprintf(str,"dupval   %d",inst->val.ival); break;
    case bump:   sprintf(str,"bump     %d", inst->val.ival); break;
    case call: s = "call"; break;
      
    case neguchar: s = "neguchar"; break;
      
    case stodouble: sprintf(str, "stodouble %d %d",
			   inst->val.ival,
			   inst->ext); break;
    case pushdouble: sprintf(str, "pushdouble  %g", inst->val.dval); break;
    case rvaldouble: sprintf(str, "rvaldouble  %d %d",
			    inst->val.ival,
			    inst->ext); break;

    case incdouble: s = "incdouble"; break;
    case decdouble: s = "decdouble"; break;
    case refdouble: s = "refdouble"; break;
    case drefdouble: s = "drefdouble "; break;
    case negdouble: s = "negdouble"; break;
    case ltdouble: s = "ltdouble"; break;
    case ledouble: s = "ledouble"; break;
    case eqdouble: s = "eqdouble"; break;
    case nedouble: s = "nedouble"; break;
    case gtdouble: s = "gtdouble"; break;
    case gedouble: s = "gedouble"; break;
    case lordouble: s = "lordouble"; break;
    case landdouble: s = "landdouble"; break;
    case notdouble: s = "notdouble"; break;
    case adddouble: s = "adddouble"; break;
    case subdouble: s = "subdouble"; break;
    case divdouble: s = "divdouble"; break;
    case multdouble: s = "multdouble"; break;
    case double2int: s = "double2int"; break;
    case double2long: s = "double2long"; break;
    case double2llong: s = "double2llong"; break;
    case double2float: s = "double2float"; break;
      
    case stoint: sprintf(str, "stoint   %d %d", 
			 inst->val.ival,
			 inst->ext); break;
    case pushint: sprintf(str, "pushint  %d", inst->val.ival); break;
    case rvalint: sprintf(str, "rvalint  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case drefint: s = "drefint   "; break;
    case refint: s = "refint   "; break;
    case decint: sprintf(str, "decint   %d", inst->val.ival); break;
    case incint: sprintf(str, "incint   %d", inst->val.ival); break;
    case negint: s = "negint"; break;
    case ltint: s = "ltint"; break;
    case leint: s = "leint"; break;
    case eqint: s = "eqint"; break;
    case neint: s = "neint"; break;
    case gtint: s = "gtint"; break;
    case geint: s = "geint"; break;
    case lorint: s = "lorint"; break;
    case landint: s = "landint"; break;
    case notint: s = "notint"; break;
    case borint: s = "borint"; break;
    case xorint: s = "xorint"; break;
    case andint: s = "andint"; break;
    case addint: s = "addint"; break;
    case subint: s = "subint"; break;
    case divint: s = "divint"; break;
    case multint: s = "multint"; break;
    case modint: s = "modint"; break;
    case lshtint: s = "lshtint"; break;
    case rshtint: s = "rshtint"; break;
    case compint: s = "compint"; break;
    case int2ptr: s = "int2ptr"; break;
    case int2uchar: s = "int2uchar"; break;
    case int2ushort: s = "int2ushort"; break;
      
    case int2double: s = "int2double"; break;
    case int2llong: s = "int2llong"; break;
    case int2long: s = "int2llong"; break;
      
      
    case stouint: sprintf(str, "stouint   %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case pushuint: sprintf(str, "pushuint  %u", inst->val.ival); break;
    case rvaluint: sprintf(str, "rvaluint  %d %d",
			   inst->val.ival,
			   inst->ext); break;
    case drefuint: s = "drefuint   "; break;
    case refuint: s = "refuint   "; break;
    case decuint: sprintf(str, "decuint   %d", inst->val.ival); break;
    case incuint: sprintf(str, "incuint   %d", inst->val.ival); break;
    case neguint: s = "neguint"; break;
    case ltuint: s = "ltuint"; break;
    case leuint: s = "leuint"; break;
    case equint: s = "equint"; break;
    case neuint: s = "neuint"; break;
    case gtuint: s = "gtuint"; break;
    case geuint: s = "geuint"; break;
    case loruint: s = "loruint"; break;

    case notuint: s = "notuint"; break;
    case boruint: s = "boruint"; break;
    case xoruint: s = "xoruint"; break;
    case anduint: s = "anduint"; break;
    case adduint: s = "adduint"; break;
    case subuint: s = "subuint"; break;
    case divuint: s = "divuint"; break;
    case multuint: s = "multuint"; break;
    case moduint: s = "moduint"; break;
    case lshtuint: s = "lshtuint"; break;
    case rshtuint: s = "rshtuint"; break;
    case compuint: s = "compuint"; break;
      
    case uint2ptr: s = "uint2ptr"; break;
    case uint2long: s = "uint2long"; break;
    case uint2llong: s = "uint2llong"; break;
    case uint2double: s = "uint2double"; break;
      
    case stolong: sprintf(str, "stolong   %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case stollong: sprintf(str, "stollong   %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case pushlong: sprintf(str, "pushlong  %ld", inst->val.lval); break;
    case pushllong: sprintf(str, "pushllong  %ld", inst->val.lval); break;
    case rvallong: sprintf(str, "rvallong  %d %d",
			   inst->val.ival,
			   inst->ext); break;
    case rvalllong: sprintf(str, "rvalllong  %d %d",
			   inst->val.ival,
			   inst->ext); break;

    case dreflong: s = "dreflong   "; break;
    case reflong: s = "reflong   "; break;
    case declong: sprintf(str, "declong   %d", inst->val.ival); break;
    case inclong: sprintf(str, "inclong   %d", inst->val.ival); break;
    case neglong: s = "neglong"; break;
    case ltlong: s = "ltlong"; break;
    case lelong: s = "lelong"; break;
    case eqlong: s = "eqlong"; break;
    case nelong: s = "nelong"; break;
    case gtlong: s = "gtlong"; break;
    case gelong: s = "gelong"; break;
    case lorlong: s = "lorlong"; break;
    case landlong: s = "landlong"; break;
    case notlong: s = "notlong"; break;
    case borlong: s = "borlong"; break;
    case xorlong: s = "xorlong"; break;
    case andlong: s = "andlong"; break;
    case addlong: s = "addlong"; break;
    case sublong: s = "sublong"; break;
    case divlong: s = "divlong"; break;
    case multlong: s = "multlong"; break;
    case modlong: s = "modlong"; break;
    case lshtlong: s = "lshtlong"; break;
    case rshtlong: s = "rshtlong"; break;
    case complong: s = "complong"; break;
      

    case drefllong: s = "drefllong   "; break;
    case refllong: s = "refllong   "; break;
    case decllong: sprintf(str, "decllong   %d", inst->val.ival); break;
    case incllong: sprintf(str, "incllong   %d", inst->val.ival); break;
    case negllong: s = "negllong"; break;
    case ltllong: s = "ltllong"; break;
    case lellong: s = "lellong"; break;
    case eqllong: s = "eqllong"; break;
    case nellong: s = "nellong"; break;
    case gtllong: s = "gtllong"; break;
    case gellong: s = "gellong"; break;
    case lorllong: s = "lorllong"; break;
    case landllong: s = "landllong"; break;
    case notllong: s = "notllong"; break;
    case borllong: s = "borllong"; break;
    case xorllong: s = "xorllong"; break;
    case andllong: s = "andllong"; break;
    case addllong: s = "addllong"; break;
    case subllong: s = "subllong"; break;
    case divllong: s = "divllong"; break;
    case multllong: s = "multllong"; break;
    case modllong: s = "modllong"; break;
    case lshtllong: s = "lshtllong"; break;
    case rshtllong: s = "rshtllong"; break;
    case compllong: s = "compllong"; break;
      


    case long2ptr: s = "long2ptr"; break;
    case long2int: s = "long2int"; break;
    case long2double: s = "long2double"; break;

    case llong2ptr: s = "llong2ptr"; break;
    case llong2int: s = "llong2int"; break;
    case llong2double: s = "llong2double"; break;
      
    case stoulong: sprintf(str, "stoulong %d %d",
			   inst->val.ival,
			   inst->ext); break;
    case pushulong: sprintf(str, "pushulong  %u", inst->val.ival); break;
    case rvalulong: sprintf(str, "rvalulong  %d %d",
			    inst->val.ival,
			    inst->ext); break;
    case drefulong: s = "drefulong"; break;
    case refulong: s = "refulong"; break;
    case deculong: sprintf(str, "deculong %d", inst->val.ival); break;
    case inculong: sprintf(str, "inculong %d", inst->val.ival); break;
    case negulong: s = "negulong"; break;
    case ltulong: s = "ltulong"; break;
    case leulong: s = "leulong"; break;
    case equlong: s = "equlong"; break;
    case neulong: s = "neulong"; break;
    case gtulong: s = "gtulong"; break;
    case geulong: s = "geulong"; break;
    case lorulong: s = "lorulong"; break;

    case notulong: s = "notulong"; break;
    case borulong: s = "borulong"; break;
    case xorulong: s = "xorulong"; break;
    case andulong: s = "andulong"; break;
    case addulong: s = "addulong"; break;
    case subulong: s = "subulong"; break;
    case divulong: s = "divulong"; break;
    case multulong: s = "multulong"; break;
    case modulong: s = "modulong"; break;
    case lshtulong: s = "lshtulong"; break;
    case rshtulong: s = "rshtulong"; break;
    case compulong: s = "compulong"; break;
      
    case ulong2ptr: s = "ulong2ptr"; break;
    case ulong2int: s = "ulong2int"; break;
    case ulong2double: s = "ulong2double"; break;
      
    case drefptr: s = "drefptr"; break;
    case drefuptr: s = "drefuptr"; break;
    case refptr: s = "refptr"; break;
    case refuptr: s = "refuptr"; break;
    
  case stouptr:  sprintf(str, "stouptr   %d %d",
			 inst->val.ival,
			 inst->ext); break;

    case stoptr: sprintf(str, "stoptr   %d %d",
			 inst->val.ival,
			 inst->ext); break;
    case pushptr: sprintf(str, "pushptr  %p", inst->val.p.p); break;

  case rvaluptr: sprintf(str, "rvaluptr  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case rvalptr: sprintf(str, "rvalptr  %d %d",
			  inst->val.ival,
			  inst->ext); break;
    case incptr: sprintf(str, "incptr   %d", inst->val.ival); break;
    case decptr: sprintf(str, "decptr   %d", inst->val.ival); break;
    case ltptr: s = "ltptr"; break;
    case leptr: s = "leptr"; break;
    case eqptr: s = "eqptr"; break;
    case neptr: s = "neptr"; break;
    case gtptr: s = "gtptr"; break;
    case geptr: s = "geptr"; break;
    case lorptr: s = "lorptr"; break;
    case landptr: s = "landptr"; break;
    case notptr: s = "notptr"; break;
    case addptr2int: s = "addptr2int"; break;
    case addint2ptr: s = "addint2ptr"; break;
    case subptrint: s = "subptrint"; break;
    case subptr: s = "subptr"; break;
    case divptr: sprintf(str, "divptr   %d", inst->val.ival); break;
    case ptr2int: s = "ptr2int"; break;
    case ptr2long: s = "ptr2long"; break;
      
    case eiccall: s = "eiccall"; break;
    case eicreturn: s = "eicreturn"; break;
      
    case assigntype: sprintf(str, "assigntype %d",
			     inst->ext); break;
      
    case stoval: s = "stoval"; break;
    case pushval: s = "pushval"; break;  
    case reducear: sprintf(str, "reducear %d",
			   inst->val.ival); break;
      
    case checkar: sprintf(str, "checkar  %d %d",
			  inst->val.ival,
			  inst->ext); break;
      
    case refmem: sprintf(str, "refmem   %d",
			 inst->val.ival); break;
    case massign: sprintf(str, "massign  %d %d",
			  inst->val.ival,
			  inst->ext); break;
      
    case fmem: sprintf(str, "fmem     %d %d",
		       inst->val.ival,
		       inst->ext); break;
    case minit: sprintf(str, "mcpy    %p %d",
		       inst->val.p.p,
		       inst->ext); break;

    case empty: s = "empty"; break;
      
    /*maks: big instructions (super operators)*/
	//case rvaluptr_bump:	sprintf(str, "rvaluptr_bump"); break;
	//case bump_pushlong:	sprintf(str, "bump_pushlong"); break;
	case rvaluptr_dreflong:	sprintf(str, "rvaluptr_dreflong"); break;
	case lelong_jmpFint:	sprintf(str, "lelong_jmpFint"); break;
	//case bump_pushint:	sprintf(str, "bump_pushint"); break;
	case drefdouble_stoval:	sprintf(str, "drefdouble_stoval"); break;
	//case rvalint_bump:	sprintf(str, "rvalint_bump"); break;

	case pushlong_lelong_jmpFint:  sprintf(str, "pushlong_lelong_jmpFint"); break;

	case bump_push_addint:	sprintf(str, "bump_push_addint"); break;
	case bump_push_adduint:	sprintf(str, "bump_push_adduint"); break;
	case bump_push_addlong:	sprintf(str, "bump_push_addlong"); break;
	case bump_push_addulong:	sprintf(str, "bump_push_addulong"); break;
	case bump_push_addllong:	sprintf(str, "bump_push_addllong"); break;
	case bump_push_adddouble:	sprintf(str, "bump_push_adddouble"); break;

	case bump_push_subint:	sprintf(str, "bump_push_subint"); break;
	case bump_push_subuint:	sprintf(str, "bump_push_subuint"); break;
	case bump_push_sublong:	sprintf(str, "bump_push_sublong"); break;
	case bump_push_subulong:	sprintf(str, "bump_push_subulong"); break;
	case bump_push_subllong:	sprintf(str, "bump_push_subllong"); break;
	case bump_push_subdouble:	sprintf(str, "bump_push_subdouble"); break;

	case bump_push_divint:	sprintf(str, "bump_push_divint"); break;
	case bump_push_divuint:	sprintf(str, "bump_push_divuint"); break;
	case bump_push_divlong:	sprintf(str, "bump_push_divlong"); break;
	case bump_push_divulong:	sprintf(str, "bump_push_divulong"); break;
	case bump_push_divllong:	sprintf(str, "bump_push_divllong"); break;
	case bump_push_divdouble:	sprintf(str, "bump_push_divdouble"); break;

	case bump_push_multint:	sprintf(str, "bump_push_multint"); break;
	case bump_push_multuint:	sprintf(str, "bump_push_multuint"); break;
	case bump_push_multlong:	sprintf(str, "bump_push_multlong"); break;
	case bump_push_multulong:	sprintf(str, "bump_push_multulong"); break;
	case bump_push_multllong:	sprintf(str, "bump_push_multllong"); break;
	case bump_push_multdouble:	sprintf(str, "bump_push_multdouble"); break;

	case bump_push_modint:	sprintf(str, "bump_push_modint"); break;
	case bump_push_moduint:	sprintf(str, "bump_push_moduint"); break;
	case bump_push_modlong:	sprintf(str, "bump_push_modlong"); break;
	case bump_push_modulong:	sprintf(str, "bump_push_modulong"); break;
	case bump_push_modllong:	sprintf(str, "bump_push_modllong"); break;

	case bump_push_lshtint:	sprintf(str, "bump_push_lshtint"); break;
	case bump_push_lshtuint:	sprintf(str, "bump_push_lshtuint"); break;
	case bump_push_lshtlong:	sprintf(str, "bump_push_lshtlong"); break;
	case bump_push_lshtulong:	sprintf(str, "bump_push_lshtulong"); break;
	case bump_push_lshtllong:	sprintf(str, "bump_push_lshtllong"); break;

	case bump_push_rshtint:	sprintf(str, "bump_push_rshtint"); break;
	case bump_push_rshtuint:	sprintf(str, "bump_push_rshtuint"); break;
	case bump_push_rshtlong:	sprintf(str, "bump_push_rshtlong"); break;
	case bump_push_rshtulong:	sprintf(str, "bump_push_rshtulong"); break;
	case bump_push_rshtllong:	sprintf(str, "bump_push_rshtllong"); break;

	case bump_push_ltint:	sprintf(str, "bump_push_ltint"); break;
	case bump_push_ltlong:	sprintf(str, "bump_push_ltlong"); break;
	case bump_push_ltuint:	sprintf(str, "bump_push_ltuint"); break;
	case bump_push_ltulong:	sprintf(str, "bump_push_ltulong"); break;
	case bump_push_ltllong:	sprintf(str, "bump_push_ltllong"); break;
	case bump_push_ltdouble:	sprintf(str, "bump_push_ltdouble"); break;
	case bump_push_ltptr:	sprintf(str, "bump_push_ltptr"); break;

	case bump_push_leint:	sprintf(str, "bump_push_leint"); break;
	case bump_push_lelong:	sprintf(str, "bump_push_lelong"); break;
	case bump_push_leuint:	sprintf(str, "bump_push_leuint"); break;
	case bump_push_leulong:	sprintf(str, "bump_push_leulong"); break;
	case bump_push_lellong:	sprintf(str, "bump_push_lellong"); break;
	case bump_push_ledouble:	sprintf(str, "bump_push_ledouble"); break;
	case bump_push_leptr:	sprintf(str, "bump_push_leptr"); break;

	case bump_push_eqint:	sprintf(str, "bump_push_eqint"); break;
	case bump_push_eqlong:	sprintf(str, "bump_push_eqlong"); break;
	case bump_push_equint:	sprintf(str, "bump_push_equint"); break;
	case bump_push_equlong:	sprintf(str, "bump_push_equlong"); break;
	case bump_push_eqllong:	sprintf(str, "bump_push_eqllong"); break;
	case bump_push_eqdouble:	sprintf(str, "bump_push_eqdouble"); break;
	case bump_push_eqptr:	sprintf(str, "bump_push_eqptr"); break;

	case bump_push_neint:	sprintf(str, "bump_push_neint"); break;
	case bump_push_nelong:	sprintf(str, "bump_push_nelong"); break;
	case bump_push_neuint:	sprintf(str, "bump_push_neuint"); break;
	case bump_push_neulong:	sprintf(str, "bump_push_neulong"); break;
	case bump_push_nellong:	sprintf(str, "bump_push_nellong"); break;
	case bump_push_nedouble:	sprintf(str, "bump_push_nedouble"); break;
	case bump_push_neptr:	sprintf(str, "bump_push_neptr"); break;

	case bump_push_gtint:	sprintf(str, "bump_push_gtint"); break;
	case bump_push_gtlong:	sprintf(str, "bump_push_gtlong"); break;
	case bump_push_gtuint:	sprintf(str, "bump_push_gtuint"); break;
	case bump_push_gtulong:	sprintf(str, "bump_push_gtulong"); break;
	case bump_push_gtllong:	sprintf(str, "bump_push_gtllong"); break;
	case bump_push_gtdouble:	sprintf(str, "bump_push_gtdouble"); break;
	case bump_push_gtptr:	sprintf(str, "bump_push_gtptr"); break;

	case bump_push_geint:	sprintf(str, "bump_push_geint"); break;
	case bump_push_gelong:	sprintf(str, "bump_push_gelong"); break;
	case bump_push_geuint:	sprintf(str, "bump_push_geuint"); break;
	case bump_push_geulong:	sprintf(str, "bump_push_geulong"); break;
	case bump_push_gellong:	sprintf(str, "bump_push_gellong"); break;
	case bump_push_gedouble:	sprintf(str, "bump_push_gedouble"); break;
	case bump_push_geptr:	sprintf(str, "bump_push_geptr"); break;

	case bump_push_lorint:	sprintf(str, "bump_push_lorint"); break;
	case bump_push_lorlong:	sprintf(str, "bump_push_lorlong"); break;
	case bump_push_loruint:	sprintf(str, "bump_push_loruint"); break;
	case bump_push_lorulong:	sprintf(str, "bump_push_lorulong"); break;
	case bump_push_lorllong:	sprintf(str, "bump_push_lorllong"); break;
	case bump_push_lordouble:	sprintf(str, "bump_push_lordouble"); break;
	case bump_push_lorptr:	sprintf(str, "bump_push_lorptr"); break;

	case bump_push_borint:	sprintf(str, "bump_push_borint"); break;
	case bump_push_borlong:	sprintf(str, "bump_push_borlong"); break;
	case bump_push_boruint:	sprintf(str, "bump_push_boruint"); break;
	case bump_push_borulong:	sprintf(str, "bump_push_borulong"); break;
	case bump_push_borllong:	sprintf(str, "bump_push_borllong"); break;

	case bump_push_xorint:	sprintf(str, "bump_push_xorint"); break;
	case bump_push_xorlong:	sprintf(str, "bump_push_xorlong"); break;
	case bump_push_xoruint:	sprintf(str, "bump_push_xoruint"); break;
	case bump_push_xorulong:	sprintf(str, "bump_push_xorulong"); break;
	case bump_push_xorllong:	sprintf(str, "bump_push_xorllong"); break;

	case bump_push_andint:	sprintf(str, "bump_push_andint"); break;
	case bump_push_andlong:	sprintf(str, "bump_push_andlong"); break;
	case bump_push_anduint:	sprintf(str, "bump_push_anduint"); break;
	case bump_push_andulong:	sprintf(str, "bump_push_andulong"); break;
	case bump_push_andllong:	sprintf(str, "bump_push_andllong"); break;

	case bump_push_reffloat:	sprintf(str, "bump_push_reffloat"); break;
	case bump_push_refshort:	sprintf(str, "bump_push_refshort"); break;
	case bump_push_refchar:	sprintf(str, "bump_push_refchar"); break;
	case bump_push_refint:	sprintf(str, "bump_push_refint"); break;
	case bump_push_refuint:	sprintf(str, "bump_push_refuint"); break;
	case bump_push_reflong:	sprintf(str, "bump_push_reflong"); break;
	case bump_push_refulong:	sprintf(str, "bump_push_refulong"); break;
	case bump_push_refllong:	sprintf(str, "bump_push_refllong"); break;
	case bump_push_refdouble:	sprintf(str, "bump_push_refdouble"); break;
	case bump_push_refptr:	sprintf(str, "bump_push_refptr"); break;
	case bump_push_refuptr:	sprintf(str, "bump_push_refuptr"); break;

	case bump_push_addptr2int:	sprintf(str, "bump_push_addptr2int"); break;
	case bump_push_addint2ptr:	sprintf(str, "bump_push_addint2ptr"); break;
	case bump_push_subptrint:	sprintf(str, "bump_push_subptrint"); break;
	case bump_push_subptr:	sprintf(str, "bump_push_subptr"); break;
	
	


	case push_addint:	sprintf(str, "push_addint"); break;
	case push_adduint:	sprintf(str, "push_adduint"); break;
	case push_addlong:	sprintf(str, "push_addlong"); break;
	case push_addulong:	sprintf(str, "push_addulong"); break;
	case push_addllong:	sprintf(str, "push_addllong"); break;
	case push_adddouble:	sprintf(str, "push_adddouble"); break;

	case push_subint:	sprintf(str, "push_subint"); break;
	case push_subuint:	sprintf(str, "push_subuint"); break;
	case push_sublong:	sprintf(str, "push_sublong"); break;
	case push_subulong:	sprintf(str, "push_subulong"); break;
	case push_subllong:	sprintf(str, "push_subllong"); break;
	case push_subdouble:	sprintf(str, "push_subdouble"); break;

	case push_divint:	sprintf(str, "push_divint"); break;
	case push_divuint:	sprintf(str, "push_divuint"); break;
	case push_divlong:	sprintf(str, "push_divlong"); break;
	case push_divulong:	sprintf(str, "push_divulong"); break;
	case push_divllong:	sprintf(str, "push_divllong"); break;
	case push_divdouble:	sprintf(str, "push_divdouble"); break;

	case push_multint:	sprintf(str, "push_multint"); break;
	case push_multuint:	sprintf(str, "push_multuint"); break;
	case push_multlong:	sprintf(str, "push_multlong"); break;
	case push_multulong:	sprintf(str, "push_multulong"); break;
	case push_multllong:	sprintf(str, "push_multllong"); break;
	case push_multdouble:	sprintf(str, "push_multdouble"); break;

	case push_modint:	sprintf(str, "push_modint"); break;
	case push_moduint:	sprintf(str, "push_moduint"); break;
	case push_modlong:	sprintf(str, "push_modlong"); break;
	case push_modulong:	sprintf(str, "push_modulong"); break;
	case push_modllong:	sprintf(str, "push_modllong"); break;

	case push_lshtint:	sprintf(str, "push_lshtint"); break;
	case push_lshtuint:	sprintf(str, "push_lshtuint"); break;
	case push_lshtlong:	sprintf(str, "push_lshtlong"); break;
	case push_lshtulong:	sprintf(str, "push_lshtulong"); break;
	case push_lshtllong:	sprintf(str, "push_lshtllong"); break;

	case push_rshtint:	sprintf(str, "push_rshtint"); break;
	case push_rshtuint:	sprintf(str, "push_rshtuint"); break;
	case push_rshtlong:	sprintf(str, "push_rshtlong"); break;
	case push_rshtulong:	sprintf(str, "push_rshtulong"); break;
	case push_rshtllong:	sprintf(str, "push_rshtllong"); break;

	case push_ltint:	sprintf(str, "push_ltint"); break;
	case push_ltlong:	sprintf(str, "push_ltlong"); break;
	case push_ltuint:	sprintf(str, "push_ltuint"); break;
	case push_ltulong:	sprintf(str, "push_ltulong"); break;
	case push_ltllong:	sprintf(str, "push_ltllong"); break;
	case push_ltdouble:	sprintf(str, "push_ltdouble"); break;
	case push_ltptr:	sprintf(str, "push_ltptr"); break;

	case push_leint:	sprintf(str, "push_leint"); break;
	case push_lelong:	sprintf(str, "push_lelong"); break;
	case push_leuint:	sprintf(str, "push_leuint"); break;
	case push_leulong:	sprintf(str, "push_leulong"); break;
	case push_lellong:	sprintf(str, "push_lellong"); break;
	case push_ledouble:	sprintf(str, "push_ledouble"); break;
	case push_leptr:	sprintf(str, "push_leptr"); break;

	case push_eqint:	sprintf(str, "push_eqint"); break;
	case push_eqlong:	sprintf(str, "push_eqlong"); break;
	case push_equint:	sprintf(str, "push_equint"); break;
	case push_equlong:	sprintf(str, "push_equlong"); break;
	case push_eqllong:	sprintf(str, "push_eqllong"); break;
	case push_eqdouble:	sprintf(str, "push_eqdouble"); break;
	case push_eqptr:	sprintf(str, "push_eqptr"); break;

	case push_neint:	sprintf(str, "push_neint"); break;
	case push_nelong:	sprintf(str, "push_nelong"); break;
	case push_neuint:	sprintf(str, "push_neuint"); break;
	case push_neulong:	sprintf(str, "push_neulong"); break;
	case push_nellong:	sprintf(str, "push_nellong"); break;
	case push_nedouble:	sprintf(str, "push_nedouble"); break;
	case push_neptr:	sprintf(str, "push_neptr"); break;

	case push_gtint:	sprintf(str, "push_gtint"); break;
	case push_gtlong:	sprintf(str, "push_gtlong"); break;
	case push_gtuint:	sprintf(str, "push_gtuint"); break;
	case push_gtulong:	sprintf(str, "push_gtulong"); break;
	case push_gtllong:	sprintf(str, "push_gtllong"); break;
	case push_gtdouble:	sprintf(str, "push_gtdouble"); break;
	case push_gtptr:	sprintf(str, "push_gtptr"); break;

	case push_geint:	sprintf(str, "push_geint"); break;
	case push_gelong:	sprintf(str, "push_gelong"); break;
	case push_geuint:	sprintf(str, "push_geuint"); break;
	case push_geulong:	sprintf(str, "push_geulong"); break;
	case push_gellong:	sprintf(str, "push_gellong"); break;
	case push_gedouble:	sprintf(str, "push_gedouble"); break;
	case push_geptr:	sprintf(str, "push_geptr"); break;

	case push_lorint:	sprintf(str, "push_lorint"); break;
	case push_lorlong:	sprintf(str, "push_lorlong"); break;
	case push_loruint:	sprintf(str, "push_loruint"); break;
	case push_lorulong:	sprintf(str, "push_lorulong"); break;
	case push_lorllong:	sprintf(str, "push_lorllong"); break;
	case push_lordouble:	sprintf(str, "push_lordouble"); break;
	case push_lorptr:	sprintf(str, "push_lorptr"); break;

	case push_borint:	sprintf(str, "push_borint"); break;
	case push_borlong:	sprintf(str, "push_borlong"); break;
	case push_boruint:	sprintf(str, "push_boruint"); break;
	case push_borulong:	sprintf(str, "push_borulong"); break;
	case push_borllong:	sprintf(str, "push_borllong"); break;

	case push_xorint:	sprintf(str, "push_xorint"); break;
	case push_xorlong:	sprintf(str, "push_xorlong"); break;
	case push_xoruint:	sprintf(str, "push_xoruint"); break;
	case push_xorulong:	sprintf(str, "push_xorulong"); break;
	case push_xorllong:	sprintf(str, "push_xorllong"); break;

	case push_andint:	sprintf(str, "push_andint"); break;
	case push_andlong:	sprintf(str, "push_andlong"); break;
	case push_anduint:	sprintf(str, "push_anduint"); break;
	case push_andulong:	sprintf(str, "push_andulong"); break;
	case push_andllong:	sprintf(str, "push_andllong"); break;

	case push_reffloat:	sprintf(str, "push_reffloat"); break;
	case push_refshort:	sprintf(str, "push_refshort"); break;
	case push_refchar:	sprintf(str, "push_refchar"); break;
	case push_refint:	sprintf(str, "push_refint"); break;
	case push_refuint:	sprintf(str, "push_refuint"); break;
	case push_reflong:	sprintf(str, "push_reflong"); break;
	case push_refulong:	sprintf(str, "push_refulong"); break;
	case push_refllong:	sprintf(str, "push_refllong"); break;
	case push_refdouble:	sprintf(str, "push_refdouble"); break;
	case push_refptr:	sprintf(str, "push_refptr"); break;
	case push_refuptr:	sprintf(str, "push_refuptr"); break;

	case push_addptr2int:	sprintf(str, "push_addptr2int"); break;
	case push_addint2ptr:	sprintf(str, "push_addint2ptr"); break;
	case push_subptrint:	sprintf(str, "push_subptrint"); break;
	case push_subptr:	sprintf(str, "push_subptr"); break;


    default:
      sprintf(str, "UNKNOWN %d", inst->opcode);
  }

  if(s) //maks
  {
	  if(i == -1)
	  {
		  fprintf(fp, "%s", s);
	  }
	  else
	  {
		  fprintf(fp, "%-20.20s", s);
	  }
  }
  else
  {
	  if(i == -1)
	  {
		  char *strchr( const char *string, int c );
		  char *c = strchr(str, ' ');
		  if(c) *c = 0;

		  fprintf(fp, "%s", str);
	  }
	  else
	  {
		fprintf(fp, "%-20.20s", str);
	  }
  }
}

#ifndef STAND_ALONE_GAME    
void EiC_listcode(code_t * CODE)
{
    extern int EiC_lineNums;
    int i, j, t, k, cols, rows, sp;
    if (CODE->nextinst < 11)
	cols = 1;
    else if (CODE->nextinst < 21)
	cols = 2;
    else
	cols = 3;
    
    rows = CODE->nextinst / cols;
    if (CODE->nextinst % cols)
	rows++;

    if(EiC_lineNums)
	sp = 30;
    else
	sp = 25;
    
    for (j = 0; j < rows; ++j) {
	for (t = 0, i = j; i < CODE->nextinst; i += rows, t++) {
	    if(EiC_lineNums)
		printf("%4d:", CODE->inst[i].line);
	    printf("%4d:", i);
	    showcode(stdout, &CODE->inst[i], i); //maks
	    if (CODE->inst[i].opcode == jmptab) {
		showjmptab(&CODE->inst[i], i, t * sp);
		t = (t + 1) * sp;
		for (k = 0; k < t; k++)
		    printf(" ");
	    }
	}
	printf("\n");
    }
    printf("\n");
}
#endif

#endif

/* END EMITTER.C
   -----------------
   ----------------- */








