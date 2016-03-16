<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 
// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
		
if (isset($_REQUEST[estuf_filter])):
	!$_REQUEST[estuf_filter] ? die() : '';	
	$sql = "
	
	SELECT  DISTINCT mu.muncod, 
					 mundescricao  
   		FROM cte.instrumentounidadeescola iue
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
		LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
   		mu.estuf = '". $_REQUEST['estuf_filter'] ."' AND e.tpcid = 1 AND ef.funid = 3 
   		ORDER BY
       		mundescricao";
	$mun = $db->carregar($sql);
	
	$options .= "<option  value=\"\">Selecione um município</option>";
	foreach ($mun as $mun):
		//$options .= simec_htmlentities("<option  value=\"{$mun[muncod]}\">{$mun[mundescricao]}</option>", ENT_QUOTES);
		$options .= "<option  value=\"{$mun[muncod]}\">{$mun[mundescricao]}</option>";
	endforeach;
	
	die($options);
	
	unset($sql,$num,$options);
endif;
	
	$sqlEstado = "
					SELECT DISTINCT estado.estuf as codigo,
					 estado.estdescricao AS descricao 
   		FROM cte.instrumentounidadeescola iue
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
		LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
		e.tpcid = 1 AND ef.funid = 3 
   		ORDER BY
       		descricao
                ";
	$estados = $db->carregar( $sqlEstado );
	$PstEstados		= $_REQUEST["estados"];
	$PstMunicipios  = $_REQUEST["municipio"];
	$municipio		= $_REQUEST["municipio"];
	
	$Escolas = array();
	if($PstEstados && $PstMunicipios){
		/*
		$sqlListaEscolas="SELECT 
							e.entnome,
							e.entid as codigo
						  FROM
							entidade.entidade as e,
							entidade.endereco as en,
							territorios.municipio as mu
						  WHERE
		 					e.funid=3 
							AND e.entid = en.entid 
							AND en.estuf = '".$PstEstados."'
							AND mu.muncod = '".$PstMunicipios."'
							AND mu.estuf = en.estuf";
		*/
		$sqlListaEscolas="
		SELECT DISTINCT e.entid as codigo,
						e.entnome AS entnome 
   		FROM cte.instrumentounidadeescola iue
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
		LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
		mu.estuf = '".$PstEstados."' AND
		mu.muncod = '".$PstMunicipios."' AND
		e.tpcid = 1 AND ef.funid = 3 
   		ORDER BY
       		entnome
		";
		$Escolas = $db->carregar( $sqlListaEscolas );
		//print_r($Escolas);
	}
