
casper0@casper:~$ cat -n /casper/casper3.c
1 #include <stdlib.h>
2 #include <unistd.h>
3 #include <string.h>
4 #include <stdio.h>
5
6 int main()
7 {
8 char pin[4];
9 char input[5]; // space for null byte
10 int i;
11 int correct;
12
13 srand(time(0));
14
15 for (i = 0; i < 4; i++) {
16 pin[i] = rand() % 10 + ’0’;
17 }
18
19 printf("Enter pincode: ");
20 scanf("%s", input);
21
22 correct = 1;
23 for (i = 0; i < 4 && correct; i++) {
24 if (pin[i] != input[i]) {
25 printf("Pin incorrect\n");
26 correct = 0;
27 }
9
28 }
29
30 if (correct) {
31 setresuid(geteuid(), geteuid(), geteuid());
32 execl("/bin/xh", "/bin/xh", NULL);
33 }
34
35 return 0;
36 }
----------------------------------------------------

// cat -n /casper/casper4.c  -- check source code
//  /casper/casper3  -- run

     1	#include <stdio.h>
     2	#include <stdlib.h>
     3	#include <string.h>
     4	
     5	void greetUser(char *s) {
     6	    printf("Hello %s!\n", s);
     7	}
     8	
     9	struct data_t {
    10	    char buf[777];
    11	    void (*fp)(char *);  // 栈溢出，覆盖返回指针fp的地址  0x80483e0 or ox8048450
    12	} somedata;
    13	
    14	int main(int argc, char **argv) {
    15	    somedata.fp = &greetUser;
    16	
    17	    if(argc < 2) { // argc:number of parameters
    18	        printf("Usage: %s YourName\n", argv[0]);  // usage /casper/casper4 YourName
    19	        exit(1);
    20	    }
    21	
    22	    strcpy(somedata.buf, argv[1]); // argv[1] 存输入,copy 到somedata.buf
    23	    (somedata.fp)(somedata.buf); // 调用greetUser（somedata.buf）
    24	
    25	    return 0;
    26	}

// 0x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e00x80483e0
// ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450ox8048450

//5 ---------------------------------------------------------------------------
     1	#include <stdio.h>
     2	#include <unistd.h>
     3	#include <string.h>
     4	
     5	int enableGlobalAdmin = 0;
     6	int enableFullAdmin = 0;
     7	
     8	struct role_t {
     9	    char rolename[32];
    10	    int authority;
    11	} defaultRole = { .rolename = "regular user", .authority = 0 };
    12	
    13	struct user_t {
    14	    char name[777];
    15	    struct role_t *role;
    16	};
    17	
    18	void inputUser(struct user_t *u, char *s) {
    19	    strcpy(u->name, s);
    20	}
    21	
    22	void greetUser(struct user_t *u) {
    23	    printf("Hello %s, you have role '%s'\n", u->name, u->role->rolename);
    24	}
    25	
    26	int main(int argc, char **argv) {
    27	    struct user_t thisUser;
    28	    thisUser.role = &defaultRole;
    29	
    30	    if (argc < 2) {
    31	        printf("ERROR\n");
    32	    }
    33	    inputUser(&thisUser, argv[1]);
    34	    greetUser(&thisUser);
    35	
    36	    if((thisUser.role)->authority == 1) {
    37	        printf("Congratulations! You are an admin\n");
    38	        setresuid(geteuid(), geteuid(), geteuid());
    39	        execl("/bin/xh", "/bin/xh", NULL);
    40	    }
    41	
    42	    return 0;
    43	}
    
    //6---------------------------------------------------------------------------
    
     1	#include <stdio.h>
     2	#include <unistd.h>
     3	
     4	void greetUser() {
     5	    char buf[777];
     6	    printf("Enter your name: ");
     7	    fflush(0);
     8	    gets(buf);
     9	    printf("Hello %s!\n", buf);
    10	}
    11	
    12	int main() {
    13	    greetUser();
    14	    return 0;
    15	}

 //7------------------------------------------------------------------------

     1	#include <stdio.h>
     2	#include <unistd.h>
     3	
     4	void greetUser() {
     5	    char buf[777];
     6	    printf("Enter your name: ");
     7	    fflush(0);
     8	    gets(buf);
     9	    printf("Hello %s!\n", buf);
    10	}
    11	
    12	int main(int argc, char **argv) {
    13	    // never add this in your own program
    14	    setresuid(geteuid(), geteuid(), geteuid());
    15	
    16	    greetUser();
    17	
    18	    return 0;
    19	}
    
    
//8 ----------------------------------------------------------------------

 不可以用buffer overflow， 1. code栈不可以执行，把shellcode放到栈里没有用。 2. 不能覆盖函数返回地址，canery bird 已经打开。
 只能改写isAdmin的内容。
 
     1	#include <stdio.h>
     2	#include <unistd.h>
     3	#include <stdlib.h>
     4	
     5	int isAdmin = 0;  全局变量，存在heap中，地址不变。
     6	
     7	void greetUser(char *s) {
     8	    char buf[777];
     9	    sprintf(buf, "Hello %s!\n", s); 把s写入buf中，可造成buffer overflow （此题无效）
    10	    printf(buf);
    11	}
    12	
    13	int main(int argc, char **argv) {
    14	    if(argc < 2) {
    15	        printf("Usage: %s YourName\n", argv[0]);
    16	        exit(1);
    17	    }
    18	
    19	    greetUser(argv[1]);
    20	
    21	    if(isAdmin) {
    22	        setresuid(geteuid(), geteuid(), geteuid());
    23	        execl("/bin/xh", "/bin/xh", NULL);
    24	    }
    25	
    26	    return 0;
    27	}


