#define MAGIC_NUMBER 'a'

#define SET_BAUD _IOW(MAGIC_NUMBER,0,int)
#define SET_STOP_BIT _IOW(MAGIC_NUMBER,1,int)