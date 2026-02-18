var L = require('leaflet');
const xhr = new XMLHttpRequest();

var routeCoords = [];
var markers = [];
var lines = [];
var map = L.map('map').setView({lon: 0.13488678725880782, lat: 52.18808662172259}, 18);
var requiredDistance = 0;

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
	userErrorBox.innerHTML = " ";
	for (let i = 0; i < markers.length; i++){
		map.removeLayer(markers[i]);
	}
	requiredDistance = 0;
	clearLines();
	clearTextBoxes();
	
	markers = [];
	routeCoords = [];
});

function isStringNumber(string){
	return !isNaN(string) && !isNaN(parseFloat(string));
}

function isValidInput(requiredLength){
	if (!isStringNumber(requiredLength) || !(requiredLength >= 0)){
		userErrorBox.innerHTML = "Invalid Desired Length!";
		return false;
	}
	if (routeCoords.length < 2){
		userErrorBox.innerHTML = "You must place at least two points on the map to create a route!";
		return false;
	}
	return true;
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

function calculateTimeToRun(distance, pace){
	// Calculates an approximate time to walk/run the route
	const walkSpeedHrs = (60 * 60)/(pace);
	const walkSpeedMins = 60/pace;
	const walkSpeedSecs = 1/pace;

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

function clearTextBoxes(){
	distanceBox = document.getElementById("distanceBox");
	numPointsBox = document.getElementById("numPointsBox");
	timeBox = document.getElementById("timeBox");	

	distanceBox.innerHTML = `Calculated Route Distance:  0km`;
	numPointsBox.innerHTML = `Number of Required Destinations:  0`;
	timeBox.innerHTML = `Approximate Time To Run: 0hrs 0mins 0secs`;
}

function updateTextBoxes(response){
	distanceBox = document.getElementById("distanceBox");
	numPointsBox = document.getElementById("numPointsBox");
	timeBox = document.getElementById("timeBox");
	pace = document.getElementById("paceInput").value * 10;

	requiredDistance = calculateRouteLength(response.requiredPoints);
	numPoints = routeCoords.length;
	timeToRun = calculateTimeToRun(requiredDistance, pace);

	distanceBox.innerHTML = `Calculated Route Distance:  ${requiredDistance}km`;
	numPointsBox.innerHTML = `Number of Required Destinations:  ${numPoints}`;
	timeBox.innerHTML = `Approximate Time To Run: ${timeToRun.hrs}hrs ${timeToRun.mins}mins ${timeToRun.secs}secs`;
}

function sendRequest(requiredRouteLength, isRoundTrip){
	userErrorBox.innerHTML = " ";
	let routeCoordsToSend = [];
	routeCoordsToSend = routeCoords.slice(0);
	if (isRoundTrip){
		routeCoordsToSend.push(routeCoords[0]);
	}
	if (isValidInput(requiredRouteLength)){
		openLoadingScreen();
		xhr.open("POST", "/calculateRoute");
		xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

		const body = JSON.stringify({
			requiredLength: parseFloat(requiredRouteLength),
			roundTrip: isRoundTrip,
			requiredPoints: routeCoordsToSend
		});
		xhr.onload = () => {
		closeLoadingScreen();
		if (xhr.readyState == 4 && xhr.status == 200) {
			response = JSON.parse(xhr.responseText);
			console.log(response);
			
			let returnedRouteCoords = response.requiredPoints;

			clearLines();
			route = L.polyline(returnedRouteCoords).addTo(map);
			lines.push(route);
			updateTextBoxes(response);
		} else {
			console.log(`Error: ${xhr.status}`);
			userErrorBox.innerHTML = "An error occured!"
		}
		};
		xhr.send(body);
	}
}

function convertPace(inputPace){
	var pace = {
		"hrs" : 0,
		"mins" : 0,
		"secs" : 0
	};

	pace.mins = Math.floor(inputPace/60);
	inputPace -= pace.mins * 60;
	pace.secs = inputPace;

	return pace;
}

document.getElementById("paceInput").oninput = function() {
	inputPace = this.value * 10;
	pace = convertPace(inputPace);
	timeToRun = calculateTimeToRun(requiredDistance, inputPace);

	document.getElementById("paceDisplay").innerHTML = `Pace: ${pace.mins}:${pace.secs/10}0 min/km`;
	timeBox.innerHTML = `Approximate Time To Run: ${timeToRun.hrs}hrs ${timeToRun.mins}mins ${timeToRun.secs}secs`;
}

document.getElementById("isMinLengthInput").addEventListener('change', function() {
  if (this.checked) {
    document.getElementById("DLSInput").style.display = "none";
  } else {
    document.getElementById("DLSInput").style.display = "inline";
  }
});

document.getElementById("submitButton").addEventListener("click", function submit() {
	if (document.getElementById("isMinLengthInput").checked){
		requiredLength = 0;
	}
	else{
		requiredLength = document.getElementById("requiredLengthInput").value;
	}
	roundTrip = document.getElementById("isRoundTripInput").checked;
	sendRequest(requiredLength, roundTrip, isMinLengthInput);
});

map.on('click', onMapClick);
