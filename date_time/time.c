#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STIME_SIZE 5
//#define DEBUG

char st[STIME_SIZE];
int true = 1;
int false =0;


int _is_valid_timespec(const char *s)
{
	int digit;
	int dash;
	int colon;

	digit = dash = colon = 0;

	while (*s) {
		if (*s >= '0' && *s <= '9') {
			++digit;
		} else if (*s == '-') {
			++dash;
			if (colon)
				return false;
		} else if (*s == ':') {
			++colon;
		} else {
			return false;
		}
		++s;
	}

	if (!digit)
		return false;

	if (dash > 1
	    || colon > 2)
		return false;

	if (dash) {
		if (colon == 0
		    && digit < 1)
			return false;
		if (colon == 1
		    && digit < 3)
			return false;
		if (colon == 2
		    && digit < 4)
			return false;
	} else {
		if (colon == 1
		    && digit < 2)
			return false;
		if (colon == 2
		    && digit < 3)
			return false;
	}
	return true;
}


int _get_colon_num(char *s, char parse)
{
	int cc;
	cc = 0;

	while(*s){
		if(*s++ == parse)
		cc++;
	}
	return ++cc;
}

// no use func
char *_parse_time(char *stime, char ch)
{
	memset(st, '0', STIME_SIZE);  // bzero(st, STIME_SIZE)
	int i = 0;

	while(strchr(stime, ch) && *stime != ch)
		st[i++] = *stime;
	return st;	
}


long int _get_time_sec(char *s)
{
	int hour=0, min=0, sec=0;
	int ntime = _get_colon_num(s, ':');
	char *tmp = strchr(s, ':');
	
	switch(ntime) {
		case 1:
			hour = atoi(s);
			break;
		case 2:
			hour = atoi(s);
			min = atoi(++tmp);
			break;
		case 3:
			hour = atoi(s);
			min = atoi(++tmp);
			sec = atoi(strchr(tmp, ':') + 1);
	}
#ifdef DEBUG
	printf("%d %d %d\n", hour, min, sec);
#endif
	return hour * 3600 + min * 60 + sec;
}

long int _get_date_sec(char *s)
{
	int ndate = _get_colon_num(s, '-');
//	int natime = _get_colon_num(s, ':');
//	long int seconds = 0;
	int year=0, mon=0, day=0;
	switch(ndate) {
		case 1:
			return _get_time_sec(s);
			break;
		case 2:
			day = atoi(s);
			break;
		case 3:
			mon = atoi(s);
			day = atoi(strchr(s, '-')+1);
			break;
		case 4:
			year = atoi(s);
			mon = atoi(strchr(s, '-')+1);
			day = atoi(strrchr(s, '-') - 2);
	}
#ifdef DEBUG
	printf("%d %d %d\n", year, mon, day);
#endif
	return (year * 365 + mon * 30 + day) * 24 * 3600 + _get_time_sec(strrchr(s, '-')+1);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{ printf("usage: %s <string[date string: 2012-12-31-06:00:00 or  1-20-10:05:30]>\n\
time format: YYYY-MM-DD-HH:MM:SS or  MM-DD-HH:MM:SS\n", strrchr(*argv, '/')+1); exit(EXIT_FAILURE); }
	/* test _get_colon_num() func
	printf("%d\n", _get_colon_num(*(argv+1), **(argv+2)));
	*/
	
	if(_get_colon_num(*(argv+1), '-') == 4 && _get_colon_num(*(argv+1), ':') == 3)
	{
		// time : start or end
		struct tm time;
		int tm_year = 1900, tm_mon = 0;
		sscanf(*(argv+1), "%d-%d-%d-%d:%d:%d", &tm_year, &tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
		time.tm_year = tm_year - 1900;
		time.tm_mon = tm_mon - 1;
#ifdef DEBUG
	printf("%d %d %d %d %d %d\n", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	printf("_get_time_sec: %ld\n", _get_time_sec("11:12:01"));
#endif
	printf("%s%ld\n", asctime(&time), mktime(&time));
	
	exit(EXIT_SUCCESS);
	}
	
	// or else doing something
	_is_valid_timespec(argv[1])	&&	printf("_get_date_sec(): %ld\n", _get_date_sec(*(argv+1))) || printf("no valid timespec found!\n");

}


