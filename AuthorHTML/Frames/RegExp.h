//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    RegExp.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Regular expression parser
//
#pragma once
#include "StdAfx.h"

class RegExp
{
public:
	RegExp();
	RegExp(const CString& regexp);
	RegExp(const CString& regexp,bool reg);

	~RegExp();

	bool SetExp(const CString& regexp);
	int  Match(CString& match);
	int  Match(const TCHAR* match);

	bool MatchPos(int pos,TCHAR car);

	// Instellingen
	bool ZetUseCase(bool useCase);
private:
	// Algemeen
	void	set_escape(TCHAR escape,TCHAR zero_one,TCHAR closure);
	int   isfactor(TCHAR c);
	TCHAR* strccat(TCHAR *s,TCHAR ch);

	// parse
	const TCHAR* makepat(const TCHAR *re,TCHAR *pat);
	const TCHAR* parse_expression();
	const TCHAR* parse_term();
	const TCHAR* parse_factor();
	TCHAR  parse_escape();
	int   parse_closure(TCHAR *pat,TCHAR c);
	const TCHAR* parse_cc1(void);
	const TCHAR* parse_dash(TCHAR *pat,TCHAR ch);

	// match
	const TCHAR* str_match(const TCHAR *s,const TCHAR *re);
	const TCHAR* re_match(const TCHAR *s,const TCHAR *pat);
	bool  re_matchpos(TCHAR car,const TCHAR *pat,int pos);
	int   match_term(int inx,const TCHAR *s,const TCHAR *pat,int aantalpos = 0);
	int   match_or(int inx,const TCHAR *s,const TCHAR *pat);
	const TCHAR* skip_term(const TCHAR *pat,int aantalpos = 0);
	int   match_0_1(int inx,const TCHAR *s,const TCHAR *pat);
	int   match_closure(int inx,const TCHAR *s,const TCHAR *pat,const TCHAR *clopat);
	short match_cc1(TCHAR c,const TCHAR *pat);

private:
	CString m_regExp;	
	CString m_regExpPat;

	// Special
	TCHAR	M_CLOSURE;       /* Closure of line		*/
	TCHAR	M_ZERO_ONE;      /* One or more characters	*/
	TCHAR	M_ESCAPE;        /* Escape character		*/
	TCHAR  m_nfac_chars[8];
	// Hulpjes

	int  m_rstart;	      /* Start  of matched substring		*/
	int  m_rlength;	      /* Length of matched substring		*/
	bool m_use_case;      /* Case is significant			*/
	bool m_sql;           /* vertaling als sql			*/
                              // ?  ==> .
                              // *  ==> .*
	const TCHAR *m_re_ptr;	      /* Global for pattern building		*/
	const TCHAR *m_s_end;	      /* Points to last char matched		*/
	const TCHAR *m_c;	          /* CString continuation pointer		*/

};

