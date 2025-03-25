//
// Created by Eden_ on 2025/3/26.
//

#ifndef CANALYZER_H
#define CANALYZER_H

//void CAnalyzer::analyze(const std::string& filepath) {
//    // 1. AST 构建
//    auto ast = CASTBuilder::build(filepath);
//
//    // 2. 头文件分析
//    CIncludeAnalyzer::analyze(filepath);
//    CIncludeGraphBuilder::build(ast);
//
//    // 3. 宏分析
//    CMacroAnalyzer::scan(ast);
//    CMacroExpansionEngine::expand(ast);
//
//    // 4. 构建符号表
//    auto symbols = CSymbolResolver::resolve(ast);
//
//    // 5. 类型检查
//    CTypeChecker::check(ast, symbols);
//
//    // 6. 控制流构建 + 流分析
//    auto cfg = CCFGBuilder::build(ast);
//    CDataFlowAnalyzer::analyze(cfg);
//    CLivenessAnalyzer::analyze(cfg);
//
//    // 7. 语义 + 结构分析
//    CSemanticChecker::check(ast, symbols);
//    CReturnChecker::check(ast);
//    CUnreachableCodeDetector::analyze(cfg);
//
//    // 8. 内存 & 指针分析
//    CMemoryLeakAnalyzer::scan(ast);
//    CPointerSafetyAnalyzer::check(ast, symbols);
//
//    // 9. 风格 & 危险 API
//    CStyleChecker::check(ast);
//    CUnsafeFunctionChecker::check(ast);
//}

#endif //CANALYZER_H
