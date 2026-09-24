//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    RegExp.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Regular expression parser
//
#include "StdAfx.h"
#include "RegExp.h"
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

#define	MAXPAT		1000 /* Max length of compiled pattern	*/
#define	TRUE		     1
#define	FALSE		     0
#define	ALMOST		  -1

/* Constant and token values						                */

#define	ENDSTR		_T('\0')	/* End of string		            */
#define	EOL		    _T('$')		/* End of line			            */
#define BOL		    _T('^')		/* Beginning of line		        */
#define	NEGATE		_T('^')		/* Negate a character class	    */
#define	CCL		    _T('[')		/* Class closing		            */
#define NCCL		  _T(']')		/* No Class closing		          */
#define CCLEND		_T(']')		/* Class closing end		        */
#define	ANY		    _T('.')		/* Any character		            */
#define	DASH		  _T('-')		/* Seperator in class length	  */
#define	OR		    _T('|')		/* Logical or for expression	  */
#define	LPAREN		_T('(')		/* Left parenthesis		          */
#define	RPAREN		_T(')')		/* Right parenthesis		        */
#define	POS_CLO		_T('+')		/* Aat least one or more char's	*/
#define	LITCHAR		_T('c')		/* Compiled literal		          */
#define	END_TERM	_T('e')		/* Compiled end of class	      */
#define	FS_DEFAULT	_T("[ \t]+")	/* Space default (white space)	*/

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4996) // Unsecure strcpy, strncpy, strcat and such.

RegExp::RegExp()
       :m_use_case(1),
        m_sql(0),
        M_CLOSURE(_T('*')),
        M_ZERO_ONE(_T('?')),
        M_ESCAPE(_T('\\'))
{
	_tcscpy_s(m_nfac_chars,8,_T("^|)]+*?"));
	return;
}

RegExp::RegExp(const CString& regexp)
       :m_use_case(1),
        m_sql(0),
        M_CLOSURE(_T('*')),
        M_ZERO_ONE(_T('?')),
        M_ESCAPE(_T('\\'))
{
	_tcscpy_s(m_nfac_chars,8,_T("^|)]+*?"));
	SetExp(regexp);
	return;
}

RegExp::RegExp(const CString& regexp,bool reg)
       :m_use_case(reg?0:1),
        m_sql(reg?0:1),
        M_CLOSURE(_T('*')),
        M_ZERO_ONE(_T('?')),
        M_ESCAPE(_T('\\'))
{
	_tcscpy_s(m_nfac_chars,8,_T("^|)]+*?"));
	SetExp(regexp);
	return;
}

bool
RegExp::SetExp(const CString& regexp)
{
	CString redExp = m_regExp;
	m_regExp = regexp;
	TCHAR pat[MAXPAT];
	if (makepat(m_regExp,pat) != NULL)
	{
		m_regExpPat = pat;
		return true;
	}
	m_regExp = redExp;
	return false;
}

int
RegExp::Match(CString& match)
{
	const TCHAR* pos = re_match(match,m_regExpPat);
	if (pos)
	{
		return pos - (const TCHAR*)match;
	}
	else
	{
		return -1;
	}
}

int
RegExp::Match(const TCHAR* match)
{
	const TCHAR* pos = re_match(match,m_regExpPat);
	if (pos)
	{
		return pos - match;
	}
	else
	{
		return -1;
	}
}

bool
RegExp::MatchPos(int pos,TCHAR car)
{
	return re_matchpos(car,m_regExpPat,pos);
}
            
RegExp::~RegExp()
{
	return;
}

bool 
RegExp::ZetUseCase(bool useCase)
{
	bool cc = m_use_case;
	m_use_case = useCase;
	return cc;
}


