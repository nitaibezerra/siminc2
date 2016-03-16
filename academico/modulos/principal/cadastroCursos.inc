<?php

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 

$curso = new cursos();

if($_REQUEST['hidAcao'] == "insert"){
	$ret = $curso->insereCurso($_REQUEST);
		
	if($ret == "1"){
		echo '<script>
				 alert("Operação realizada com sucesso!");
				 window.location.href = "academico.php?modulo=principal/listaCursos&acao=A&entid='.$entidcampus.'";
			  </script>
			 ';
	}else{
		echo '<script>
				 alert("Operação não realizada!");
			  </script>
			 ';
	}
}

if($_POST['hidAcao'] == "edit"){
	$ret = $curso->alteraCurso($_POST);
	if($ret == "1"){
		echo '<script>
				 alert("Operação realizada com sucesso!");
				 window.location.href = "academico.php?modulo=principal/listaCursos&acao=A&entid='.$entidcampus.'";
			  </script>
			 ';
	}else{
		echo '<script>
				 alert("Operação não realizada!");
			  </script>
			 ';
	}
}

if($_REQUEST['tipoCurso'] == 1){
	$labelCodigo = "Código INEP";
	$nomeCodigo = "curcodinep";
}else{
	$labelCodigo = "Código CAPES";
	$nomeCodigo = "curcodcapes";	
}

if ( $_REQUEST['curid'] || !empty($_SESSION["academico"]["curid"]) ){
	
	$curid = $_SESSION["academico"]["curid"] ? $_SESSION["academico"]["curid"] : $_REQUEST['curid'];
	$_SESSION["academico"]["curid"] = $curid;
	
	$arDados = $curso->verificaCurso( $curid );
	
	if($arDados){
		$curcodinep  	= $arDados['curcodinep']; 
		$curcodcapes 	= $arDados['curcodcapes'];
		$curdsc 	 	= $arDados['curdsc'];
		$stcid 		 	= $arDados['stcid'];
		$pgcid 		 	= $arDados['pgcid'];
		$curobs 	 	= $arDados['curobs'];
		$turidprevisto  = $arDados['turidprevisto'];
		$turidexecutado = $arDados['turidexecutado'];
		$curinicioprev  = $arDados['curinicioprev'];
		$curinicioexec  = $arDados['curinicioexec'];
	}else{
		$curcodinep  	= ""; 
		$curcodcapes 	= "";
		$curdsc 	 	= "";
		$stcid 		 	= "";
		$pgcid 		 	= "";
		$curobs 	 	= "";
		$turidprevisto  = "";
		$turidexecutado = "";
		$curinicioprev  = "";
		$curinicioexec  = "";
	}
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Cadastro de Cursos", "");

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 

$existecampus = academico_existecampus( $entidcampus );

if( !$existecampus ){
	echo "<script>
			alert('O Campus informado não existe!');
			history.back(-1);
		  </script>";
	die;
}
// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus);

if($entidcampus){
	$_SESSION['academico']['entidadenivel'] = 'campus';
}

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);
?>
<body>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script	src="/includes/calendario.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" value="<?=$entidcampus; ?>" name="entidcampus" id="entidcampus">
<input type="hidden" value="<?=$_REQUEST['tipoCurso'] ?>" name="tpcid" id="tpcid">
<input type="hidden" value="" name="hidAcao" id="hidAcao">
<input type="hidden" value="<?=$curid ?>" name="curid" id="curid">

<table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th style="text-align: left;" colspan="2">Dados do Curso</th>
	</tr>
	<tr>
		<td class="SubTituloDireita" width='250px;'><?=$labelCodigo; ?>:</td>
		<td><?=campo_texto( $nomeCodigo, 'N', 'S', '', 50, 11, '', '','','','','id="'.$nomeCodigo.'"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><?=campo_texto( 'curdsc', 'S', 'S', 'Nome', 77, 150, '', '','','','','id="curdsc"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Situação do Curso:</td>
		<?
			$sql = "SELECT stcid as codigo, 
						stcdsc as descricao 
					FROM 
						academico.situacaocurso
					order by stcdsc";
		?>
		<td><?=$db->monta_combo("stcid",$sql, 'S','-- Selecione --', '', '', '', 250, 'S','stcid', '', '', 'Situação do Curso'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<?
			$sql = "SELECT 
					  pgcid as codigo,
					  pgcdsc as descricao
					FROM 
					  academico.programacurso
					order by pgcdsc";
		?>
		<td><?=$db->monta_combo("pgcid",$sql, 'S','-- Selecione --', '', '', '', 250, 'N','pgcid'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Observação:</td>
		<td><?=campo_textarea('curobs','N', 'S', '', 80, 05, 200,''); ?></td>
	</tr>
	<tr>
		<th style="text-align: left;" colspan="2">Execução do Curso</th>
	</tr>
	<tr>
		<td class="SubTituloDireita">Dados:</td>
		<td colspan="2">
			<table id="tblDados" class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
				<tr>
					<th colspan="2">TURNO</th>
					<th colspan="2">INÍCIO</th>
				</tr>
				<tr>
					<td>Previsto</td>
					<td>Executado</td>
					<td>Previsto</td>
					<td>Executado</td>
				</tr>
				<tr>
					<?
					 $sql = "SELECT 
							  turid as codigo,
							  (CASE WHEN turdsc = 'D' THEN 'Diurno'
							  	   WHEN turdsc = 'N' THEN 'Noturno' END) as descricao
							FROM 
							  academico.turno";
					?>
					<td>
						<?php 
							if($turidprevisto){
								echo $db->monta_combo("turidprevisto",$sql, 'N','-- Selecione --', '', '', '', 100, 'N','turidprevisto');
								echo '<input type="hidden" id="turidprevisto" name="turidprevisto" value="'.$turidprevisto.'">';							
							}else{
								echo $db->monta_combo("turidprevisto",$sql, 'S','-- Selecione --', '', '', '', 100, 'N','turidprevisto');	
							}
						?>
					</td>
					<td>
						<?php 
							if($turidexecutado){
								echo $db->monta_combo("turidexecutado",$sql, 'N','-- Selecione --', '', '', '', 100, 'N','turidexecutado');
								echo '<input type="hidden" id="turidexecutado" name="turidexecutado" value="'.$turidexecutado.'">';							
							}else{
								echo $db->monta_combo("turidexecutado",$sql, 'S','-- Selecione --', '', '', '', 100, 'N','turidexecutado');	
							}
						?>
					</td>	
					<td align="center">
						<?= campo_texto( 'curinicioprev', 'N', 'S', '', 4, 6, '', '','','','','id="curinicioprev"'); ?>
					</td>
					<td align="center">
						<?= campo_texto( 'curinicioexec', 'N', 'S', '', 4, 6, '', '','','','','id="curinicioprev"'); ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<th colspan="2" style="text-align: left;">
			<input style="cursor: pointer;"  type="button" value="Gravar" name="btnGravar" onclick="gravar();">
			<input style="cursor: pointer;"  type="button" value="Voltar" name="btnVoltar" onclick="voltar();">
		</th>
	</tr>
</table>
</form>
<script type="text/javascript">
	function voltar(){
		window.location.href = "academico.php?modulo=principal/listaCursos&acao=A&entid="+$('entidcampus').value;
	}
	function gravar(){
		if($('curid').value != ""){
			$('hidAcao').value = "edit";
		}else{
			$('hidAcao').value = "insert";
		}
		validaForm('formulario', 'curdsc#stcid', 'texto#select', 'true' );
	}
</script>
</body>