Summary: A simple non-curses based HTML display program
Name: vilistextum
Version: 2.6.6
Release: 1
Source: vilistextum-2.6.6.tar.gz
Copyright: GPL version 2
Group: System/Utilities
Packager:  Joshua Jensen <jjensen@cisco.com>
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)

%description
Vilistextum is a html to text / ascii converter specifically programmed
to get the best out of incorrect html.

%prep
%setup -q

%build
pwd
%configure
make
%makeinstall

%install
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT
install -D src/vilistextum $RPM_BUILD_ROOT/usr/bin/vilistextum
install -D doc/vilistextum.1 $RPM_BUILD_ROOT/%{_mandir}/man1/vilistextum.1


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/vilistextum
%{_mandir}/man1/*
%doc doc/*html
%doc doc/*css
%doc src/kilistextum.kaptn
