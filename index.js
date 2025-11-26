var routeCoords = [];
var markers = [];
var lines = [];
var map = L.map('map').setView({lon: 0.13488678725880782, lat: 52.18808662172259}, 19);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://openstreetmap.org/copyright">OpenStreetMap contributors</a>'
}).addTo(map);

function onMapClick(e) {
    routeCoords.push(e.latlng); 
    route = L.polyline(routeCoords).addTo(map);
    lines.push(route);
    let newMarker = L.marker(e.latlng).addTo(map);
    markers.push(newMarker);
}   

function clearPoints() {
    for (let i = 0; i < markers.length; i++){
        map.removeLayer(markers[i]);
    }
    for (let i = 0; i < lines.length; i++){
        map.removeLayer(lines[i]);
    }
    
    markers = [];
    lines = [];
    routeCoords = [];
}

map.on('click', onMapClick);
