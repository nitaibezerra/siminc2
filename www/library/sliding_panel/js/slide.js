jQuery(document).ready(function() {
	
	// Expand Panel
	jq("#open").click(function(){
        jq("div#panel_geral").slideDown("slow");
	
	});	
	
	// Collapse Panel
    jq("#close").click(function(){
        jq("div#panel_geral").slideUp("slow");
	});		
	
	// Switch buttons from "Log In | Register" to "Close Panel" on click
    jq("#toggle a").click(function () {
        jq("#toggle a").toggle();
	});

//    jq('#toppanel > div#panel_geral').css('height', (window.innerHeight - 60));
//    jQuery("#div_detalhe_pendencias_obras").load('/obras2/ajax.php?detalhar_pendencias_obras=1&muncod='+jQuery(this).attr('muncod')+'&estuf='+jQuery(this).attr('estuf'));
});
