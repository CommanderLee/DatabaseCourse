/* google maps -----------------------------------------------------*/
google.maps.event.addDomListener(window, 'load', initialize);

/* Process some data of the Google Map API. */
var myMap, myMarker;
var MAX_K = 10;
var MAX_PANEL = 3;
var kNearMarkers = new Array(10);
var myKeyWords;

function initialize() {

	/* Singapore */
	var myLatlng = new google.maps.LatLng(1.315, 103.844);
	var mapOptions = {
		center: myLatlng,
		zoom: 12,
		mapTypeId: google.maps.MapTypeId.ROADMAP
	};

	myMap = new google.maps.Map(document.getElementById("map-canvas"), mapOptions);
	myMarker = new google.maps.Marker({
		position: myLatlng,
		map: myMap,
		title: "User",
		icon: "http://learn.tsinghua.edu.cn:8080/2011011236/PICs/user_position_ico.ico",
		animation: google.maps.Animation.BOUNCE
	});

	for (var i = 0; i < MAX_K; i++){
		kNearMarkers[i] = new google.maps.Marker({
			position: myLatlng,
			map: myMap,
			visible: false
		});
	}
	myKeyWords = "";
	
	google.maps.event.addListener(myMap, 'click', function(event) {
		changeMarker(event.latLng);
		updateMap();
	}); 
	
	$( "#user_pos" ).text( "User Position: " + myMarker.getPosition().toUrlValue() );
	
	/* Check the status of input. */
	$( '#KeyID' ).on( 'input propertychange', function() {
		myKeyWords = this.value;
		updateMap();
	});
};

/* Change the marker of the user */
function changeMarker(location) {
	myMarker.setPosition(location);
	myMap.setCenter(location);
	$( "#user_pos" ).text( "User Position: " + location.toUrlValue() );
};

/* Update the map when the user click on the map or type new keywords */
function updateMap() {
	if (myKeyWords == "") {
		for (var i = 0; i < MAX_K; i++){
			kNearMarkers[i].setVisible(false);
		}
		return ;
	}
	$.post("/search/", {
		userPos:myMarker.getPosition().toUrlValue(),
		keyWords:myKeyWords,
	}, function(data, status){
		/* Cancel all the old k-near-markers */
		for (var i = 0; i < MAX_K; i++){
			kNearMarkers[i].setVisible(false);
		}
		
		/* Return result from django backend */
		var dataJSON = $.parseJSON(data);
		
		$("#BestMap").attr("src", dataJSON.BestMap);
		
		/* Display the new k-near-markers*/
		$.each(dataJSON.Pos, function(i, item){
			if (i < MAX_K){
				//alert(item.Lat + ';' + item.Lng);
				kNearMarkers[i].setPosition({lat:item.Lat, lng:item.Lng});
				kNearMarkers[i].setVisible(true);
				kNearMarkers[i].setTitle(item.Name + "\nAddr: " + item.Addr 
				+ ".\nPCode: " + item.Pcode);
				
				if (i < MAX_PANEL) {
					$("#name"+i).text(item.Name);
					$("#info"+i).text("Address: " + item.Addr + ". PCode: " + item.Pcode);
				}
			}
		});
		//$( "#rtnMsg" ).text( data );
	});
	//$( "#test_input" ).text( this.value );
};
/* end google maps -----------------------------------------------------*/