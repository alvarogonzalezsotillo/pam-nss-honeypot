#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <nss.h>
#include <syslog.h>
#include <stdarg.h>
// #include <test_auth.h>

#define LOGFILE "/tmp/libnss_honeypot.log"
// #define LOGFILE NULL

// LOG TO SYSLOG AND LOGFILE
void mylog( const char* fmt, ... )
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

  syslog(LOG_ERR, "%s", buf);
  
  va_end(ap);  
}

/**
 * Prepare buffers.
 */
enum nss_status _nss_honeypot_setpwent(void)
{
  mylog( "_nss_honeypot_setpwent() called" );
  return NSS_STATUS_SUCCESS;
}

/**
 * Free buffers.
 */
enum nss_status _nss_honeypot_endpwent(void)
{
  mylog( "_nss_honeypot_endpwent() called" );
  return NSS_STATUS_SUCCESS;
}

// SOME GLOBAL CONSTANTS, AVOID MALLOC/FREE ERRORS, I HOPE
const char* pw_passwd="creoquetendriaqueestarhasheadaparafuncionaroalgo";
const char* pw_gecos="nombrerealperoesmentira";
const char* pw_dir="/tmp";
const char* pw_shell="/bin/false";

// DECIDE IF THE USER IS IN THIS DATABASE
enum nss_status _nss_honeypot_getpwnam_r(const char *name, struct passwd *result,
                                         char *buffer, size_t buflen, int *errnop)
{
  mylog( "_nss_honeypot_getpwnam_r() called %s size:%d", name, buflen );
       
  result->pw_name =   (char*)name;
  result->pw_passwd = (char*)pw_passwd;
  result->pw_gecos =  (char*)pw_gecos;
  result->pw_dir =    (char*)pw_dir;
  result->pw_shell =  (char*)pw_shell;

  // EVERY POSSIBLE USER IS IN DATABASE
  return NSS_STATUS_SUCCESS;
} 
