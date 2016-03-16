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
				($_REQUEST['tsptempoclassif'][$n] == '')? $_REQUEST['tsptempoclassif'][$n] = '0:00' : $_REQUEST['tsptempoclassif'][$n] = $_REQUEST['tsptempoclassif'][$n].':00';
				//coloca os dois pontos caso não houver
				$pos = strpos($_REQUEST['tsptempo'][$n], ':');
				if($pos===false){
					$_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n].":00";
				}
				
				$posc = strpos($_REQUEST['tsptempoclassif'][$n], ':');
				if($posc===false){
					$_REQUEST['tsptempoclassif'][$n] = $_REQUEST['tsptempoclassif'][$n].":00";
				}
				
				/*
				$sql = "INSERT INTO demandas.tiposervicoprioridade 
				(priid, tipid, tsptempoclassif, tsptempo, tsppontuacao) values 
				( '$priid', '$tipid' ,'".$_REQUEST['tsptempoclassif'][$n]."' ,'".$_REQUEST['tsptempo'][$n]."',
				'".$_REQUEST['pontuacao'][$n]."');";
				$db->executar($sql);
				*/
				
				if($_REQUEST['tsptempo'][$n]){
					$tspAtendimento = explode(":",$_REQUEST['tsptempo'][$n]);
				}
				
				
				//nova regra para aceitar tsptempo maior que 24h				
				$sql = "INSERT INTO demandas.tiposervicoprioridade 
				(priid, tipid, tsptempoclassif, tsphora, tspminuto, tsppontuacao) values 
				( '$priid', '$tipid' ,'".$_REQUEST['tsptempoclassif'][$n]."' ,'".$tspAtendimento[0]."','".$tspAtendimento[1]."',
				'". formata_valor_sql($_REQUEST['pontuacao'][$n]) ."');";
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
				($_REQUEST['tsptempoclassif'][$n] == '')? $_REQUEST['tsptempoclassif'][$n] = '0:00' : $_REQUEST['tsptempoclassif'][$n] = $_REQUEST['tsptempoclassif'][$n];
				//coloca os dois pontos caso não houver
				$pos = strpos($_REQUEST['tsptempo'][$n], ':');
				if($pos===false){
					$_REQUEST['tsptempo'][$n] = $_REQUEST['tsptempo'][$n].":00";
				} 

				$posc = strpos($_REQUEST['tsptempoclassif'][$n], ':');
				if($posc===false){
					$_REQUEST['tsptempoclassif'][$n] = $_REQUEST['tsptempoclassif'][$n].":00";
				} 
				
				if($_REQUEST['tsptempo'][$n]){
					$tspAtendimento = explode(":",$_REQUEST['tsptempo'][$n]);
				}
							
				$sql = "INSERT INTO demandas.tiposervicoprioridade 
				(priid, tipid, tsptempoclassif, tsphora, tspminuto, tsppontuacao) values 
				( '$priid','".$_POST['tipid']."','".$_REQUEST['tsptempoclassif'][$n]."','".$tspAtendimento[0]."', '".$tspAtendimento[1]."',
				'". formata_valor_sql($_REQUEST['pontuacao'][$n]) ."');";
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
					(priid, tipid, tsptempoclassif, tsphora, tspminuto, tsppontuacao) values 
					( '{$i['priid']}','".$_SESSION['tipid']."','0:00','0','0','0');";
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


monta_titulo( 'Catálago de Serviço', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );


$permissaoSalvar = true;

