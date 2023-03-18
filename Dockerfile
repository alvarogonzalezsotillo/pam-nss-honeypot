FROM debian

# APT CACHE
RUN rm -f /etc/apt/apt.conf.d/docker-clean; echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked --mount=type=cache,target=/var/lib/apt,sharing=locked

# INSTALL TOOLS
RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y tmux openssh-server build-essential libpam-dev libnss3-dev sudo

COPY ./pam-nss-modules ./pam-nss-modules

RUN ls -l ./pam-nss-modules

RUN cd ./pam-nss-modules && bash -x ./build-install.sh

# AVOID ERROR: Missing privilege separation directory: /run/sshd
RUN mkdir /run/sshd

EXPOSE 22

#CMD ["/usr/sbin/sshd","-d","-D"]
# CMD [ "/usr/bin/tail","-f","/dev/null"]
CMD /etc/init.d/ssh start && tail -f /dev/null
