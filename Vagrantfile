# -*- mode: ruby -*-
# vi: set ft=ruby :


def base_configuration(config)
  config.vm.box = "debian/bullseye64"

  config.vbguest.auto_update = true
  config.vbguest.no_remote = false

  config.vm.provision "shell", inline: <<-EOS
    sudo apt-get update
    sudo apt-get -y install tmux build-essential # for guest additions
  EOS
  
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--memory", 1024]
    vb.customize ["modifyvm", :id, "--groups", "/vagrant"]
    vb.customize ["modifyvm", :id, "--nested-hw-virt", "on"]
  end
end

def install_pam_nss_module(config)
  config.vm.provision "shell", inline: <<-EOF
    sudo apt-get update
    sudo apt-get -y install libpam-dev libnss3-dev
    cd /vagrant/pam-nss-modules
    sudo ./build-install.sh > build-install.log
  EOF
end

def set_hostname(config,hostname)
  config.vm.hostname = hostname
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--name", hostname]
  end
end


Vagrant.configure("2") do |config|
  
  config.vm.define "honeypot" do |honeypot|
    base_configuration(honeypot)
    set_hostname(honeypot,"honeypot")
    install_pam_nss_module(honeypot)
  end

  config.vm.define "nat-firewall" do |natfirewall|
    base_configuration(natfirewall)
    set_hostname(natfirewall,"nat-firewall")
  end
end
