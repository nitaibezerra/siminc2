<?php

function inserirContrato(){
	global $db;
	
	//verifica vigencia do contrato
	$dtini = formata_data_sql($_POST['crtdtinicio']).' 00:00:00';			
	$dtfim = formata_data_sql($_POST['crtdtfim']).' 23:59:59';
	
	$sqlverifica = "SELECT COUNT(crtid) 
					FROM demandas.contrato 
					WHERE crtstatus = 'A'
					AND ordid IN(".$_POST['ordid'].")
					AND ( ('$dtini' BETWEEN crtdtinicio AND crtdtfim) OR ('$dtfim' BETWEEN crtdtinicio AND crtdtfim))";
	$existe = $db->PegaUm($sqlverifica);
	
	if($existe > 0){
		echo '<script>
				alert("Já existe um contrato nesta vigência de datas início e fim.");
				history.back();
			  </script>';
		die;		
	}
	
	$valor = str_replace(',','.',str_replace('.','',$_POST['crtvlponto']));
	$cnpj = str_replace('/','',str_replace('-','',str_replace('.','',$_POST['crtnucnpj'])));
	
	
	$sql = " INSERT INTO demandas.contrato
			 (
			 	crtnucontrato, 
			 	crtnucnpj,
			 	crtnoempresa,
			 	crtvlponto,
			 	crtdtinicio,
			 	crtdtfim,
			 	ordid,
			 	crtstatus
			 ) VALUES (
			 	'".$_POST['crtnucontrato']."',
			 	'".$cnpj."',
			 	'".$_POST['crtnoempresa']."',
			 	'".$valor."',
			 	'".formata_data_sql($_POST['crtdtinicio'])."',  
			 	'".formata_data_sql($_POST['crtdtfim'])." 23:59:59',
			 	".$_POST['ordid'].",
			 	'A'
			 );";
	$db->executar($sql);
	$db->commit();
}


function selecionarContrato(){
	global $db;
	$sql = "SELECT 
				o.crtnucontrato,
				o.crtnucnpj,
				o.crtnoempresa,
				o.crtvlponto,
				to_char(o.crtdtinicio::timestamp,'DD/MM/YYYY') as crtdtinicio,	
				to_char(o.crtdtfim::timestamp,'DD/MM/YYYY') as crtdtfim,
				o.crtid,
				o.ordid
			FROM  
				demandas.contrato o 
			WHERE 
				o.crtid = '".$_SESSION['crtid']."'";
	return $db->carregar($sql);
}



function alterarContrato(){
	global $db;
	
	//verifica vigencia do contrato
	$dtini = formata_data_sql($_POST['crtdtinicio']).' 00:00:00';			
	$dtfim = formata_data_sql($_POST['crtdtfim']).' 23:59:59';
	
	$sqlverifica = "SELECT COUNT(crtid) 
					FROM demandas.contrato 
					WHERE crtstatus = 'A'
					AND crtid NOT IN(".$_POST['crtid'].")
					AND ordid IN(".$_POST['ordid'].")
					AND ( ('$dtini' BETWEEN crtdtinicio AND crtdtfim) OR ('$dtfim' BETWEEN crtdtinicio AND crtdtfim))";
	$existe = $db->PegaUm($sqlverifica);
	
	if($existe > 0){
		echo '<script>
				alert("Já existe um contrato nesta vigência de datas início e fim.");
				history.back();
			  </script>';
		die;		
	}
	
	$valor = str_replace(',','.',str_replace('.','',$_POST['crtvlponto']));
	$cnpj = str_replace('/','',str_replace('-','',str_replace('.','',$_POST['crtnucnpj'])));
	
	$sql = "UPDATE 
				demandas.contrato 
			SET 
				crtnucontrato = '".$_POST['crtnucontrato']."',
				crtnucnpj = '".$cnpj."',
				crtnoempresa = '".$_POST['crtnoempresa']."',
				crtvlponto = '".$valor."',
				crtdtinicio = '".formata_data_sql($_POST['crtdtinicio'])."',
				crtdtfim = '".formata_data_sql($_POST['crtdtfim'])." 23:59:59',
				ordid = ".$_POST['ordid']."
			WHERE 
				crtid = '".$_POST['crtid']."';";

	$db->executar($sql);					
	$db->commit();
}



