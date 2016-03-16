function iniciaAjax()
{
	//verifica se o navegador e o Iternet Explorer ou outros navegadores
	if(window.ActiveXObject)
	{
		//estancia o objeto ActiveX
		ajax = new ActiveXObject("Microsoft.XMLHTTP");				
	}
	else
	{
		ajax = new XMLHttpRequest();
	}
	
	return ajax;
}

function carregando()
{
	//limpa os municipios ja existentes
	document.getElementById('municipios').innerHTML = "";
	//pega o local onde a combo de municipios serão exibidos
	var local = document.getElementById('municipios');
	
	//cria uma combo select
	var combo = document.createElement('select');
	combo.setAttribute('name','municipios');
	combo.setAttribute('id','municipios');	
	
	var opcao = document.createElement('option');
	opcao.setAttribute('value', 00);
	opcao.appendChild(document.createTextNode("Carregando..."));
	
	//adiciona essa opcão na combo
	combo.appendChild(opcao);
	
	//coloca a combo dentro do div
	local.appendChild(combo);
}

function mostrarMunicipios(idMunicipios)
{
	//informa que está sendo carregando as cidades
	carregando();
	
	
	//inicia o AJAX
	ajax = iniciaAjax();
	
	ajax.onreadystatechange = mostrarMunicipios2;
	
	//abre a conexão com o servidor
	ajax.open("GET", "municipios_xml.php?idMunicipios="+idMunicipios);
	
	//envia a requisição para o servidor
	ajax.send();
}

function mostrarMunicipios2()
{
	//verifica o status da requisição, se for o processamento está completo 
	if (ajax.readyState == 4) 
	{     		
		//verifica o número do status, se for diferente de 200 tem algum erro 
		if (ajax.status == 200) 
		{
            var xml = ajax.responseXML;
			if(xml != null)
			{
				if(xml.hasChildNodes())
				{	
					//limpa os municipios já existentes
					document.getElementById('municipios').innerHTML = "";
					
					//pega o local onde a combo de cidades será exibida]
					var local = document.getElementById('municipios');
					
					//cria uma combo select
					var combo = document.createElement('select');
					combo.setAttribute('name','municipios');
					combo.setAttribute('id','municipios');
					
					//pega todas as cidades qae retornou do XML
					var nos = xml.getElementsByTagName('municipios');
					
					//faz um loop para percorrer todas as tags produto
					for(cont = 0; cont < nos.length; cont++)
					{
						//verifica se não e o IE
						if(window.ActiveXObject)
						{						
							var idCidade = nos[cont].childNodes[0].firstChild.nodeValue;
							var cidade = nos[cont].childNodes[1].firstChild.nodeValue;
						}
						else
						{
							var idCidade = nos[cont].childNodes[1].firstChild.nodeValue;
							var cidade = nos[cont].childNodes[3].firstChild.nodeValue;
						}	
						
						//cria um option do select
						var opcao = document.createElement('option');
						opcao.setAttribute('value', idmunicipios);
						opcao.appendChild(document.createTextNode(municipios));
						
						//adiciona essa opção na combo
						combo.appendChild(opcao);
						
					}
					
					//coloca a combo dentro do div
					local.appendChild(combo);
				}
			}
        } 
		else 
		{
            alert("Houve um problema ao carregar a lista de municipios:\n" + ajax.statusText);
        }		
    } 	
}	