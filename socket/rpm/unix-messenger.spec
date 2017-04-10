Summary: Comunication platform
Name:    unix-messenger
Version: 0.0.1
Release: 1
Source0: %{name}-%{version}.tar.gz
License: Various
Url: https://github.com/luanguimaraesla/%{name}
BuildRequires: gcc, tmux

%description
Message tool for Unix system

%prep
%setup -q

%build
make

%install
%make_install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc README LICENSE

%changelog
* Tue Mar 17 2017 Luan Guimarães <livreluan@gmail.com> - 0.0.1-1
- Initial package for CentOS
