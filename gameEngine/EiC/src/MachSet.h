/* MachSet.h
 *
 *	(C) Copyright May  7 1995, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */

typedef enum { /* Stack Machine Instruction Set */
  halt = 400,

  bump,

  /* If jumps are altered, added to or rearranged,
   * then check with the routine analyseCode -
   * in file optomizer.c
   */

  __eicsetjmp, __eiclongjmp,

  jmptab, jmpu,
  jmpFint, jmpFlng,  jmpFdbl, jmpFptr, jmpFllng,
  jmpTint, jmpTlng,  jmpTdbl, jmpTptr, jmpTllng,
  

  dupval,dupdown,
  lval,

  ixa,

  /* get operators */
  rvalchar,rvaluchar,
  rvalshort,rvalushort,
  rvalint,rvaluint,
  rvallong,rvalulong,
  rvalllong,
  rvalptr,rvaluptr,
  rvalfloat,rvaldouble,

 
  /* store operator */
  stochar,
  stoshort,
  stoint,stouint,
  stolong,stoulong,
  stollong,stouptr,
  stoptr,
  stofloat,stodouble,
  stoval,


  incdouble, decdouble,
  pushdouble,
  drefdouble, refdouble,
  negdouble, adddouble, subdouble, divdouble, multdouble,
  ltdouble, ledouble, eqdouble, nedouble, gtdouble,
  gedouble, lordouble, landdouble, notdouble,
  double2char, double2int, double2long, double2llong, double2float,


  incint, decint,
  pushint,
  drefint, refint,
  negint, addint, subint, divint, multint,
  ltint, leint, eqint, neint, gtint,
  geint, lorint, landint, notint, andint, borint,
  xorint, modint, lshtint, rshtint, compint,
  int2uchar, int2ushort, int2ptr, int2double,
  int2long, int2llong,

  incuint, decuint,
    pushuint,
  drefuint, refuint,
  neguint, adduint, subuint, divuint, multuint,
  ltuint, leuint, equint, neuint, gtuint,
  geuint, loruint, notuint, anduint, boruint,
  xoruint, moduint, lshtuint, rshtuint, compuint,
  uint2char, uint2ptr, uint2double,
  uint2long, uint2llong,

  inclong, declong,
    pushlong,
  dreflong, reflong,
  neglong, addlong, sublong, divlong, multlong,
  ltlong, lelong, eqlong, nelong, gtlong,
  gelong, lorlong, landlong, notlong, andlong, borlong,
  xorlong, modlong, lshtlong, rshtlong, complong,
  long2char, long2int, long2ptr, long2double,
  long2llong,

  inculong, deculong,
    pushulong,
  drefulong, refulong,
  negulong, addulong, subulong, divulong, multulong,
  ltulong, leulong, equlong, neulong, gtulong,
  geulong, lorulong, notulong, andulong, borulong,
  xorulong, modulong, lshtulong, rshtulong, compulong,
  ulong2char, ulong2int, ulong2ptr, ulong2double,
  ulong2llong,

  incllong, decllong,
    pushllong,
  drefllong, refllong,
  negllong, addllong, subllong, divllong, multllong,
  ltllong, lellong, eqllong, nellong, gtllong,
  gellong, lorllong, landllong, notllong, andllong, borllong,
  xorllong, modllong, lshtllong, rshtllong, compllong,
  llong2char, llong2int, llong2long, llong2ptr, llong2double,
  

  drefptr, refptr, incptr, decptr,
        pushptr,
  addptr2int, addint2ptr, subptrint,
  subptr, divptr,
  ltptr, leptr, eqptr, neptr, gtptr,
  geptr, lorptr, landptr, notptr,
  ptr2int, ptr2long,

  drefuchar, drefchar, refchar,  neguchar,
   
  
  char2int, char2ptr, char2double,
  
  drefushort, drefshort, refshort,
   
  
  
  dreffloat, reffloat,
   

  assigntype,  pushval,
  massign,  fmem, mset, minit,
  refmem, reducear, checkar, checkar2,
  call, eiccall, endfunc, eicreturn, empty,

  lda,


  /* unique unsafe pointer instructions */
    drefuptr, refuptr,

  /*maks: big instructions (super operators)*/
  rvaluptr_bump,
  bump_pushlong,
  rvaluptr_dreflong,
  lelong_jmpFint,
  bump_pushint,
  drefdouble_stoval,
  rvalint_bump,
  pushlong_lelong_jmpFint,
  

	
	bump_push_addint,
	bump_push_adduint,	
	bump_push_addlong,	
	bump_push_addulong,
	bump_push_addllong,
	bump_push_adddouble,

	bump_push_subint,	
	bump_push_subuint,	
	bump_push_sublong,	
	bump_push_subulong,
	bump_push_subllong,
	bump_push_subdouble,
	
	bump_push_divint,	
	bump_push_divuint,	
	bump_push_divlong,	
	bump_push_divulong,
	bump_push_divllong,
	bump_push_divdouble,

	bump_push_multint,
	bump_push_multuint,
	bump_push_multlong,
	bump_push_multulong,
	bump_push_multllong,
	bump_push_multdouble,

	bump_push_modint,	
	bump_push_moduint,	
	bump_push_modlong,	
	bump_push_modulong,
	bump_push_modllong,

	bump_push_lshtint,	
	bump_push_lshtuint,
	bump_push_lshtlong,
	bump_push_lshtulong,
	bump_push_lshtllong,

	bump_push_rshtint,	
	bump_push_rshtuint,
	bump_push_rshtlong,
	bump_push_rshtulong,
	bump_push_rshtllong,

	bump_push_ltint,
	bump_push_ltuint,	
	bump_push_ltlong,	
	bump_push_ltulong,	
	bump_push_ltllong,	
	bump_push_ltdouble,
	bump_push_ltptr,
  
	bump_push_leint,
	bump_push_leuint,	
	bump_push_lelong,	
	bump_push_leulong,	
	bump_push_lellong,	
	bump_push_ledouble,
	bump_push_leptr,

	bump_push_eqint,
	bump_push_equint,	
	bump_push_eqlong,	
	bump_push_equlong,	
	bump_push_eqllong,	
	bump_push_eqdouble,
	bump_push_eqptr,

	bump_push_neint,
	bump_push_neuint,	
	bump_push_nelong,	
	bump_push_neulong,	
	bump_push_nellong,	
	bump_push_nedouble,
	bump_push_neptr,

	bump_push_gtint,
	bump_push_gtuint,	
	bump_push_gtlong,	
	bump_push_gtulong,	
	bump_push_gtllong,	
	bump_push_gtdouble,
	bump_push_gtptr,

	bump_push_geint,
	bump_push_geuint,	
	bump_push_gelong,	
	bump_push_geulong,	
	bump_push_gellong,	
	bump_push_gedouble,
	bump_push_geptr,

	bump_push_lorint,
	bump_push_loruint,	
	bump_push_lorlong,	
	bump_push_lorulong,	
	bump_push_lorllong,	
	bump_push_lordouble,
	bump_push_lorptr,

	bump_push_borint,
	bump_push_boruint,	
	bump_push_borlong,	
	bump_push_borulong,	
	bump_push_borllong,

	bump_push_xorint,
	bump_push_xoruint,	
	bump_push_xorlong,	
	bump_push_xorulong,	
	bump_push_xorllong,

	bump_push_andint,
	bump_push_anduint,	
	bump_push_andlong,	
	bump_push_andulong,	
	bump_push_andllong,
	
	bump_push_reffloat,
	bump_push_refshort,
	bump_push_refchar,	
	bump_push_refint,	
	bump_push_refuint,	
	bump_push_reflong,	
	bump_push_refulong,
	bump_push_refllong,
	bump_push_refdouble,
	bump_push_refptr,
	bump_push_refuptr,

	bump_push_addptr2int,
	bump_push_addint2ptr,	
	bump_push_subptrint,	
	bump_push_subptr,
	



	push_addint,
	push_adduint,	
	push_addlong,	
	push_addulong,
	push_addllong,
	push_adddouble,

	push_subint,	
	push_subuint,	
	push_sublong,	
	push_subulong,
	push_subllong,
	push_subdouble,
	
	push_divint,	
	push_divuint,	
	push_divlong,	
	push_divulong,
	push_divllong,
	push_divdouble,

	push_multint,
	push_multuint,
	push_multlong,
	push_multulong,
	push_multllong,
	push_multdouble,

	push_modint,	
	push_moduint,	
	push_modlong,	
	push_modulong,
	push_modllong,

	push_lshtint,	
	push_lshtuint,
	push_lshtlong,
	push_lshtulong,
	push_lshtllong,

	push_rshtint,	
	push_rshtuint,
	push_rshtlong,
	push_rshtulong,
	push_rshtllong,

	push_ltint,
	push_ltuint,	
	push_ltlong,	
	push_ltulong,	
	push_ltllong,	
	push_ltdouble,
	push_ltptr,
  
	push_leint,
	push_leuint,	
	push_lelong,	
	push_leulong,	
	push_lellong,	
	push_ledouble,
	push_leptr,

	push_eqint,
	push_equint,	
	push_eqlong,	
	push_equlong,	
	push_eqllong,	
	push_eqdouble,
	push_eqptr,

	push_neint,
	push_neuint,	
	push_nelong,	
	push_neulong,	
	push_nellong,	
	push_nedouble,
	push_neptr,

	push_gtint,
	push_gtuint,	
	push_gtlong,	
	push_gtulong,	
	push_gtllong,	
	push_gtdouble,
	push_gtptr,

	push_geint,
	push_geuint,	
	push_gelong,	
	push_geulong,	
	push_gellong,	
	push_gedouble,
	push_geptr,

	push_lorint,
	push_loruint,	
	push_lorlong,	
	push_lorulong,	
	push_lorllong,	
	push_lordouble,
	push_lorptr,

	push_borint,
	push_boruint,	
	push_borlong,	
	push_borulong,	
	push_borllong,

	push_xorint,
	push_xoruint,	
	push_xorlong,	
	push_xorulong,	
	push_xorllong,

	push_andint,
	push_anduint,	
	push_andlong,	
	push_andulong,	
	push_andllong,
	
	push_reffloat,
	push_refshort,
	push_refchar,	
	push_refint,	
	push_refuint,	
	push_reflong,	
	push_refulong,
	push_refllong,
	push_refdouble,
	push_refptr,
	push_refuptr,

	push_addptr2int,
	push_addint2ptr,	
	push_subptrint,	
	push_subptr



} MachSet_t;


#define IsSto_op(x)  ((x) >= stochar && (x)<=stoval)
#define IsGet_op(x)  ((x) >= rvalchar && (x)<=rvaldouble)









