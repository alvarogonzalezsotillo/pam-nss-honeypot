# -*- mode: ruby -*-
# vi: set ft=ruby :


def guest_additions(config)

  config.vbguest.auto_update = true
  config.vbguest.no_remote = false

  config.vm.provision "shell", inline: <<-EOS
    sudo apt-get update
    sudo apt-get -y install tmux tcpdump build-essential
  EOS
end

def set_hostname(config,hostname)
  config.vm.hostname = hostname
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--name", hostname]
  end
end


def base_configuration(config,hostname)
  config.vm.box = "debian/bullseye64"

  guest_additions(config)

  set_hostname(config,hostname)
  
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

HONEYPOT_IP = "10.4.5.45"
NAT_INTERNAL_IP = "10.4.5.27"
NAT_SSH_EXTERNAL_PORT = 9876

def internal_network(config,this_is_honeypot)
  ip = this_is_honeypot ? HONEYPOT_IP : NAT_INTERNAL_IP
  
  config.vm.network "private_network", :ip => ip, :adapter => 2, virtualbox__intnet: true
  if this_is_honeypot
    # EL GATEWAY ES EL OTRO
    config.vm.provision "shell", run: "always", inline: <<-EOS
       ip route delete default
       ip route add default via #{NAT_INTERNAL_IP}
    EOS
  else
    # ESTE ES EL GATEWAY
    config.vm.network "forwarded_port", guest: 9876, host: 9876
    config.vm.provision "shell", run: "always", inline: <<-EOF
      sysctl -w net.ipv4.ip_forward=1
      iptables -P INPUT ACCEPT
      iptables -P FORWARD ACCEPT
      iptables -P OUTPUT ACCEPT
      iptables -t nat -F
      iptables -t mangle -F
      iptables -F
      iptables -X
      iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
      iptables -t nat -A PREROUTING -p tcp --dport #{NAT_SSH_EXTERNAL_PORT} -i eth0 -j DNAT --to #{HONEYPOT_IP}:22
    EOF
  end
end


Vagrant.configure("2") do |config|
  
  config.vm.define "honeypot" do |honeypot|
    base_configuration(honeypot, "honeypot")
    install_pam_nss_module(honeypot)
    internal_network(honeypot,true)
  end

  config.vm.define "nat-firewall" do |natfirewall|
    base_configuration(natfirewall, "nat-firewall")
    internal_network(natfirewall,false)
  end
end