// ******* ALGEMEEN ********//
void	
RegExp::set_escape(TCHAR /* escape */,TCHAR zero_one,TCHAR closure)
{
	if((zero_one==_T('?') || closure==_T('*')) && (zero_one==_T('_') || closure==_T('%')))
	{
		M_ZERO_ONE = zero_one;
		M_CLOSURE  = closure;
		M_ESCAPE   = _T('\\'); 	/* Not settable as yet!		*/
		m_nfac_chars[5] = M_ZERO_ONE;
		m_nfac_chars[6] = M_CLOSURE;	

		/* Alsoo see the code sections marked "SQL" !!!		*/
		return;
	}
}

int
RegExp::isfactor(TCHAR c)
{
	return (_tcschr(m_nfac_chars,c) == NULL ? TRUE : FALSE);
}


TCHAR*
RegExp::strccat(TCHAR *s,TCHAR ch)
{
	register int len = _tcslen(s);
	
	s[len++] = ch;
	s[len]   = ENDSTR;
	return s;
}


// ******* PARSE ********//
const TCHAR*
RegExp::makepat(const TCHAR *re,TCHAR *pat)
{
	const TCHAR *t;
	
	m_re_ptr = re;
	if((t = parse_expression()) == NULL)
	{
		return NULL;
	}
	else
	{
		if(*m_re_ptr != ENDSTR)
		{
			free((void*)t);
			return NULL;
		}
		else
		{
			_tcscpy(pat,t);
			free((void*)t);
			return pat;
		}
	}
}

const TCHAR*
RegExp::parse_expression()
{
	TCHAR pat[MAXPAT];
	const TCHAR *arg1;
	
	pat[0] = ENDSTR;
	if((arg1 = parse_term()) == NULL)
	{
		return NULL;
	}
	while(*m_re_ptr == OR)
	{
		/* Parse all subsequent terms	*/
		strccat(pat,OR);
		_tcscat (pat,arg1);
		strccat(pat,END_TERM);
		free((void*)arg1);
		++m_re_ptr;
		if((arg1 = parse_term()) == NULL)
		{
			return NULL;
		}
	}
	_tcscat (pat,arg1);
	strccat(pat,END_TERM);
	free((void*)arg1);
	
	return _tcsdup(pat);
}


const TCHAR* 
RegExp::parse_term()
{
	TCHAR pat[MAXPAT];
	
	pat[0] = ENDSTR;
	if(*m_re_ptr == BOL)
	{
		strccat(pat,*m_re_ptr++);
	}
	do
	{
		if (const TCHAR *t = parse_factor())
		{
			_tcscat(pat,t);
			free((void*)t);
		}
		else
		{
			return NULL;
		}
	}
	while(isfactor(*m_re_ptr));

	return _tcsdup(pat);
}


const TCHAR*
RegExp::parse_factor()
{
	const TCHAR *t;
	TCHAR pat[MAXPAT];

	pat[0] = ENDSTR;
	switch(*m_re_ptr)
	{
		case LPAREN:	/* Parenthesised expression	*/
				++m_re_ptr;
				t = parse_expression();
				_tcscat(pat,t);
				free((void*)t);
				if(*m_re_ptr++ != RPAREN)
				{
					return NULL;
				}
				break;
		case CCL:	/* Character class		*/
				++m_re_ptr;
				t = parse_cc1();
				_tcscat(pat,t);
				free((void*)t);
				if(*m_re_ptr++ != CCLEND)
				{
					return NULL;
				}
				break;
		case ANY:	/* . or $ operators			*/
		case EOL:	strccat(pat,*m_re_ptr++);
				break;
/* VARIABLE SETTABLE CHARACTERS FOR SQL !!!				*/
		case _T('\\'):	/* ESCAPE Escape character		*/
				++m_re_ptr;
				strccat(pat,LITCHAR);
				strccat(pat,parse_escape());
				break;
		case _T('*'):	/* CLOSURE */
				if(M_CLOSURE == _T('*')) 
        {
  				return NULL;
        }
				goto literal;
		case _T('%'):	if(M_CLOSURE == _T('%')) 
				{
					return NULL;
				}
				goto literal;
		case _T('?'):	/* ZERO_ONE	*/
				if(M_ZERO_ONE == _T('?')) 
				{
					return NULL;
				}
				goto literal;
		case _T('_'):	
				if(M_ZERO_ONE == _T('_')) 
				{
					return NULL;
				}
				goto literal;
		case POS_CLO:
		case NEGATE:
		case CCLEND:
		case RPAREN:
		case OR:	/* Not valid characters		*/
				return NULL;
		default:	/* Literal character		*/

literal:strccat(pat,LITCHAR);
				strccat(pat,*m_re_ptr++);
				break;
	}
	/* Now check for a closure	*/
	if(*m_re_ptr == M_CLOSURE || *m_re_ptr == M_ZERO_ONE || *m_re_ptr == POS_CLO)
	{
		if (m_sql && *m_re_ptr == M_CLOSURE)
		{
			strccat(pat,M_CLOSURE);
			strccat(pat,1);
			strccat(pat,ANY);
			m_re_ptr++;
			//if(parse_closure(pat,*m_re_ptr++) == FALSE)
			//  return NULL;
		}
		else if (m_sql && *m_re_ptr == M_ZERO_ONE)
		{
			strccat(pat,_T('.'));
			m_re_ptr++;
		}
		else
		{
			if(parse_closure(pat,*m_re_ptr++) == FALSE)
			{
				return NULL;
			}
		}
	}
	return _tcsdup(pat);
}

