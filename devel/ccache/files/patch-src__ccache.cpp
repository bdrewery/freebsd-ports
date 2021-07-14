- Determine whether cc(1) is clang or gcc at compile-time.
- Don't hash -fcolor-diagnostics; make will auto use it while make -j will not.
  There's no reason to not use the cache in either of these cases if it is
  already available.
-bdrewery


--- src/ccache.cpp.orig	2021-05-09 11:39:46.000000000 -0700
+++ src/ccache.cpp	2021-07-12 10:39:09.312440000 -0700
@@ -345,6 +345,16 @@ guess_compiler(string_view path)
     return CompilerType::nvcc;
   } else if (name == "pump" || name == "distcc-pump") {
     return CompilerType::pump;
+#if defined(CC_IS_CLANG) || defined(CC_IS_GCC)
+  } else if (name.find("cc") != nonstd::string_view::npos ||
+    named.find("CC") != nonstd::string_view::npos ||
+    name.find("c++") != nonstd::string_view::npos) {
+#if defined(CC_IS_CLANG)
+    return CompilerType::clang;
+#elif defined(CC_IS_GCC)
+    return CompilerType::gcc;
+#endif
+#endif
   } else {
     return CompilerType::other;
   }
@@ -1478,6 +1488,7 @@ hash_common_info(const Context& ctx,
     }
   }
 
+#if 0
   // Possibly hash GCC_COLORS (for color diagnostics).
   if (ctx.config.compiler_type() == CompilerType::gcc) {
     const char* gcc_colors = getenv("GCC_COLORS");
@@ -1486,6 +1497,7 @@ hash_common_info(const Context& ctx,
       hash.hash(gcc_colors);
     }
   }
+#endif
 }
 
 static bool
@@ -1586,6 +1598,13 @@ calculate_result_name(Context& ctx,
 
     // -Wl,... doesn't affect compilation (except for clang).
     if (Util::starts_with(args[i], "-Wl,") && !is_clang) {
+      continue;
+    }
+
+    /* Colors do not affect compilation. */
+    if (Util::starts_with(args[i], "-fcolor-diagnostics") ||
+      args[i] == "-fdiagnostics-color" ||
+      args[i] == "-fdiagnostics-color=always") {
       continue;
     }
 
