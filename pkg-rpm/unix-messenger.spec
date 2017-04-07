Summary: Comunication platform
Name:    unix-messenger
Version: 0.0.1
Release: 1
Source0: unix-messenger-%{version}.tar.gz
License: Various
Group: Development/Tools
BuildRoot: %{_tmppath}/unix-messenger-%{version}-%{release}-buildroot
Prefix: %{_prefix}
Vendor:  Luan Guimarães <livreluan@gmail.com>
Url: https://github.com/luanguimaraesla/unix-messenger
BuildRequires: gcc, tmux

%description
Integrated software development platform (Python dependencies).

%prep
%setup -q

%build
cd %{_builddir}
make

%install
cd %{_builddir}
%make_install

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/lib/colab
%defattr(,root,root)
