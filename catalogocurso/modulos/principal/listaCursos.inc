<?php 

if($_SESSION['catalogo']){
	unset($_SESSION['catalogo']);
	echo "<script>
			window.location = 'catalogocurso.php?modulo=inicio&acao=C';
		  </script>";
}

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

$arrCoords = Array();
if( !$db->testa_superuser() ){
	$arrCoords = recuperaCoordenacaoResponssavel();
	$pflcod    = pegaPerfil($_SESSION['usucpf']);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas();

?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_lista_cursos.js"></script>
<script type="text/javascript" src="geral/funcoes.js"></script>
<form method="post" enctype="multipart/form-data" name="frmCatalogo" id="frmCatalogo">
<table align="center" width="95%" border="0" class="tabela"  cellpadding="5" cellspacing="1">
	<tr class="SubTituloEsquerda">
		<td style="FONT-SIZE: 12pt;" colspan="2">
			<center>
				<b>Lista de Cursos</b>
			</center>
		</td>
	</tr>
	<tr class="FundoTitulo">
		<td colspan="2">
			<center><img border="0" align="top" src="../imagens/obrig.gif" >Indica campo obrigatório</center>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="10%">
			Status&nbsp;
		</td>
		<td>
			<?php monta_checkbox_status_pesquisa($_POST); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Área Temática&nbsp;
		</td>
		<td>&nbsp;
			<?php monta_combo_areaTematica_pesquisa( $_POST ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Código do Curso&nbsp;
		</td>
		<td>&nbsp;
			<?php 
				$curid = $_POST['curid'];?>
			<?=campo_texto('curid', 'N', 'S', 'Código do Curso', '10', '10','',''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Nome do Curso&nbsp;
		</td>
		<td>&nbsp;
			<?php 
				$curdesc = $_POST['curdesc'];?>
			<?=campo_texto('curdesc', 'N', 'S', 'Nome do Curso', '70', '255','',''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Nivel do Curso&nbsp;
		</td>
		<td>&nbsp;
			<?php monta_combo_nivelCurso_pesquisa( $_POST ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Rede&nbsp;
		</td>
		<td>
			<?php monta_checkbox_rede_pesquisa($_POST); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Coordenação responsável no<br> MEC ou na CAPES
		</td>
		<td>
			<?php
					if( !$db->testa_superuser() ){
						$arrCoords = recuperaCoordenacaoResponssavel();
					}
					
				//	$sql = "SELECT 
				//				co_interno_uorg as codigo, 
				//				sg_unidade_org||' - '||no_unidade_org as descricao
				//			FROM 
				//				siorg.tb_seo_unidade_org
				//			WHERE
				//				SUBSTRING(co_unidade_org FROM 1 FOR 1) = '0'";
				//  $db->monta_combo('co_interno_uorg', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '', 'S', 'co_interno_uorg');
					$sql = "SELECT 
								coordid as codigo, 
								coordsigla||' - '||coorddesc as descricao 
							FROM 
								catalogocurso.coordenacao
							WHERE
								coordstatus = 'A'
							".(is_array($arrCoords) && count($arrCoords)>0   ? " AND coordid in (".implode(",",$arrCoords).")"  : "")."
							ORDER BY
								coordsigla";
					$coordid = $_REQUEST['coordid'];
					$db->monta_combo('coordid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '', 'N', 'coordid'); 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Data de Criação&nbsp;
		</td>
		<td>&nbsp;
			<?php 
				$dt1 = $_POST['dt1'];
				$dt2 = $_POST['dt2'];?>
			<?=campo_data2('dt1','N','S','','DD/MM/YYYY','','', '', '', '', 'dt1' ) ?>
			&nbsp;
			<?=campo_data2('dt2','N','S','','DD/MM/YYYY','','', '', '', '', 'dt2' ) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"> </td>
		<td>&nbsp;
			<input type="button" id="pesquisar" value="Pesquisar"/>
		</td>
	</tr>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;" colspan="2">
			<span style="float:left;font-weight:bold;">
				&nbsp;
				<?php if( $db->testa_superuser() || $pflcod == PERFIL_ADMINISTRADOR || count($arrCoords)>0 ){?>
				<img border="0" align="absmiddle" onclick="window.location = 'catalogocurso.php?modulo=principal/cadCatalogo&acao=A';" title="Gerar Guia de Tramitações Antigas" style="cursor: pointer" src="../imagens/gif_inclui.gif">
				Adicionar Curso
				<?php }else{?>
				<label style="color:red">Usuário sem coordenação vinculada.</label>
				<?php }?>
			</span>
		</td>
	</tr>
</table>

<input type="hidden" value="" id="req" name="req"/>

</form>

	
<?php listaCursos($_REQUEST);?>
