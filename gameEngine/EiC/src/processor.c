//Must be included into interpre.c


	switch (ip->opcode) {
	case bump:
	      AdjustTop(ip->val.ival);
	    ToP += ip->val.ival;
	    break;
	case jmpFint: jmpfTYPE(ival); break;
	case jmpFlng: jmpfTYPE(lval); break;
	case jmpFdbl: jmpfTYPE(dval); break;
	case jmpFptr: jmpfTYPE(p.p); break;
	case jmpFllng: jmpfTYPE(llval); break;
	      
	case jmpTint: jmptTYPE(ival); break;
	case jmpTlng: jmptTYPE(lval); break;
	case jmpTdbl: jmptTYPE(dval); break;
	case jmpTptr: jmptTYPE(p.p); break;
	case jmpTllng: jmptTYPE(llval); break;

	case jmpu: ip += ip->val.ival - 1; break;
	case dupval:
	    /* should really adjustTop here !!*/
	    STK[ToP + 1].v = STK[ToP].v;
	    ToP += ip->val.ival;
	    break;
	case jmptab:
	{
	      struct {
		  int n;
		  val_t *loc;
	      } *tab;
	      int i;
	      tab = ip->val.p.p;
	      for (i = 1; i < tab->n; i += 2)
		  if (tab->loc[i].ival == STK[ToP].v.ival) {
		      ip += tab->loc[i + 1].ival - 1;
		      break;
		  }
	      if (i >= tab->n)
		  ip += tab->loc[0].ival - 1;
	  }
	    break;


	  /* specific float stuff */

	case dreffloat: drefTYPE(float, dval); break;
	case reffloat: refTYPE(float, dval); break;
	case rvalfloat:
	    STK[ToP].v.dval=AR[ip->ext][ip->val.ival].v.fval;
	    break;
	case stofloat:
	    AR[ip->ext][ip->val.ival].v.fval = (float)STK[ToP].v.dval;
	    break;

	    
	  /* specific short stuff */
	case rvalshort:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.sval;
	    break;
	case rvalushort:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.usval;
	    break;
	case drefushort: drefTYPE(unsigned short, ival); break; 
	case drefshort: drefTYPE(short, ival); break;
	case refshort: refTYPE(short, ival); break;
	case stoshort:
	    AR[ip->ext][ip->val.ival].v.sval = STK[ToP].v.ival;
	    break;

	  /* specific char stuff */ 
	case rvalchar:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.cval;
	    break;
	case rvaluchar:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.ucval;
	    break;
	case stochar:
	    AR[ip->ext][ip->val.ival].v.cval = STK[ToP].v.ival;
	    break;
	case drefuchar: drefTYPE(unsigned char, ival); break;
	case drefchar: drefTYPE(char, ival); break;
	case refchar: refTYPE(char, ival); break;
	case neguchar: STK[ToP].v.uival = 256 - STK[ToP].v.uival; break;

	  /* specific int stuff */
	case incint: STK[ToP].v.ival += ip->val.ival; break;
	case decint: STK[ToP].v.ival -= ip->val.ival; break;
	case drefint: drefTYPE(int, ival); break;
	case refint: refTYPE(int, ival); break;
	case stoint: stoTYPE(ival); break;
	case rvalint: rvalTYPE(ival); break;
	case pushint: pushTYPE(ival); break;
	case negint: negTYPE(ival); break;
	case addint: addTYPE(ival); break;
	case subint: subTYPE(ival); break;
	case divint: divTYPE(ival); break;
	case multint: multTYPE(ival); break;
	case modint: modTYPE(ival); break;
	case lshtint: lshtTYPE(ival); break;
	case rshtint: rshtTYPE(ival); break;
	case ltint: ltTYPE(ival); break;
	case leint: leTYPE(ival); break;
	case eqint: eqTYPE(ival); break;
	case neint: neTYPE(ival); break;
	case gtint: gtTYPE(ival); break;
	case geint: geTYPE(ival); break;
	case lorint: lorTYPE(ival); break;
	case landint: landTYPE(ival); break;
	case notint: notTYPE(ival); break;
	case borint: borTYPE(ival); break;
	case xorint: xorTYPE(ival); break;
	case andint: andTYPE(ival); break;
	case compint: compTYPE(ival); break;
	case int2double: castTYPES(ival, dval, double); break;
	case int2ptr: castTYPES(ival, p.p, void *); break;
	case int2long: castTYPES(ival, lval, long); break;
	case int2llong: castTYPES(ival, llval, eic_llong); break;
	case int2uchar: castTYPES(ival, lval, unsigned char); break;
	case int2ushort: castTYPES(ival, lval, unsigned short); break;
	    
	  /* unsigned int stuff */
	case incuint: STK[ToP].v.uival += ip->val.uival; break;
	case decuint: STK[ToP].v.uival -= ip->val.uival; break;
	case drefuint: drefTYPE(unsigned, uival); break;
	case refuint: refTYPE(unsigned, uival); break;
	case stouint: stoTYPE(uival); break;
	case rvaluint: rvalTYPE(uival); break;
	case pushuint: pushTYPE(uival); break;
	case neguint: negTYPE(uival); break;
	case adduint: addTYPE(uival); break;
	case subuint: subTYPE(uival); break;
	case divuint: divTYPE(uival); break;
	case multuint: multTYPE(uival); break;
	case moduint: modTYPE(uival); break;
	case lshtuint: lshtTYPE(uival); break;
	case rshtuint: rshtTYPE(uival); break;
	case ltuint: ltTYPE(uival); break;
	case leuint: leTYPE(uival); break;
	case equint: eqTYPE(uival); break;
	case neuint: neTYPE(uival); break;
	case gtuint: gtTYPE(uival); break;
	case geuint: geTYPE(uival); break;
	case loruint: lorTYPE(uival); break;

	case notuint: notTYPE(uival); break;
	case boruint: borTYPE(uival); break;
	case xoruint: xorTYPE(uival); break;
	case anduint: andTYPE(uival); break;
	case compuint: compTYPE(uival); break;
	case uint2double: castTYPES(uival, dval, double); break;
	case uint2ptr: castTYPES(uival, p.p, void *); break;
	case uint2long: castTYPES(uival, lval, long); break;
	case uint2llong: castTYPES(uival, llval, eic_llong); break;

	  /* specific long stuff */
	case inclong: STK[ToP].v.lval += ip->val.ival; break;
	case declong: STK[ToP].v.lval -= ip->val.ival; break;
	case dreflong: drefTYPE(long, lval); break;
	case reflong: refTYPE(long, lval); break;
	case stolong: stoTYPE(lval); break;
	case rvallong: rvalTYPE(lval); break;
	case pushlong: pushTYPE(lval); break;
	case neglong: negTYPE(lval); break;
	case addlong: addTYPE(lval); break;
	case sublong: subTYPE(lval); break;
	case divlong: divTYPE(lval); break;
	case multlong: multTYPE(lval); break;
	case modlong: modTYPE(lval); break;
	case lshtlong: lshtTYPE(lval); break;
	case rshtlong: rshtTYPE(lval); break;
	case ltlong: ltTYPE(lval); break;
	case lelong: leTYPE(lval); break;
	case eqlong: eqTYPE(lval); break;
	case nelong: neTYPE(lval); break;
	case gtlong: gtTYPE(lval); break;
	case gelong: geTYPE(lval); break;
	case lorlong: lorTYPE(lval); break;
	case landlong: landTYPE(lval); break;
	case notlong: notTYPE(lval); break;
	case borlong: borTYPE(lval); break;
	case xorlong: xorTYPE(lval); break;
	case andlong: andTYPE(lval); break;
	case complong: compTYPE(lval); break;
	case long2double: castTYPES(lval, dval, double); break;
	case long2ptr: castTYPES(lval, p.p, void *); break;
	case long2llong: castTYPES(lval, llval, eic_llong); break;
	case long2int: castTYPES(lval, ival, int); break;

	    /* unsigned long stuff */
	case inculong: STK[ToP].v.ulval += ip->val.ival; break;
	case deculong: STK[ToP].v.ulval -= ip->val.ival; break;
	case drefulong: drefTYPE(unsigned long, ulval); break;
	case refulong: refTYPE(unsigned long, ulval); break;
	case stoulong: stoTYPE(ulval); break;
	case rvalulong: rvalTYPE(ulval); break;
	case pushulong: pushTYPE(ulval); break;
	case negulong: negTYPE(ulval); break;
	case addulong: addTYPE(ulval); break;
	case subulong: subTYPE(ulval); break;
	case divulong: divTYPE(ulval); break;
	case multulong: multTYPE(ulval); break;
	case modulong: modTYPE(ulval); break;
	case lshtulong: lshtTYPE(ulval); break;
	case rshtulong: rshtTYPE(ulval); break;
	case ltulong: ltTYPE(ulval); break;
	case leulong: leTYPE(ulval); break;
	case equlong: eqTYPE(ulval); break;
	case neulong: neTYPE(ulval); break;
	case gtulong: gtTYPE(ulval); break;
	case geulong: geTYPE(ulval); break;
	case lorulong: lorTYPE(ulval); break;

	case notulong: notTYPE(ulval); break;
	case borulong: borTYPE(ulval); break;
	case xorulong: xorTYPE(ulval); break;
	case andulong: andTYPE(ulval); break;
	case compulong: compTYPE(ulval); break;
	case ulong2double: castTYPES(ulval, dval, double); break;
	case ulong2ptr: castTYPES(ulval, p.p, void *); break;
	case ulong2int: castTYPES(ulval, ival, int); break;
	case ulong2llong: castTYPES(ulval, llval, eic_llong); break;



	  /* specific long long stuff */
	case incllong: STK[ToP].v.llval += ip->val.ival; break;
	case decllong: STK[ToP].v.llval -= ip->val.ival; break;
	case drefllong: drefTYPE(eic_llong, llval); break;
	case refllong: refTYPE(eic_llong, llval); break;
	case stollong: stoTYPE(llval); break;
	case rvalllong: rvalTYPE(llval); break;
	case pushllong: pushTYPE(llval); break;
	case negllong: negTYPE(llval); break;
	case addllong: addTYPE(llval); break;
	case subllong: subTYPE(llval); break;
	case divllong: divTYPE(llval); break;
	case multllong: multTYPE(llval); break;
	case modllong: modTYPE(llval); break;
	case lshtllong: lshtTYPE(llval); break;
	case rshtllong: rshtTYPE(llval); break;
	case ltllong: ltTYPE(llval); break;
	case lellong: leTYPE(llval); break;
	case eqllong: eqTYPE(llval); break;
	case nellong: neTYPE(llval); break;
	case gtllong: gtTYPE(llval); break;
	case gellong: geTYPE(llval); break;
	case lorllong: lorTYPE(llval); break;
	case landllong: landTYPE(llval); break;
	case notllong: notTYPE(llval); break;
	case borllong: borTYPE(llval); break;
	case xorllong: xorTYPE(llval); break;
	case andllong: andTYPE(llval); break;
	case compllong: compTYPE(llval); break;
	case llong2double: castTYPES(llval, dval, double); break;
	case llong2ptr: castTYPES(llval, p.p, void *); break;
	case llong2int: castTYPES(llval, ival, int); break;
	case llong2long: castTYPES(llval, lval, long); break;


	    /* specific double stuff */
	case incdouble:STK[ToP].v.dval += ip->val.ival; break;
	case decdouble:STK[ToP].v.dval -= ip->val.ival; break;
	case drefdouble: drefTYPE(double, dval); break;
	case refdouble: refTYPE(double, dval); break;
	case stodouble: stoTYPE(dval); break;
	case rvaldouble: rvalTYPE(dval); break;
	case pushdouble: pushTYPE(dval); break;
	case negdouble: negTYPE(dval); break;
	case adddouble: addTYPE(dval); break;
	case subdouble: subTYPE(dval); break;
	case divdouble: divTYPE(dval); break;
	case multdouble: multTYPE(dval); break;
	case ltdouble: ltTYPE(dval); break;
	case ledouble: leTYPE(dval); break;
	case eqdouble: eqTYPE(dval); break;
	case nedouble: neTYPE(dval); break;
	case gtdouble: gtTYPE(dval); break;
	case gedouble: geTYPE(dval); break;
	case lordouble: lorTYPE(dval); break;
	case landdouble: landTYPE(dval); break;
	case notdouble: notTYPE(dval); break;

	case double2int: castTYPES(dval, uival, unsigned int); break;
	case double2long: castTYPES(dval, ulval, unsigned long); break;
	case double2llong: castTYPES(dval, llval, eic_llong); break;
	case double2float: castTYPES(dval, fval, float); break;


	  /*specific pointer stuff */
	case incptr: STK[ToP].v.p.p = (char *) STK[ToP].v.p.p +
			 ip->val.ival; break;
	case decptr: STK[ToP].v.p.p = (char *) STK[ToP].v.p.p -
			 ip->val.ival; break;
	case lda:
#if 0
	    STK[ToP].v.p = AR[1][ip->val.ival].v.p;
	    STK[ToP].v.p.p = (char*)STK[ToP].v.p.sp + ip->ext;
	    STK[ToP].v.p.sp = STK[ToP].v.p.p;
#else

	    {
		ptr_t *q = &AR[1][ip->val.ival].v.p;
		ptr_t *a = &STK[ToP].v.p;
		
		a->p   = a->sp = (char*)q->sp + ip->ext;
		a->ep  = q->ep;

		if(vp) { /* patch previous lda assignment */
		    ((ptr_t*)vp)->ep = (char*)a->p;
		}
		/* Take advantage of the fact that the next InSt
		 * has the location of where `a' is to be stored.
		 */
		vp = &AR[1][(ip+1)->val.ival].v.p;
		
	    }
	    
#endif
	    
	    break;
	case ixa:
	    ToP--;
	    STK[ToP].v.p.p = (char *) STK[ToP].v.p.p + STK[ToP+1].v.ival*ip->val.ival;
	    break;
	case addptr2int: ToP--;
	    STK[ToP].v.p.p = (char *) STK[ToP].v.p.p + STK[ToP + 1].v.ival; break;
	case addint2ptr: ToP--;
	    STK[ToP].v.p.p = STK[ToP].v.ival
		+ (char *) STK[ToP+1].v.p.p; break;
	case subptrint: ToP--;
	    STK[ToP].v.p.p = (char *) STK[ToP].v.p.p - STK[ToP + 1].v.ival; break;
	case subptr:
	    ToP--;
	    STK[ToP].v.ival =
		(int) ((long) STK[ToP].v.p.p - (long) STK[ToP+1].v.p.p);
	    break;

	case drefptr:   drefTYPE(ptr_t, p); break;

	case drefuptr:  drefTYPE(void**,p.p);
	                STK[ToP].v.p.sp = 0;
			STK[ToP].v.p.ep = (void *) ULONG_MAX;
			break;

	case refptr: refTYPE(ptr_t, p); break;
	case refuptr: refTYPE(void *, p.p); break;

	case stoptr: stoTYPE(p); break;
	case stouptr: stoTYPE(p.p); break;
	    
	case rvalptr: rvalTYPE(p); break;
	case rvaluptr: rvalTYPE(p);
	  STK[ToP].v.p.sp = 0;
	  STK[ToP].v.p.ep = (void*) ULONG_MAX; /* set to a very high value */
	    break;

	case pushptr: pushTYPE(p); break;
	case ltptr: ltTYPE(p.p); break;
	case leptr: leTYPE(p.p); break;
	case eqptr: eqTYPE(p.p); break;
	case neptr: neTYPE(p.p); break;
	case gtptr: gtTYPE(p.p); break;
	case geptr: geTYPE(p.p); break;
	case lorptr: lorTYPE(p.p); break;
	case landptr: landTYPE(p.p); break;
	case notptr: notTYPE(p.p); break;
	case ptr2int: castTYPES(p.p, ival, int); break;
	case ptr2long: castTYPES(p.p, lval, long); break;
	    
	case lval: /* on the fly safe pointer */
	    STK[ToP].v.p.p = &AR[ip->ext][ip->val.ival].v;
	    STK[ToP].v.p.ep = (char *) STK[ToP].v.p.p + (size_t) 
		ip->val.p.ep;
	    STK[ToP].v.p.sp = STK[ToP].v.p.p;
	    break;
	case assigntype: assignTYPE; break;
	case stoval:
	    switch(ip->ext) {
	      case t_char:
	      case t_uchar: STK[ToP].v.cval = STK[ToP].v.ival; break;
	      case t_short:
	      case t_ushort: STK[ToP].v.sval = STK[ToP].v.ival; break;
	      case t_float:  STK[ToP].v.fval = STK[ToP].v.dval;
/*	      case t_struct:
	      case t_union:
		printf("stoVa1l with struct/union\n");
*/
	    }

	    stoVAL; break;
	case pushval: pushVAL; break;
	    
	case eiccall:

	    if(!((symentry_t*)STK[ToP - 1].v.p.p)->tag) {
		AdjustTop(6);
		STK[ToP + 1].v.ival = ip;
		STK[ToP + 1].type = (void*)EiC_CurrentFile; /* save file */
		STK[ToP + 2].v.p.p = InSt;
		STK[ToP + 3].v.lval = AR[1] - env->LAR;
		STK[ToP + 4].v.ival = env->lsp - STK[ToP].v.ival;
	    
		AR[1] = &env->LAR[env->lsp] /* - STK[ToP].v.ival] */ ;

		code = ! STK[ToP - 1].v.p.p ? NULL :
		    AR[0][((symentry_t *)
			   STK[ToP - 1].v.p.p)->val.ival].v.p.p;

		if (code == NULL) {
		    if(STK[ToP - 1].v.p.p) {
			EiC_formatMessage("Link error: undefined function :-> %s\n",
					  ((symentry_t *) STK[ToP - 1].v.p.p)->id);
		    } else
			EiC_formatMessage("Link error: possible usage of a function pointer"
					  " before assignment.\n");
		    env->lsp = 0;
		    //raise(SIGINT); //maks: non exit on error
			goto codeCrash;
		}
	    
		EiC_CurrentFile = codeName(code);

		InSt = code->inst;
		ip = InSt - 1;
		ToP += 5;
		
		//if(EiC_traceON) lastln = -1; //maks: performance

		break;
	    }

	case call: //maks
		{
		unsigned int ToPaux;

		/*argc = ARGC;
		hold_AR = AR[2];
		hold_AR1 = AR[1];
		ARGC = STK[ToP].v.ival;
		
		//AR[2] = &env->LAR[env->lsp - ARGC];
		AR[2] = &env->LAR[env->lsp];

		if(ip->ext)
			STK[ToP - 1].v = STK[ToP - 1].v.vfunc ();
		else
			STK[ToP - 1].v.vfunc();

		//env->lsp -= STK[ToP].v.ival;
		if(env->lsp >= STK[ToP].v.ival) //maks: direct execution
			env->lsp -= STK[ToP].v.ival;
		else
			env->lsp = 0; 

		ARGC = argc;
		AR[2] = hold_AR;
		AR[1] = hold_AR1;
		ToP--;*/
		
		//Save state into stack
		AdjustTop(2);
		STK[ToP + 1].v.ival = ARGC;
		STK[ToP + 2].v.ival = env->lsp;

		//Save into locals
		argc = ARGC;
		hold_AR = AR[2];
		hold_AR1 = AR[1];
		ARGC = STK[ToP].v.ival;
		
		//AR[2] = &env->LAR[env->lsp - ARGC];
		AR[2] = &env->LAR[env->lsp];

		ToPaux = ToP;
		ToP += 2;

		//Go
		if(ip->ext)
			STK[ToPaux - 1].v = STK[ToPaux - 1].v.vfunc ();
		else
			STK[ToPaux - 1].v.vfunc();


		//Restore state
		ToP -= 2;		// over write code pointer
		env->lsp = STK[ToP + 2].v.ival;
		argc = STK[ToP + 1].v.ival;



		//env->lsp -= STK[ToP].v.ival;
		if(env->lsp >= STK[ToP].v.ival) //maks: direct execution
			env->lsp -= STK[ToP].v.ival;
		else
			env->lsp = 0; 



		ARGC = argc;
		AR[2] = hold_AR;
		AR[1] = hold_AR1;

		ToP--;
		}
		break;

	case eicreturn:
		if(ToP == 0) //maks: direct execution
		{
			env->lsp = 0;
			ON = 0;  
			EiC_STaCK_VaLuE = STK[ToP].v; 
			goto codeEnd;
			break;
		}

	    ToP -= 6;		/* over write code pointer */

	    ip = STK[ToP + 2].v.ival;
	    EiC_CurrentFile = (char*)STK[ToP+2].type;
	    
	    InSt = STK[ToP + 3].v.p.p;
	
	    env->lsp = STK[ToP + 5].v.ival;
	    STK[ToP].v = STK[ToP + 6].v;

	    AR[1] = &env->LAR[STK[ToP + 4].v.ival];
	    /*if(EiC_traceON) //maks: performance
		{
			EiC_eicpop(&names,&v);
			EiC_formatMessage("\n[%s] ", (char*)v.p.p);
			lastln = -1;
	    }*/
	    break;


#if 1
	case __eiclongjmp:
	{

	    _EiC_jmp_buf * reg;

	    reg = (_EiC_jmp_buf *) ((char*)STK[ToP].v.p.p - STK[ToP+1].v.ival);

	    ip = reg->p;
	    EiC_CurrentFile = reg->file;
	    InSt = reg->inst;
	    env->lsp = reg->lsp;
	    AR[1] = &env->LAR[reg->offset]; /*reg->ar; */

	    if(STK[ToP+1].v.ival == 0)
		 STK[reg->top].v.ival = 1;
	    else
		 STK[reg->top].v.ival = STK[ToP+1].v.ival;

	    ToP = reg->top;


	}

	break;
	    
	case __eicsetjmp:
	{

	    _EiC_jmp_buf * reg;

	    reg = (_EiC_jmp_buf *) STK[ToP].v.p.p;

	    reg->p = ip;
	    reg->file = (void*)EiC_CurrentFile; /* save file */
	    reg->inst = InSt;
	    reg->lsp =  env->lsp;
	    reg->offset = AR[1] - env->LAR;
	    reg->ar = AR[1];
	    reg->top = ToP;

	    STK[ToP].v.ival = 0;

	}

	break;
#endif

        case massign:   
	    {
		val_t *v = &AR[ip->ext][ip->val.ival].v;
		v->p.sp = (void*)xcalloc(STK[ToP].v.ival,1);
		v->p.ep = (char*)v->p.sp + STK[ToP].v.ival;
		vp = NULL;
		
	    }
	    break;
	case fmem: FMEM; break;
	case refmem: refMEM; break;
	case minit:
	    memcpy(STK[ToP].v.p.p,ip->val.p.p, ip->ext); 
	    break;
	case reducear:
	    //env->lsp -= ip->val.ival;
		if(env->lsp >= ip->val.ival) //maks: direct execution
			env->lsp -= ip->val.ival;
		else
			env->lsp = 0;
	    break;
	case checkar:{
	      size_t d;
	      ptrdiff_t d2;
	      d = env->LARsize - env->lsp;
	      if (d < ip->val.ival) {
		  /* printf("expanding AR %d\n",env->LARsize);*/
		  d2 = (AR[1] - env->LAR);
		  env->LARsize += ip->val.ival - d;
		  env->LAR =
		      (AR_t *) xrealloc(env->LAR,
					env->LARsize * sizeof(AR_t));
		  AR[1] = &env->LAR[(size_t) d2];
	      }
	      if (ip->ext == 0) {
		  env->lsp += ip->val.ival;
		  /*AR[1][0].v.p.p = NULL;*/
	      }
	  }
	    break;
	case halt: 
		{
			STK[ToP].type = ip->val.p.p;
			ON = 0;  
			EiC_STaCK_VaLuE = STK[ToP].v; 
			goto codeEnd;
			break;
		}

	case empty: break;
	    
	}
	ip++;