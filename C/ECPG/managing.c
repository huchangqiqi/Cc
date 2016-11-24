/* Processed by ecpg (4.12.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "managing.pgc"
#include<stdio.h>

/* exec sql begin declare section */
 

#line 4 "managing.pgc"
 char dbname [ 1024 ] ;
/* exec sql end declare section */
#line 5 "managing.pgc"


int
main()
{
  { ECPGconnect(__LINE__, 0, "tcp:postgresql://localhost:5432/test1" , "postgres" , "postgres" , "con1", 0); }
#line 10 "managing.pgc"

  { ECPGconnect(__LINE__, 0, "test2" , "postgres" , "postgres" , "con2", 0); }
#line 11 "managing.pgc"

  { ECPGconnect(__LINE__, 0, "test3" , "postgres" , "postgres" , "con3", 0); }
#line 12 "managing.pgc"


  { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_database ( )", ECPGt_EOIT, 
	ECPGt_char,(dbname),(long)1024,(long)1,(1024)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 14 "managing.pgc"

  printf("current = %s (should be testdb3)\n",dbname);

  { ECPGdo(__LINE__, 0, 1, "con2", 0, ECPGst_normal, "select current_database ( )", ECPGt_EOIT, 
	ECPGt_char,(dbname),(long)1024,(long)1,(1024)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 17 "managing.pgc"

  printf("current = %s (should be testdb2)\n", dbname);

  { ECPGsetconn(__LINE__, "con1");}
#line 20 "managing.pgc"


  { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_database ( )", ECPGt_EOIT, 
	ECPGt_char,(dbname),(long)1024,(long)1,(1024)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 22 "managing.pgc"

  { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into t1 values ( 123 )", ECPGt_EOIT, ECPGt_EORT);}
#line 23 "managing.pgc"

  printf("current = %s (should be testdb1)\n",dbname);

  { ECPGdisconnect(__LINE__, "ALL");}
#line 26 "managing.pgc"

  return 0;

}
