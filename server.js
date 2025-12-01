const express = require("express");
const app = express();
const path = require("path");

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({extended : true}))

app.get("/", (req, res) =>{
    res.render("index.ejs");
});
    
app.post("/calculateRoute", (req, res, next) =>{
    res.render("index.ejs", {requiredLength : req.body.requiredLength, roundTrip : req.body.roundTrip});
});

app.listen(3000);
