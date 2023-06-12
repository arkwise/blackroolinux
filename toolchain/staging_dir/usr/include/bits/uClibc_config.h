#if !defined __FEATURES_H && !defined __need_uClibc_config_h
# error Never include <bits/uClibc_config.h> directly; use <features.h> instead
#endif

#define __UCLIBC_MAJOR__ 0
#define __UCLIBC_MINOR__ 9
#define __UCLIBC_SUBLEVEL__ 29
/* Automatically generated make config: don't edit */
/* Mon Feb 18 15:18:57 2008 */
#undef __TARGET_alpha__
#undef __TARGET_arm__
#undef __TARGET_avr32__
#undef __TARGET_bfin__
#undef __TARGET_cris__
#undef __TARGET_e1__
#undef __TARGET_frv__
#undef __TARGET_h8300__
#undef __TARGET_hppa__
#undef __TARGET_i386__
#undef __TARGET_i960__
#undef __TARGET_ia64__
#undef __TARGET_m68k__
#undef __TARGET_microblaze__
#define __TARGET_mips__ 1
#undef __TARGET_nios__
#undef __TARGET_nios2__
#undef __TARGET_powerpc__
#undef __TARGET_sh__
#undef __TARGET_sh64__
#undef __TARGET_sparc__
#undef __TARGET_v850__
#undef __TARGET_vax__
#undef __TARGET_x86_64__

/* Target Architecture Features and Options */
#define __TARGET_ARCH__ "mips"
#define __FORCE_OPTIONS_FOR_ARCH__ 1
#define __ARCH_CFLAGS__ "-mno-split-addresses"
#define __CONFIG_MIPS_O32_ABI__ 1
#undef __CONFIG_MIPS_N32_ABI__
#undef __CONFIG_MIPS_N64_ABI__
#undef __CONFIG_MIPS_ISA_1__
#undef __CONFIG_MIPS_ISA_2__
#undef __CONFIG_MIPS_ISA_3__
#undef __CONFIG_MIPS_ISA_4__
#define __CONFIG_MIPS_ISA_MIPS32__ 1
#undef __CONFIG_MIPS_ISA_MIPS64__
#define __TARGET_SUBARCH__ ""
#undef __UCLIBC_FORMAT_ELF__
#undef __UCLIBC_FORMAT_FDPIC_ELF__
#define __UCLIBC_FORMAT_FLAT__ 1
#undef __UCLIBC_FORMAT_FLAT_SEP_DATA__
#undef __UCLIBC_FORMAT_SHARED_FLAT__
#define __ARCH_ANY_ENDIAN__ 1
#define __ARCH_LITTLE_ENDIAN__ 1
#undef __ARCH_WANTS_BIG_ENDIAN__
#define __ARCH_WANTS_LITTLE_ENDIAN__ 1
#undef __ARCH_HAS_MMU__
#define __UCLIBC_HAS_FLOATS__ 1
#define __UCLIBC_HAS_FPU__ 1
#define __DO_C99_MATH__ 1
#define __KERNEL_HEADERS__ "/usr/src/buildroot-20071216/toolchain_build_mipsel/linux/include"
#define __UCLIBC_UCLINUX_BROKEN_MUNMAP__ 1
#define __EXCLUDE_BRK__ 1
#define __HAVE_DOT_CONFIG__ 1

/* General Library Settings */
#undef __HAVE_NO_PIC__
#define __DOPIC__ 1
#define __HAVE_NO_SHARED__ 1
#define __ARCH_HAS_NO_LDSO__ 1
#define __UCLIBC_CTOR_DTOR__ 1
#undef __HAS_NO_THREADS__
#define __UCLIBC_HAS_THREADS__ 1
#undef __PTHREADS_DEBUG_SUPPORT__
#undef __LINUXTHREADS_OLD__
#define __UCLIBC_HAS_LFS__ 1
#define __MALLOC__ 1
#undef __MALLOC_SIMPLE__
#undef __MALLOC_STANDARD__
#define __MALLOC_GLIBC_COMPAT__ 1
#define __UCLIBC_DYNAMIC_ATEXIT__ 1
#undef __COMPAT_ATEXIT__
#undef __UCLIBC_SUSV3_LEGACY__
#undef __UCLIBC_SUSV3_LEGACY_MACROS__
#define __UCLIBC_HAS_SHADOW__ 1
#undef __UCLIBC_HAS_PROGRAM_INVOCATION_NAME__
#define __UCLIBC_HAS___PROGNAME__ 1
#undef __UNIX98PTY_ONLY__
#define __ASSUME_DEVPTS__ 1
#define __UCLIBC_HAS_TM_EXTENSIONS__ 1
#define __UCLIBC_HAS_TZ_CACHING__ 1
#define __UCLIBC_HAS_TZ_FILE__ 1
#define __UCLIBC_HAS_TZ_FILE_READ_MANY__ 1
#define __UCLIBC_TZ_FILE_PATH__ "/etc/TZ"