//Empresa  TYPE MÁQUINAS E SERVIÇOS LTDA.
$perfil = arrayPerfil();
if( in_array(DEMANDA_PERFIL_EMPRESA_TYPE,$perfil) ){
	$permissaoSalvar = false;
}

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
	if(document.formCatalogo.ordid.value == '2' || document.formCatalogo.ordid.value == '13' || document.formCatalogo.ordid.value == '14' || document.formCatalogo.ordid.value == '23' || document.formCatalogo.ordid.value == '24'){
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
    	if(document.formCatalogo.tsptempo[i].value == '' || document.formCatalogo.tsptempoclassif[i].value == '' || document.formCatalogo.pontuacao[i].value == ''){
    		tsptempo = true;
    	}
    }
    if(tsptempo == true){
    	alert('Informe todos os campos do Tempo de Serviço (Classificação e Atendimento) e Pontuação!');
    	return;
    }

	
	//valida hora
    for (i=0;i<loops;i++) {
    	if(document.formCatalogo.tsptempoclassif[i].value != ''){
    		if(document.formCatalogo.tsptempoclassif[i].value.substr(0,2) > 23 || document.formCatalogo.tsptempoclassif[i].value.substr(3,2) > 59){
    			alert('Tempo de serviço (Classificação) inválido!');
    			document.formCatalogo.tsptempoclassif[i].focus();
    			return;
    			break;
    		}
    	}

    	if(document.formCatalogo.tsptempo[i].value != ''){
    		var valor = document.formCatalogo.tsptempo[i].value;
    		var retorno = valor.split(":");
    		//if(document.formCatalogo.tsptempo[i].value.substr(0,2) > 23 || document.formCatalogo.tsptempo[i].value.substr(3,2) > 59){
    		//if(document.formCatalogo.tsptempo[i].value.length < 5 || document.formCatalogo.tsptempo[i].value.substr(3,2) > 59){
    		if(document.formCatalogo.tsptempo[i].value.length < 5 || retorno[1] > 59){
    			alert('Tempo de serviço (Atendimento) inválido!');
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
	
	if(ordid == 2 || ordid == 13 || ordid == 14 || ordid == 23 || ordid == 24){ //2=REDES - 13=Gestão Documentos CGI
	
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


function Anexar(tipid, ordid){

	w = window.open('?modulo=sistema/apoio/catalogoAnexo&acao=A&tipid='+tipid+'&ordid='+ordid,'Anexo','scrollbars=yes,resizable=yes,location=no,toolbar=yes,menubar=no,width=780,height=400,left=250,top=125'); 
	w.focus();	
	
}

</script>

</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formCatalogo" name="formCatalogo" action="" method="post">

<input type="hidden" name="pesquisa" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="tipid" value="<? print $dados[0]['tipid']; ?>"  />
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Status:</td>
		<td>
			<input type='radio' name='tipstatus' value='A' <?if($dados[0]['tipstatus'] == 'A' || $dados[0]['tipstatus'] == '' || $_REQUEST['tipstatus'] == 'A') echo "checked" ?>> Ativo
			&nbsp;&nbsp;
			<input type='radio' name='tipstatus' value='I' <?if($dados[0]['tipstatus'] == 'I' || $_REQUEST['tipstatus'] == 'I') echo "checked" ?>> Inativo
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Origem da demanda:</td>
		<td>
			<?php
				$sql = "SELECT
						 ordid AS codigo,
						 orddescricao AS descricao
						FROM
						 demandas.origemdemanda WHERE ordstatus = 'A' 
						ORDER BY
						 orddescricao;";
				$ordid = $dados[0]['ordid'] ? $dados[0]['ordid'] : $_REQUEST['ordid'];
				$db->monta_combo("ordid",$sql,'S',"-- Informe a Origem da demanda --",'filtraCelula','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<?$displayCel = 'none';
	  if($ordid==2 || $ordid==13 || $ordid==14 || $ordid==23 || $ordid==24) $displayCel = '';
	?>
	<tr id="tr_celula" style="display:<? echo $displayCel;  ?>">
		<td align='right' class="SubTituloDireita" >
		Célula:
		</td>
		<td id="td_celula">
		<?php
			if($ordid==2 || $ordid==13 || $ordid==14 || $ordid==23 || $ordid==24){
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
				$celid = $dados[0]['celid'] ? $dados[0]['celid'] : $_REQUEST['celid'];
				$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', '', '');
				echo obrigatorio(); 
			}
		?>
		 </td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Nome do tipo de serviço:</td>
		<td><? $tipnome = $dados[0]['tipnome']  ? $dados[0]['tipnome'] : $_POST['tipnome'];; ?>
		<?= campo_texto( 'tipnome', 'S', 'S', '', 60, 150, '', ''); ?></td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Descrição do tipo de serviço:</td>
		<td><? $tipdescricao = $dados[0]['tipdescricao']; ?>
		<?= campo_textarea('tipdescricao', 'S', 'S', '', 75, 5, 4000); ?></td>	
	</tr>	
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Serviço é quantificável:</td>
		<td><? $tipquantidade = $dados[0]['tipquantidade']  ? $dados[0]['tipquantidade'] : $_POST['tipquantidade'];?>
			<? if($_POST['pesquisa']=='1') $tipquantidade = FALSE; ?>
			<input type="radio" name="tipquantidade" class="normal" value="TRUE" <?if($tipquantidade == true) echo "checked";?>> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="tipquantidade" class="normal" value="FALSE" <?if($tipquantidade == false || $tipquantidade == 'f') echo "checked";?>> Não
		</td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Serviço possui nº de serie:</td>
		<td><? $tipnumserie = $dados[0]['tipnumserie']  ? $dados[0]['tipnumserie'] : $_POST['tipnumserie'];?>
			<? if($_POST['pesquisa']=='1') $tipnumserie = FALSE; ?>
			<input type="radio" name="tipnumserie" class="normal" value="TRUE" <?if($tipnumserie == true) echo "checked";?>> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="tipnumserie" class="normal" value="FALSE" <?if($tipnumserie == false || $tipnumserie == 'f') echo "checked";?>> Não
		</td>
	</tr>			
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Serviço possui nº de patrimonio:</td>
		<td><? $tipnumpatr = $dados[0]['tipnumpatr']  ? $dados[0]['tipnumpatr'] : $_POST['tipnumpatr'];?>
			<? if($_POST['pesquisa']=='1') $tipnumpatr = FALSE; ?>
			<input type="radio" name="tipnumpatr" class="normal" value="TRUE" <?if($tipnumpatr == true) echo "checked";?>> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="tipnumpatr" class="normal" value="FALSE" <?if($tipnumpatr == FALSE || $tipnumpatr == 'f') echo "checked";?>> Não
		</td>
	</tr>			
	<tr>
		<td class="SubTituloDireita">Nível de serviço:</td>
		<td>
			<?php
				$sql = "SELECT
						 niaid AS codigo,
						 niadescricao AS descricao
						FROM
						 demandas.nivelatendimento
						WHERE
						 niastatus = 'A' 
						ORDER BY
						 niadescricao;";
				$niaid = $dados[0]['niaid'];
				$db->monta_combo("niaid",$sql,'S',"-- Informe o nível de serviço --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Dificuldade:</td>
		<td>
			<?php
				$sql = "SELECT
						 grsid AS codigo,
						 grsdescricao AS descricao
						FROM
						 demandas.grauseveridade
						WHERE
						 grsstatus = 'A' 
						ORDER BY
						 grsid;";
				$grsid = $dados[0]['grsid'];
				$db->monta_combo("grsid",$sql,'S',"-- Informe a dificuldade --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>	
	<tr>
		<td colspan="2">
		<?php
			$sql_novo = "SELECT
							 pridsc,
							 '<input type=\"hidden\" name=\"priid[]\" value=\"'|| priid ||'\" /><input onkeypress=\"return somenteNumeros(event);\" type=\'text\' id=\"tsptempo\" style=\"width:80px;\";  maxlength=\"6\" name=\"tsptempo[]\" style=\"text-align: left; width: 10ex;\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###:##\',this.value);\"  class=\"normal\" >' AS tsptempo,
							 '<input onkeypress=\"return somenteNumeros(event);\" type=\'text\' id=\"tsptempoclassif\" style=\"width:80px;\";  maxlength=\"5\" name=\"tsptempoclassif[]\" style=\"text-align: left; width: 10ex;\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'##:##\',this.value);\"  class=\"normal\" >' AS tsptempoclassif,
							 '<input onkeypress=\"return somenteNumeros(event);\" type=\'text\' id=\"pontuacao\" style=\"width:80px;\";  maxlength=\"10\" name=\"pontuacao[]\" style=\"text-align: left; width: 10ex;\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###,##\',this.value);\"  class=\"normal\" >' AS pontuacao
						 FROM
							 demandas.prioridade
						 WHERE
							 pristatus = 'A' 
						 order by priid";	
		
			$sql_update = "
							SELECT 
								p.pridsc, 
								'<input type=\"hidden\" name=\"priid[]\" value=\"'|| tp.priid || '\" /><input onkeypress=\"return somenteNumeros(event);\" type=\'text\' id=\"tsptempo\" style=\"width:80px;\";  maxlength=\"6\" value=\"' || lpad(CAST (tp.tsphora AS text),3,'0') ||':'|| lpad(CAST (tp.tspminuto AS text),2,'0')  ||'\" name=\"tsptempo[]\" style=\"text-align: left; width: 10ex;\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###:##\',this.value);\" class=\"normal\" >' AS tsptempo,
								'<input onkeypress=\"return somenteNumeros(event);\" type=\'text\' id=\"tsptempoclassif\" style=\"width:80px;\";  maxlength=\"5\" value=\"' || SUBSTR (CAST (tp.tsptempoclassif AS text), 1, 5) ||'\" name=\"tsptempoclassif[]\" style=\"text-align: left; width: 10ex;\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'##:##\',this.value);\" class=\"normal\" >'::text AS tsptempoclassif,
							 	CASE WHEN tp.tsppontuacao > 0 THEN
									'<input type=\'text\' id=\"pontuacao\" size=\"12\" maxlength=\"10\" value=\"' || trim(to_char(tp.tsppontuacao,'999g999g999d99')) || '\" name=\"pontuacao[]\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###,##\',this.value);\" class=\"normal\" >'::text 
								ELSE
									'<input type=\'text\' id=\"pontuacao\" size=\"12\" maxlength=\"10\" value=\"\" name=\"pontuacao[]\"  onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###,##\',this.value);\" class=\"normal\" >'::text
								END AS pontuacao
							FROM demandas.tiposervicoprioridade AS tp 
							LEFT JOIN demandas.prioridade AS p ON tp.priid = p.priid 
							WHERE tipid = {$dados[0]['tipid']} 
							--and tp.tspstatus = 'A'
							order by tp.priid";			
			
			(!$dados[0]['tipid'])? $sql = $sql_novo : $sql = $sql_update;
			//$cabecalho = array( "<div align=left><b>Grau de severidade</b></div>", "<div align=left><b>Tempo de serviço</b></div>", "<div align=left><b>Severidade padrão</b></div>", "<div align=left><b>Situação da severidade</b></div>");
			//$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '');
			$GrauServ = $db->carregar($sql);
		?>	
			<table width="45%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
				<tr>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3;">
						<b>Prioridade</b>
					</td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3;">
						<b>Tempo de serviço (Classificação)</b>
					</td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3;">
						<b>Tempo de serviço (Atendimento)</b>
					</td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3;">
						<b>Pontuação</b>
					</td>
				</tr>
				<?foreach($GrauServ as $GrauServ){ ?>
				<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
					<td><?=$GrauServ['pridsc']?></td>
					<td><?=$GrauServ['tsptempoclassif']?></td>
					<td><?=$GrauServ['tsptempo']?></td>
					<td><?=$GrauServ['pontuacao']?></td>
				</tr>
				<?}?>
			</table>				

		</td>
	</tr>				
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
			<?php if($permissaoSalvar){?>
	    	<input type='button' onclick="validaForm();" class='botao' value='Salvar' name='cadastrar' />
	    	<?php }?>
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
	
	$op = "'<center><a href=\"demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> &nbsp; <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a> &nbsp; <a href=\"javascript:Anexar(' || s.tipid || ',' || s.ordid || ');\"><img border=0 src=\"../imagens/anexo.gif\" /></a></center>'";
	if(!$permissaoSalvar) $op = "'<center><a href=\"demandas.php?modulo=sistema/apoio/catalagoServico&acao=A&tipid=' || s.tipid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> &nbsp; <a href=\"javascript:Anexar(' || s.tipid || ',' || s.ordid || ');\"><img border=0 src=\"../imagens/anexo.gif\" /></a></center>'";
	
	if($ordid == '2' || $ordid == '13' || $ordid == '14' || $ordid == '23' || $ordid == '24'){
		$sql = "SELECT o.orddescricao, 
						s.tipnome,
						ce.celnome,
						(CASE WHEN s.tipstatus = 'A' THEN
							'ATIVO'
						ELSE
							'INATIVO'
						END) AS status, 
						$op AS op 
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
						$op AS op 
			FROM demandas.tiposervico AS s 
		INNER JOIN demandas.origemdemanda AS o ON s.ordid = o.ordid
			WHERE s.tipstatus = '".$_REQUEST['tipstatus']."'
			$and
			ORDER BY 2";
		//dbg($sql);
		$cabecalho = array( "Origem da Demanda","Serviço", "Status","<center>Ação</center>");
	}
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
</body>
</html>
