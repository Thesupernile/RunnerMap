const express = require("express");
const app = express();
const path = require("path");
const MappingComponent = require('bindings')('MappingComponent');

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({extended : false}));
app.use(express.json());

app.get("/", (req, res) =>{
    res.render("index.ejs", {requiredLength : 0});
});
    
app.post("/calculateRoute", (req, res, next) =>{
    requestValid = true;
    if (requestValid){
        let requestBody = req.body;
        console.log(requestBody);
        // Calculate a route
        console.log(MappingComponent.CalculateRoute(JSON.stringify(requestBody.requiredPoints)));
        // Send back response to client
        res.send(requestBody);
    }
});

app.listen(3000);
