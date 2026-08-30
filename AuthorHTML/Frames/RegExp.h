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
	int  Match(const char* match);

	bool MatchPos(int pos,char car);

	// Instellingen
	bool ZetUseCase(bool useCase);
private:
	// Algemeen
	void	set_escape(char escape,char zero_one,char closure);
	int   isfactor(char c);
	char* strccat(char *s,char ch);

	// parse
	const char* makepat(const char *re,char *pat);
	const char* parse_expression();
	const char* parse_term();
	const char* parse_factor();
	char  parse_escape();
	int   parse_closure(char *pat,char c);
	const char* parse_cc1(void);
	const char* parse_dash(char *pat,char ch);

	// match
	const char* str_match(const char *s,const char *re);
	const char* re_match(const char *s,const char *pat);
	bool  re_matchpos(char car,const char *pat,int pos);
	int   match_term(int inx,const char *s,const char *pat,int aantalpos = 0);
	int   match_or(int inx,const char *s,const char *pat);
	const char* skip_term(const char *pat,int aantalpos = 0);
	int   match_0_1(int inx,const char *s,const char *pat);
	int   match_closure(int inx,const char *s,const char *pat,const char *clopat);
	short match_cc1(char c,const char *pat);

private:
	CString m_regExp;	
	CString m_regExpPat;

	// Special
	char	M_CLOSURE;       /* Closure of line		*/
	char	M_ZERO_ONE;      /* One or more characters	*/
	char	M_ESCAPE;        /* Escape character		*/
	char  m_nfac_chars[8];
	// Hulpjes

	int  m_rstart;	      /* Start  of matched substring		*/
	int  m_rlength;	      /* Length of matched substring		*/
	bool m_use_case;      /* Case is significant			*/
	bool m_sql;           /* vertaling als sql			*/
                              // ?  ==> .
                              // *  ==> .*
	const char *m_re_ptr;	      /* Global for pattern building		*/
	const char *m_s_end;	      /* Points to last char matched		*/
	const char *m_c;	          /* CString continuation pointer		*/

};

