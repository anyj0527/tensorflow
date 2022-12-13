Name:           tensorflow2
Summary:        TensorFlow 2.x
Version:        2.11.0
Release:        1
License:        Apache-2.0
Source0:        %{name}-%{version}.tar.gz
Source1001:     tensorflow2.manifest
Source1002:     tensorflow2-lite.pc.in
Source2001:     abseil-cpp.tar.gz
Source2002:     egl_headers.tar.gz
Source2003:     fft2d.tar.gz
Source2004:     fxdiv.tar.gz
Source2005:     opencl_headers.tar.gz
Source2006:     pthreadpool.tar.gz
Source2007:     XNNPACK.tar.gz
# from https://github.com/anyj0527/XNNPACK/commit/72c14f4ed54239d62a2769b14b570be70edc8a7a
Source2008:     XNNPACK-armv7l.tar.gz
Source2009:     clog.tar.gz
Source2010:     eigen.tar.gz
Source2011:     flatbuffers.tar.gz
Source2012:     gemmlowp.tar.gz
Source2013:     opengl_headers.tar.gz
Source2014:     ruy.tar.gz
Source2015:     cpuinfo.tar.gz
Source2016:     farmhash.tar.gz
Source2017:     fp16.tar.gz
Source2018:     neon2sse.tar.gz
Source2019:     psimd.tar.gz
Source2020:     vulkan_headers.tar.gz
Source2021:     cpuinfo-xnnpack.tar.gz
Source2022:     clog-xnnpack.tar.gz

BuildRequires: cmake
BuildRequires: ninja

%description
TensorFlow is an end-to-end open source platform for machine learning. It has
a comprehensive, flexible ecosystem of tools, libraries, and community resources
that lets researchers push the state-of-the-art in ML and developers easily
build and deploy ML-powered applications.

TensorFlow was originally developed by researchers and engineers working on the
Google Brain team within Google's Machine Intelligence Research organization to
conduct machine learning and deep neural networks research. The system is general
enough to be applicable in a wide variety of other domains, as well.

TensorFlow provides stable Python and C++ APIs, as well as non-guaranteed backward
compatible API for other languages.

%package lite-devel
Summary: TensorFlow Lite development headers and object file

%description lite-devel
TensorFlow Lite development headers and object file

%package lite-flatbuf-schema
Summary: TensorFlow Lite schema file
Requires: tensorflow2-lite-devel = %{version}-%{release}

%description lite-flatbuf-schema
TensorFlow Lite schema file

%ifnarch %arm
%package lite-util
Summary: TensorFlow Lite developer util

%description lite-util
It includes a executable to benchmark any tflite model
%endif

%define USE_XNNPACK   "ON"
%define USE_OPENCL    "ON"
%define SHARED_LIB    "OFF"

%prep
%setup -q
cp %{SOURCE1001} .
mkdir -p externals
pushd externals
cp %{SOURCE2001} .
cp %{SOURCE2002} .
cp %{SOURCE2003} .
cp %{SOURCE2004} .
cp %{SOURCE2005} .
cp %{SOURCE2006} .

# detect armv7l
%ifarch armv7l
cp %{SOURCE2008} ./XNNPACK.tar.gz
%else
cp %{SOURCE2007} .
%endif

cp %{SOURCE2009} .
cp %{SOURCE2010} .
cp %{SOURCE2011} .
cp %{SOURCE2012} .
cp %{SOURCE2013} .
cp %{SOURCE2014} .
cp %{SOURCE2015} .
cp %{SOURCE2016} .
cp %{SOURCE2017} .
cp %{SOURCE2018} .
cp %{SOURCE2019} .
cp %{SOURCE2020} .
cp %{SOURCE2021} .
cp %{SOURCE2022} .
popd

%build
mkdir -p build
pushd build

EXTRA_CFLAGS="${CFLAGS}"
EXTRA_CXXFLAGS="${CXXFLAGS}"

%ifarch %arm aarch64
  EXTRA_CFLAGS="${EXTRA_CFLAGS} -funsafe-math-optimizations"
  EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -funsafe-math-optimizations"
  %ifarch %arm
    EXTRA_CFLAGS="${EXTRA_CFLAGS} -mno-unaligned-access"
    EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -mno-unaligned-access"
  %endif
  %ifarch armv7l
    EXTRA_CFLAGS="${EXTRA_CFLAGS} -mfloat-abi=softfp -march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations -mfp16-format=ieee"
    EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -mfloat-abi=softfp -march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations -mfp16-format=ieee"
  %endif
  %ifarch armv7hl
    EXTRA_CFLAGS="${EXTRA_CFLAGS} -mfloat-abi=hard -march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations -mfp16-format=ieee"
    EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -mfloat-abi=hard -march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations -mfp16-format=ieee"
  %endif
