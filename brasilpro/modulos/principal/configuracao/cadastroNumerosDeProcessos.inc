<?php
include APPRAIZ . "includes/Snoopy.class.php";

// OBS: funções aplicadas apenas para os Estados

/************************************************************	
* 	RECUPERA MUNICÍPIOS
* 	(FUNÇÃO AJAX)
* 	REQUEST				: estadoescolha
*   CHAMADA JAVASCRIPT	: exibirMunicipios(value)
* 	DESCRIÇÃO: 
*   recupera todos os municípios de um estado selecionado no combo.
*************************************************************/
if ($_REQUEST['estadoescolha']) {
	$estado = $_REQUEST['estado'];
	$listaMunicipios = "SELECT 	muncod as codigo,
								mundescricao as descricao
	                    FROM territorios.municipio
	                    WHERE estuf = '".$estado."'
	                    ORDER BY mundescricao ASC";
	echo $db->monta_combo("muncod", $listaMunicipios,'S' , "Selecione o Município", '', '', '', '300', 'N', 'muncod');
    die();
}

/************************************************************	
* 	RECUPERA MUNICÍPIOS MODO EDITAR DADOS
* 	(FUNÇÃO AJAX)
* 	REQUEST				: editarmunicipio
*   CHAMADA JAVASCRIPT	: editarNumProcesso(estuf, numProcesso, muncod)
* 	DESCRIÇÃO: 
*   Monta uma lista de municípios para que o usuario possa escolher qual editar ou inserir o número de processo.
*************************************************************/
if ($_REQUEST['editarmunicipio']) {
	$estado = $_REQUEST['estado'];
	$listaMunicipios = "SELECT 	muncod as codigo,
								mundescricao as descricao
	                    FROM territorios.municipio
	                    WHERE estuf = '".$estado."'
	                    ORDER BY mundescricao ASC";
	$muncod = $_REQUEST['muncod'];
	echo $db->monta_combo("muncod", $listaMunicipios,'S' , "Selecione o Município", '', '', '', '300', 'N', 'muncod',true,$muncod);
    die();
}

/************************************************************	
* 	RECUPERA ESTADOS MODO EDITAR DADOS
* 	(FUNÇÃO AJAX)
* 	REQUEST				: editarestado
*   CHAMADA JAVASCRIPT	: editarNumProcesso(estuf, numProcesso, muncod)
* 	DESCRIÇÃO: 
*   Mostra o estado para que possa ser editado ou inserir o número de processo.
*************************************************************/
if ($_REQUEST['editarestado']) {
	$listaEstados = "SELECT e.estuf as codigo,
                        e.estuf || ' - ' || e.estdescricao as descricao
                 FROM territorios.estado e
                 inner join cte.instrumentounidade iu on iu.estuf = e.estuf 
                 where iu.itrid in (3,4)
                 ORDER BY estdescricao ASC ";
	$estuf = $_REQUEST['instrumento'];
	$estado = $db->monta_combo("estuf", $listaEstados, 'S', "Todas as Unidades Federais", '', '', '', '200', 'N', 'estuf',true,$estuf);
	echo $estado;
    die();
}

/************************************************************	
* 	SALVAR DADOS
* 	(SUBIMIT DA PÁGINA)
* 	REQUEST				: formularioSumit
* 	DESCRIÇÃO: 
*   Edita ou insere o número de processo.
*************************************************************/
if ($_REQUEST['formularioSumit'] == '1') {
	$erro = 0;
	$idMunicipio = $_REQUEST['muncod'];
	$siglaEstado = $_REQUEST['estuf'];
	
	if($idMunicipio){ //se for município
		$sql = "SELECT inuid FROM cte.instrumentounidade where muncod = '".$idMunicipio."' and itrid in(3,4)";
	}else{ // se for estado
		$sql = "SELECT inuid FROM cte.instrumentounidade where estuf = '".$siglaEstado."' and itrid in(3,4)";
	}
	$inuid = $db->pegaUm($sql);
	unset($sql);
	
	$sql = "SELECT numprocesso FROM cte.instrumentounidadeprocesso WHERE inuid = ".$inuid;
	$numprocessoExistente = $db->pegaUm($sql);
	
	// deleta o número atual
	$sql = "delete from cte.instrumentounidadeprocesso where inuid = '".$inuid."';"; 
	$db->executar($sql);
	unset($sql);
	
	// insere o número
	$sql = "insert into cte.instrumentounidadeprocesso (inuid, numprocesso, indstatus) 
			values (".$inuid.",'".$_REQUEST['numeroProcesso']."', 'A' )";
	$db->executar($sql);
	$db->commit();
	echo '<script type="text/javascript">'
            .'alert("O número do processo ('.$_REQUEST['numeroProcesso'].') cadastrado com sucesso.");'
            .'</script>';
}

