#ifndef	_LEX_H
#	define	_LEX_H

typedef	struct	_TOKENSTRUCT
{
	int	val;
	char	*token;
}	TOKENSTRUCT;

#define	TOKEN_END	-2
#define	TOKEN_USER	0
#define	NEW_LINE	-3

class	TK
	{
	public:
		int		Value;
		char	*Str;
		TK ();
		TK (const	int);
		TK (const	int, int, char *);

		~TK ();
	};

class	Lex
	{
		FILE	*fp;
		void	*Tree;
		TK		Tk;
	public:
		Lex();
		~Lex();
		BOOL	Start (char	*filename, TOKENSTRUCT	*TKS);
		BOOL	Stop ();
		int		Peek (TK	*);
		int		Get (TK	*);
		int		GetString ( TK *, char );
	};	

#define	TOKEN_END	-2
#define	TOKEN_USER	0
#define	kNEW_LINE	-3

#endif
