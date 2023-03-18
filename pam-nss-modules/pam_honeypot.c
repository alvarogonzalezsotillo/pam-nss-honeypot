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
// #define LOGFILE NULL

// LOG TO SYSLOG AND LOGFILE
void mylog( pam_handle_t *pamh, const char* fmt, ... )
{
  va_list ap;
  char buf[1000];
  FILE *file;

  va_start(ap, fmt);

  vsnprintf (buf, sizeof(buf)-1, fmt, ap );
  
  if( LOGFILE != NULL ){
    file = fopen( LOGFILE, "a" );
    fprintf( file, "%s\n", buf );
    fclose(file);
  }

  pam_syslog(pamh,LOG_ERR,buf);
  
  va_end(ap);  
}

int is_not_logged_user(const char* user )
{
  // LIST OF USERS THAT ARE NOT LOGGED (REAL USERS WHOSE PASSWORDS SHOULD NOT BE EXPOSED)
  static const char* not_logged_users[] = {  };
  
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

  // PRINT ARGS IF ANY, JUST FOR DEBUGGING
  for( int i = 0 ; i < argc ; i += 1 ){
    mylog(pamh," argv[%d] >%s<", i, argv[i] );
  }

  // GET PASSWORD AND PRINT IT TO mylog
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

  // NEVER LET ANYONE IN, LEAVE THAT RESPONSIBILITY TO OTHER PAM MODULES
  return PAM_PERM_DENIED;
}