/************************************************************	
* 	RETORNA PESQUISA DO USUÁRIO.
* 	(FUNÇÃO AJAX)
* 	REQUEST				: lista
*   CHAMADA JAVASCRIPT	: function pesquisa()
* 	DESCRIÇÃO: 
*   Retorna a busca do usuário.
*************************************************************/
if ($_REQUEST['lista']) {
	switch($_REQUEST['lista']) {
		// lista todos os estados e município com números de processo
		case $_REQUEST['semnumprocesso'] == 0 && !$_REQUEST['estado']:
			$select = "	'<center><img id=\"Editar\" name=\"Editar\" 
						onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| coalesce(iup.numprocesso,'') ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
						src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";	
			$selectNumProcesso = ",iup.numprocesso";	
			$from = " from  cte.instrumentounidade  iu
				  	  left join cte.instrumentounidadeprocesso iup on iu.inuid = iup.inuid  ";
			$where = "and iu.estuf is not null";
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" ,"Número do Processo" );
			break; 
			
		// lista todos os estados e município sem números de processo	
		case $_REQUEST['semnumprocesso'] == 1 && !$_REQUEST['estado']: 
			$select ="	'<center><img id=\"Editar\" name=\"Editar\" 
						onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| '' ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
						src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";
			$from 	=" from cte.instrumentounidade iu ";
			$where = " and iu.inuid not in (select inuid from cte.instrumentounidadeprocesso) and iu.estuf is not null ";
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" );
			break; 
			
		// todos de um estado especifico sem número de processo	
		case $_REQUEST['semnumprocesso'] == 1 && $_REQUEST['estado'] && !$_REQUEST['muncod']: 
			$select = "'<center><img id=\"Editar\" name=\"Editar\" 
						onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| '' ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
						src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";
			$from = " from cte.instrumentounidade iu ";
			$where = " and iu.inuid not in (select inuid from cte.instrumentounidadeprocesso) and  
					  ( e.estuf = '".$_REQUEST['estado']." '
					   or m.estuf = '".$_REQUEST['estado']."')  and iu.estuf is not null";			
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" );
			break;
			
		// município selecionado sem número de processo 	
		case $_REQUEST['semnumprocesso'] == 1 && $_REQUEST['estado'] && $_REQUEST['muncod'] : 
			$select = "'<center><img id=\"Editar\" name=\"Editar\" 
						onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| '' ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
						src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";
			$from = "from cte.instrumentounidade iu ";
			$where = "and iu.inuid not in (select inuid from cte.instrumentounidadeprocesso) and 
					 m.muncod = '".$_REQUEST['muncod']."'  and iu.estuf is not null";
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" );
			break;

		// todos de um estado especifico com e sem número de processo	
		case $_REQUEST['estado'] && !$_REQUEST['muncod'] :
			$select = "'<center><img id=\"Editar\" name=\"Editar\" 
					  onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| coalesce(iup.numprocesso,'') ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
					  src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";
			$selectNumProcesso = ",iup.numprocesso";	
			$from 	= "from cte.instrumentounidade iu
					   left join cte.instrumentounidadeprocesso iup on iu.inuid = iup.inuid ";
			$where 	= " and  (e.estuf = '".$_REQUEST['estado']."' 
						or m.estuf = '".$_REQUEST['estado']."' )  and iu.estuf is not null";
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" ,"Número do Processo" );
			break;
			
		// município selecionado com ou sem número de processo 	
		case $_REQUEST['muncod'] && $_REQUEST['estado'] :
			$select = "'<center><img id=\"Editar\" name=\"Editar\" 
					  onclick=\"return editarNumProcesso(''' || Case m.muncod when trim(m.muncod) then m.estuf else  e.estuf end || ''','''|| coalesce(iup.numprocesso,'') ||''','''|| Case m.muncod when trim(m.muncod) then m.muncod else  '0' end ||''')\" 
					  src=\"/imagens/edit_on.gif\" title=\"Editar\" style=\"border:none\" alt=\"Editar número de processo\" /></center>' as editar,";
			$selectNumProcesso = ",iup.numprocesso";	
			$from 	= "from cte.instrumentounidade iu
					   left join cte.instrumentounidadeprocesso iup on iu.inuid = iup.inuid 
					  ";
			$where 	= " and m.muncod = '".$_REQUEST['muncod']."'  and iu.estuf is not null";
			$cabecalhoLista = array( "Editar","Descrição Estado / Município", "Estado ou Município" ,"Número do Processo" );
			break;
	}

	$sql="select ".$select."
				Case m.muncod when trim(m.muncod) then m.estuf || ' - ' || m.mundescricao else  e.estuf || ' - ' || e.estdescricao end as descr,
				Case m.muncod when trim(m.muncod) then 'Município' else  'Estado' end as tipo
				".$selectNumProcesso."
		 ".$from."
		 left join territorios.municipio m on  m.muncod = iu.muncod and m.estuf = iu.mun_estuf
		 left join territorios.estado e on e.estuf = iu.estuf
		 where iu.itrid in ( 3,4 ) 
			". $where ."
		 order by e.estuf ";
	//dbg($sql,1);
	echo $db->monta_lista( $sql, $cabecalhoLista, 50, 10, 'N', '', '' );
	die();
}

