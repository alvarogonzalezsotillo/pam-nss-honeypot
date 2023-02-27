PAM_DIR=/lib/x86_64-linux-gnu/security
NSS_DIR=/lib/x86_64-linux-gnu


build_pam(){
    gcc -fPIC -fno-stack-protector -c ./pam_honeypot.c && \
        ld -x --shared -o pam_honeypot.so pam_honeypot.o && \
        sudo install -m 0644 pam_honeypot.so $PAM_DIR
}

build_nss(){
    gcc -fPIC -shared -o libnss_honeypot.so.2 -Wl,-soname,libnss_honeypot.so.2 libnss_honeypot.c && \
        sudo install -m 0644 libnss_honeypot.so.2 $NSS_DIR && \
        sudo /sbin/ldconfig -n /$NSS_DIR /usr/lib        
}


build_pam
build_nss

