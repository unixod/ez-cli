addTest("cli::Parameter" parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("cli::Lexer" lexer
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::C_string_view" c_string_view
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::Uninitialized" uninitialized
    LIBS ez::cli ez::cli::tests_config)

addTest("Custom positional parameter" custom_positional_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Incomplete custom positional parameter" incomplete_custom_positional_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Ambiguous custom parameter" ambiguous_custom_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Inconsistent custom parameter" inconsistent_custom_parameter
    LIBS ez::cli ez::cli::tests_config)
