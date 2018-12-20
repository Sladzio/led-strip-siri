var express = require('express');
var app = express();
const exe = require('child_process').exec;

app.get('/',function(req,res) {
    console.log(__dirname);
  res.end("House Automation Server is running!");
});

app.get('/status',function(req,res) {
  var cmd = 'sudo .' + __dirname +'/rf24-RGB-remote ' + 1 + ' '+ 0;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.set('Content-Type', 'text/plain');
        res.send(stdout);
  });
});
app.get('/on',function(req,res) {
  var cmd = 'sudo ./rf24-RGB-remote ' + 1 + ' '+ 1;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.set('Content-Type', 'text/plain');
        res.send("OK");
  });
});

app.get('/off',function(req,res) {
  var cmd = 'sudo ./rf24-RGB-remote ' + 1 + ' '+ 2;
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.set('Content-Type', 'text/plain');
        res.send("OK");
  });
});

app.get('/set/:id?',function(req,res) {
    var cmd = "";
    if(req.params.id)
    {
        cmd = 'sudo ./rf24-RGB-remote ' + 2 + ' '+ req.params.id;
    }
    else
    {
         cmd = 'sudo ./rf24-RGB-remote ' + 4 + ' '+ 0;
    }
  
  exe( cmd ,(error, stdout, stderr) => 
  {
     if (error)
     {
      throw error;
     }
        res.status(200);
        res.set('Content-Type', 'text/plain');
        res.send(stdout);
  });
});

app.listen(3001);