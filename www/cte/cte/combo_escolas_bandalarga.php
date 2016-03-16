<?php
	
// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	$sqlEstado = "
                    select
                        estuf as codigo,
                        estdescricao as descricao
                    from territorios.estado
                    order by
                        estdescricao
                ";
	$estados = $db->carregar( $sqlEstado );
	$PstEstados		= $_REQUEST["estados"];
	$PstMunicipios  = $_REQUEST["municipio"];
	
	if(isset($PstMunicipios)){
		$sqlListaEscolas="select e.entnome, e.entid as codigo
		from entidade.entidade as e, entidade.endereco as en, territorios.municipio as mu
		where e.funid=3 
		and e.entid = en.entid 
		and en.estuf = '".$PstEstados."'
		and mu.muncod = '".$PstMunicipios."'
		and mu.estuf = en.estuf";
		
		$Escolas = $db->carregar( $sqlListaEscolas );
		//print_r($Escolas);
	}
?>
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
		<script type="text/javascript" src="/includes/ajaxPopupEstadosMunicipios.js"></script>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <tr>
      <td>Estados:</td>
      <td>
      <select name="estados" id="estados" onChange="mostrarMunicipios(this.value)">
      <?
      	foreach($estados as $estados){
			$ID	  = $estados['codigo'];
			$Nome = $estados['descricao'];
      ?>
        <option  value="<?=$ID?>"><?=$Nome?></option>
      <?
		}
      ?>
      </select>
      </td>
      </tr>
      <tr>
      <td style="visibility:hidden;" id="Titulo"> Municipios:</td>
      <td><span id="municipios"></span> <span style="visibility:hidden;" id="btn" > <input id="button" type="submit" name="button" type="button" value="Filtrar" /></span></td>
      </tr>
          <? 
  if(isset($PstMunicipios)){
  
  ?>
   <tr>
   <td>
  Todos: <input name="todos" type="checkbox" id="todos"  onclick="selecionaTodos();" >
  </td>
  <td></td>
  </tr>
  <?
  	$maximo = count($Escolas);	
  	foreach($Escolas as $Escolas){
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $Escolas['codigo']?>" 
      type="checkbox" 
      id="checkbox<?= $Escolas['codigo']?>" 
      value="<?= $Escolas['codigo']?>"
      title="<?= $Escolas['entnome']?>"
      onclick="obterMarcados('checkbox<?= $Escolas['codigo']?>', '<?= $Escolas['codigo']?>','<?= $Escolas['entnome']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$Escolas['entnome'] ?></td>

    </tr>
      <?
  	}
  }
  ?>
      
      </table>
      </form>
<script language="javascript">
function obterMarcados(Nome,Valor,Estado) {  
	checkBox = document.getElementById(Nome); 
	if ( checkBox.checked ) {    
		if((opener.document.formulario.escolas.options.length == 1) && (opener.document.formulario.escolas.options[0].value == "")){
			opener.document.formulario.escolas.options[0] = null;
		}
	  	var d=opener.document.formulario.escolas.options.length++;
		opener.document.formulario.escolas.options[d].text = Estado;
		opener.document.formulario.escolas.options[d].value = Valor;
		opener.document.formulario.escolas.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.escolas.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.escolas.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.escolas.options.length++;
				opener.document.formulario.escolas.options[d].text = textocombogeral;
				opener.document.formulario.escolas.options[d].value = "";
				opener.document.formulario.escolas.options[d].setAttribute("","");
			} 
		}
	}   
} 

function selecionaTodos(){
	for (i=0;i<document.formulario.elements.length;i++){ 
		if(document.formulario.elements[i].type == "checkbox"){
      		checkBox = document.getElementById(document.formulario.elements[i].id);
      		if ( checkBox.checked == false && checkBox.id !== 'todos') {    
		         document.formulario.elements[i].checked=true;
				 Nome = document.formulario.elements[i].name;
				 Valor = document.formulario.elements[i].value;
				 Estado = document.formulario.elements[i].title;
				 obterMarcados( Nome, Valor,Estado );
			 }
			else if(checkBox.id !== 'todos') {
			document.formulario.elements[i].checked=false;
			 Nome = document.formulario.elements[i].name;
			 Valor = document.formulario.elements[i].value;
			 Estado = document.formulario.elements[i].title;
			 obterMarcados( Nome, Valor,Estado );
			}
		}
	}
}
/*

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
*/
</script>
