// edited by cris, 20120329 7:28 pm

var lsObject = new Array();
var query = new Array();

function lsSearch(idSuffix) {
	if(lsObject[idSuffix] == null)
	{
		lsObject[idSuffix] = new ch.ethz.livingscience.gwtclient.api.LivingScienceSearch();
		// TODO this "-1" should be replaced by a count, because why might want to have maps or whataver in multiple windows within a dialog
		lsObject[idSuffix].setMap("mapcontainer-"+idSuffix+"-1");
	}
	if (!lsObject[idSuffix]) return;
	document.getElementById("watchProgress-"+idSuffix+"-1").innerHTML = "Please wait...";
	document.getElementById("searchResults-"+idSuffix+"-1").innerHTML = "";

	//Create a search query from the selected users
	query[idSuffix] = "";
	userListCheckboxes = jQuery(':checkbox[id|="edit-list"]:checked').each(function(index){
		//userListCheckboxes = jQuery("#user_list-list-"+idSuffix).find(':checkbox:checked').each(function(index){
		// TODO handle this part to have a better search quiery. Look for the username/first name/last name
		query[idSuffix] += jQuery(this).parent().parent().next().next().next().next().next().next().text();
		query[idSuffix] += " ";
		query[idSuffix] +=  jQuery(this).parent().parent().next().next().next().next().next().next().next().text();
		console.log("search for " + query[idSuffix]);
		
	});
	console.log("query: " + query[idSuffix]);
	lsObject[idSuffix].searchAuthor(query[idSuffix], function(publications) 
	{
		var shortInfo =  "<p>" + lsObject[idSuffix].getTotalResults() + " results for: "+query[idSuffix]+"</p>";
		var authorIndices = lsObject[idSuffix].getAuthorStats();
		if (authorIndices)
		{
			shortInfo += "<p>H-Index: " + authorIndices.hindex+ " | G-Index: " + authorIndices.gindex+ " | Total Citations: " + authorIndices.citationcount+ "</p>";
		}
		shortInfo += "<p id=\"authorImage" + idSuffix + "\" />";
		document.getElementById("watchProgress-"+idSuffix+"-1").innerHTML = shortInfo;
		
		// display result list
		document.getElementById("searchResults-"+idSuffix+"-1").innerHTML = "";
		lsObject[idSuffix].generateList(0, 10,  "searchResults-"+idSuffix+"-1");
		
		// search for an author image
		lsObject[idSuffix].addAuthorImage("authorImage" + idSuffix, 100); // 100 px is image width
	});
}

/**
 * Function to get the value of the specified field from the checked checkboxes and return as an array
 * @param fieldValue
 * @param idSuffix
 */
function getCheckedFieldValue(fieldValue, idSuffix) {
	userListCheckboxes = jQuery("#user_list-list-"+idSuffix).find('a').each(function(index){
		// TODO handle this part to have a better search quiery. Look for the username/first name/last name
		if (jQuery(this).html() == fieldValue) {
			query[idSuffix] += jQuery(this).parent().parent().next().next().next().text() + " " + jQuery(this).parent().parent().next().next().next().next().text();
			query[idSuffix] += " ";

		}		
	});	
}
