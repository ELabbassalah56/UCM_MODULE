Summary: ucm-package-manager version 1.0-r1
Name: ucm-package-manager
Version: 1.0
Release: r1
License: AUTOSAR
Group: devel
Packager: Poky <poky@yoctoproject.org>
BuildRequires: apd-cmake-modules-native
BuildRequires: ara-arxmls-native
BuildRequires: ara-com-lib
BuildRequires: ara-core
BuildRequires: ara-exec
BuildRequires: ara-gen-native
BuildRequires: ara-log
BuildRequires: ara-per
BuildRequires: boost
BuildRequires: cmake-native
BuildRequires: gtest
BuildRequires: ninja-native
BuildRequires: poco
BuildRequires: python3-native
BuildRequires: ucm-arxmls-native
BuildRequires: virtual/i586-poky-linux-compilerlibs
BuildRequires: virtual/i586-poky-linux-gcc
BuildRequires: virtual/libc
Requires: dlt-daemon >= 1.0
Requires: libPocoFoundation.so.60
Requires: libPocoZip.so.60
Requires: libboost-filesystem1.68.0 >= 1.68.0
Requires: libboost-system1.68.0 >= 1.68.0
Requires: libboost-thread1.68.0 >= 1.68.0
Requires: libboost_filesystem.so.1.68.0
Requires: libboost_system.so.1.68.0
Requires: libboost_thread.so.1.68.0
Requires: libc.so.6
Requires: libc.so.6(GLIBC_2.0)
Requires: libc.so.6(GLIBC_2.1.3)
Requires: libc.so.6(GLIBC_2.2)
Requires: libc.so.6(GLIBC_2.28)
Requires: libc.so.6(GLIBC_2.3.4)
Requires: libc.so.6(GLIBC_2.4)
Requires: libc.so.6(GLIBC_2.7)
Requires: libc6 >= 2.28
Requires: libdlt.so.2
Requires: libgcc1 >= 8.2.0
Requires: libgcc_s.so.1
Requires: libgcc_s.so.1(GCC_3.0)
Requires: libgcc_s.so.1(GLIBC_2.0)
Requires: libpocofoundation60 >= 1.9.0
Requires: libpocozip60 >= 1.9.0
Requires: libpthread.so.0
Requires: libpthread.so.0(GLIBC_2.0)
Requires: libpthread.so.0(GLIBC_2.1)
Requires: libpthread.so.0(GLIBC_2.3.2)
Requires: libpthread.so.0(GLIBC_2.3.3)
Requires: libstdc++.so.6
Requires: libstdc++.so.6(CXXABI_1.3)
Requires: libstdc++.so.6(CXXABI_1.3.11)
Requires: libstdc++.so.6(CXXABI_1.3.3)
Requires: libstdc++.so.6(CXXABI_1.3.5)
Requires: libstdc++.so.6(CXXABI_1.3.7)
Requires: libstdc++.so.6(CXXABI_1.3.9)
Requires: libstdc++.so.6(GLIBCXX_3.4)
Requires: libstdc++.so.6(GLIBCXX_3.4.11)
Requires: libstdc++.so.6(GLIBCXX_3.4.14)
Requires: libstdc++.so.6(GLIBCXX_3.4.15)
Requires: libstdc++.so.6(GLIBCXX_3.4.18)
Requires: libstdc++.so.6(GLIBCXX_3.4.19)
Requires: libstdc++.so.6(GLIBCXX_3.4.20)
Requires: libstdc++.so.6(GLIBCXX_3.4.21)
Requires: libstdc++.so.6(GLIBCXX_3.4.22)
Requires: libstdc++.so.6(GLIBCXX_3.4.9)
Requires: libstdc++6 >= 8.2.0
Requires: libvsomeip.so.2
Requires: poco
Requires: rtld(GNU_HASH)
Requires: ucm-packages
Requires: vsomeip >= 2.10.21

%description
ucm-package-manager version 1.0-r1.

