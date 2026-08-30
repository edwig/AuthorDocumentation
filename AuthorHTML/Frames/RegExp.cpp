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

#define	ENDSTR		'\0'	/* End of string		            */
#define	EOL		    '$'		/* End of line			            */
#define BOL		    '^'		/* Beginning of line		        */
#define	NEGATE		'^'		/* Negate a character class	    */
#define	CCL		    '['		/* Class closing		            */
#define NCCL		  ']'		/* No Class closing		          */
#define CCLEND		']'		/* Class closing end		        */
#define	ANY		    '.'		/* Any character		            */
#define	DASH		  '-'		/* Seperator in class length	  */
#define	OR		    '|'		/* Logical or for expression	  */
#define	LPAREN		'('		/* Left parenthesis		          */
#define	RPAREN		')'		/* Right parenthesis		        */
#define	POS_CLO		'+'		/* Aat least one or more char's	*/
#define	LITCHAR		'c'		/* Compiled literal		          */
#define	END_TERM	'e'		/* Compiled end of class	      */
#define	FS_DEFAULT	"[ \t]+"	/* Space default (white space)	*/

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4996) // Unsecure strcpy, strncpy, strcat and such.

RegExp::RegExp()
       :m_use_case(1),
        m_sql(0),
        M_CLOSURE('*'),
        M_ZERO_ONE('?'),
        M_ESCAPE('\\')
{
	strcpy_s(m_nfac_chars,8,"^|)]+*?");
	return;
}

RegExp::RegExp(const CString& regexp)
       :m_use_case(1),
        m_sql(0),
        M_CLOSURE('*'),
        M_ZERO_ONE('?'),
        M_ESCAPE('\\')
{
	strcpy_s(m_nfac_chars,8,"^|)]+*?");
	SetExp(regexp);
	return;
}

RegExp::RegExp(const CString& regexp,bool reg)
       :m_use_case(reg?0:1),
        m_sql(reg?0:1),
        M_CLOSURE('*'),
        M_ZERO_ONE('?'),
        M_ESCAPE('\\')
{
	strcpy_s(m_nfac_chars,8,"^|)]+*?");
	SetExp(regexp);
	return;
}

bool
RegExp::SetExp(const CString& regexp)
{
	CString redExp = m_regExp;
	m_regExp = regexp;
	char pat[MAXPAT];
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
	const char* pos = re_match(match,m_regExpPat);
	if (pos)
	{
		return pos - (const char*)match;
	}
	else
	{
		return -1;
	}
}

int
RegExp::Match(const char* match)
{
	const char* pos = re_match(match,m_regExpPat);
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
RegExp::MatchPos(int pos,char car)
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
RegExp::set_escape(char /* escape */,char zero_one,char closure)
{
	if((zero_one=='?' || closure=='*') && (zero_one=='_' || closure=='%'))
	{
		M_ZERO_ONE = zero_one;
		M_CLOSURE  = closure;
		M_ESCAPE   = '\\'; 	/* Not settable as yet!		*/
		m_nfac_chars[5] = M_ZERO_ONE;
		m_nfac_chars[6] = M_CLOSURE;	

		/* Alsoo see the code sections marked "SQL" !!!		*/
		return;
	}
}

int
RegExp::isfactor(char c)
{
	return (strchr(m_nfac_chars,c) == NULL ? TRUE : FALSE);
}


char*
RegExp::strccat(char *s,char ch)
{
	register int len = strlen(s);
	
	s[len++] = ch;
	s[len]   = ENDSTR;
	return s;
}


// ******* PARSE ********//
const char*
RegExp::makepat(const char *re,char *pat)
{
	const char *t;
	
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
			strcpy(pat,t);
			free((void*)t);
			return pat;
		}
	}
}

const char*
RegExp::parse_expression()
{
	char pat[MAXPAT];
	const char *arg1;
	
	pat[0] = ENDSTR;
	if((arg1 = parse_term()) == NULL)
	{
		return NULL;
	}
	while(*m_re_ptr == OR)
	{
		/* Parse all subsequent terms	*/
		strccat(pat,OR);
		strcat (pat,arg1);
		strccat(pat,END_TERM);
		free((void*)arg1);
		++m_re_ptr;
		if((arg1 = parse_term()) == NULL)
		{
			return NULL;
		}
	}
	strcat (pat,arg1);
	strccat(pat,END_TERM);
	free((void*)arg1);
	
	return strdup(pat);
}


const char* 
RegExp::parse_term()
{
	char pat[MAXPAT];
	
	pat[0] = ENDSTR;
	if(*m_re_ptr == BOL)
	{
		strccat(pat,*m_re_ptr++);
	}
	do
	{
		if (const char *t = parse_factor())
		{
			strcat(pat,t);
			free((void*)t);
		}
		else
		{
			return NULL;
		}
	}
	while(isfactor(*m_re_ptr));

	return strdup(pat);
}


