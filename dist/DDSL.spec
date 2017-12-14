Summary: 	Dijkstra's Data Science Language
Name: 		DDSL
Version: 	0.0.1
Release: 	1%{?dist}
Group: 		Development/Libraries
License: 	MIT

Source0: 	%{name}-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:	git
BuildRequires:	gcc-c++
BuildRequires:	arrayfire
BuildRequires:	caffe
Requires:	git
Requires:	rpm-build
Requires:	arrayfire

BuildArch:	noarch

%define gitbin %(git --exec-path)

%description
Dijkstra's Data Science Language

%prep
%setup -q -n %{name}-%{version}

%build
%cmake
make %{?_smp_mflags}

%install


%check
ctest -V %{?_smp_mflags}