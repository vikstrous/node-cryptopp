{
  'targets': [
    {
      'target_name': 'cryptopp',
      'sources' : [
        'cryptopp.cc'
      ],
      'cflags_cc!': [ '-fno-rtti', '-Wunused-variable' ],
      'cflags_cc': [ '-O3', '-fexceptions', '-g' ],
      'include_dirs':[
        'cryptopp'
      ],
      'libraries': [ '-lcryptopp' ],
      'ldflags': [ '-L../cryptopp' ]
    }
  ]
}