TCHAR 
RegExp::parse_escape(void)
{
	int ch;
	
	switch(*m_re_ptr)
	{
		case _T('b'):	++m_re_ptr;	return _T('\b');
		case _T('t'):	++m_re_ptr;	return _T('\t');
		case _T('f'):	++m_re_ptr;	return _T('\f');
		case _T('n'):	++m_re_ptr;	return _T('\n');
		case _T('r'):	++m_re_ptr;	return _T('\r');
		case _T('0'):
		case _T('1'):
		case _T('2'):
		case _T('3'):
		case _T('4'):
		case _T('5'):
		case _T('6'):
		case _T('7'):	ch = *m_re_ptr++ - _T('0');
				if(*m_re_ptr >= _T('0') && *m_re_ptr < _T('8'))
				{
					ch <<= 3;
					ch  += (TCHAR)(*m_re_ptr++ - _T('0'));
				}
				if(*m_re_ptr >= _T('0') && *m_re_ptr < _T('8'))
				{
					ch <<= 3;
					ch  += (TCHAR)(*m_re_ptr++ - _T('0'));
				}
				return (TCHAR)ch;
		default:	return *m_re_ptr++;
	}
}

int
RegExp::parse_closure(TCHAR *pat,TCHAR c)
{
	memmove((void*)(pat+2),(void*)pat,_tcslen(pat)+1);
	pat[0] = c;
	int len = _tcslen(pat + 2);
	if(len > 255)
	{
		return FALSE;
	}
	else
	{
		pat[1] = (TCHAR)len;
		return TRUE;
	}
}

const TCHAR*
RegExp::parse_cc1(void)
{
	TCHAR pat[MAXPAT];
	short first=TRUE;
	
	_tcscpy(pat,_T("[ "));
	if(*m_re_ptr == NEGATE)
	{
		pat[0] = NCCL;
		++m_re_ptr;
	}
	while(*m_re_ptr != CCLEND && *m_re_ptr != ENDSTR)
	{
		if(*m_re_ptr == DASH && first == FALSE)
		{
			if(*++m_re_ptr == NCCL)
			{
				strccat(pat,DASH);
			}
			else
			{
				parse_dash(pat,*m_re_ptr++);
			}
		}
		else
		{
			if(*m_re_ptr == M_ESCAPE)
			{
				++m_re_ptr;
				strccat(pat,(TCHAR)parse_escape());
			}
			else
			{
				strccat(pat,*m_re_ptr++);
			}
		}
		first = FALSE;
	}
	int len = _tcslen(pat+2);
	if(len > 255)
	{
		return NULL;
	}
	else
	{
		pat[1] = (TCHAR)len;
		return _tcsdup(pat);
	}
}

