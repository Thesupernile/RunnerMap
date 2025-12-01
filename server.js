const express = require("express");
const app = express();
const path = require("path");

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({extended : true}));
app.use(express.json());

app.get("/", (req, res) =>{
    res.render("index.ejs");
});
    
app.post("/calculateRoute", (req, res, next) =>{
    requestValid = true;
    if (requestValid){
        // Send off request to calculate a route
        res.send({requiredPoints : 0});
    }
    else{
        res.render("index.ejs", {requiredLength : req.body.requiredLength, roundTrip : req.body.roundTrip, errorMessage: "Invalid Request"});
    }
});

app.listen(3000);
