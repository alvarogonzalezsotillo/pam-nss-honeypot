# -*- mode: ruby -*-
# vi: set ft=ruby :


def base_configuration(config)
  config.vm.box = "debian/bullseye64"

  if Vagrant.has_plugin?("vagrant-vbguest")
    config.vbguest.auto_update = false
  end
  
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--memory", 1024]
    vb.customize ["modifyvm", :id, "--groups", "/vagrant"]
  end


  config.vm.provision "shell", inline: <<-EOF
    ls -la /vagrant
  EOF
  
end

def set_hostname(config,hostname)
  config.vm.hostname = hostname
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--name", hostname]
  end
end


Vagrant.configure("2") do |config|
  
  config.vm.define "honeypot" do |web|
    base_configuration(web)
    set_hostname(web,"honeypot")
  end

  config.vm.define "nat-firewall" do |web|
    base_configuration(web)
    set_hostname(web,"nat-firewall")
  end
end
