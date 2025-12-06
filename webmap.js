const { response } = require('express');
var L = require('leaflet');
const xhr = new XMLHttpRequest();

var routeCoords = [];
var lineCoords = [];
var markers = [];
var lines = [];
var map = L.map('map').setView({lon: 0.13488678725880782, lat: 52.18808662172259}, 18);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
	attribution: '&copy; <a href="https://openstreetmap.org/copyright">OpenStreetMap contributors</a>',
	subdomains: 'abcd',
	maxZoom: 21
}).addTo(map);

function onMapClick(e) {
	routeCoords.push(e.latlng); 
	let newMarker = L.marker(e.latlng).addTo(map);
	markers.push(newMarker);
}

document.getElementById("clearButton").addEventListener("click", function clearPoints() {
	for (let i = 0; i < markers.length; i++){
		map.removeLayer(markers[i]);
	}
	for (let i = 0; i < lines.length; i++){
		map.removeLayer(lines[i]);
	}
	
	markers = [];
	lines = [];
	routeCoords = [];
});

function sendRequest(requiredRouteLength, isRoundTrip){
	xhr.open("POST", "/calculateRoute");
	xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	const body = JSON.stringify({
		requiredLength: requiredRouteLength,
		roundTrip: isRoundTrip,
		requiredPoints: routeCoords
	});
	xhr.onload = () => {
	if (xhr.readyState == 4 && xhr.status == 200) {
		response = JSON.parse(xhr.responseText);
		console.log(response);
		
		routeCoords = response.requiredPoints;

		route = L.polyline(routeCoords).addTo(map);
		lines.push(route);

	} else {
		console.log(`Error: ${xhr.status}`);
	}
	};
	xhr.send(body);
}

document.getElementById("submitButton").addEventListener("click", function clearPoints() {
	requiredLength = document.getElementById("requiredLengthInput").value;
	roundTrip = document.getElementById("isRoundTripInput").checked;
	sendRequest(requiredLength, roundTrip);
});

map.on('click', onMapClick);
