#ifndef	_DPRINTF_H_
#	define	_DPRINTF_H_

/*
20000211	ljz	Removed MajorEvent and MinorEvent instances of 'Debug'
			Added UserLog and TroubleLog
			Added bAddTimeStamps member
20011110        mvh     Replaced MessagePipeName by FilePipeName
                        Blocked out critical section
*/

class	Debug
{
private:
	int			BDebug;
	FILE*			Debugfp;
//	CRITICAL_SECTION	CriticalFile;
	int			CloseOnOff;
	int			UseMessagePipe;
	char			FilePipeName[256];
	int			UseUDP;
	char			bAddTimeStamps;
public:
	Socket			SocketUDP;	
public:
	Debug();
	~Debug();
	BOOL	DescribeSource;
	void	AddTimeStamps(char YesOrNo) {bAddTimeStamps = YesOrNo;};
	void	On(FILE	*);
	void	On();
	void	On(char	*filename);
	void	OnMsgPipe(char	*MsgPipeName);
	void	OnMsgPipe(char	*MsgPipeName, BOOL);
	void	OnUDP(char *Host, char *IP);
	void	Off();
	int	printf(char	*, ...);
};

extern	Debug	SystemDebug;
extern	Debug	OperatorConsole;
extern	Debug	AccessUpdate;
//extern	Debug	MajorEvent;
//extern	Debug	MinorEvent;
extern	Debug	UserLog;
extern	Debug	TroubleLog;

#endif