/************************************************************	
* 	MONTA TELA INICIAL
*************************************************************/
// MONTA COMBO DE ESTADOS
$listaEstados = "SELECT e.estuf as codigo,
                        e.estuf || ' - ' || e.estdescricao as descricao
                 FROM territorios.estado e
                 inner join cte.instrumentounidade iu on iu.estuf = e.estuf 
                 where iu.itrid in (3,4)
                 ORDER BY estdescricao ASC ";
$estuf = $_REQUEST['estuf']; 
$estado = $db->monta_combo("estuf", $listaEstados, 'S', "Todas as Unidades Federais", 'exibirMunicipios', '', '', '200', 'N', 'estuf',true);

// MONTA COMBO DE MUNICÍPIOS EM BRANCO
$listaMunicipios = "SELECT 	muncod as codigo,
							mundescricao as descricao
	                    FROM territorios.municipio
	                    where muncod is null";

// MONTA CABEÇALHO DA PÁGINA OBS: var $titulo_modulo componente
$cabecalhoPagina = monta_titulo( $titulo_modulo, '&nbsp;' );

?>
<html>
<head>
<script language="JavaScript" type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/prototype.js"></script>
</head>
<body>
<form action="" method="post" name="cadastroProcessos">
<input type="hidden" id="formularioSumit" name="formularioSumit" value="0"/>
<input type="hidden" id="editar" name="editar" value="0"/>
<?php $cabecalhoPagina; ?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
  <tr>
    <td class="SubTituloDireita">Estado:</td>
    <td id="estados"><?php echo $estado; ?> </td>
  </tr>
	<!-- 
  <tr>
    <td class="SubTituloDireita">Município:</td>
    <td id="municipio"><?php echo $db->monta_combo("muncod", $listaMunicipios,'S' , "Selecione um Estado...", '', '', '', '300', 'N', 'muncod');?></td>
  </tr>
   	-->
  <tr>
    <td class="SubTituloDireita">Número de processo:</td>
    <td> <input type="text" name="numeroProcesso" id="numeroProcesso" /> </td>
  </tr>
  <tr>
    <td  class="SubTituloDireita">  Estado / Município sem número de processos: </td>
    <td>
    <input type="checkbox" name="semNumProcessos" id="semNumProcessos" />
   </td>
  </tr>
 <tr>
    <td> </td>
    <td>
   		<input type="submit" name="Submit" id="Gravar" value="Gravar" onclick="return submeter();" style="visibility:hidden;" />
   		<input type="button" name="Submit" id="Pesquisar" value="Pesquisar" onclick="return pesquisa();" style="visibility:visible;" />
   </td>
  </tr>
