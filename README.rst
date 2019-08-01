
====================================================
volatile: The Multithreaded Programmer's Best Friend
====================================================


.. contents::


Summary
-------

When writing multithreaded programs, you can use **volatile** to your advantage.
You must stick to the following rules:

 * Define all shared objects as **volatile**.
 * Don't use **volatile** directly with primitive types.
 * When defining shared classes, use **volatile** member functions to express *thread safety*.


Sample
------


.. include:: volatile.cpp
    :literal:


References
----------

 * https://think-async.com/Asio/
 * http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 * https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/
 * https://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
 * https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
 * http://ltp.sourceforge.net/coverage/lcov.php

___________________________________________________________

Generated with docutils_

.. _docutils: http://docutils.sourceforge.net/rst.html
