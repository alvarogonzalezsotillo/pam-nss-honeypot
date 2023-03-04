# -*- mode: ruby -*-
# vi: set ft=ruby :


def base_configuration(config)
  config.vm.box = "debian/bullseye64"

  if Vagrant.has_plugin?("vagrant-vbguest")
    config.vbguest.auto_update = true
  end
  
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--memory", 1024]
    vb.customize ["modifyvm", :id, "--groups", "/vagrant"]
    vb.customize ["modifyvm", :id, "--nested-hw-virt", "on"]
  end
end

def install_pam_nss_module(config)
  config.vm.provision "shell", inline: <<-EOF
    sudo apt-get update
    sudo apt-get install build-essential
    cd /vagrant/pam-nss-modules
    sudo ./build-install.sh
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
