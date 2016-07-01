<?php
include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$visualiza = $_REQUEST['visualiza'];
$habil = $visualiza ? 'N' : 'S';

if( $visualiza ){
	$titulo = 'Visualizar Curso';
} elseif( $_REQUEST['curid'] ){
	$titulo = 'Alterar Curso';
} else {
	$titulo = 'Cadastrar Curso';
}


$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo, '' );


if( $_POST ){

	if( $_POST['curid'] ){
		$curid = $_POST['curid'];
		$sql = "UPDATE siscap.curso SET
			            areid = ".$_POST['areid'].",
			            curdsc = '".trim($_POST['curdsc'])."',
			            curobj = '".trim($_POST['curobj'])."',
			            curcont = '".trim($_POST['curcont'])."',
			            curinfo = '".trim($_POST['curinfo'])."'
			    WHERE
			    	curid = ".$_POST['curid'];

		$db->executar( $sql );
	} elseif($_POST['curdsc']) {
		$sql = "SELECT count(curid) FROM siscap.curso WHERE curdsc = '".trim($_POST['curdsc'])."' and areid = {$_POST['areid']}";
		$total = $db->pegaUm( $sql );

		if( $total == 0 ){
			$sql = "INSERT INTO siscap.curso(
				            areid, curdsc, curobj, curcont, curpublico, curcarga,
				            curqtdpart, curinfo)
				    VALUES ( ".$_POST['areid'].", '".trim($_POST['curdsc'])."', '".trim($_POST['curobj'])."', '".trim($_POST['curcont'])."', '1', '1',
				            '1', '".trim($_POST['curinfo'])."') RETURNING curid";
			$curid = $db->pegaUm( $sql );
		} else {
			echo "<script>
					document.getElementById('aguarde').style.visibility = 'hidden';
					document.getElementById('aguarde').style.display = 'none';
					alert('Já existe um curso cadastrado com este nome para está área!');
					//window.location.href='/siscap/siscap.php?modulo=principal/gerenciaCurso&acao=A';
				</script>";
			//exit();
		}
	}
	if( $curid ){
		$db->commit();
		echo "<script>
				document.getElementById('aguarde').style.visibility = 'hidden';
				document.getElementById('aguarde').style.display = 'none';
				alert('Operação realizada com sucesso!');
				window.location.href='/siscap/siscap.php?modulo=principal/listaCurso&acao=A';
			</script>";
	}
}

if( $_REQUEST['curid'] ){
	$sql = "SELECT * FROM siscap.curso WHERE curid = ".$_REQUEST['curid'];
	$curso = $db->pegaLinha( $sql );
}

?>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" id="curid" name="curid" value="<?=$_REQUEST['curid']?>" >
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">Nome do Curso:</td>
			<td colspan="3" width="35%">
				<?
				$curdsc = $curso['curdsc'] ? $curso['curdsc'] : $_REQUEST['curdsc'];

				echo campo_texto('curdsc', 'S', $habil, 'Nome do Curso', 80, 100, '', '', '', '', 0, 'id=curdsc' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Área:</td>
			<td colspan="3" width="35%">
				<?
				$areid = $curso['areid'] ? $curso['areid'] : $_REQUEST['areid'];

				$sql = "SELECT areid as codigo, aredsc as descricao FROM siscap.area WHERE arestatus = 'A' ORDER BY aredsc";
				$arPeriodo = $db->carregar( $sql );
				foreach( $arPeriodo as $per ){
					if( $per['descricao'] == 'Outras áreas' ){
						$arr2[] = $per;
					} else {
						$arr[] = $per;
					}
				}
				array_push( $arr, $arr2[0] );
				$db->monta_combo("areid", $arr, $habil, 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Área' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Objetivos de Aprendizagem:</td>
			<td colspan="3" width="35%">
				<?
				$curobj = $curso['curobj'] ? $curso['curobj'] : $_REQUEST['curobj'];
				echo campo_textarea("curobj", 'S', $habil, '', 85, 5, 800, '', '', '', '', "Objetivos de Aprendizagem");
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Principais Conteúdos:</td>
			<td colspan="3" width="35%">
				<?
				$curcont = $curso['curcont'] ? $curso['curcont'] : $_REQUEST['curcont'];
				echo campo_textarea("curcont", 'S', $habil, '', 85, 5, 5000, '', '', '', '', "Principais Conteúdos");
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Outras Informações:</td>
			<td colspan="3" width="35%">
				<?
				$curinfo = $curso['curinfo'] ? $curso['curinfo'] : $_REQUEST['curinfo'];
				echo campo_textarea("curinfo", 'N', $habil, '', 85, 5, 500, '', '', '', '', "Outras Informações");
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<?php if( $habil == 'S' ){ ?>
					<input type="button" id="btSalvar" value="Salvar" onclick="salvarCurso();" />
				<?php } ?>
				<input type="button" id="btVoltar" value="Voltar" onclick="voltarCurso();" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript">
	form		=	$("formulario");
	btVoltar	=	$("btVoltar");

	function salvarCurso(){
		btSalvar	=	$("btSalvar");
		btSalvar.disabled = true;
		btVoltar.disabled  = true;

		var camposObri 		= "#curdsc#areid#curobj#curcont";
		var tiposCamposObri	= '#texto#select#textarea#textarea';


		if(!validaForm('formulario',camposObri,tiposCamposObri,false)){
			btSalvar.disabled = false;
			btVoltar.disabled  = false;
			return;
		}
		var fieldName = "#curdsc#curobj#curcont#curinfo";
		var labelCampo = "#Nome do Curso#Objetivos de Aprendizagem#Principais Conteúdos#Outras Informações";

		if(!naoSomenteNumeros('formulario', fieldName, labelCampo)){
			btSalvar.disabled = false;
			btVoltar.disabled  = false;
			return;
		}
		form.submit();
	}
	function voltarCurso(){
		window.location.href = 'siscap.php?modulo=principal/listaCurso&acao=A';
	}
</script>