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

/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
  return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  printf("Acct mgmt\n");
  return PAM_SUCCESS;
}

#define MAX_USERFILE_SIZE 1024
#define USERSFILE "users"


#define LOGFILE "/tmp/pam_honeypot.log"

void mylog( const char* fmt, ... )
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
  
}

/* expected hook, this is where, we do all the required backdoor changes */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
  //Declaring required variables
  int retval;
  struct passwd pwd;
  const char * password=NULL;
  char *buf;
  size_t bufsize;
  int s;
  bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
    bufsize = 16384;
  buf = malloc(bufsize);
  if (buf == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  const char* pUsername;
  // pam_get_user asks and accepts the username
  retval = pam_get_user(pamh, &pUsername, "Username: ");
  if (retval != PAM_SUCCESS) {
    return retval;
  }


  printf("Welcome -- %s\n", pUsername);
  mylog( "Username:%s", pUsername );
  
  retval = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&password);
  if ( !password ) {
    mylog("Can't get password with pam_get_item");
  }
  else{
    mylog("Password:%s", password);
  }

  retval = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
  if ( !password ) {
    mylog("Can't get password with pam_get_authtok");
  }
  else{
    mylog("Password:%s", password);
  }
  
  return PAM_PERM_DENIED;
}