%package -n ucm-package-manager-test
Summary: ucm-package-manager version 1.0-r1
License: AUTOSAR
Group: devel
Requires: dlt-daemon >= 1.0
Requires: libPocoFoundation.so.60
Requires: libPocoZip.so.60
Requires: libboost-filesystem1.68.0 >= 1.68.0
Requires: libboost-system1.68.0 >= 1.68.0
Requires: libboost-thread1.68.0 >= 1.68.0
Requires: libboost_filesystem.so.1.68.0
Requires: libboost_system.so.1.68.0
Requires: libboost_thread.so.1.68.0
Requires: libc.so.6
Requires: libc.so.6(GLIBC_2.0)
Requires: libc.so.6(GLIBC_2.1)
Requires: libc.so.6(GLIBC_2.1.3)
Requires: libc.so.6(GLIBC_2.2)
Requires: libc.so.6(GLIBC_2.28)
Requires: libc.so.6(GLIBC_2.3.4)
Requires: libc.so.6(GLIBC_2.4)
Requires: libc.so.6(GLIBC_2.7)
Requires: libc6 >= 2.28
Requires: libdlt.so.2
Requires: libgcc1 >= 8.2.0
Requires: libgcc_s.so.1
Requires: libgcc_s.so.1(GCC_3.0)
Requires: libgcc_s.so.1(GLIBC_2.0)
Requires: libpocofoundation60 >= 1.9.0
Requires: libpocozip60 >= 1.9.0
Requires: libpthread.so.0
Requires: libpthread.so.0(GLIBC_2.0)
Requires: libpthread.so.0(GLIBC_2.1)
Requires: libpthread.so.0(GLIBC_2.3.2)
Requires: libpthread.so.0(GLIBC_2.3.3)
Requires: libstdc++.so.6
Requires: libstdc++.so.6(CXXABI_1.3)
Requires: libstdc++.so.6(CXXABI_1.3.11)
Requires: libstdc++.so.6(CXXABI_1.3.3)
Requires: libstdc++.so.6(CXXABI_1.3.5)
Requires: libstdc++.so.6(CXXABI_1.3.7)
Requires: libstdc++.so.6(CXXABI_1.3.9)
Requires: libstdc++.so.6(GLIBCXX_3.4)
Requires: libstdc++.so.6(GLIBCXX_3.4.11)
Requires: libstdc++.so.6(GLIBCXX_3.4.14)
Requires: libstdc++.so.6(GLIBCXX_3.4.15)
Requires: libstdc++.so.6(GLIBCXX_3.4.18)
Requires: libstdc++.so.6(GLIBCXX_3.4.19)
Requires: libstdc++.so.6(GLIBCXX_3.4.20)
Requires: libstdc++.so.6(GLIBCXX_3.4.21)
Requires: libstdc++.so.6(GLIBCXX_3.4.22)
Requires: libstdc++.so.6(GLIBCXX_3.4.9)
Requires: libstdc++6 >= 8.2.0
Requires: libvsomeip.so.2
Requires: rtld(GNU_HASH)
Requires: vsomeip >= 2.10.21

%description -n ucm-package-manager-test
ucm-package-manager version 1.0-r1.

%package -n ucm-package-manager-dbg
Summary: ucm-package-manager version 1.0-r1 - Debugging files
License: AUTOSAR
Group: devel
Recommends: boost-filesystem-dbg
Recommends: boost-system-dbg
Recommends: boost-thread-dbg
Recommends: dlt-daemon-dbg
Recommends: libc6-dbg
Recommends: libgcc-s-dbg
Recommends: libstdc++-dbg
Recommends: poco-foundation-dbg
Recommends: poco-zip-dbg
Recommends: vsomeip-dbg

%description -n ucm-package-manager-dbg
ucm-package-manager version 1.0-r1.  This package contains ELF symbols and
related sources for debugging purposes.

%package -n ucm-package-manager-dev
Summary: ucm-package-manager version 1.0-r1 - Development files
License: AUTOSAR
Group: devel
Requires: ucm-package-manager = 1.0-r1
Recommends: ara-com-lib-dev
Recommends: ara-core-dev
Recommends: ara-exec-dev
Recommends: ara-log-dev
Recommends: ara-per-dev
Recommends: boost-dev
Recommends: boost-filesystem-dev
Recommends: boost-system-dev
Recommends: boost-thread-dev
Recommends: dlt-daemon-dev
Recommends: gtest-dev
Recommends: libc6-dev
Recommends: libgcc-s-dev
Recommends: libstdc++-dev
Recommends: poco-dev
Recommends: poco-foundation-dev
Recommends: poco-zip-dev
Recommends: ucm-packages-dev
Recommends: vsomeip-dev

%description -n ucm-package-manager-dev
ucm-package-manager version 1.0-r1.  This package contains symbolic links,
header files, and related items necessary for software development.

