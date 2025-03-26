# ================================
# CodeAnalyzer - analyzers/meson.build
# Unified aggregation of analyzer submodules (modular build)
# ================================


# ================================
# Core Analyzer Library
# ================================

# Collect Core analyzer sources
core_analyzer_sources = files(
    'private/core/analyzer/Analyzer.cpp',
    'private/core/context/AnalysisContext.cpp',
    'private/core/ir/ComplexityAnalyzer.cpp',
    'private/core/ir/ControlFlowAnalyzer.cpp',
    'private/core/ir/DependencyGraph.cpp',
    'private/core/ir/IRIndex.cpp',
    'private/core/report/ReportBuilder.cpp',
    'private/core/rule_engine/RuleEngine.cpp',
    'private/core/rule_engine/RuleRegistry.cpp'
)

# Core analyzer headers to be installed
core_public_headers = files(
    'public/core/analyzer/Analyzer.h',
    'public/core/context/AnalysisContext.h',
    'public/core/factory/AnalyzerFactory.h',
    'public/core/factory/CompileCommandDriverFactory.h',
    'public/core/ir/ComplexityAnalyzer.h',
    'public/core/ir/ControlFlowAnalyzer.h',
    'public/core/ir/DependencyGraph.h',
    'public/core/ir/IRIndex.h',
    'public/core/report/AnalysisResult.h',
    'public/core/report/ReportBuilder.h',
    'public/core/rule_engine/RuleEngine.h',
    'public/core/rule_engine/RuleRegistry.h'
)

# Build Core analyzer as a static library
core_analyzer_lib = static_library(
    'core_analyzer',
    core_analyzer_sources,
    include_directories: [
        include_directories('public/core'),
        include_directories('private/core')
    ],
    install: false
)

# Declare Core analyzer dependency
core_analyzer_dep = declare_dependency(
    include_directories: include_directories('public/core'),
    link_with: core_analyzer_lib
)

# Install headers related to Core analyzer
install_headers(core_public_headers, subdir: 'code_analyzer/analyzers/core')


# ================================
# Aggregated Analyzer Library
# ================================

analyzers_lib = static_library(
    'analyzers',
    sources: [],
    include_directories: include_directories('public'),
    link_with: [
        #    c_analyzer_lib,
        #    cpp_analyzer_lib,
        #    java_analyzer_lib,
        core_analyzer_lib
    ],
    install: true
)

analyzers_dep = declare_dependency(
    include_directories: include_directories('public'),
    link_with: analyzers_lib
)

# Install general analyzer interface headers
install_headers([
                    'public/IAnalyzer.h',
                    'public/ICompileCommandDriver.h',
                    'public/Language.h'
                ], subdir: 'code_analyzer/analyzers')
