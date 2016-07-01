<?php

function inserirContrato(){
	global $db;
	
	//verifica vigencia do contrato
	$peremesinicioref 	= (int) $_POST['peremesinicioref'];
	$pereanoinicioref 	= (int) $_POST['pereanoinicioref'];
	
	$peremesfimref 	= (int) $_POST['peremesfimref'];
	$pereanofimref 	= (int) $_POST['pereanofimref'];
		
	$dtini = formata_data_sql($_POST['perdtinirenovacao']).' 00:00:00';			
	$dtfim = formata_data_sql($_POST['perdtfimrenovacao']).' 23:59:59';
	
	$sqlverifica = "SELECT COUNT(preid) 
					FROM fiesabatimento.parametrosrenovacao 
					WHERE prestatus = 'A'
					AND ( ('$dtini' BETWEEN perdtinirenovacao AND perdtfimrenovacao) OR ('$dtfim' BETWEEN perdtinirenovacao AND perdtfimrenovacao))";
	$existe = $db->PegaUm($sqlverifica);
	
	if($existe > 0){
		echo '<script>
				alert("Já existe uma renovação nesta vigência de datas de início e Término.");
				history.back();
			  </script>';
		die;		
	}
	
	$sql = " INSERT INTO fiesabatimento.parametrosrenovacao
			 (
			 	peremesinicioref,
			 	pereanoinicioref,
			 	peremesfimref,
			 	pereanofimref,
			 	perdtinirenovacao,
			 	perdtfimrenovacao,
			 	perusucpf,
			 	prestatus
			 ) VALUES (
			 	$peremesinicioref,
				$pereanoinicioref,
				$peremesfimref,
				$pereanofimref,
			 	'".formata_data_sql($_POST['perdtinirenovacao'])."',  
			 	'".formata_data_sql($_POST['perdtfimrenovacao'])."',
			 	'".$_SESSION['usucpf']."',
			 	'A'
			 );";
	$db->executar($sql);
	$db->commit();
}


function selecionarContrato(){
	global $db;
	$sql = "SELECT 
				peremesinicioref,
			 	pereanoinicioref,
			 	peremesfimref,
			 	pereanofimref,
				to_char(o.perdtinirenovacao::timestamp,'DD/MM/YYYY') as perdtinirenovacao,	
				to_char(o.perdtfimrenovacao::timestamp,'DD/MM/YYYY') as perdtfimrenovacao,
				o.preid
			FROM  
				fiesabatimento.parametrosrenovacao o 
			WHERE 
				o.preid = '".$_SESSION['preid']."'";
	return $db->carregar($sql);
}



function alterarContrato(){
	global $db;
	
	//verifica vigencia do contrato
	$peremesinicioref 	= (int) $_POST['peremesinicioref'];
	$pereanoinicioref 	= (int) $_POST['pereanoinicioref'];
	
	$peremesfimref 	= (int) $_POST['peremesfimref'];
	$pereanofimref 	= (int) $_POST['pereanofimref'];
	
	$dtini = formata_data_sql($_POST['perdtinirenovacao']).' 00:00:00';			
	$dtfim = formata_data_sql($_POST['perdtfimrenovacao']).' 23:59:59';
	
	$sqlverifica = "SELECT COUNT(preid) 
					FROM fiesabatimento.parametrosrenovacao 
					WHERE prestatus = 'A'
					AND preid NOT IN(".$_POST['preid'].")
					AND ( ('$dtini' BETWEEN perdtinirenovacao AND perdtfimrenovacao) OR ('$dtfim' BETWEEN perdtinirenovacao AND perdtfimrenovacao))";
	$existe = $db->PegaUm($sqlverifica);
	
	if($existe > 0){
		echo '<script>
				alert("Já existe uma renovação nesta vigência de datas de início e Término.");
				history.back();
			  </script>';
		die;		
	}
	
	$sql = "UPDATE 
				fiesabatimento.parametrosrenovacao 
			SET 
				peremesinicioref = $peremesinicioref,
			 	pereanoinicioref = $pereanoinicioref,
			 	peremesfimref = $peremesfimref,
			 	pereanofimref = $pereanofimref,
				perdtinirenovacao = '".formata_data_sql($_POST['perdtinirenovacao'])."',
				perdtfimrenovacao = '".formata_data_sql($_POST['perdtfimrenovacao'])."',
				perusucpf = '".$_SESSION['usucpf']."'
			WHERE 
				preid = '".$_POST['preid']."';";

	$db->executar($sql);					
	$db->commit();
}



