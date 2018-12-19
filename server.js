var express = require('express');
var app = express();
const exe = require('child_process').exec;

app.get('/',function(req,res) {
    console.log(__dirname);
  res.send("House Automation Server is running!");
});

app.get('/status',function(req,res) {
  var cmd = 'sudo ./rf24-RGB-remote ' + 1 + ' '+ 0;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.send(stdout);
  });
});
app.post('/on',function(req,res) {
  var cmd = 'sudo ./rf24-RGB-remote ' + 1 + ' '+ 1;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.send(stdout);
  });
});

app.post('/off',function(req,res) {
  var cmd = 'sudo ./rf24-RGB-remote ' + 1 + ' '+ 2;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.send(stdout);
  });
});

app.listen(3000);