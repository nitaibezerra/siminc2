$(function () {
	$("#demo").jstree({ 
		"json_data" : {
			"data" : [
				{ 
					"data" : "A node", 
					"state" : "closed"
				},
				{ 
					"attr" : { "id" : "li.node.id3" }, 
					"data" : { 
						"title" : "Long format demo", 
						"attr" : { "href" : "#" } 
					} 
				}
			],
			"ajax" : {
				"url" : "server.php",
				"data" : function (n) { 
					return { id : n.attr ? n.attr("id") : 0 }; 
				}
			}
		},
		"plugins" : [ "themes", "json_data" ]
	});
});