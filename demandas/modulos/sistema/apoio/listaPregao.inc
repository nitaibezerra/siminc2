<?php


function montaCelula($ordid){
	global $db;
 
	$sql = "SELECT
			 c.celid AS codigo,
			 c.celnome as DESCRICAO
			FROM
			 demandas.celula c
			WHERE
			 c.ordid = ".$ordid." AND
			 c.celstatus = 'A'
			ORDER BY
			 c.celnome;";
	$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --','', '' );
	echo obrigatorio();
		
}


function inserirSevico(){
	global $db;
	
	($_POST['celid'] == '') ? $celid = "NULL" : $celid = " '".$_POST['celid']."'";
	
	$sql = " INSERT INTO demandas.tiposervico
			(ordid, niaid, tipnome, tipquantidade, tipnumserie, tipnumpatr, tipdescricao, tipstatus, grsid, celid) VALUES
			('".$_POST['ordid']."', '".$_POST['niaid']."', '".$_POST['tipnome']."', ".$_POST['tipquantidade'].", ".$_POST['tipnumserie'].", ".$_POST['tipnumpatr'].", 
			'".$_POST['tipdescricao']."', '".$_POST['tipstatus']."', ".$_POST['grsid'].", ".$celid.")  RETURNING tipid";
	$tipid = $db->pegaUm($sql);
	
	$n = 0;
	
	foreach ($_REQUEST['priid'] as $priid ) {
				
				($_REQUEST['tsptempo'][$n] == '')? $_REQUEST['tsptempo'][$n] = '0:00' : $_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n].':00';
				//coloca os dois pontos caso não houver
				$pos = strpos($_REQUEST['tsptempo'][$n], ':');
				if($pos===false){
					$_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n].":00";
				}
				
				$sql = "INSERT INTO demandas.tiposervicoprioridade 
				(priid, tipid, tsptempo, tsppontuacao) values 
				( '$priid', '$tipid' ,'".$_REQUEST['tsptempo'][$n]."',
				'".$_REQUEST['pontuacao'][$n]."');";
				$db->executar($sql);
				$n++;
			}	
	$db->commit();
}

function editarSevico(){
	global $db;
	
	($_POST['celid'] == '') ? $celid = "NULL" : $celid = " '".$_POST['celid']."'";
	
	$sql = " UPDATE demandas.tiposervico SET 
			ordid = '".$_POST['ordid']."',
			niaid = '".$_POST['niaid']."',
			tipnome = '".$_POST['tipnome']."',
			tipdescricao = '".$_POST['tipdescricao']."',
			tipquantidade = ".$_POST['tipquantidade'].",
			tipnumserie = ".$_POST['tipnumserie'].",
			tipnumpatr = ".$_POST['tipnumpatr'].",
			tipstatus = '".$_POST['tipstatus']."',
			grsid = ".$_POST['grsid'].",
			celid = ".$celid."
			 WHERE tipid = {$_POST['tipid']}";
	//dbg($sql,1);
	$db->executar($sql); 
	$sql = "DELETE FROM demandas.tiposervicoprioridade WHERE tipid = {$_POST['tipid']}";
	$db->executar($sql);
	$n = 0;
	foreach ($_REQUEST['priid'] as $priid ) {
				
				($_REQUEST['tsptempo'][$n] == '')? $_REQUEST['tsptempo'][$n] = '0:00' : $_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n];
				//coloca os dois pontos caso não houver
				$pos = strpos($_REQUEST['tsptempo'][$n], ':');
				if($pos===false){
					$_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n].":00";
				} 
				
				$sql = "INSERT INTO demandas.tiposervicoprioridade 
				(priid, tipid, tsptempo, tsppontuacao) values 
				( '$priid','".$_POST['tipid']."','".$_REQUEST['tsptempo'][$n]."',
				'".$_REQUEST['pontuacao'][$n]."');";
				$db->executar($sql);
				$n++;
			}
	$db->commit();
}

function deletarServico(){
	global $db;
	$sql = "UPDATE demandas.tiposervicoprioridade 
		SET tspstatus = 'I' WHERE tipid = {$_SESSION['tipid']}";
	$db->executar($sql);
	$sql = "UPDATE demandas.tiposervico 
		SET tipstatus = 'I' WHERE tipid = {$_SESSION['tipid']}";
	$db->executar($sql);
	$db->commit();
}

