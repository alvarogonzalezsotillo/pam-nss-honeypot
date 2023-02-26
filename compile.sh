

PAM_DIR=/lib/x86_64-linux-gnu/security

gcc -fPIC -fno-stack-protector -c ./pam_honeypot.c && \
    ld -x --shared -o pam_honeypot.so pam_honeypot.o && \
    sudo cp pam_honeypot.so $PAM_DIR && \
    sudo chmod 644  $PAM_DIR/pam_honeypot.so
