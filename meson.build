project('IPv4h','c')
args = [
  '-Wall',
  ]

src = files('src/main.c', 'src/ipv4hlib.c')

include = include_directories('include')

executable('ipv4h',src,include_directories: include, c_args: args)
