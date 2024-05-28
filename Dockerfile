FROM debian:stretch

# APT CACHE. CHECK DATE, BECAUSE SOME IMAGES ARE INCOMPATIBLE.
RUN date
RUN apt-get clean && apt-get update && apt-get -y upgrade


# INSTALL TOOLS
RUN apt-get install -y tmux openssh-server build-essential libpam-dev libnss3-dev sudo

# CREATE FAKE USER, LINK TO USER 1000 OF CONTAINER HOST
RUN useradd --uid 1000 --user-group legituser-notfake

# COMPILE AND INSTALL HONEYPOT
COPY ./pam-nss-modules ./pam-nss-modules
RUN cd ./pam-nss-modules && bash -x ./build-install.sh

# AVOID ERROR: Missing privilege separation directory: /run/sshd
RUN mkdir /run/sshd

# DATA WILL BE IN /var/honeypot
VOLUME /var/honeypot

# HOPE THIS PASSWORD WILL BE DIFICULT
RUN bash -c "echo root:\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM"
RUN bash -c "chpasswd root:\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM"

# EXPOSE PORT 22 AND RUN SSHD
EXPOSE 22
ENTRYPOINT  /etc/init.d/ssh start && tail -f /dev/null
