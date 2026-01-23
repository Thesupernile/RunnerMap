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

function clearLines(){
	for (let i = 0; i < lines.length; i++){
		map.removeLayer(lines[i]);
	}
	lines = [];
}

function openLoadingScreen(){
	document.getElementById("loadBox").style.display = "inline";
}

function closeLoadingScreen(){
	document.getElementById("loadBox").style.display = "none";
}

document.getElementById("clearButton").addEventListener("click", function clearPoints() {
	for (let i = 0; i < markers.length; i++){
		map.removeLayer(markers[i]);
	}
	clearLines();
	
	markers = [];
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

function haversine(lat1, lon1, lat2, lon2){
	// Returns (in km) distance between two points
	const PI = 3.141592;
	const EARTHRADIUS = 6357;

	const lat1Rad = lat1 * (PI/180);
	const lat2Rad = lat2 * (PI/180);
	const lon1Rad = lon1 * (PI/180);
	const lon2Rad = lon2 * (PI/180);

	let distance = 2 * EARTHRADIUS * Math.asin(Math.sqrt( Math.pow(Math.sin((lat2Rad - lat1Rad)/2), 2) + Math.cos(lat1Rad) * Math.cos(lat2Rad) * Math.pow(Math.sin((lon2Rad - lon1Rad)/2), 2)))
	return distance
}

function calculateRouteLength(route){
	// Calculates the route length
	distance = 0;
	for (let i = 1; i < route.length; i++){
		let currentNodeCoords = route[i];
		let previousNodeCoords = route[i-1];

		distance += haversine(previousNodeCoords.lat, previousNodeCoords.lng, currentNodeCoords.lat, currentNodeCoords.lng);
	}

	// Rounding to 2DP
	return Math.round(distance * 100) / 100;
}

function calculateTimeToRun(distance){
	// Calculates an approximate time to walk/run the route
	const walkSpeedHrs = 4.8;		// Speed in km/h (this is an average from the internet)
	const walkSpeedMins = walkSpeedHrs / 60;
	const walkSpeedSecs = walkSpeedMins / 60;

	var timeToRun = {
		"hrs" : 0,
		"mins" : 0,
		"secs" : 0
	};

	// Calculate the hours
	timeToRun.hrs = Math.floor(distance / walkSpeedHrs);
	distance = distance - (timeToRun.hrs * walkSpeedHrs);
	// Calculate the minutes
	timeToRun.mins = Math.floor(distance / walkSpeedMins);
	distance = distance - (timeToRun.mins * walkSpeedMins);
	// Calculate the seconds
	timeToRun.secs = Math.round(distance / walkSpeedSecs);

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
	openLoadingScreen();
	userErrorBox.innerHTML = " ";
	if (isRoundTrip){
		routeCoords.push(routeCoords[0]);
	}
	if (isValidInput()){
		xhr.open("POST", "/calculateRoute");
		xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

		const body = JSON.stringify({
			requiredLength: requiredRouteLength,
			roundTrip: isRoundTrip,
			requiredPoints: routeCoords
		});
		xhr.onload = () => {
		closeLoadingScreen();
		if (xhr.readyState == 4 && xhr.status == 200) {
			response = JSON.parse(xhr.responseText);
			console.log(response);
			
			routeCoords = response.requiredPoints;

			clearLines();
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