const char*
RegExp::parse_factor()
{
	const char *t;
	char pat[MAXPAT];

	pat[0] = ENDSTR;
	switch(*m_re_ptr)
	{
		case LPAREN:	/* Parenthesised expression	*/
				++m_re_ptr;
				t = parse_expression();
				strcat(pat,t);
				free((void*)t);
				if(*m_re_ptr++ != RPAREN)
				{
					return NULL;
				}
				break;
		case CCL:	/* Character class		*/
				++m_re_ptr;
				t = parse_cc1();
				strcat(pat,t);
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
		case '\\':	/* ESCAPE Escape character		*/
				++m_re_ptr;
				strccat(pat,LITCHAR);
				strccat(pat,parse_escape());
				break;
		case '*':	/* CLOSURE */
				if(M_CLOSURE == '*') 
        {
  				return NULL;
        }
				goto literal;
		case '%':	if(M_CLOSURE == '%') 
				{
					return NULL;
				}
				goto literal;
		case '?':	/* ZERO_ONE	*/
				if(M_ZERO_ONE == '?') 
				{
					return NULL;
				}
				goto literal;
		case '_':	
				if(M_ZERO_ONE == '_') 
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
			strccat(pat,'.');
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
	return strdup(pat);
}

char 
RegExp::parse_escape(void)
{
	int ch;
	
	switch(*m_re_ptr)
	{
		case 'b':	++m_re_ptr;	return '\b';
		case 't':	++m_re_ptr;	return '\t';
		case 'f':	++m_re_ptr;	return '\f';
		case 'n':	++m_re_ptr;	return '\n';
		case 'r':	++m_re_ptr;	return '\r';
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':	ch = *m_re_ptr++ - '0';
				if(*m_re_ptr >= '0' && *m_re_ptr < '8')
				{
					ch <<= 3;
					ch  += (unsigned char)(*m_re_ptr++ - '0');
				}
				if(*m_re_ptr >= '0' && *m_re_ptr < '8')
				{
					ch <<= 3;
					ch  += (unsigned char)(*m_re_ptr++ - '0');
				}
				return (char)ch;
		default:	return *m_re_ptr++;
	}
}

int
RegExp::parse_closure(char *pat,char c)
{
	memmove((void*)(pat+2),(void*)pat,strlen(pat)+1);
	pat[0] = c;
	int len = strlen(pat + 2);
	if(len > 255)
	{
		return FALSE;
	}
	else
	{
		pat[1] = (char)len;
		return TRUE;
	}
}

const char*
RegExp::parse_cc1(void)
{
	char pat[MAXPAT];
	short first=TRUE;
	
	strcpy(pat,"[ ");
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
				strccat(pat,(char)parse_escape());
			}
			else
			{
				strccat(pat,*m_re_ptr++);
			}
		}
		first = FALSE;
	}
	int len = strlen(pat+2);
	if(len > 255)
	{
		return NULL;
	}
	else
	{
		pat[1] = (char)len;
		return strdup(pat);
	}
}

const char*
RegExp::parse_dash(char *pat,char ch)
{
	int ch1;
	
	for(ch1=pat[strlen(pat)-1]+1;ch1 <= ch;++ch1)
	{
		strccat(pat,(char)ch1);
	}
	return pat;
}



// ********* match ******///
const char*
RegExp::str_match(const char *s,const char *re)
{
	char pat[MAXPAT];
	
	pat[0] = ENDSTR;
	if(makepat(re,pat) == NULL)
	{
		return NULL;
	}
	return re_match(s,pat);
}

const char* 
RegExp::re_match(const char *s,const char *pat)
{
	const char *c = s;
	
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
RegExp::re_matchpos(char car,const char *pat,int pos)
{
	char c[2];
	c[0] = car;
	c[1] = ENDSTR;

	if (pos > 0)
	{
		pat = skip_term(pat,pos);
	}
	return match_term(pos,c,pat,1) != 0;
}

int 
RegExp::match_term(int inx,const char *s,const char *pat,int aantalpos)
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
						      if(toupper(*s++) != toupper(*++pat))
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
			case '?':	/* ZERO_ONE	IN * and ? types */
                if(*pat != M_ZERO_ONE)
                {
                  return true;
                }
                return match_0_1(inx,s,pat);
      case '_': /* ZERO_ONE IN % and _ types */
                if(*pat != M_ZERO_ONE)
                {
                  return true;
                }
					      return match_0_1(inx,s,pat);
			case '*':	/* CLOSURE IN * and ? types */
                if(*pat != M_CLOSURE)
                {
                  return true;
                }
 						    char clopat[MAXPAT];
						    memset(clopat,0,MAXPAT);
						    strncpy(clopat,pat+2,*(pat+1));
						    return match_closure(inx,s,pat,clopat);
     case '%':  /* CLOSURE IN % and _ types */
                if(*pat != M_CLOSURE)
                {
                  return true;
                }
                // Fall through
     case POS_CLO:	
                {
						      char posclopat[MAXPAT];
						      memset(posclopat,0,MAXPAT);
						      strncpy(posclopat,pat+2,*(pat+1));
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
RegExp::match_or(int inx,const char *s,const char *pat)
{
	char workpat[MAXPAT];
	const char *t2 = NULL;
		
	workpat[0] = ENDSTR;
	++pat;
	const char* junk = skip_term(pat);
	strncat(workpat,pat,junk - pat);
	strcat (workpat,skip_term(junk));
	const char* t1 = (match_term(inx,s,workpat) != FALSE) ? m_s_end : NULL;
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

const char*
RegExp::skip_term(const char *pat,int aantalpos)
{
	register short nterm = 1;
	
	while(nterm > 0)
	{
		switch(*pat)
		{
			case OR:	++nterm;
			case CCL:
			case NCCL:
			case '*':	/* CLOSURE */
      case '%':
			case '?':	/* ZERO_ONE	*/
      case '_':
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
RegExp::match_0_1(int inx,const char *s,const char *pat)
{
	const char *save_s = s;
	
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
RegExp::match_closure(int inx,const char *s,const char *pat,const char *clopat)
{
	const char *save_s = s;

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
RegExp::match_cc1(char c,const char *pat)
{
	register short x;
	char cc1 = *pat++;

	for(x=*pat;x>0;--x)
	{
		if(c==pat[x])
		{
			return (cc1 == CCL);
		}
	}
	return (cc1 != CCL);
}

