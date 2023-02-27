#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <nss.h>
#include <syslog.h>
#include <stdarg.h>


// #include <test_auth.h>



#define LOGFILE "/tmp/libnss_honeypot.log"

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


/** Holds a temporary user name. */
static char *temp_username = NULL;

/**
 * Prepare buffers.
 */
enum nss_status _nss_honeypot_setpwent(void)
{
	syslog(LOG_ERR, "_nss_honeypot_setpwent() called");
        mylog( "_nss_honeypot_setpwent() called" );
	return NSS_STATUS_SUCCESS;
}

/**
 * Free buffers.
 */
enum nss_status _nss_honeypot_endpwent(void)
{
	syslog(LOG_ERR, "_nss_honeypot_endpwent() called");
        mylog( "_nss_honeypot_endpwent() called" );

	if (temp_username) {
		free(temp_username);
		temp_username = NULL;
	}

	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_honeypot_getpwent_r(struct passwd *result, char *buffer, size_t buflen, int *errnop)
{
	syslog(LOG_ERR, "_nss_honeypot_getpwent_r() called");
        mylog( "_nss_honeypot_getpwent_r() called" );
	return NSS_STATUS_SUCCESS;
}


enum nss_status _nss_honeypot_getpwuid_r(uid_t uid, struct passwd *result, char *buffer, size_t buflen, int *errnop)
{

  // struct passwd
  // {
  //   char *pw_name;		/* Username.  */
  //   char *pw_passwd;		/* Hashed passphrase, if shadow database
  //                                    not in use (see shadow.h).  */
  //   __uid_t pw_uid;		/* User ID.  */
  //   __gid_t pw_gid;		/* Group ID.  */
  //   char *pw_gecos;		/* Real name.  */
  //   char *pw_dir;			/* Home directory.  */
  //   char *pw_shell;		/* Shell program.  */
  // };
	syslog(LOG_ERR, "_nss_honeypot_getpwuid_r() called");
        mylog( "_nss_honeypot_getpwuid_r() called %d", uid );
	//return NSS_STATUS_SUCCESS;
        return NSS_STATUS_NOTFOUND;
}

int append_to_buffer(const char* str, char **buffer, size_t *buflen )
{
  int len = 0;
  
  if( buflen <= 0 ){
    return -1;
  }

  strncpy(*buffer, str, *buflen-1 );
  len = strnlen(*buffer, *buflen-1);
  (*buffer)[len]='\0';
  *buffer += len+1;

  return 0;
}

enum nss_status _nss_honeypot_getpwnam_r(const char *name, struct passwd *result,
                                      char *buffer, size_t buflen, int *errnop)
{
	syslog(LOG_ERR, "_nss_honeypot_getbynam_r() called");
        mylog( "_nss_honeypot_getpwnam_r() called %s size:%d", name, buflen );
        //return NSS_STATUS_SUCCESS;
        size_t remaining_buf = buflen;
        char* buf = buffer;
        
        result->pw_name = buf;
        append_to_buffer(name,&buf,&remaining_buf);
        mylog( "   pw_name:%s %p", result->pw_name, result->pw_name );

        result->pw_passwd = buf;
        append_to_buffer("secreto",&buf,&remaining_buf);
        mylog( "   pw_passwd:%s %p", result->pw_passwd, result->pw_passwd );

        result->pw_gecos = buf;
        append_to_buffer("nombrereal",&buf,&remaining_buf);
        mylog( "   pw_gecos:%s %p", result->pw_gecos, result->pw_gecos );

        result->pw_dir = buf;
        append_to_buffer("/home",&buf,&remaining_buf);
        mylog( "   pw_dir:%s %p", result->pw_dir, result->pw_dir );

        result->pw_shell = buf;
        append_to_buffer("/bin/false",&buf,&remaining_buf);
        mylog( "   pw_shell:%s %p", result->pw_shell, result->pw_shell );


        return NSS_STATUS_SUCCESS;
} 
