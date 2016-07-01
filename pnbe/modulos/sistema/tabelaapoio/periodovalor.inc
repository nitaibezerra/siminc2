<?php



function inserirValor(){
	global $db;
	
	$valor = $_POST['vaavalor'];
	$valor = str_replace('.','',$valor);
	$valor = str_replace(',','.',$valor);
	
	$dtini = $_POST['dtini'];
	$dtfim = $_POST['dtfim'];
	if($dtini && $dtfim){
		
		$dtinip = formata_data_sql($dtini);
		$dtfimp = formata_data_sql($dtfim);
		
		
		$andperiodo = "
						AND (
						     (
						     to_char(vaadatainicial, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or 
						     to_char(vaadatafinal, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or 
						     (
						     '$dtinip' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
							or 
						     '$dtfimp' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
						     )
						   )		
					  ";
	}
	
	$sql = "SELECT count(vaaid) as total FROM proinfantil.valoraluno  
			WHERE vaastatus = 'A' 
			and timid = ".$_POST['timid']." 
			and vaatipo = '".$_POST['vaatipo']."'
			$andperiodo";
			
	$total = $db->pegaUm($sql);
	
	if($total == 0){
		$sql = " INSERT INTO proinfantil.valoraluno
				 (
				 	timid, 
				 	vaadatainicial,
				 	vaadatafinal,
				 	vaavalor,
				 	vaatipo,
				 	vaastatus
				 ) VALUES (
				 	'".$_POST['timid']."',
				 	'".$dtinip."',  
				 	'".$dtfimp."',
				 	'".$valor."',
				 	'".$_POST['vaatipo']."',
				 	'A'
				 );";
		$db->executar($sql);
		$db->commit();
	}
	else{
		print '<script>
					alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
					history.back();
			  </script>';
		exit;
	}
	
}

function selecionarValor(){
	global $db;
	$sql = "SELECT 
				vaaid, 
				timid,
				vaatipo,
				to_char(vaadatainicial::timestamp,'DD/MM/YYYY') as dtini,
				to_char(vaadatafinal::timestamp,'DD/MM/YYYY') as dtfim,
				vaavalor
			FROM  
				proinfantil.valoraluno  
			WHERE 
				vaaid = '".$_SESSION['vaaid']."'
	";
	
	return $db->carregar($sql);
}


function alterarValor(){
	global $db;
	
	$valor = $_POST['vaavalor'];
	$valor = str_replace('.','',$valor);
	$valor = str_replace(',','.',$valor);
	
	$dtini = $_POST['dtini'];
	$dtfim = $_POST['dtfim'];
	if($dtini && $dtfim){
		
		$dtinip = formata_data_sql($dtini);
		$dtfimp = formata_data_sql($dtfim);
		
		
		$andperiodo = "
						AND (
						     (
						     to_char(vaadatainicial, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or 
						     to_char(vaadatafinal, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or 
						     (
						     '$dtinip' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
							or 
						     '$dtfimp' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
						     )
						   )		
					  ";
	}
	
	$sql = "SELECT count(vaaid) as total FROM proinfantil.valoraluno  
			WHERE vaastatus = 'A' 
			and timid = ".$_POST['timid']."
			and vaatipo = '".$_POST['vaatipo']."'
			AND vaaid not in (".$_POST['vaaid'].")
			$andperiodo";
	$total = $db->pegaUm($sql);
	
	if($total == 0){
		$sql = "UPDATE 
					proinfantil.valoraluno 
				SET 
					timid = '".$_POST['timid']."',
					vaadatainicial = '".$dtinip."',
					vaadatafinal = '".$dtfimp."',
					vaavalor = '".$valor."',
					vaatipo = '".$_POST['vaatipo']."'
				WHERE 
					vaaid = '".$_POST['vaaid']."';";
	
		$db->executar($sql);					
		$db->commit();
	}
	else{
		print '<script>
					alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
					history.back();
			  </script>';
		exit;
	}
		
}


function deletarValor(){
	global $db;
	
	$sql = " UPDATE proinfantil.valoraluno SET vaastatus='I' WHERE vaaid=".$_SESSION['vaaid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['timid'] && $_POST['dtini'] && !$_POST['vaaid']){
	inserirValor();
	unset($_SESSION['vaaid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A'</script>";
	//header( "Location: proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A" );
	exit();
	
}

if($_POST['timid'] && $_POST['dtini'] && $_POST['vaaid']){
	alterarValor();
	unset($_SESSION['vaaid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A'</script>";
	//header( "Location: proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A" );
	exit();
}

if($_GET['vaaid'] && $_GET['op3']){
	session_start();
	$_SESSION['vaaid'] = $_GET['vaaid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A" );
	exit();
}

if($_SESSION['vaaid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarValor();	
		unset($_SESSION['vaaid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarValor();	
		unset($_SESSION['vaaid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Cadastro de Período Valor Etapa', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="vaaid" value="<? echo $dados[0]['vaaid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr >
		<td align='right' class="subtitulodireita">Etapa / Modalidade:</td>
		<td>
			<?php
				$sql = "SELECT
						 timid AS codigo,
						 timdescricao AS descricao
						FROM
						 proinfantil.tipomodalidade 
						 WHERE timstatus = 'A' 
						ORDER BY
						 2;";
				$timid = $dados[0]['timid'] ? $dados[0]['timid'] : $_REQUEST['timid'];
				$db->monta_combo("timid",$sql,'S',"-- Selecione --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr >
		<td align='right' class="subtitulodireita">Tipo:</td>
		<td>
			<?php
				$sql = "SELECT 'I' AS codigo, 'Integral' AS descricao
						UNION
						SELECT 'P' AS codigo, 'Parcial' AS descricao";
				$vaatipo = $dados[0]['vaatipo'] ? $dados[0]['vaatipo'] : $_REQUEST['vaatipo'];
				$db->monta_combo("vaatipo",$sql,'S',"-- Selecione --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Período:</td>
	<td>
		<? $dtini = $dados[0]['dtini'];  ?>
		<?= campo_data( 'dtini', 'S', 'S', '', '' ); ?>
		&nbsp;
		à
		&nbsp;
		<? $dtfim = $dados[0]['dtfim'];  ?>
		<?= campo_data( 'dtfim', 'S', 'S', '', '' ); ?>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor</td>
	<td>
		<? 
			$vaavalor = $dados[0]['vaavalor'];
			if($vaavalor) $vaavalor = number_format($vaavalor,2,",",".");  
		?>
		<?= campo_texto( 'vaavalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, ''); ?>
	</td>
	</tr>
	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
	</table>
	<?php
	$sql = "SELECT
				'<a href=\"proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A&vaaid=' || o.vaaid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A&vaaid=' || o.vaaid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as opcao, 
				t.timdescricao,
				CASE WHEN o.vaatipo = 'I' THEN
					'Integral'
				ELSE
					'Parcial'
				END as tipo,
				to_char(o.vaadatainicial::date,'DD/MM/YYYY') as dtini,
				to_char(o.vaadatafinal::date,'DD/MM/YYYY') as dtfim,		
				o.vaavalor as valor
			FROM 
				proinfantil.valoraluno o
			INNER JOIN
				proinfantil.tipomodalidade t on t.timid = o.timid
			WHERE
				vaastatus = 'A'
		  	ORDER BY 
				 2";
	$cabecalho = array( "Opções","Etapa / Modalidade","Tipo","Data Início","Data Fim","Valor");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
	</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.timid.value == ''){
		alert ('O campo Etapa / Modalidade deve ser preenchido.');
		document.formulario.timid.focus();
		return false;
	}
	if(document.formulario.vaatipo.value == ''){
		alert ('O campo Tipo deve ser preenchido.');
		document.formulario.vaatipo.focus();
		return false;
	}
	if(document.formulario.dtini.value == ''){
		alert ('O campo Período Início deve ser preenchido.');
		document.formulario.dtini.focus();
		return false;
	}
	if(document.formulario.dtfim.value == ''){
		alert ('O campo Período Fim deve ser preenchido.');
		document.formulario.dtfim.focus();
		return false;
	}
	if(document.formulario.dtini.value != '' && document.formulario.dtfim.value != ''){ 
		if (!validaDataMaior(document.formulario.dtini, document.formulario.dtfim)){
			alert("O Período Início não pode ser maior que o Período Fim.");
			document.formulario.dtfim.focus();
			return false;
		}
	}	
	if(document.formulario.vaavalor.value == ''){
		alert ('O campo Valor deve ser preenchido.');
		document.formulario.vaavalor.focus();
		return false;
	}

	document.formulario.submit();

}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>