%files
%defattr(-,-,-,-)
%dir "/var"
%dir "/opt"
%dir "/var/lib"
%dir "/var/lib/apd_ucm"
%dir "/var/lib/apd_ucm/sample"
%dir "/var/lib/apd_ucm/tmp"
"/var/lib/apd_ucm/sample/PROCESSES.json"
%dir "/opt/package_manager"
%dir "/opt/package_manager/var"
%dir "/opt/package_manager/etc"
%dir "/opt/package_manager/bin"
"/opt/package_manager/var/kvsdatabase.json"
"/opt/package_manager/etc/MANIFEST.json"
"/opt/package_manager/etc/vsomeip.json"
"/opt/package_manager/bin/package_manager"

%files -n ucm-package-manager-test
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/var"
%dir "/usr/bintest"
%dir "/usr/share"
%dir "/usr/var/apdtest"
%dir "/usr/var/apdtest/ucm"
%dir "/usr/var/apdtest/ucm/opt"
%dir "/usr/var/apdtest/ucm/opt/swcls"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticExample"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticManager"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A"
"/usr/var/apdtest/ucm/opt/swcls/SWCL_MANIFEST.json"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/var"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/etc"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/bin"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/var/kvsdatabase.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/etc/calculator_cfg.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/etc/MANIFEST.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticExample/bin/Calculator"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/var"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/etc"
%dir "/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/bin"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/var/kvsdatabase.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/etc/MANIFEST.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/etc/dm_config.json"
"/usr/var/apdtest/ucm/opt/swcls/TEST_VENDOR_A/DiagnosticManager/bin/DiagnosticManager"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticExample/var"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticExample/etc"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticExample/bin"
"/usr/var/apdtest/ucm/opt/DiagnosticExample/var/kvsdatabase.json"
"/usr/var/apdtest/ucm/opt/DiagnosticExample/etc/calculator_cfg.json"
"/usr/var/apdtest/ucm/opt/DiagnosticExample/etc/MANIFEST.json"
"/usr/var/apdtest/ucm/opt/DiagnosticExample/bin/Calculator"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticManager/var"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticManager/etc"
%dir "/usr/var/apdtest/ucm/opt/DiagnosticManager/bin"
"/usr/var/apdtest/ucm/opt/DiagnosticManager/var/kvsdatabase.json"
"/usr/var/apdtest/ucm/opt/DiagnosticManager/etc/MANIFEST.json"
"/usr/var/apdtest/ucm/opt/DiagnosticManager/etc/dm_config.json"
"/usr/var/apdtest/ucm/opt/DiagnosticManager/bin/DiagnosticManager"
"/usr/bintest/apd-ucm-testpackage-manager"
%dir "/usr/share/apdtest"
%dir "/usr/share/tests"
%dir "/usr/share/apdtest/ucm"
"/usr/share/apdtest/ucm/GetHistoryTestDatabase.json"
"/usr/share/tests/apd-ucm-testpackage-manager"

%files -n ucm-package-manager-dbg
%defattr(-,-,-,-)
%dir "/opt"
%dir "/usr"
%dir "/opt/package_manager"
%dir "/opt/package_manager/bin"
%dir "/opt/package_manager/bin/.debug"
"/opt/package_manager/bin/.debug/package_manager"
%dir "/usr/bintest"
%dir "/usr/src"
%dir "/usr/bintest/.debug"
"/usr/bintest/.debug/apd-ucm-testpackage-manager"
%dir "/usr/src/debug"
%dir "/usr/src/debug/ucm-package-manager"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/ara_com_main-package_manager_swc.cpp"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/error_domain_ucmerrordomain.h"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_actiontype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_activateoptiontype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_transferidtype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_gethistorytype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_ucminfoversion.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_logvectortype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_logentrytype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swpackagestatetype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_gethistoryvectortype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swpackageinfovectortype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_ucminfoname.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swinfoname.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_logleveltype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/packagemanagement_common.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/packagemanagement_skeleton.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_strongrevisionlabelstring.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swclusterinfovectortype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swclusterinfotype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_logmessagetype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_ucminfovectorofstringtype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_ucminfotype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swclusterstatetype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_resulttype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_bytevectortype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_swpackageinfotype.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_ucminforelease.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/includes/ara/ucm/pkgmgr/impl_type_packagemanagerstatustype.h"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/vsomeip_service_mapping-package_manager_swc.cpp"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm"
%dir "/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm/pkgmgr"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm/pkgmgr/adapter_packagemanagement.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm/pkgmgr/errors_packagemanagement.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm/pkgmgr/service_desc_packagemanagement.h"
"/usr/src/debug/ucm-package-manager/1.0-r1/ucm-package-manager-1.0/gen/vsomeip/ara/ucm/pkgmgr/service_desc_packagemanagement.cpp"

%files -n ucm-package-manager-dev
%defattr(-,-,-,-)

