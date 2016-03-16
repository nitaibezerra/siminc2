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

$dadosEquipe = recuperaDadosEquipe($_REQUEST);

if(is_array($dadosEquipe)){
	foreach($dadosEquipe as $k => $dado){
		$$k = $dado;
	}
}

monta_abas();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_equipe.js"></script>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr class="SubTituloEsquerda">
		<td style="font-size: 12pt; text-align: center;">Equipe</td>
	</tr>
	<tr class="FundoTitulo">
		<td style="text-align: center;">
			<img border="0" align="top" src="../imagens/obrig.gif" >Indica campo obrigatório
		</td>
	</tr>
</table>
<?php 
if( $_SESSION['catalogo']['curid'] ){
	montaCabecalho( $_SESSION['catalogo']['curid'] );
}
?>
<form method="post" enctype="multipart/form-data" name="frmEquipe" id="frmEquipe">
	<input type="hidden" value="" id="req"   name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?=$permissoes['gravar'] ?>" id="gravar" name="gravar"/>
	<input type="hidden" value="<?=$_REQUEST['eqcid'] ?>" id="eqcid" name="eqcid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%" rowspan="6">
				Equipe&nbsp;
			</td>
			<td colspan="4">
				Categoria de Membro de Equipe: <br>
				<?php monta_combo_categoriaMembroEquipe($_POST); ?>&nbsp;
				<b>Bolsista?</b>&nbsp;
				<?php monta_radio_bolsistas($_POST); ?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td colspan="4"> 
				Valor Unitário da Bolsa: <br/>
				<?=campo_texto('eqcvalorunitario', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Valor Unitário da Bolsa', '16', '14', '###.###.###,##', 'id="eqcvalorunitario"') ?>
			</td>
		</tr>
		<tr>
			<td width="3%">
				Quantidade:<br>
				<?=campo_texto('qtdfuncao', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5','##','','','','','id="qtdfuncao"'); ?> 
			</td>
			<td width="26%">
				Função:<br>
				<?=campo_texto('eqcfuncao', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '39', '50','','','','','','id="eqcfuncao"'); ?> 
				&nbsp; por
			</td>
			<td width="10%">
				Mínimo:<br>
				<?=campo_texto('eqcminimo', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '10', '10','','','','','','id="eqcminimo"'); ?> 
				&nbsp; a
			</td>
			<td width="10%">
				Máximo:<br>
				<?=campo_texto('eqcmaximo', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '10', '10','','','','','','id="eqcmaximo"'); ?> 
			</td>
			<td>
				Unidade de Referência: <br>
				<?php monta_combo_unidadeReferencia($_POST); ?>
			</td>
		</tr>
		<tr>
			<td colspan="4"> 
				Carga Horária por mês: <br/>
				<?=campo_texto('eqccargahorariames', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Carga Horária por mês', '16', '9', '', 'id="eqccargahorariames"') ?>
			</td>
		</tr>		
		<tr>
			<td colspan="5">
				Nivel de Escolaridade Permitido:<br>
				<?php 
					$sql = "(SELECT 
								to_char(pk_cod_escolaridade,'9') as codigo, 
								pk_cod_escolaridade||' - '||no_escolaridade as descricao,
								h.nivel
				  			FROM 
				  				educacenso_".(ANO_CENSO).".tab_escolaridade e
				  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade)
				  			UNION ALL
				  			(SELECT 
								to_char(pk_pos_graduacao,'9')||'0' as codigo, 
								pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao,
								h.nivel
							FROM 
								educacenso_".(ANO_CENSO).".tab_pos_graduacao e
				  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer)";
					$niveis = $db->carregar($sql);
					$nv = Array();
					if(is_array($niveis)){
						foreach($niveis as $nivel){
							if(!in_array($nivel['nivel'],$nv)){
								array_push($nv, $nivel['nivel']);
							}
							$checked = '';
							if( is_array($cod_escolaridade) ){
								foreach($cod_escolaridade as $resp){
									if( $resp['codigo'] == $nivel['codigo'] ){
										$checked = 'checked="checked"';
									}
								}
							}
							echo "<br>
								  <input type=\"checkbox\" id=\"".$nivel['nivel']."\" class=\"".implode(" ",$nv)."\" 
								  value=\"".$nivel['codigo']."\" $checked ".($permissoes['gravar'] ? '' : 'disabled="disabled"')." name=\"cod_escolaridade[]\"> ".$nivel['descricao'];
						}
					}
				?>
			</td>
		</tr>
		<tr>
			<td width="25%" colspan="2">
				Atribuição<br>
				<?=campo_textarea('eqcatribuicao', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Atribuição', '50', '4', '5000','','','','','',$eqcatribuicao); ?>
			</td>
			<td  colspan="3">
				Outros Requisitos
				<?=campo_textarea('eqcoutrosreq', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Outros Requisitos', '50', '4', '5000','','','','','',$eqcoutrosreq); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td colspan="5" style="text-align: center; background-color:#E8E8E8;"> 
				<input type="button" id="voltar" value="Voltar"/>
				<input type="button" id="salvar" value="Salvar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
				<input type="button" id="salvarC" value="Salvar e Continuar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
				<input type="button" id="pesquisar" value="Pesquisar"/>
				<input type="button" id="proximo" value="Próximo"/>
			</td>
		</tr>
	</table>
</form>
<?php listaEquipes($_POST);?>