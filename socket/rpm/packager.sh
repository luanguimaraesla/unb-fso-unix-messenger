#! /bin/bash

PACKAGE_NAME="unix-messenger"

# Install Development Tools
sudo yum groupinstall "Development Tools" 
sudo yum install rpm-build rpmdevtools

# Install Dependencies
sudo yum install tmux gcc make

# Generate RPM package tree
rpmdev-setuptree

sleep 2

# Move Tarball to the RPM SOURCE dir
cp *.tar.gz $HOME/rpmbuild/SOURCES
cp *.spec   $HOME/rpmbuild/SPECS

# Generate rpm package
rpmbuild -ba $PACKAGE_NAME.spec
