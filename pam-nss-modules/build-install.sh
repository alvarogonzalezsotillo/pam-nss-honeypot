# DEBIAN
PAM_DIR=/lib/x86_64-linux-gnu/security
NSS_DIR=/lib/x86_64-linux-gnu

# RASPBIAN
# PAM_DIR=/lib/arm-linux-gnueabihf/security
# NSS_DIR=/lib/arm-linux-gnueabihf


build_pam(){
    gcc -fPIC -fno-stack-protector -c ./pam_honeypot.c && \
        ld -x --shared -o pam_honeypot.so pam_honeypot.o && \
        sudo install -m 0644 pam_honeypot.so $PAM_DIR
}

build_nss(){
    gcc -fPIC -shared -o libnss_honeypot.so.2 -Wl,-soname,libnss_honeypot.so.2 libnss_honeypot.c && \
        sudo install -m 0644 libnss_honeypot.so.2 $NSS_DIR && \
        sudo /sbin/ldconfig -n /$NSS_DIR /usr/lib && \
	sudo /sbin/ldconfig 
}

clean(){
    rm "*.so" "*.so.2"
}

install_nss(){
    if grep honeypot /etc/nsswitch.conf | grep passwd
    then
        echo libnss_honeypot already installed
    else
        sudo sed -ie "/^passwd/ s/$/ honeypot/" /etc/nsswitch.conf
    fi
}

install_pam(){
    if grep pam_honeypot /etc/pam.d/sshd
    then
        echo pam_honeypot already installed
    else
        sudo sed -i '1i auth optional pam_honeypot.so' /etc/pam.d/sshd
    fi
}

configure_sshd(){
    sudo sed -i 's/.*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config 
    sudo sed -i 's/.*PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config
    sudo systemctl restart sshd || true
}

build_pam && install_pam
build_nss && install_nss
configure_sshd

