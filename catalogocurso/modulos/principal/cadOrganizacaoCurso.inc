<?php 
if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
	die();
}

if( $_SESSION['catalogo']['curid'] ){
	$permissoes = testaPermissao();
}else{
	$permissoes['gravar'] = true;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$_SESSION['catalogo']['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

if( $_REQUEST['orcid'] ){
	$dadosOrganizacao = recuperaOrganizacaoCurso($_REQUEST);
	if(is_array($dadosOrganizacao)){
		foreach($dadosOrganizacao as $k => $dado){
			$$k = $dado;
		}
	}
}

monta_abas();

?>
<style>
textarea.disabled2{
border:none;
border-left:#888888 3px solid;
olor:#404040;
width:64ex;
text-align:;
}
select.disabled2{
border:none;
border-left:#888888 3px solid;
olor:#404040;
width:64ex;
text-align:;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_organizacao.js"></script>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr class="SubTituloEsquerda">
		<td style="FONT-SIZE: 12pt;">
			<center>
				Organização do Curso
			</center>
		</td>
	</tr>
	<tr class="FundoTitulo">
		<td>
			<center><img border="0" align="top" src="../imagens/obrig.gif" >Indica campo obrigatório</center>
		</td>
	</tr>
</table>
<?php 
if( $_SESSION['catalogo']['curid'] ){
	montaCabecalho( $_SESSION['catalogo']['curid'] );
}
?>
<form method="post" name="frmOrganizacao" id="frmOrganizacao">
	<input type="hidden" value="" id="req"  name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?=$permissoes['gravar']?>" id="gravar" name="gravar"/>
	<input type="hidden" value="<?=$_REQUEST['orcid'] ?>" id="orcid" name="orcid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">
				Tipo&nbsp;
			</td>
			<td>
				<?php 
					$sql = "SELECT DISTINCT
								tioid
							FROM
								catalogocurso.organizacaocurso
							WHERE
								orcstatus = 'A'
								AND curid = ".$_SESSION['catalogo']['curid'];
					$tioid = $db->pegaUm($sql);
					if( $tioid ){
						$disabledTioid = true;
					}
					monta_combo_tipo( $_POST ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Nome&nbsp;
			</td>
			<td>
				<?=campo_texto('orcdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '60', '250','','', '', '', '', 'id="orcdesc"', '', ''); ?> 
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Modalidade de Ensino&nbsp;
			</td>
			<td>
				<?php monta_combo_modalidadeCurso( $_POST ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Hora/Aula(s)&nbsp;
			</td>
			<td>
				<br>
				<b>Carga horária:</b><br>&nbsp;&nbsp;
				Mínimo:&nbsp;
				<?=campo_texto('orcchmim', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '6', '15', '[#]', '', '', '', '', 'id="orcchmim"','','') ?>&nbsp;Horas.&nbsp;
				Máximo:&nbsp;
				<?=campo_texto('orcchmax', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '6', '15', '[#]', '', '', '', '', 'id="orcchmax"','','') ?>&nbsp;Horas.<br><br>
				<div id="preexigida">
					<b>Carga horária presencial exigida(%):</b><br>&nbsp;&nbsp;
					Mínimo:&nbsp;
					<?=campo_texto('orcpercpremim', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5', '###', '', '', '', '', 'id="orcpercpremim"','','') ?>&nbsp;%.&nbsp;
					Máximo:&nbsp;
					<?=campo_texto('orcpercpremax', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5', '###', '', '', '', '', 'id="orcpercpremax"','','') ?>&nbsp;%.<br><br>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Descrição da Subdivisão&nbsp;
			</td>
			<td>
				<?=campo_textarea('orcementa', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Descrição da Subdivisão', '64', '4', '5000'); ?>
			</td>
		</tr>
		<tr>
			<td colspan="2"> 
				<center>
					<?php 
						$trava = !( $tioid!=TO_CRITERIO_IES&&$permissoes['gravar'] );
					?>
					<input type="button" id="voltar"    value="Voltar"/>
					<input type="button" id="salvar"    <?=$trava ? "disabled" : "" ?> value="Salvar"/>
					<input type="button" id="salvarC"   <?=$trava ? "disabled" : "" ?> value="Salvar e Continuar"/>
					<input type="button" id="pesquisar" value="Pesquisar"/>
					<input type="button" id="proximo"   value="Próximo"/>
				</center>
			</td>
		</tr>
	</table>
</form>
<?php listaOrganizacao($_POST);?>