function carregarServico(){
	global $db;
	//Função para alimentar a tabela tiposervicoprioridade com todos os Graus de Severidade
	$sql = "SELECT priid FROM demandas.prioridade WHERE pristatus = 'A'";
	$grs = $db->carregar($sql);
	$grs = $grs ? $grs : array();
	
			foreach ($grs AS $i){
				$sql = "SELECT COUNT(priid) AS linhas FROM demandas.tiposervicoprioridade WHERE priid = '{$i['priid']}' AND tipid = {$_SESSION['tipid']}";
				$res = $db->carregar($sql);
				if($res[0]['linhas'] == 0){
					$sql = "INSERT INTO demandas.tiposervicoprioridade 
					(priid, tipid, tsptempo, tsppontuacao) values 
					( '{$i['priid']}','".$_SESSION['tipid']."','0:00','0');";
					$db->executar($sql);
				}
			}	
			
	$sql = "SELECT *   
			FROM  demandas.tiposervico AS t     
			WHERE t.tipid = '".$_SESSION['tipid']."'
	";
	return $db->carregar($sql);
}



if ($_REQUEST['ordidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaCelula($_POST['ordidAjax']);
	exit;
}


if($_POST['niaid'] && $_POST['tipdescricao'] && $_POST['ordid'] && $_POST['priid'] && !$_POST['tipid'] && !$_POST['pesquisa'] ){
	inserirSevico();
	//print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/catalagoServico&acao=A" );
	?>
		<script>
			alert('Operação realizada com sucesso!');
			location.href="demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&ordid=<?=$_POST['ordid']?>&pesquisa=1";
		</script>
	<? 
	exit();
}

if($_POST['niaid'] && $_POST['tipdescricao'] && $_POST['ordid'] && $_POST['priid'] && $_POST['tipid'] && !$_POST['pesquisa']){
	editarSevico();
	unset($_SESSION['tipid']);
	unset($_SESSION['op']);
	//print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/catalagoServico&acao=A" );
	?>
		<script>
			alert('Operação realizada com sucesso!');
			location.href="demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&ordid=<?=$_POST['ordid']?>&pesquisa=1";
		</script>
	<? 
	exit();
}

if($_GET['tipid'] && $_GET['op']){
	session_start();
	$_SESSION['tipid'] = $_GET['tipid'];
	$_SESSION['op'] = $_GET['op'];
	header( "Location: demandas.php?modulo=sistema/apoio/catalagoServico&acao=A" );
	exit();
}

if($_SESSION['tipid'] && $_SESSION['op']){
	if($_SESSION['op'] == 'delete'){
		deletarServico();	
		unset($_SESSION['tipid']);
		unset($_SESSION['op']);
		//print "<script>alert('Operação realizada com sucesso!');</script>";
		//header( "Location: demandas.php?modulo=sistema/apoio/equipeSistema&acao=A" );
		?>
			<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=sistema/apoio/catalagoServico&acao=A';
			</script>
		<? 
		exit();
	}
	if($_SESSION['op'] == 'update'){
		$dados = carregarServico();	
		unset($_SESSION['tipid']);
		unset($_SESSION['op']);
	}
	
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';


monta_titulo( 'Cadastro de Pregões', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script> 
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">

d = document;
  
function validaForm(){
	if(document.formCatalogo.ordid.value == ''){
		alert ('Informe a Origem da demanda.');
		document.formCatalogo.ordid.focus();
		return;
	}
	if(document.formCatalogo.ordid.value == '2' || document.formCatalogo.ordid.value == '13' || document.formCatalogo.ordid.value == '14'){
		if(document.formCatalogo.celid.value == ''){
			alert ('Informe a Célula.');
			document.formCatalogo.celid.focus();
			return;
		}
	}
	
	if(document.formCatalogo.tipnome.value == ''){
		alert ('O campo Nome do tipo de serviço deve ser preenchido.');
		document.formCatalogo.tipnome.focus();
		return;
	}
	if(document.formCatalogo.tipdescricao.value == ''){
		alert ('O campo Descrição do tipo de serviço deve ser preenchido.');
		document.formCatalogo.tipdescricao.focus();
		return;
	}
	if(document.formCatalogo.niaid.value == ''){
		alert ('Informe o nível do serviço.');
		document.formCatalogo.niaid.focus();
		return;
	}
	if(document.formCatalogo.grsid.value == ''){
		alert ('Informe a dificuldade do serviço.');
		document.formCatalogo.grsid.focus();
		return;
	}

    //Validação de Tempo de Serviço e Situação da Severidade
    tsptempo = false;
    loops = document.formCatalogo.tsptempo.length;
    for (i=0;i<loops;i++) {
    	if(document.formCatalogo.tsptempo[i].value == '' || document.formCatalogo.pontuacao[i].value == ''){
    		tsptempo = true;
    	}
    }
    if(tsptempo == true){
    	alert('Informe todos os campos do Tempo de Serviço e Pontuação!');
    	return;
    }

	
	//valida hora
    for (i=0;i<loops;i++) {
    	if(document.formCatalogo.tsptempo[i].value != ''){
    		if(document.formCatalogo.tsptempo[i].value.substr(0,2) > 23 || document.formCatalogo.tsptempo[i].value.substr(3,2) > 59){
    			alert('Tempo de serviço inválido!');
    			document.formCatalogo.tsptempo[i].focus();
    			return;
    			break;
    		}
    	}
    }
	
    document.formCatalogo.submit();
}


function pesq(){
	if(document.formCatalogo.ordid.value == '' && document.formCatalogo.tipnome.value == ''){
		alert ('Informe a Origem da demanda ou o Nome do tipo de serviço.');
		document.formCatalogo.ordid.focus();
		return;
	}
	document.formCatalogo.pesquisa.value="1";
    document.formCatalogo.submit();
}



//Função de Exclusão de Registros
function exclusao(url) {
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}
//Somente Números
function somenteNumeros(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8) || (key==58));
    {
    	return true;
    }
}


function filtraCelula(ordid) {

	trCel  = d.getElementById('tr_celula');
	tdCel  = d.getElementById('td_celula');
	
	if(ordid == 2 || ordid == 13 || ordid == 14){ //2=REDES - 13=Gestão Documentos CGI
	
		select = tdCel.getElementsByTagName('select')[0];
			
		
		// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
		if (select)
			select.disabled = true;
			
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=sistema/apoio/catalagoServico&acao=A', {
								        method:     'post',
								        parameters: '&ordidAjax=' + ordid,
								        onComplete: function (res)
								        {		
								        	//alert(res.responseText);	
											tdCel.innerHTML = res.responseText;
											trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
											tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
								        }
	
									  });
									  
	}
	else{
		tdCel.innerHTML = '';
		trCel.style.display = 'none';
		tdCel.style.display = 'none';
	}
}


