build_pam_lastlog2 = not get_option('build-pam-lastlog2') \
    .require(build_liblastlog2) \
    .require(conf.get('HAVE_SECURITY_PAM_MODULES_H').to_string() == '1') \
    .disabled()
summary('pam_lastlog2', build_pam_lastlog2 ? 'enabled' : 'disabled', section : 'components')

cc = meson.get_compiler('c')
pkg = import('pkgconfig')
lib_pam_lastlog2_sources = '''
  src/pam_lastlog2.c
'''.split()

subdir('man')

pamlibdir = get_option('pamlibdir')
if pamlibdir == ''
        pamlibdir = get_option('libdir') / 'security'
endif

if build_pam_lastlog2
  pam_lastlog2_sym = 'src/pam_lastlog2.sym'
  pam_lastlog2_sym_path = '@0@/@1@'.format(meson.current_source_dir(), pam_lastlog2_sym)

  libpam = cc.find_library('pam')

  pam_lastlog2 = shared_module(
    'pam_lastlog2',
    lib_pam_lastlog2_sources,
    name_prefix : '',
    include_directories : [dir_include],
    link_args : ['-Wl,--version-script=@0@'.format(pam_lastlog2_sym_path)],
    link_depends : pam_lastlog2_sym,
    dependencies : [libpam, lastlog2_dep],
    install : build_liblastlog2,
    install_dir : pamlibdir,
  )
  manadocs += lib_pam_lastlog2_manadocs
endif
