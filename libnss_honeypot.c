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
enum nss_status _nss_test_setpwent(void)
{
	syslog(LOG_ERR, "_nss_test_setpwent() called");
        mylog( "_nss_test_setpwent() called" );
	return NSS_STATUS_SUCCESS;
}

/**
 * Free buffers.
 */
enum nss_status _nss_test_endpwent(void)
{
	syslog(LOG_ERR, "_nss_test_endpwent() called");
        mylog( "_nss_test_endpwent() called" );

	if (temp_username) {
		free(temp_username);
		temp_username = NULL;
	}

	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_test_getpwent_r(struct passwd *result, char *buffer, size_t buflen, int *errnop)
{
	syslog(LOG_ERR, "_nss_test_getpwent_r() called");
        mylog( "_nss_test_getpwent_r() called" );
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_test_getpwbyuid_r(uid_t uid, struct passwd *result, char *buffer, size_t buflen, int *errnop)
{
	syslog(LOG_ERR, "_nss_test_getpwbyuid_r() called");
        mylog( "_nss_test_getpwbyuid_r() called" );
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_test_getpwbynam_r(const char *name, struct passwd *result,
                                      char *buffer, size_t buflen, int *errnop)
{
	struct TEST_auth_response response;
	json_t *local_user = NULL;

	syslog(LOG_ERR, "_nss_test_getpwbynam_r() called");
        mylog( "_nss_test_getpwbynam_r() called" );
	if (test_auth_query_local_user(name, "localhost", &response) == 0) {
		if ((local_user = json_object_get(response.response, "local_user"))) {
			temp_username = strdup(json_string_value(local_user));
			result->pw_name = temp_username;
			return NSS_STATUS_SUCCESS;
		}
	}
	
	test_auth_free_response(&response);

	return NSS_STATUS_SUCCESS;
}
