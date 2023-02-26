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
	syslog(LOG_ERR, "_nss_honeypot_getpwuid_r() called");
        mylog( "_nss_honeypot_getpwuid_r() called" );
	return NSS_STATUS_SUCCESS;        
}


enum nss_status _nss_honeypot_getpwnam_r(const char *name, struct passwd *result,
                                      char *buffer, size_t buflen, int *errnop)
{
	syslog(LOG_ERR, "_nss_honeypot_getbynam_r() called");
        mylog( "_nss_honeypot_getpwnam_r() called" );
        strncpy((char*)name,buffer,buflen-1);
        buffer[buflen-1]='\0';
        result->pw_name = buffer;
        return NSS_STATUS_SUCCESS;
} 
