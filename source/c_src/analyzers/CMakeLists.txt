# ================================
# CodeAnalyzer - CMakeLists.txt
#
# Unified aggregation of analyzer submodules (modular build)
# ================================

# ================================
# Core Analyzer Library
# ================================
# Collect Core analyzer sources
set(CORE_ANALYZER_SOURCES
        private/core/analyzer/Analyzer.cpp
        private/core/context/AnalysisContext.cpp
        private/core/ir/ComplexityAnalyzer.cpp
        private/core/ir/ControlFlowAnalyzer.cpp
        private/core/ir/DependencyGraph.cpp
        private/core/ir/IRIndex.cpp
        private/core/report/ReportBuilder.cpp
        private/core/rule_engine/RuleEngine.cpp
        private/core/rule_engine/RuleRegistry.cpp
)

# Core analyzer headers to be installed
set(CORE_PUBLIC_HEADERS
        public/core/analyzer/Analyzer.h
        public/core/context/AnalysisContext.h
        public/core/factory/AnalyzerFactory.h
        public/core/factory/CompileCommandDriverFactory.h
        public/core/ir/ComplexityAnalyzer.h
        public/core/ir/ControlFlowAnalyzer.h
        public/core/ir/DependencyGraph.h
        public/core/ir/IRIndex.h
        public/core/report/AnalysisResult.h
        public/core/report/ReportBuilder.h
        public/core/rule_engine/RuleEngine.h
        public/core/rule_engine/RuleRegistry.h
)

# Build Core analyzer as a static library
add_library(core_analyzer STATIC ${CORE_ANALYZER_SOURCES})

# Include directories for Core analyzer
target_include_directories(core_analyzer
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/core
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/core
)

# ================================
# Aggregated Analyzer Library
# ================================
add_library(analyzers STATIC)

# Link the core analyzer (and potentially other analyzer libraries in the future)
target_link_libraries(analyzers PRIVATE core_analyzer)

# Include directories for all analyzers
target_include_directories(analyzers
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
)

