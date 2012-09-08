var cryptopp = require('bindings')('cryptopp.node');
var fs = require('fs');
var input = fs.readFileSync("/usr/share/dict/american-english");

cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);
cryptopp.sha1(input);

throw "making v8 clean up after itself";