const TCHAR*
RegExp::parse_dash(TCHAR *pat,TCHAR ch)
{
	int ch1;
	
	for(ch1=pat[_tcslen(pat)-1]+1;ch1 <= ch;++ch1)
	{
		strccat(pat,(TCHAR)ch1);
	}
	return pat;
}



// ********* match ******///
const TCHAR*
RegExp::str_match(const TCHAR *s,const TCHAR *re)
{
	TCHAR pat[MAXPAT];
	
	pat[0] = ENDSTR;
	if(makepat(re,pat) == NULL)
	{
		return NULL;
	}
	return re_match(s,pat);
}

const TCHAR* 
RegExp::re_match(const TCHAR *s,const TCHAR *pat)
{
	const TCHAR *c = s;
	
	m_s_end = NULL;
	while(*c != ENDSTR)
	{
		if(match_term(c - s,c,pat) != FALSE)
		{
			m_rstart  = c - s;
			m_rlength = m_s_end - s;
			return c;
		}
		++c;
	}
	m_rstart = m_rlength = 0;
	return NULL;
}

bool 
RegExp::re_matchpos(TCHAR car,const TCHAR *pat,int pos)
{
	TCHAR c[2];
	c[0] = car;
	c[1] = ENDSTR;

	if (pos > 0)
	{
		pat = skip_term(pat,pos);
	}
	return match_term(pos,c,pat,1) != 0;
}

int 
RegExp::match_term(int inx,const TCHAR *s,const TCHAR *pat,int aantalpos)
{
	m_s_end = s;
	if(*pat == ENDSTR)
	{
		return FALSE;
	}
	do
	{
		switch(*pat)
		{
			case BOL:	/* Match beginning of line	*/
					      if(inx != 0)
					      {
						      return FALSE;
					      }
					      ++pat;
					      break;
			case LITCHAR:	/* Match literal character	*/
					      if(m_use_case)
					      {
						      if(*s++ != *++pat)
						      {
							      return FALSE;
						      }
					      }
					      else
					      {
						      if(_totupper(*s++) != _totupper(*++pat))
						      {
							      return FALSE;
						      }
					      }
					      ++pat;
					      break;
			case END_TERM:	/* Skip end-of-term character	*/
					      ++pat;
					      break;
			case ANY:	/* Match any character .....	*/
					      if(*s++ == ENDSTR)
					      {
						      return FALSE;
					      }
					      ++pat;
					      break;
			case OR:	return match_or(inx,s,pat);
			case CCL:	/* Character class requires	*/
			case NCCL:	/* Special proccessing		*/
					      if(*s == ENDSTR)
					      {
						      return FALSE;
					      }
					      if(!match_cc1(*s++,pat++))
					      {
						      return FALSE;
					      }
					      pat += *pat + 1;
					      break;
			case EOL:	/* Match end of CString		*/
					      if(*s != ENDSTR)
					      {
						      return FALSE;
					      }
					      ++pat;
					      break;
			case _T('?'):	/* ZERO_ONE	IN * and ? types */
                if(*pat != M_ZERO_ONE)
                {
                  return true;
                }
                return match_0_1(inx,s,pat);
      case _T('_'): /* ZERO_ONE IN % and _ types */
                if(*pat != M_ZERO_ONE)
                {
                  return true;
                }
					      return match_0_1(inx,s,pat);
			case _T('*'):	/* CLOSURE IN * and ? types */
                if(*pat != M_CLOSURE)
                {
                  return true;
                }
 						    TCHAR clopat[MAXPAT];
						    memset(clopat,0,MAXPAT);
						    _tcsnccpy(clopat,pat+2,*(pat+1));
						    return match_closure(inx,s,pat,clopat);
     case _T('%'):  /* CLOSURE IN % and _ types */
                if(*pat != M_CLOSURE)
                {
                  return true;
                }
                // Fall through
     case POS_CLO:	
                {
						      TCHAR posclopat[MAXPAT];
						      memset(posclopat,0,MAXPAT);
						      _tcsnccpy(posclopat,pat+2,*(pat+1));
						      return match_closure(inx,s,pat,posclopat);
					      }
			default:	return TRUE;	/* Cant't happen */
		}
		m_s_end = s;
		if (aantalpos > 0)
		{
			--aantalpos;
			if (aantalpos == 0)
			{
				break;
			}
		}
	}
	while(*pat != ENDSTR);
	return TRUE;
}