function Anexar(param){

	w = window.open('?modulo=sistema/apoio/catalogoAnexo&acao=A&tipid='+param,'Anexo','scrollbars=yes,resizable=yes,location=no,toolbar=yes,menubar=no,width=780,height=400,left=250,top=125'); 
	w.focus();	
	
}


function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
	}
}

</script>

</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formCatalogo" name="formCatalogo" action="" method="post">

<input type="hidden" name="pesquisa" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="tipid" value="<? print $dados[0]['tipid']; ?>"  />
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita" width="40%">Status:</td>
		<td>
			<input type='radio' name='tipstatus' value='A' <?if($dados[0]['tipstatus'] == 'A' || $dados[0]['tipstatus'] == '' || $_REQUEST['tipstatus'] == 'A') echo "checked" ?>> Ativo
			&nbsp;&nbsp;
			<input type='radio' name='tipstatus' value='I' <?if($dados[0]['tipstatus'] == 'I' || $_REQUEST['tipstatus'] == 'I') echo "checked" ?>> Inativo
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Nº do Pregão:</td>
		<td><?= campo_texto( 'nome', 'S', 'S', '', 40, 40, '', ''); ?></td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Nº do Processo:</td>
		<td><?= campo_texto( 'nome', 'S', 'S', '', 40, 40, '', ''); ?></td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Objeto:</td>
		<td><?= campo_texto( 'nome', 'S', 'S', '', 60, 60, '', ''); ?></td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Observações:</td>
		<td><? $tipdescricao = $dados[0]['tipdescricao']; ?>
		<?= campo_textarea('obs', 'S', 'S', '', 75, 5, 4000); ?></td>	
	</tr>	
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Fornecedores:</td>
		<td>
			<div id="fornecedor_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fornecedor' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
			<div id="fornecedor_campo_on" style="display:none;">
			
			<?
			
			$sql_combo = "SELECT '1' AS codigo, 'Fornecedor 1' AS descricao
							union
							SELECT '2' AS codigo, 'Fornecedor 2' AS descricao";
													
		 	combo_popup( 'fornecedor', $sql_combo, 'Selecione o(s) Fornecedor(s)', '400x400', 0, array(), '', 'S', true, true );
			
			?>
			</div>
			<? if ($fornecedor) { ?> <script type="text/javascript"> onOffCampo( 'fornecedor' ); </script> <? } ?>
		</td>
	</tr>
		
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='button' onclick="validaForm();" class='botao' value='Salvar' name='cadastrar' />
	    	&nbsp;
	    	<input type='button' class='botao' onclick="location.href='demandas.php?modulo=sistema/apoio/catalagoServico&acao=A';" value='Cancelar'name='cancelar' />
	    	&nbsp;
	    	<input type="button" name="Pesquisar" value="Pesquisar" onclick="pesq();">	    	
		</td>			
	</tr>				
