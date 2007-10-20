Summary: a HTML to text converter
Name: vilistextum
Version: 2.6.9
Release: 1
Source: vilistextum-2.6.9.tar.gz
Copyright: GPL version 2
Group: System/Utilities
Packager:  Patric Mueller <bhaak@gmx.net>
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)

%description
Vilistextum is a small and fast HTML to text / ascii converter
specifically programmed to get the best out of incorrect html. It is
quite fault-tolerant and deals well with badly-formed or otherwise
quirky HTML. It is able to optimize for ebook reading, collapse
multiple blank lines, and create footnotes out of links.


%prep
%setup -q
%configure

%build
make

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT"
%makeinstall

strip $RPM_BUILD_ROOT%{_bindir}/*

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
%{_bindir}/*
%{_mandir}/man1/*
%doc README CHANGES COPYING vilistextum.spec
%doc doc/*.html
%doc src/kilistextum.kaptn
