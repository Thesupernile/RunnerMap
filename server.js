const express = require("express");
const app = express();
const path = require("path");

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));

app.get("/", (req, res)=>{
    res.redirect('index.html');
})

// app.get("/index.css", (req, res)=> {
//     res.sendFile(path.resolve(__dirname, "views/index.CSS"));
// })

// app.get("/index.js", (req, res)=> {
//     res.sendFile(path.resolve(__dirname, "views/index.js"));
// })

// app.get("/leaflet.css", (req, res)=> {
//     res.sendFile(path.resolve(__dirname, "node_modules/leaflet/dist/leaflet.css"));
// })

// app.get("/webmap.js", (req, res)=> {
//     res.sendFile(path.resolve(__dirname,"webmap.js"));
// })

app.listen(80);
