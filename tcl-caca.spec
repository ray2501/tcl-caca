#
# spec file for package tcl-caca
#

%{!?directory:%define directory /usr}
%define packagename caca

Name:           tcl-caca
Version:        0.8
Release:        0
Summary:        Tcl bindings for Colour ASCII Art library (libcaca) 
License:        MIT
Group:          Development/Libraries/Tcl
Url:            https://github.com/ray2501/tcl-caca
Source:         %{name}-%{version}.tar.gz
BuildRequires:  autoconf
BuildRequires:  make
BuildRequires:  gcc
BuildRequires:  tcl-devel >= 8.6
BuildRequires:  libcaca-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
This package is Tcl bindings for Colour ASCII Art library (libcaca).

%prep
%setup -q -n %{name}-%{version}

%build
./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{packagename}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%doc README.md LICENSE
%{tcl_archdir}

%changelog

