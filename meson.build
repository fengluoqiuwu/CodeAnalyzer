project('CodeAnalyzer',
        ['c', 'cpp'],
        version: '0.1.0',
        license: 'CC-BY-NC-4.0',
        meson_version: '>=1.2.99',
        default_options: [
                'buildtype=debugoptimized',
                'b_ndebug=if-release',
                'c_std=c2x',
                'cpp_std=c++23'
        ]
)

# Get compiler information for C and C++
cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

# ----------------------------
# ✅ Compiler version checks
# ----------------------------
# Ensure the compiler supports C23 / C++23
cc_id = cc.get_id()

if cc_id == 'gcc'
        if not cc.version().version_compare('>=13.0')
                error('CodeAnalyzer requires GCC >= 13.0 for full C23/C++23 support')
        endif

elif cc_id == 'clang'
        error('❌ Clang is not officially supported for this project (C23/C++23 incomplete)')

elif cc_id == 'msvc'
        # Visual Studio 2022 17.6+ → MSVC version 19.36+
        if not cc.version().version_compare('>=19.36')
                error('CodeAnalyzer requires MSVC >= 19.36 (Visual Studio 2022 version 17.6+) for C++23')
        endif

else
        error('❌ Unsupported compiler: ' + cc_id)
endif

# ----------------------------
# ✅ Python.h header check
# ----------------------------
# Used by pybind11 to build Python bindings.
py = import('python').find_installation(pure: false)
py_dep = py.dependency()

if not cc.has_header('Python.h', dependencies: py_dep)
        error('❌ Missing Python.h. Please install python-dev / python-devel')
endif

# ----------------------------
# ✅ Add global compile arguments
# ----------------------------
# Disable strict aliasing (safe for complex systems with aggressive optimization)
add_project_arguments(
        cc.get_supported_arguments('-fno-strict-aliasing'),
        language: ['c', 'cpp']
)

# ----------------------------
# ✅ Floating-point math handling for Clang
# ----------------------------
# Check whether Clang supports strict floating-point exception behavior.
cc_id = cc.get_id()
if cc_id.startswith('clang')
        trapping_math = '-ftrapping-math'
        if cc.has_argument(trapping_math)
                if cc.compiles('int main() { return 0; }', args: [trapping_math, '-Werror'])
                        trapping_math = [trapping_math, '-DNPY_HAVE_CLANG_FPSTRICT']
                else
                        trapping_math = [trapping_math, '-Wno-unsupported-floating-point-opt']
                        message('Clang does not fully support strict floating-point exceptions. Compatibility workaround enabled.')
                endif
                add_project_arguments(trapping_math, language: ['c', 'cpp'])
        endif
endif

# ----------------------------
# ✅ macOS specific linker flags
# ----------------------------
# On macOS 14+, avoid the new linker which may cause compatibility issues
if host_machine.system() == 'darwin' and cc.has_link_argument('-Wl,-ld_classic')
        add_project_link_arguments('-Wl,-ld_classic', language: ['c', 'cpp'])
endif

# ----------------------------
# ✅ Subdirectory structure
# ----------------------------
subdir('source')
