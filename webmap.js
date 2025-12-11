var L = require('leaflet');
const xhr = new XMLHttpRequest();

var routeCoords = [];
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

function isStringNumber(string){
	return !isNaN(string) && !isNaN(parseFloat(string));
}

function isValidInput(){
	requiredLength = document.getElementById("requiredLengthInput").value;
	if (isStringNumber(requiredLength) && requiredLength >= 0){
		return true;
	}
	return false;
}

function calculateRouteLength(route){
	// Calculates the route length

	return 0;
}

function calculateTimeToRun(distance){
	// Calculates an approximate time to walk/run the route
	var timeToRun = {
		"hrs" : 0,
		"mins" : 0,
		"secs" : 0
	};

	return timeToRun;
}

function updateTextBoxes(response){
	distanceBox = document.getElementById("distanceBox");
	numPointsBox = document.getElementById("numPointsBox");
	timeBox = document.getElementById("timeBox");

	requiredDistance = calculateRouteLength(response.requiredPoints);
	numPoints = response.requiredPoints.length;
	timeToRun = calculateTimeToRun(requiredDistance);

	distanceBox.innerHTML = `Calculated Route Distance:  ${requiredDistance}km`;
	numPointsBox.innerHTML = `Number of Required Destinations:  ${numPoints}`;
	timeBox.innerHTML = `Approximate Time To Run: ${timeToRun.hrs}hrs ${timeToRun.mins}mins ${timeToRun.secs}secs`;
}

function sendRequest(requiredRouteLength, isRoundTrip){
	userErrorBox.innerHTML = " ";
	if (isValidInput()){
		xhr.open("POST", "/calculateRoute");
		xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

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
			updateTextBoxes(response);
		} else {
			console.log(`Error: ${xhr.status}`);
			userErrorBox.innerHTML = "An error occured!"
		}
		};
		xhr.send(body);
	}
	else{
		userErrorBox.innerHTML = "Invalid Input!";
	}
}

document.getElementById("submitButton").addEventListener("click", function clearPoints() {
	requiredLength = document.getElementById("requiredLengthInput").value;
	roundTrip = document.getElementById("isRoundTripInput").checked;
	sendRequest(requiredLength, roundTrip);
});

map.on('click', onMapClick);