function deletarContrato(){
	global $db;
	
	$sql = "UPDATE demandas.contrato SET crtstatus='I' WHERE crtid=".$_SESSION['crtid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['crtnucontrato'] && !$_POST['crtid']){
	inserirContrato();
	unset($_SESSION['crtid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='demandas.php?modulo=sistema/apoio/contrato&acao=A'</script>";
	exit();
	
}

if($_POST['crtnucontrato'] && $_POST['crtid']){
	alterarContrato();
	unset($_SESSION['crtid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='demandas.php?modulo=sistema/apoio/contrato&acao=A'</script>";
	exit();
}

if($_GET['crtid'] && $_GET['op3']){
	session_start();
	$_SESSION['crtid'] = $_GET['crtid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: demandas.php?modulo=sistema/apoio/contrato&acao=A" );
	exit();
}

if($_SESSION['crtid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarContrato();	
		unset($_SESSION['crtid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='demandas.php?modulo=sistema/apoio/contrato&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarContrato();	
		unset($_SESSION['crtid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Contratos - Valor do Ponto', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>

<form id="formulario" name="formulario" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="crtid" value="<? echo $dados[0]['crtid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Número do Contrato:</td>
		<td>
			<? $crtnucontrato = $dados[0]['crtnucontrato'];  ?>
			<?= campo_texto( 'crtnucontrato', 'S', 'S', '', 20, 250, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">CNPJ da Empresa:</td>
		<td>
			<? $crtnucnpj = $dados[0]['crtnucnpj']; 
			   if($crtnucnpj) $crtnucnpj = formatar_cpf_cnpj($crtnucnpj);
			?>
			<?= campo_texto( 'crtnucnpj', 'S', 'S', '', 20, 18, '##.###.###/####-##', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome da Empresa:</td>
		<td>
			<? $crtnoempresa = $dados[0]['crtnoempresa'];  ?>
			<?= campo_texto( 'crtnoempresa', 'S', 'S', '', 40, 250, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Origem:</td>
		<td>
			<?
			$ordid = $dados[0]['ordid'];
			$sql = "SELECT 
					 ordid AS codigo,
					 upper(orddescricao) AS descricao
					FROM 
					 demandas.origemdemanda
					WHERE
					 ordstatus = 'A'
					ORDER BY 
					 2";
			$db->monta_combo( 'ordid', $sql, 'S', '-- Informe a Origem --','','','','','S');
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Valor do Ponto:</td>
		<td>
			<? $crtvlponto = $dados[0]['crtvlponto'];  
			   if($crtvlponto) $crtvlponto = number_format($crtvlponto,2,",",".");
			?>
			<?= campo_texto( 'crtvlponto', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data Início:</td>
		<td>
			<? $crtdtinicio = $dados[0]['crtdtinicio'];  ?>
			<?= campo_data( 'crtdtinicio', 'S', 'S', '', '' );?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data Fim:</td>
		<td>
			<? $crtdtfim = $dados[0]['crtdtfim'];  ?>
			<?= campo_data( 'crtdtfim', 'S', 'S', '', '' );?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
</table>
</form>

<?php
	$sql = "SELECT
				'<a href=\"demandas.php?modulo=sistema/apoio/contrato&acao=A&crtid=' || o.crtid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/contrato&acao=A&crtid=' || o.crtid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as acao, 
				o.crtnucontrato||' ' as nucontrato,
				substr(crtnucnpj,1,2)||'.'||substr(crtnucnpj,3,3)||'.'||substr(crtnucnpj,6,3)||'/'||substr(crtnucnpj,9,4)||'-'||substr(crtnucnpj,13,2) as cnpj,
				o.crtnoempresa,
				od.orddescricao,
				o.crtvlponto,
				to_char(o.crtdtinicio::timestamp,'DD/MM/YYYY') as dtini,	
				to_char(o.crtdtfim::timestamp,'DD/MM/YYYY') as dtfim
			FROM 
				demandas.contrato o
			INNER JOIN demandas.origemdemanda od ON od.ordid = o.ordid
			WHERE
				crtstatus = 'A'
		  	ORDER BY 
				 o.crtdtinicio";
	
	$cabecalho = array( "Ação","Nº do Contrato","CNPJ","Empresa","Origem","Valor do Ponto","Data Início","Data Fim");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>


<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.crtnucontrato.value == ''){
		alert ('O campo Número do Contrato deve ser preenchido.');
		document.formulario.crtnucontrato.focus();
		return false;
	}
	if(document.formulario.crtnucnpj.value == ''){
		alert ('O campo CNPJ deve ser preenchido.');
		document.formulario.crtnucnpj.focus();
		return false;
	}
	if(document.formulario.crtnoempresa.value == ''){
		alert ('O campo Nome da Empresa deve ser preenchido.');
		document.formulario.crtnoempresa.focus();
		return false;
	}
	if(document.formulario.ordid.value == ''){
		alert ('O campo Origem deve ser preenchido.');
		document.formulario.ordid.focus();
		return false;
	}
	if(document.formulario.crtvlponto.value == ''){
		alert ('O campo Valor do Ponto deve ser preenchido.');
		document.formulario.crtvlponto.focus();
		return false;
	}
	if(document.formulario.crtdtinicio.value == ''){
		alert ('O campo Data Início deve ser preenchido.');
		document.formulario.crtdtinicio.focus();
		return false;
	}
	if(document.formulario.crtdtfim.value == ''){
		alert ('O campo Data Fim deve ser preenchido.');
		document.formulario.crtdtfim.focus();
		return false;
	}
	
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>