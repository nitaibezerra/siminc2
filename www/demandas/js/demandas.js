/*
function alteraIcone(id) {
	var img    = 'img_'+id;
	var tabela = document.getElementById('tabela');

	var i = document.getElementById(img);
	
	if (i && i.src.search("mais.gif") > 0) {
		document.getElementById(img).src = "../imagens/menos.gif";	
		carregarDemandas('ajax.php', 'tipo=carregar_demandas&usucpfexecutor='+id+'');
		
	} else {
		document.getElementById(img).src = "../imagens/mais.gif";	
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(id+"_") >= 0) {
				tabela.rows[i].style.display = "none";
			}
		}
	}
}
*/