%endif

# VD/TV: Disable a link-time optimization (requested on 2020-06-09)
%if "%{?profile}" == "tv"
  EXTRA_CFLAGS="${EXTRA_CFLAGS} -fno-lto"
  EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -fno-lto"
  %ifarch armv7l
    EXTRA_CFLAGS="${EXTRA_CFLAGS} -march=armv7-a -mtune=cortex-a8"
    EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -march=armv7-a -mtune=cortex-a8"
    EXTRA_CFLAGS=${EXTRA_CFLAGS/-mcpu=cortex-a53/}
    EXTRA_CXXFLAGS=${EXTRA_CXXFLAGS/-mcpu=cortex-a53/}
  %endif
%endif

cmake \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
  -GNinja \
  -DCMAKE_C_FLAGS="${EXTRA_CFLAGS}" \
  -DCMAKE_CXX_FLAGS="${EXTRA_CXXFLAGS}" \
  -DTFLITE_BUILD_SHARED_LIB="%{SHARED_LIB}" \
  -DTFLITE_ENABLE_XNNPACK="%{USE_XNNPACK}" \
  -DTFLITE_ENABLE_GPU="%{USE_OPENCL}" \
  ../tensorflow/lite

cmake --build . %{?_smp_mflags}

%ifnarch %arm
  cmake --build . %{?_smp_mflags} -t benchmark_model
%endif

popd

%install
mkdir -p %{buildroot}%{_libdir}
mkdir -p %{buildroot}%{_libdir}/pkgconfig
mkdir -p %{buildroot}%{_includedir}/tensorflow2/lite
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/tensorflow2/lite/schema
pushd %{buildroot}%{_includedir}/tensorflow2
ln -sf . %{buildroot}%{_includedir}/tensorflow2/tensorflow
# TF2Lite users do: #include <tensorflow/FILENAME.h>
popd

cp %{SOURCE1002} .
sed -i 's:@libdir@:%{_libdir}:g
    s:@includedir@:%{_includedir}/tensorflow2/:g' ./tensorflow2-lite.pc.in

# Put the generated files into the buildroot folder
## install built static library and benchmark binary
%if %{SHARED_LIB} == "ON"
  install -m 0644 ./build/libtensorflow-lite.so %{buildroot}%{_libdir}/libtensorflow2-lite.so
%else
  install -m 0644 ./build/libtensorflow-lite-bundled.a %{buildroot}%{_libdir}/libtensorflow2-lite.a
%endif

%ifnarch %arm
  install -m 0655 ./build/tools/benchmark/benchmark_model %{buildroot}%{_bindir}/tflite_benchmark_model
%endif

## install headers
pushd tensorflow/lite
find . -name "*.h" -type f -exec cp --parents {} %{buildroot}%{_includedir}/tensorflow2/lite \;
popd

### rename tensorflow -> tensorflow2
pushd %{buildroot}%{_includedir}/tensorflow2/
for HEADER in `find -name '*.h'`; do
  sed -i 's|#\W*include\W*<tensorflow/|#include <tensorflow2/|' ${HEADER}
  sed -i 's|#\W*include\W*"tensorflow/|#include "tensorflow2/|' ${HEADER}
done
popd

## install flatbuf schema
install -m 0644 tensorflow/lite/schema/schema.fbs %{buildroot}%{_datadir}/tensorflow2/lite/schema

## install pc file
install -m 0644 tensorflow2-lite.pc.in %{buildroot}%{_libdir}/pkgconfig/tensorflow2-lite.pc

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files lite-devel
%defattr(-,root,root,-)
%manifest tensorflow2.manifest
%license LICENSE
%if %{SHARED_LIB} == "ON"
  %{_libdir}/libtensorflow2-lite.so
%else
  %{_libdir}/libtensorflow2-lite.a
%endif
%{_libdir}/pkgconfig/tensorflow2-lite.pc
%{_includedir}/tensorflow2/lite
%{_includedir}/tensorflow2/tensorflow

%files lite-flatbuf-schema
%{_datadir}/tensorflow2/lite/schema/schema.fbs

%ifnarch %arm
%files lite-util
%{_bindir}/tflite_benchmark_model
%endif

%changelog
* Mon Nov 28 2022 Yongjoo Ahn <yongjoo1.ahn@samsung.com>
 - Initial package te use TensorFlow Lite v2.11.0