int 
RegExp::match_or(int inx,const TCHAR *s,const TCHAR *pat)
{
	TCHAR workpat[MAXPAT];
	const TCHAR *t2 = NULL;
		
	workpat[0] = ENDSTR;
	++pat;
	const TCHAR* junk = skip_term(pat);
	_tcsnccat(workpat,pat,junk - pat);
	_tcscat (workpat,skip_term(junk));
	const TCHAR* t1 = (match_term(inx,s,workpat) != FALSE) ? m_s_end : NULL;
	if(t1 == NULL || *m_s_end != ENDSTR)
	{
		t2 = (match_term(inx,s,junk)!=FALSE) ? m_s_end : NULL;
		if(t1 != NULL && (t2 == NULL || t1 > t2))
		{
			m_s_end = t1;
		}
	}
	return (t1 == NULL && t2 == NULL) ? FALSE : TRUE;
}

const TCHAR*
RegExp::skip_term(const TCHAR *pat,int aantalpos)
{
	register short nterm = 1;
	
	while(nterm > 0)
	{
		switch(*pat)
		{
			case OR:	++nterm;
			case CCL:
			case NCCL:
			case _T('*'):	/* CLOSURE */
      case _T('%'):
			case _T('?'):	/* ZERO_ONE	*/
      case _T('_'):
			case POS_CLO:	++pat;
					pat += *pat;
					break;
			case END_TERM:	--nterm;
					break;
			case LITCHAR:	++pat;
					break;
		}
		++pat;
		if (aantalpos > 0)
		{
			--aantalpos;
			if (aantalpos == 0)
			{
				break;
			}
		}
	}
	return pat;
}

int 
RegExp::match_0_1(int inx,const TCHAR *s,const TCHAR *pat)
{
	const TCHAR *save_s = s;
	
	if(match_term(inx,s,pat+2) == TRUE)
	{
		return TRUE;
	}
	else
	{
		if(match_term(inx,save_s,pat+2+*(pat+1)) == FALSE)
		{
			return FALSE;
		}
		else
		{
			return ALMOST;
		}
	}
}

int
RegExp::match_closure(int inx,const TCHAR *s,const TCHAR *pat,const TCHAR *clopat)
{
	const TCHAR *save_s = s;

	if(match_term(inx,s,clopat) == TRUE)
	{
		save_s = m_s_end;
		if(match_closure(inx,save_s,pat,clopat) == TRUE)
		{
			return TRUE;
		}
		else
		{
  			if (match_term(inx,save_s,pat+2+*(pat+1)))
			{
				return TRUE;
			}
			// De closure mag altijd en car terug
  			if (*pat == M_CLOSURE && match_term(inx,s,pat+2+*(pat+1)))
			{
				return TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		if(*pat != M_CLOSURE)
		{
			return FALSE;
		}
		else
		{
			if(match_term(inx,save_s,pat+2+*(pat+1)) == TRUE)
			{
				return ALMOST;
			}
			else
			{
				return FALSE;
			}
		}
	}
}

short 
RegExp::match_cc1(TCHAR c,const TCHAR *pat)
{
	register short x;
	TCHAR cc1 = *pat++;

	for(x=*pat;x>0;--x)
	{
		if(c==pat[x])
		{
			return (cc1 == CCL);
		}
	}
	return (cc1 != CCL);
}