</table>
</form>
<br>
<?
	//filtro
	if($_REQUEST['pesquisa'] || $ordid || $tipnome || $celid){
		
		if($_REQUEST['ordid']){	$and .= " AND o.ordid = ".$_REQUEST['ordid']." ";}
		elseif($ordid){	$and .= " AND o.ordid = ".$ordid." ";}
		
		if($_REQUEST['celid']){	$and .= " AND s.celid = ".$_REQUEST['celid']." ";}
		elseif($celid){	$and .= " AND s.celid = ".$celid." ";}
		
		
		
		$_POST['tipnome'] = str_replace("'", "", $_POST['tipnome']);
		$tipnome = str_replace("'", "", $tipnome);
		
		if($_POST['tipnome']) {$and .= " AND upper(s.tipnome) like '%".strtoupper($_POST['tipnome'])."%' ";}
		else if($tipnome) {$and .= " AND upper(s.tipnome) like '%".strtoupper($tipnome)."%' ";}

	}
	
	if(!$_REQUEST['tipstatus']) $_REQUEST['tipstatus'] = 'A';
	
	if($ordid == '2' || $ordid == '13' || $ordid == '14'){
		$sql = "SELECT o.orddescricao, 
						s.tipnome,
						ce.celnome,
						(CASE WHEN s.tipstatus = 'A' THEN
							'ATIVO'
						ELSE
							'INATIVO'
						END) AS status, 
						'<center><a href=\"demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> &nbsp; <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a> &nbsp; <a href=\"javascript:Anexar(' || s.tipid || ');\"><img border=0 src=\"../imagens/anexo.gif\" /></a></center>' AS op 
			FROM demandas.tiposervico AS s 
		INNER JOIN demandas.origemdemanda AS o ON s.ordid = o.ordid
		LEFT JOIN demandas.celula AS ce ON ce.celid = s.celid
			WHERE s.tipstatus = '".$_REQUEST['tipstatus']."'
			$and
			ORDER BY 2";
		//dbg($sql);
		$cabecalho = array( "Origem da Demanda", "Serviço", "Célula", "Status","Ação");
	}else{
		$sql = "SELECT o.orddescricao, 
						s.tipnome,
						(CASE WHEN s.tipstatus = 'A' THEN
							'ATIVO'
						ELSE
							'INATIVO'
						END) AS status, 
						'<center><a href=\"demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> &nbsp; <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a> &nbsp; <a href=\"javascript:Anexar(' || s.tipid || ');\"><img border=0 src=\"../imagens/anexo.gif\" /></a></center>' AS op 
			FROM demandas.tiposervico AS s 
		INNER JOIN demandas.origemdemanda AS o ON s.ordid = o.ordid
			WHERE s.tipstatus = 'x'
			$and
			ORDER BY 2";
		//dbg($sql);
		$cabecalho = array( "Origem da Demanda","Serviço", "Status","<center>Ação</center>");
	}
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
</body>
</html>
