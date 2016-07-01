/*** (INÍCIO) AJAX REQUESTS ***/

/*
function carregarDemandas(url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponsecarregarDemandas
	 }  
	);  
}
*/

/*** (INÍCIO) AJAX RESPONSES ***/

/*
function getResponsecarregarDemandas(oReq) {
  if(oReq.responseText.search("@") >= 0) {
	arrayDemandas = oReq.responseText.split('@');
	arrayAux = arrayDemandas[1].split('||');	
	
	indexLinhaUF = document.getElementById(arrayAux[0]).rowIndex;		
	corLinhaUF = document.getElementById(arrayAux[0]).style.backgroundColor;	
	
	if(corLinhaUF == "rgb(250, 250, 250)")
		cor = "#fafafa";
	else
		cor = "#f0f0f0";
	
	for(i=1; i < arrayDemandas.length; i++) {
		arrayAux = arrayDemandas[i].split('||');
		
		tr = $('tabela').insertRow(++indexLinhaUF);
		tr.id = arrayAux[0]+"_"+ arrayAux[1];
		
		
		if(cor == "#fafafa") {
			tr.style.backgroundColor = "#f0f0f0";
			cor = "#f0f0f0";
		}
		else {
			tr.style.backgroundColor = "#fafafa";
			cor = "#fafafa";
		}	       
        td1 = tr.insertCell(0);		
        td1.style.align="left";
		td1.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\">";
        
		td2 = tr.insertCell(1);
		td2.style.textAlign = "left";
		td2.innerHTML = "";
		
		td3 = tr.insertCell(2);
		td3.style.textAlign = "left";
		td3.innerHTML = arrayAux[3];
		            
		td4 = tr.insertCell(3);
		td4.style.textAlign = "left";
		td4.innerHTML = arrayAux[4]
		
		td5 = tr.insertCell(4);
		td5.style.textAlign = "center";
		td5.innerHTML = arrayAux[5]
		
		td6 = tr.insertCell(5);
		td6.style.textAlign = "center";
		td6.innerHTML = arrayAux[6];		
	}
  }
  else {
  	document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
  }
}
*/

/*** (FIM) AJAX RESPONSES ***/
