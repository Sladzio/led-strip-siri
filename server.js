var express = require('express');
var app = express();
const exe = require('child_process').exec;

app.get('/',function(req,res) {
    console.log(__dirname);
  res.send("House Automation Server is running!");
});

app.get('/status',function(req,res) {
  var cmd = 'sudo ' + __dirname + '/rf24-RGB-remote ' + 1 + ' '+ 0;
  exe( cmd ,(error, stdout, stderr) => {
     if (error)
     {
      throw error;
     }
        res.send(stdout);
     });
});

app.listen(8080);