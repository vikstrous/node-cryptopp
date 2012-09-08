var cryptopp = require('bindings')('cryptopp.node');
var colorize = require('colorize');
var crypto = require('crypto');
var fs = require('fs');
var cconsole = colorize.console;

//SHA1, SHA224, SHA256, SHA384, SHA512, Tiger, Whirlpool, RIPEMD160, RIPEMD320, RIPEMD128, RIPEMD256, Weak1::MD2, Weak1::MD4, Weak1::MD5
//TODO: make my library deal with buffers directly - and maybe wtih strings too without copying them?
// var input = fs.readFileSync("/usr/share/dict/american-english").toString();
var input = fs.readFileSync("/bin/ls").toString();
var runs = 10000;

for (var alg in {'sha1':'', 'sha224':'', 'sha256':'', 'sha384':'',
	'sha512':'', 'tiger':'', 'whirlpool':'', 'ripemd160':'', 'ripemd320':'',
	'ripemd128':'', 'ripemd256':'', 'md4':'', 'md5':''}){
	var time1, time2;
	try{
		time1 = new Date();
		console.log(alg);
		console.time("cyrpto");
		for (var i = 0; i < runs; i++){
			crypto.createHash(alg).update(input).digest('hex');
		}
		console.timeEnd("cyrpto");
		time1 = new Date() - time1;
	} catch(e) {
		console.log("cyrpto: not supported");
		time1 = null;
	}
	try{
		time2 = new Date();
		console.time("cyrptopp");
		for (var i = 0; i < runs; i++){
			cryptopp[alg](input);
		}
		console.timeEnd("cyrptopp");
		time2 = new Date() - time2;
		if(time1) {
			if(time2/time1 < 1){
				cconsole.log("#green["+(time2/time1)+"]");
			} else {
				cconsole.log("#red["+(time2/time1)+"]");
			}
		}
	} catch(e) {
		console.log("cyrptopp: not supported");
	}
}