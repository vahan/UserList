function lsSearch() {
	var livingscience;
	if(!livingscience) 
	{
		livingscience = new ch.ethz.livingscience.gwtclient.api.LivingScienceSearch();
		livingscience.setMap("mapcontainer");
	}
	if (!livingscience) return;
	document.getElementById("watchProgress").innerHTML = "Please wait...";
	
	//Create a search query from the selected users
	var query = "";
	//userListCheckboxes = jQuery(':checkbox[id|="edit-list"]:checked').each(function(index){
		userListCheckboxes = jQuery(':checkbox:checked').each(function(index){
		query += jQuery(this).parent().parent().next().text();
		query += " ";
	});
	
	console.log("query: " + query);
	livingscience.search(query, function(publications) {
		//console.log("lavash");
		document.getElementById("watchProgress").innerHTML = "Enjoy ;)";
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
		document.getElementById("searchResults").innerHTML = html;
	});
}
