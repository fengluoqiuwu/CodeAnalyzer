# ================================
# CodeAnalyzer - source/meson.build
# Central build dispatcher for submodules
# ================================

# Build the core C/C++ source code
subdir('c_src')

# Conditionally build CLI interface
if get_option('enable-cli')
    subdir('cli')
endif

# Conditionally build Python bindings
if get_option('enable-python-bindings')
    subdir('bindings')
endif

# Always include third-party dependencies
subdir('third_party')
