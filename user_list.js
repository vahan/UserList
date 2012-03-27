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
	//userListCheckboxes = jQuery(':checkbox[id|="edit-list"]:checked').each(function(index){
		userListCheckboxes = jQuery("#user_list-list-"+idSuffix).find(':checkbox:checked').each(function(index){
		// TODO handle this part to have a better search quiery. Look for the username/first name/last name
		query[idSuffix] += jQuery(this).parent().parent().next().next().next().text() + " " + jQuery(this).parent().parent().next().next().next().next().text();
		query[idSuffix] += " ";
	});
	console.log("query: " + query[idSuffix]);
	lsObject[idSuffix].search(query[idSuffix], function(publications) {
		//console.log("lavash");
		document.getElementById("watchProgress-"+idSuffix+"-1").innerHTML = "<span>Results for '"+query[idSuffix]+"'</span>";
		var html = "";
		if (publications)
		{
		    for (var i=0; i<publications.length; i++)
			{
				var pub = publications[i];
				html += "<p>";
				if (pub.authors)
				{
					for (var j=0; j<pub.authors.length; j++)
					{
						var author = pub.authors[j];
						html += author.name;
						if (j < pub.authors.length - 1) html += ", ";
					}
				}
				html += " (" + pub.year + "): <b>" + pub.title + "</b>";
				html += "</p>";
			}
		}
		document.getElementById("searchResults-"+idSuffix+"-1").innerHTML = html;
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