function deletarContrato(){
	global $db;
	
	$sql = "UPDATE fiesabatimento.parametrosrenovacao SET prestatus='I' WHERE preid=".$_SESSION['preid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['perdtinirenovacao'] && !$_POST['preid']){
	inserirContrato();
	unset($_SESSION['preid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A'</script>";
	exit();
	
}

if($_POST['perdtinirenovacao'] && $_POST['preid']){
	alterarContrato();
	unset($_SESSION['preid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A'</script>";
	exit();
}

if($_GET['preid'] && $_GET['op3']){
	session_start();
	$_SESSION['preid'] = $_GET['preid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A" );
	exit();
}

if($_SESSION['preid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarContrato();	
		unset($_SESSION['preid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarContrato();	
		unset($_SESSION['preid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Parâmetros de Renovação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>

<form id="formulario" name="formulario" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="preid" value="<? echo $dados[0]['preid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?if($dados[0]['preid']){ ?>
		<tr>
			<td align='right' class="SubTituloDireita">Código:</td>
			<td>
				<? $codigo = $dados[0]['preid'];  ?>
				<?= campo_texto( 'codigo', 'N', 'N', '', 5, 20, '', '','','','','','');?>
			</td>
		</tr>
	<?}?>
	<tr>
		<td class="SubTituloDireita">Período de Renovação do Abatimento:</td>
		<td>	
			<?
				$sql = "select '01' as codigo, 'Janeiro' as descricao
	  	   						union all
	  	   						select '02' as codigo, 'Fevereiro' as descricao
	  	   						union all
	  	   						select '03' as codigo, 'Março' as descricao
	  	   						union all
	  	   						select '04' as codigo, 'Abril' as descricao
	  	   						union all
	  	   						select '05' as codigo, 'Maio' as descricao
	  	   						union all
	  	   						select '06' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '07' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '08' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '09' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";
				
				$db->monta_combo( 'peremesinicioref', $sql, 'S', '--','','','','','N','peremesinicioref',false,$dados[0]['peremesinicioref'] );
			?>
			 / 
			<? 
				$anoini = (int) 2013;
				$anofim = (int) date("Y");
				$sqlano = "";
				for($i=$anoini; $i<=$anofim; $i++){
					$sqlano .= " select '$i' as codigo, '$i' as descricao ";
					if($i != $anofim) $sqlano .= " union ";
				}
				$sqlano .= "order by 1";
				$db->monta_combo( 'pereanoinicioref', $sqlano, 'S', '--','','','','','S','pereanoinicioref',false,$dados[0]['pereanoinicioref'] );									
			?> 
			&nbsp;&nbsp;
			à
			&nbsp;&nbsp;
			<?
				$sql = "select '01' as codigo, 'Janeiro' as descricao
  	   						union all
  	   						select '02' as codigo, 'Fevereiro' as descricao
  	   						union all
  	   						select '03' as codigo, 'Março' as descricao
  	   						union all
  	   						select '04' as codigo, 'Abril' as descricao
  	   						union all
  	   						select '05' as codigo, 'Maio' as descricao
  	   						union all
  	   						select '06' as codigo, 'Junho' as descricao
  	   						union all
  	   						select '07' as codigo, 'Julho' as descricao
  	   						union all
  	   						select '08' as codigo, 'Agosto' as descricao
  	   						union all
  	   						select '09' as codigo, 'Setembro' as descricao
  	   						union all
  	   						select '10' as codigo, 'Outubro' as descricao
  	   						union all
  	   						select '11' as codigo, 'Novembro' as descricao
  	   						union all
  	   						select '12' as codigo, 'Dezembro' as descricao
  	   						";
				$db->monta_combo( 'peremesfimref', $sql, 'S', '--','','','','','N','peremesfimref',false,$dados[0]['peremesfimref'] );
			?>
			 / 
			<? 
				$anoini = (int) 2013;
				$anofim = (int) date("Y");
				$sqlano = "";	
				for($i=$anoini; $i<=$anofim; $i++){
					$sqlano .= " select '$i' as codigo, '$i' as descricao ";
					if($i != $anofim) $sqlano .= " union ";
				}
				$sqlano .= "order by 1";
				$db->monta_combo( 'pereanofimref', $sqlano, 'S', '--','','','','','S','pereanofimref',false,$dados[0]['pereanofimref'] );								
			?> 
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Data de Início para Renovar o Abatimento:</td>
		<td>
			<? $perdtinirenovacao = $dados[0]['perdtinirenovacao'];  ?>
			<?= campo_data( 'perdtinirenovacao', 'S', 'S', '', '' );?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data de Término para Renovar o Abatimento:</td>
		<td>
			<? $perdtfimrenovacao = $dados[0]['perdtfimrenovacao'];  ?>
			<?= campo_data( 'perdtfimrenovacao', 'S', 'S', '', '' );?>
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
				'<a href=\"fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A&preid=' || o.preid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a>
				 <!-- 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'fiesabatimento.php?modulo=sistema/renovacao_fies1&acao=A&preid=' || o.preid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>
				 -->' as acao, 
				o.preid||' ' as cod,
				to_char(o.perdtinirenovacao::timestamp,'DD/MM/YYYY') as dtini,	
				to_char(o.perdtfimrenovacao::timestamp,'DD/MM/YYYY') as dtfim,
				peremesinicioref||'/'||pereanoinicioref ||' à '|| peremesfimref||'/'||pereanofimref as periodo
			FROM 
				fiesabatimento.parametrosrenovacao o
			WHERE
				prestatus = 'A'
		  	ORDER BY 
				 o.perdtinirenovacao";
	
	$cabecalho = array( "Ação","Código","Data de Início","Data de Término", "Período");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>


<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.peremesinicioref.value == '' ||
		document.formulario.pereanoinicioref.value == '' ||
		document.formulario.peremesfimref.value == '' ||
		document.formulario.pereanofimref.value == ''){
		alert ('O campo Período de Renovação do Abatimento deve ser preenchido.');
		//document.formulario.peremesinicioref.focus();
		return false;
	}
	if(document.formulario.perdtinirenovacao.value == ''){
		alert ('O campo Data de Início deve ser preenchido.');
		document.formulario.perdtinirenovacao.focus();
		return false;
	}
	if(document.formulario.perdtfimrenovacao.value == ''){
		alert ('O campo Data de Término deve ser preenchido.');
		document.formulario.perdtfimrenovacao.focus();
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