?>
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
		<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr>
      <td class="SubTituloDireita">Estados:</td>
      <td>
      <select name="estados" id="estados" onChange="carregaMunicipio(this.value)">
      	<option value="">Selecione um estado</option>
      <?
      	foreach($estados as $estados){
			$ID	    = $estados['codigo'];
			$Nome   = $estados['descricao'];
			$select = $PstEstados == $ID ? 'selected' : ''; 
      ?>
        <option  value="<?=$ID?>" <?=$select?>><?=$Nome?></option>
      <?
		}
      ?>
      </select>
      </td>
      </tr>
      <tr>
      <td style="visibility:<? echo strlen($PstEstados) ? 'visible' : 'hidden'; ?>;" id="Titulo" class="SubTituloDireita"> Municipios:</td>
      <td>
      	<span id="municipios">
      	<?php
      	    if ($PstEstados){
				$sql = "SELECT
						 DISTINCT mu.muncod AS codigo,
						 mundescricao AS descricao
						FROM
						 cte.instrumentounidadeescola iue
						 JOIN entidade.entidade e ON e.entid = iue.entid
						 INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
						 JOIN entidade.endereco ende ON ende.entid = e.entid
						 JOIN territorios.municipio mu ON mu.muncod = ende.muncod
						-- JOIN territorios.estado ON estado.estuf = mu.estuf
						 JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid 
							AND (
								entdreg_medio_prof = '1' OR 
								entdreg_medio_medio = '1' OR 
								entdreg_medio_normal = '1' OR
								entdreg_medio_integrado = '1' 
							    )
						 JOIN cte.conteudoppp cpp ON cpp.entid = e.entid
						 JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid
						 JOIN cte.cursotecnico ct ON cont.crsid =  ct.crsid
						WHERE
						 mu.estuf = '".$PstEstados."' AND
						 e.tpcid = 1 AND
						 ef.funid = 3
						ORDER BY
						 mundescricao;";
				$db->monta_combo('municipio', $sql, 'S', 'Selecione o município','document.formulario.submit();','','');
      	    }
      	?>
      	</span> 
      	<span style="visibility:hidden;" id="btn" > <!-- <input id="button" type="submit" name="button" type="button" value="Filtrar" />--></span>
      	</td>
      </tr>
          <? 
  if(is_array($Escolas) && !empty($Escolas)){
  
  ?>
   <tr>
	   <td class="SubTituloEsquerda" colspan="2">
	  Todos: <input name="todos" type="checkbox" id="todos"  onclick="selecionaTodos(this, '');" >
	  </td>
  </tr>
  <?
  	$maximo  = count($Escolas);	
//  	$Escolas = is_array($Escolas) && !empty($Escola) ? $Escolas : array(); 
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
  }elseif ($PstMunicipios){
  ?>
	<tr>
		<td colspan="2" style="color:red; text-align:center;">
			Nenhum registro foi encontrado
		</td>
	</tr>
  <?php
  }
  ?>
      <TR>
      	<td class="SubTituloDireita"></td>
      	<TD class="SubTituloEsquerda"><input name="ok" value="Ok" type="button" onclick="window.close();">&nbsp<input name="ok" value="Fechar" type="button" onclick="window.close();">&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<input name="limpar" value="Limpar filtro Escolas" type="button" onclick="selecionaTodos(document.getElementById('todos'), 1);"></TD>
      </TR>
      </table>
      </form>
<script language="javascript">
objWinPai = opener.document.getElementById('escolas');
//function obterMarcados(Nome,Valor,Estado) {  
function obterMarcados() {  
	d 	      = document;
	forms     = d.formulario;
	
	objWinPai.innerHTML = '';
	var opt = '', a=0;
	
	for (i=0; i < forms.elements.length; i++) {
		if (forms.elements[i].type == 'checkbox' &&	forms.elements[i].id !== 'todos' && forms.elements[i].checked) {

			opt += "<option value=\"" + forms.elements[i].value + "\">" + forms.elements[i].title + "</option>";
			objWinPai.innerHTML = opt;
			a++;
		}else if (forms.elements[i].type == 'checkbox' &&	forms.elements[i].id !== 'todos') {	
			d.getElementById('todos').checked = false;
		}	
	}
	if (a == 0)
		objWinPai.innerHTML = "<option value=\"\">Duplo clique para selecionar da lista</option>";
} 

function selecionaTodos(obj, det){
	if (!obj){
		obterMarcados()		
		return;
	}	
	if (det !== '')
		obj.checked = false;
		
	var param = obj.checked ? true : false;	
	
	for (i=0; i < document.formulario.elements.length; i++){ 

		if(document.formulario.elements[i].type == "checkbox"){

      		checkBox = document.getElementById(document.formulario.elements[i].id);

		    if (checkBox.id !== 'todos') 
		    	 document.formulario.elements[i].checked=param;
		    
		}
	}
	obterMarcados()
}


function carregaMunicipio (uf){	
  if (uf == ''){
  	document.formulario.municipio ? document.formulario.municipio.value = '' : ''; 
  	document.formulario.submit();
  	return;
  }
  var req = new Ajax.Request('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/combo_escolas.php', {
        method: 'post',
        parameters: '&estuf_filter=' + uf ,
        onComplete: function (res)
        {
        	vis = res.responseText.length > 0 ? 'visible' : 'hidden';
        	d   = document;
			d.getElementById('Titulo').style.visibility = vis;			
        	mun = d.getElementById('municipios');
        	
        	mun.style.visibility = vis;

        	selectMun 			= document.createElement('<select>');
        	selectMun.name		= 'municipio';
        	selectMun.onchange	= function () {document.formulario.submit();};
        	selectMun.innerHTML = res.responseText; 
        	mun.innerHTML 		= '';
        	mun.appendChild(selectMun);
		}
	});
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
