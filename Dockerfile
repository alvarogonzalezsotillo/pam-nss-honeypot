FROM debian

# APT CACHE
RUN rm -f /etc/apt/apt.conf.d/docker-clean; echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked --mount=type=cache,target=/var/lib/apt,sharing=locked


RUN \ # AVOID ERROR: Missing privilege separation directory: /run/sshd
    mkdir /run/sshd && \ 
    \ # HOPE THIS PASSWORD WILL BE DIFICULT
    bash -c "chpasswd root:\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM\$RANDOM" && \ 
    \ # CREATE FAKE USER, LINK TO USER 1000 OF CONTAINER HOST
    RUN useradd --uid 1000 --user-group legituser-notfake && \
    \ # INSTALL TOOLS
    apt-get update && \
    apt-get upgrade && \
    apt-get install -y tmux openssh-server build-essential libpam-dev libnss3-dev sudo


# COMPILE AND INSTALL HONEYPOT
COPY ./pam-nss-modules ./pam-nss-modules
RUN cd ./pam-nss-modules && bash -x ./build-install.sh

# DATA WILL BE IN /var/honeypot
VOLUME /var/honeypot

# EXPOSE PORT 22 AND RUN SSHD
EXPOSE 22
ENTRYPOINT  /etc/init.d/ssh start && tail -f /dev/null
