var L = require('leaflet');

var routeCoords = [];
var markers = [];
var lines = [];
var map = L.map('map').setView({lon: 0.13488678725880782, lat: 52.18808662172259}, 18);
var requiredDistance = 0;
var maxElevation = 0;
var elevationGain = 0;
var netElevation = 0;

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
	attribution: '&copy; <a href="https://openstreetmap.org/copyright">OpenStreetMap contributors</a>',
	subdomains: 'abcd',
	maxZoom: 21
}).addTo(map);

function onMapClick(e) {
	routeCoords.push(e.latlng); 
	let newMarker = L.marker(e.latlng, {draggable: true, autoPan: true}).addTo(map);
	// Code to handle dragging
	newMarker.on("dragend", function(event){
		var newPos = newMarker.getLatLng();
		let markerIndex = markers.indexOf(newMarker);
		// Checks against -1, which is returned by indexOf if the marker is not present in the list
		if (markerIndex != -1){
			routeCoords[markerIndex] = newPos;
			clearLines();
		}
		else{console.log("ERROR!: A dragged marker was not found in marker list.")}
	});
	// Code to handle clicking
	newMarker.on("click", function(event){
		// Shift or control clicking a point removes it (may be changed to right click in the future)
		if (event.originalEvent.shiftKey == true || event.originalEvent.ctrlKey == true){
			// Handle removing the marker from the map
			let markerIndex = markers.indexOf(newMarker);
			if (markerIndex != -1){
				markers.splice(markerIndex, 1);
				routeCoords.splice(markerIndex, 1);
				newMarker.remove();
				clearLines();
			}
			else{console.log("ERROR!: A marker requesting to be deleted is not in the marker list.")}
		}
	});
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
	if (markers.length < 2){
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
	maxElevationBox = document.getElementById("maxElevationBox");
	netElevationBox = document.getElementById("netElevationBox");
	elevationGainBox = document.getElementById("elevationGainBox");

	distanceBox.innerHTML = `Calculated Route Distance: `;
	numPointsBox.innerHTML = `Number of Required Destinations:  `;
	timeBox.innerHTML = `Approximate Time To Run: `;
	maxElevationBox.innerHTML = `Maximum Elevation: `;
	netElevationBox.innerHTML = `Net Elevation: `;
	elevationGainBox.innerHTML = `Elevation Gain: `;
}

function updateTextBoxes(response){
	distanceBox = document.getElementById("distanceBox");
	numPointsBox = document.getElementById("numPointsBox");
	timeBox = document.getElementById("timeBox");
	pace = document.getElementById("paceInput").value * 10;

	requiredDistance = calculateRouteLength(response.requiredPoints);
	numPoints = markers.length;
	timeToRun = calculateTimeToRun(requiredDistance, pace);
	calculateElevations(response.requiredPoints);

	distanceBox.innerHTML = `Calculated Route Distance:  ${requiredDistance}km`;
	numPointsBox.innerHTML = `Number of Required Destinations:  ${numPoints}`;
	timeBox.innerHTML = `Approximate Time To Run: ${timeToRun.hrs}hrs ${timeToRun.mins}mins ${timeToRun.secs}secs`;
}

function calculateElevations(routeCoords){
	const MAXPOINTSPERREQUEST = 100;
	const POINTSAMPLINGINTERVAL = 10;
	maxElevation = 0;
	elevationGain = 0;
	netElevation = 0;
	elevationData = [];
	currentRequest = 1;
	requestsRequired = Math.ceil(routeCoords.length / (MAXPOINTSPERREQUEST * POINTSAMPLINGINTERVAL));
	// Split routeCoords into sets of 100 since 100 points is the maximum allowed in one API request
	// And we only take the elevation of every 10 points
	for (let i = 0; i < routeCoords.length; i += (MAXPOINTSPERREQUEST * POINTSAMPLINGINTERVAL)){
		rawSubroute = routeCoords.slice(i, i + (MAXPOINTSPERREQUEST * POINTSAMPLINGINTERVAL));
		subroute = []
		// Adding every tenth point to the subroute
		for (let j = 0; j < routeCoords.length; j += POINTSAMPLINGINTERVAL){
			subroute.push(routeCoords[j]);
		}

		// The HTTP request needs a separate list of lats and lons so we divide them here
		subroutelats = []
		subroutelons = []
		subroute.forEach(coord => {
			subroutelats.push(coord.lat);
			subroutelons.push(coord.lng);
		});

		// Format the HTTP request URL
		let HTTPrequest = "https://api.open-meteo.com/v1/elevation?latitude=";
		subroutelats.forEach(lat => {
			HTTPrequest = HTTPrequest + lat.toString();
			HTTPrequest += ",";
		});
		// Remove the eccess comma at the end
		HTTPrequest = HTTPrequest.slice(0,-1);
		HTTPrequest = HTTPrequest + "&longitude=";
		subroutelons.forEach(lng => {
			HTTPrequest = HTTPrequest + lng.toString();
			HTTPrequest += ",";
		});
		// Remove the eccess comma at the end
		HTTPrequest = HTTPrequest.slice(0,-1);

		// Make the HTTP request
		const xhr = new XMLHttpRequest();
		let elevationSubData = []
		xhr.addEventListener("readystatechange", function() {
			// State 4 means okay
			if(this.readyState === 4) {
				elevationSubData = JSON.parse(xhr.response).elevation;
				elevationData = elevationData.concat(elevationSubData);
				// Only update on the last time
				if (currentRequest === requestsRequired){
					updateElevationBoxes(elevationData);
				}
				currentRequest++;
			}
		});
		xhr.open("GET", HTTPrequest)
		xhr.send()
	}
}

function updateElevationBoxes(elevationData){
	// Calculate the required information from returned values (max elevation, net elevation and elevation gain)
	netElevation = elevationData[elevationData.length-1] - elevationData[0];

	maxElevation = elevationData[0];
	for (let i = 1; i < elevationData.length; i++){
		if (elevationData[i] > maxElevation){
			maxElevation = elevationData[i];
		}
		// Find elevation gain
		if (elevationData[i] > elevationData[i-1]){
			elevationGain += (elevationData[i] - elevationData[i-1]);
		}
	}

	maxElevationBox = document.getElementById("maxElevationBox");
	netElevationBox = document.getElementById("netElevationBox");
	elevationGainBox = document.getElementById("elevationGainBox");

	maxElevationBox.innerHTML = `Maximum Elevation: ${maxElevation}m`;
	netElevationBox.innerHTML = `Net Elevation: ${netElevation}m`;
	elevationGainBox.innerHTML = `Elevation Gain: ${elevationGain}m`;
}

function sendRequest(requiredRouteLength, isRoundTrip){
	userErrorBox.innerHTML = " ";
	let routeCoordsToSend = [];
	routeCoordsToSend = routeCoords.slice(0);
	if (isRoundTrip){
		routeCoordsToSend.push(routeCoords[0]);
	}
	if (isValidInput(requiredRouteLength)){
		const xhr = new XMLHttpRequest();
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
			
			let returnedRouteCoords = response.requiredPoints;

			clearLines();
			route = L.polyline(returnedRouteCoords, {color: "#ff8b17"}).addTo(map);
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

isSideBarOpen = true;
sideBarSize = document.getElementById("sideBar").style.width;
document.getElementById("map").style.width = "70%";
map.invalidateSize();


document.getElementById("sideBarButton").addEventListener("click", function() {
	// Remove the side bar and resize the map accordingly
	sideBar = document.getElementById("sideBar");
	mapElement = document.getElementById("map");
	sideBarButton = document.getElementById("sideBarButton");

	if (isSideBarOpen){
		mapElement.style.width = "100%";
		sideBar.style.display = "none";
		sideBarButton.style.left = "98.5%";
		sideBarButton.innerHTML = "<";
		isSideBarOpen = false;
	}
	else{
		mapElement.style.width = "70%";
		sideBar.style.display = "inline";
		sideBarButton.style.left = "68.6%";
		sideBarButton.innerHTML = ">";
		isSideBarOpen = true;
	}
	// Tells leaflet to update the map
	map.invalidateSize();
});

map.on('click', onMapClick);
