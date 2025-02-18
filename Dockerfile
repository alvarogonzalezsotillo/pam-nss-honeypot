FROM debian:12.8

# APT CACHE. CHECK DATE, BECAUSE SOME IMAGES ARE INCOMPATIBLE.
RUN date
RUN apt-get clean && apt-get update && apt-get -y upgrade && apt-get install -y tmux openssh-server build-essential gcc libpam-dev libnss3-dev sudo


# CREATE FAKE USER, LINK TO USER 1000 OF CONTAINER HOST
# INSTALL TOOLS
RUN bash -c "chpasswd root:\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM"  && \
    useradd --uid 1000 --user-group legituser-notfake 

RUN rm -rf /var/lib/apt/lists/*


# COMPILE AND INSTALL HONEYPOT
COPY ./pam-nss-modules ./pam-nss-modules
RUN cd ./pam-nss-modules && bash -x ./build-install-nss-pam.sh

# DATA WILL BE IN /var/honeypot
VOLUME /var/honeypot

# EXPOSE PORT 22 AND RUN SSHD
EXPOSE 22
ENTRYPOINT  /etc/init.d/ssh start && tail -f /dev/null




