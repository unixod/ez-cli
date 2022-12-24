addTest("cli::Parameter" parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("cli::Lexer" lexer
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::C_string_view" c_string_view
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::Uninitialized" uninitialized
    LIBS ez::cli ez::cli::tests_config)
