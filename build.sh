
docker_build(){
    PROGRESS_NO_TRUNC=1 docker build --progress plain -t pam-nss-honeypot .
}

docker_run(){
    docker rm pam-nss-honeypot
    docker run -p 22022:22 --volume var-honeypot:/var/honeypot --hostname thisis.notahoney.pot --name pam-nss-honeypot pam-nss-honeypot 
}

docker_build && docker_run

