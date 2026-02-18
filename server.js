const express = require("express");
const app = express();
const path = require("path");
const MappingComponent = require('bindings')('MappingComponent');

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({extended : false}));
app.use(express.json());

function checkRequestValid(requestBody){
    if (requestBody.requiredPoints.length < 2){
        return false;
    }
    else if (requestBody.requiredLength < 0){
        return false;
    }
    return true;
}

app.get("/", (req, res) =>{
    res.render("index.ejs", {requiredLength : 0, minLength : true});
});
    
app.post("/calculateRoute", (req, res, next) =>{
    let requestBody = req.body;
    requestValid = checkRequestValid(requestBody);
    if (requestValid){
        try{
            let response = MappingComponent.CalculateRoute(JSON.stringify(requestBody.requiredPoints), requestBody.requiredLength);
            // Send back response to client
            res.send(response);
        }
        catch(error){
            return res.status(500).send({message: `${error}`});
        }
    }
    else{
        return res.status(400).send({message: "Request Invalid"});
    }
});

app.listen(3000);
