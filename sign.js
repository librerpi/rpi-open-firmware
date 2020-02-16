// usage: node sign.js lk.bin recovery.bin <keygoeshere>
var fs = require("fs");
var crypto = require("crypto");

console.log(process.argv);

var input = process.argv[2];
var output = process.argv[3];
var key = Buffer.from(process.argv[4], "hex");

blob = fs.readFileSync(input);

var hmac = crypto.createHmac("sha1", key);
hmac.update(blob);
var signature = hmac.digest();

var signed = Buffer.concat([blob, signature]);
console.log("writing to",output);
fs.writeFileSync(output, signed);
