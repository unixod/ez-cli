addTest("cli::Parameter" parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("cli::Lexer" lexer
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::C_string_view" c_string_view
    LIBS ez::cli ez::cli::tests_config)

addTest("utils::Uninitialized" uninitialized
    LIBS ez::cli ez::cli::tests_config)

addTest("ez::cli::Positional_parameter" positional_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("ez::cli::Regular_parameter" regular_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("ez::cli::Boolean_parameter" boolean_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Custom positional parameter" custom_positional_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Custom regular parameter" custom_regular_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Custom boolean parameter" custom_boolean_parameter
    LIBS ez::cli ez::cli::tests_config)

addTest("Ambiguous custom parameter" ambiguous_custom_parameter
    LIBS ez::cli ez::cli::tests_config)
