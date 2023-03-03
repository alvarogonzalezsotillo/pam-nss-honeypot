#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <syslog.h>

#define LOGFILE "/tmp/pam_honeypot.log"


void mylog( pam_handle_t *pamh, const char* fmt, ... )
{
  va_list ap;
  char buf[1000];
  FILE *file;

  va_start(ap, fmt);

  
  vsnprintf (buf, sizeof(buf)-1, fmt, ap );
  file = fopen( LOGFILE, "a" );
  fprintf( file, "%s\n", buf );
  fclose(file);
  
  va_end(ap);  

  pam_syslog(pamh,LOG_ERR,buf);

}

const char* not_logged_users[] = { "alvaro", "jaime", "git" };

int is_not_logged_user(const char* user )
{
  int i = 0;
  for( i = 0 ; i < sizeof(not_logged_users)/sizeof(*not_logged_users) ; i += 1 ){
    if( strcmp(user,not_logged_users[i]) == 0 ){
      return 1;
    }  
  }
  return 0;
}

/* expected hook, this is where, we do all the required backdoor changes */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
  //Declaring required variables
  int retval;
  const char* pUsername = NULL;
  const char* password = NULL;  
  // pam_get_user asks and accepts the username
  retval = pam_get_user(pamh, &pUsername, "Username: ");
  if (retval != PAM_SUCCESS) {
    return retval;
  }

  for( int i = 0 ; i < argc ; i += 1 ){
    mylog(pamh," argv[%d] >%s<", i, argv[i] );
  }
  
  retval = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
  if ( !password ) {
    mylog(pamh,"Can't get password with pam_get_authtok!:%s", pUsername);
  }
  else{

    if( is_not_logged_user(pUsername) ){
      password = "*******";
    }
    
    mylog(pamh,"Username:>%s< Password:>%s<", pUsername, password);
  }
  
  return PAM_PERM_DENIED;
}
