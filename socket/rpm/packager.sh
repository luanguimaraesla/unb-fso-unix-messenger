#! /bin/bash

PACKAGE_NAME="unix-messenger"

# Install Development Tools
sudo yum groupinstall "Development Tools" 

# Generate RPM package tree
rpmdev-setuptree

# Move Tarball to the RPM SOURCE dir
if [ "$USER" == "root" ]; then
  cp *.tar.gz /$USER/rpmbuild/SOURCES
  cp *.spec   /$USER/rpmbuild/SPECS
else
  cp *.tar.gz /home/$USER/rpmbuild/SOURCES
  cp *.spec   /home/$USER/rpmbuild/SPECS
fi

# Generate rpm package
rpmbuild -ba $PACKAGE_NAME.spec