</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
<td id="obs"></td>
</tr>
</table>
<div id="lista"></div>

<script type="text/javascript">
// btn pesquisa
function pesquisa(){
	var estado 				= document.getElementById('estuf').value;
	//var municipio 			= document.getElementById('muncod').value; 
	var municipio = 0;
	var numerodeprocesso 	= document.getElementById('numeroProcesso').value;
	var semNumProcessos 	= document.getElementById('semNumProcessos').checked;
	if(semNumProcessos == true){
		semNumProcessos = 1;
	}else{
		semNumProcessos = 0;
	}
	new Ajax.Request(window.location.href,
                         {
                             parameters: 'lista=lista&semnumprocesso='+semNumProcessos+'&estado='+estado+'&muncod='+municipio,
                             onComplete: function(e)
                             {
                                 $('lista').innerHTML = e.responseText;
                             }
                         });
}

// btn da lista (editar dados)
function editarNumProcesso(estuf, numProcesso, muncod){
	if(estuf){
		 new Ajax.Request(window.location.href,
                         {
                             parameters: 'editarestado=estado&instrumento=' + estuf,
                             onComplete: function(e)
                             {
                                 $('estados').innerHTML = e.responseText;
                             }
                         });
         $('numeroProcesso').value = numProcesso;
         $('editar').value = 1;
         $('Pesquisar').disabled="disabled";
         $('Gravar').disabled="";
	}
	if(muncod){
		new Ajax.Request(window.location.href,
                         {
                             parameters: 'editarmunicipio=municipio&estado=' + estuf + '&muncod='+ muncod,
                             onComplete: function(e)
                             {
                                 $('municipio').innerHTML = e.responseText;
                             }
                         });
                          $('Pesquisar').style.visibility="hidden";
                          $('Gravar').style.visibility="visible";
	}
}

// exibir municipios de um estado
function exibirMunicipios(value)
{
        new Ajax.Request(window.location.href,
                         {
                             parameters: 'estadoescolha=estado&estado=' + value,
                             onComplete: function(e)
                             {
                                 $('municipio').innerHTML = e.responseText;
                             }
                         });
}

// garvar dados
function submeter(){
  		var numerodeProcesso = document.getElementById('numeroProcesso').value;
  		//var muncod = document.getElementById('muncod').value;
  		if(numerodeProcesso){
  			document.getElementById('formularioSumit').value = 1;
  			formulario.submit();
  		}else{
  			alert("Insira um número de processo para o processo.");
  			return false;
  		}
}

//paginação da lista
function pagina(numero) {
	var numero = numero;
	var estado 				= document.getElementById('estuf').value;
	//var municipio 			= document.getElementById('muncod').value;
	var municipio = 0;
	var numerodeprocesso 	= document.getElementById('numeroProcesso').value;
	var semNumProcessos 	= document.getElementById('semNumProcessos').checked;
	if(semNumProcessos == true){
		semNumProcessos = 1;
	}else{
		semNumProcessos = 0;
	}

	new Ajax.Request(window.location.href,
                         {
                             parameters: 'lista=lista&semnumprocesso='+semNumProcessos+'&estado='+estado+'&muncod='+municipio+'&numero='+numero,
                             onComplete: function(e)
                             {
                                 $('lista').innerHTML = e.responseText;
                             }
                         });
                   
}
</script>
</body>
</html>