<?php

if($_POST['acao']){
	extract($_POST);
	
	$obUnidadeMedida = new UnidadeMedida();
	$obUnidadeMedida->undid    	= $undid;
	$obUnidadeMedida->unddsc 	= $unddsc;
	$obUnidadeMedida->undtipo   = $undtipo;
	$obUnidadeMedida->gumid    	= $gumid;
	$obUnidadeMedida->salvar();
	$boSucesso = $obUnidadeMedida->commit();
	if(!$boSucesso){
		$obUnidadeMedida->rollback();
	}
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/listaUnidadeMedida","");
	unset($obUnidadeMedida);
	die;
}

$obUnidadeMedida = new UnidadeMedida($_GET['undid']);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function() {
	$('#formulario').submit(function(){
		if($('#unddsc').val() == ""){
			alert('O campo Descrição e obrigatório.');
			return false;
		}
		if($('#undtipo').val() == ""){
			alert('O campo Tipo e obrigatório.');
			return false;
		}
		if($('#gumid').val() == ""){
			alert('O campo Grupo e obrigatório.');
			return false;
		}
	});
});
//-->
</script>
<form method="post" name="formulario" id="formulario" action="par.php?modulo=principal/cadUnidadeMedida&acao=A">
<input type="hidden" name="acao" id="acao" value="1" />
<input type="hidden" name="undid" id="undid" value="<?php echo $obUnidadeMedida->undid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="font-weight: bold" colspan="2">
					Unidade Medida
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Descrição
			</td>
			<td><?php
					$unddsc = $obUnidadeMedida->unddsc;
					echo campo_texto( 'unddsc', 'S', $permissao_formulario, 'Descriçao', 40, 250, '', '','','','','id="unddsc"'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Tipo
			</td>
			<td>
				<?php
					$arTipo = array(
						array(
							'codigo'=>'E',
							'descricao'=>'Estadual'
						),
						array(
							'codigo'=>'M',
							'descricao'=>'Municipal'
						)
					);
					$undtipo = $obUnidadeMedida->undtipo;
					$db->monta_combo( "undtipo", $arTipo, 'S', 'Selecione...','', '', '', '300', 'N','undtipo');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Grupo
			</td>
			<td>
				<?php 
					$sql = "SELECT 
								gumid as codigo,
								gumdsc as descricao
				            FROM 
				            	par.grupounidademedida
				            WHERE 
				            	gumstatus = 'A'";
					$gumid = $obUnidadeMedida->gumid;
					$db->monta_combo("gumid", $sql, "S", "Selecione...", '', '', '', '300', 'N','gumid');
				?>
			</td>
		</tr>
	</table>
	<div id=buttonAcao>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					<input type="submit" id="btnSalvar" value="Salvar"  /> <input type="button" value="Lista" onclick="window.location.href='par.php?modulo=principal/listaUnidadeMedida&acao=A';" />
				</td>
			</tr>
	</table>
	</div>
</form>