/* Advanced Library Settings */
#define __UCLIBC_PWD_BUFFER_SIZE__ 256
#define __UCLIBC_GRP_BUFFER_SIZE__ 256

/* Networking Support */
#undef __UCLIBC_HAS_IPV6__
#undef __UCLIBC_HAS_RPC__
#undef __UCLIBC_USE_NETLINK__
#undef __UCLIBC_HAS_BSD_RES_CLOSE__

/* String and Stdio Support */
#undef __UCLIBC_HAS_STRING_GENERIC_OPT__
#undef __UCLIBC_HAS_STRING_ARCH_OPT__
#undef __UCLIBC_HAS_CTYPE_TABLES__
#undef __UCLIBC_HAS_WCHAR__
#undef __UCLIBC_HAS_LOCALE__
#undef __UCLIBC_HAS_GLIBC_CUSTOM_PRINTF__
#undef __USE_OLD_VFPRINTF__
#define __UCLIBC_PRINTF_SCANF_POSITIONAL_ARGS__ 9
#define __UCLIBC_HAS_SCANF_GLIBC_A_FLAG__ 1
#undef __UCLIBC_HAS_STDIO_BUFSIZ_NONE__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_256__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_512__
#define __UCLIBC_HAS_STDIO_BUFSIZ_1024__ 1
#undef __UCLIBC_HAS_STDIO_BUFSIZ_2048__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_4096__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_8192__
#define __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_NONE__ 1
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_4__
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_8__
#undef __UCLIBC_HAS_STDIO_SHUTDOWN_ON_ABORT__
#define __UCLIBC_HAS_STDIO_GETC_MACRO__ 1
#define __UCLIBC_HAS_STDIO_PUTC_MACRO__ 1
#define __UCLIBC_HAS_STDIO_AUTO_RW_TRANSITION__ 1
#undef __UCLIBC_HAS_FOPEN_LARGEFILE_MODE__
#define __UCLIBC_HAS_FOPEN_EXCLUSIVE_MODE__ 1
#define __UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__ 1
#define __UCLIBC_HAS_PRINTF_M_SPEC__ 1
#undef __UCLIBC_HAS_ERRNO_MESSAGES__
#define __UCLIBC_HAS_SIGNUM_MESSAGES__ 1
#undef __UCLIBC_HAS_SYS_SIGLIST__
#undef __UCLIBC_HAS_GNU_GETOPT__
#define __UCLIBC_HAS_GETOPT_LONG__ 1
#undef __UCLIBC_HAS_GNU_GETSUBOPT__

/* Big and Tall */
#define __UCLIBC_HAS_REGEX__ 1
#define __UCLIBC_HAS_REGEX_OLD__ 1
#define __UCLIBC_HAS_FNMATCH__ 1
#define __UCLIBC_HAS_FNMATCH_OLD__ 1
#undef __UCLIBC_HAS_WORDEXP__
#undef __UCLIBC_HAS_FTW__
#define __UCLIBC_HAS_GLOB__ 1
#undef __UCLIBC_HAS_GNU_GLOB__

/* Library Installation Options */
#define __RUNTIME_PREFIX__ "/"
#define __DEVEL_PREFIX__ "/usr/"

/* Security options */
#undef __UCLIBC_HAS_ARC4RANDOM__
#undef __HAVE_NO_SSP__
#undef __UCLIBC_HAS_SSP__
#undef __UCLIBC_BUILD_NOEXECSTACK__

/* uClibc development/debugging options */
#define __CROSS_COMPILER_PREFIX__ "/usr/src/buildroot-20071216/build_mipsel/staging_dir/usr/bin/mipsel-linux-uclibc-"
#define __UCLIBC_EXTRA_CFLAGS__ "-fno-jump-tables"
#undef __DODEBUG__
#define __DOSTRIP__ 1
#undef __DOASSERTS__
#undef __UCLIBC_MALLOC_DEBUGGING__
#define __WARNINGS__ "-Wall"
#undef __EXTRA_WARNINGS__
#undef __DOMULTI__
#undef __UCLIBC_MJN3_ONLY__
