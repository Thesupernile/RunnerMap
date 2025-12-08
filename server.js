const express = require("express");
const app = express();
const path = require("path");

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({extended : false}));
app.use(express.json());

app.get("/", (req, res) =>{
    res.render("index.ejs");
});
    
app.post("/calculateRoute", (req, res, next) =>{
    requestValid = true;
    if (requestValid){
        let requestBody = req.body;
        console.log(requestBody);
        // Send off request to calculate a route

        // Send back response to client
        const testResponse = JSON.parse('{"requiredPoints": [{"lat":52.18758977414756,"lng":0.13508141040802005},{"lat":52.188332743039304,"lng":0.1358217000961304}]}');
        res.send(requestBody);
    }
});

app.listen(3000);
