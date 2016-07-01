<?php 

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

$_REQUEST['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

if( $_SESSION['catalogo']['curid'] ){
	$permissoes = testaPermissao();
}else{
	$permissoes['gravar'] = true;
}

$dadosPublico = recuperaDadosPublicoAlvo($_REQUEST);
if(is_array($dadosPublico)){
	foreach($dadosPublico as $k => $dado){
		$$k = $dado;
	}
}

$_POST['cod_mod_ensino'] = recuperaRedesCurso($_REQUEST);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_publico.js"></script>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr class="SubTituloEsquerda">
		<td style="FONT-SIZE: 12pt;">
			<center>
				Público-Alvo
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
<form method="post" name="frmPublicoAlvo" id="frmPublicoAlvo">
	<input type="hidden" value="" id="req" name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?=$_REQUEST['curid'] ?>" id="curid" name="curid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">
				Função Exercida&nbsp;
			</td>
			<td>
				<?php combo_popup_funcaoExercida($_POST); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
				<img align="middle" border="0" src="../imagens/ajuda.png" width="13px" style="margin-left:15px;margin-bottom:5px;cursor:help" title="A quem se dirige o curso.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">

				Nivel de escolaridade permitido&nbsp;

			</td>
			<td>
<!--				<?php monta_combo_nivelEscolaridade( $_POST ); ?>-->
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
		<tr id="tr_formacao" style="display<?=in_array($dadosPublico['pacod_escolaridade'],Array(6,7))?"table-row":"none" ?>">
			<td class="SubTituloDireita">
				Área de Formação&nbsp;
			</td>
			<td>
				<?php combo_popup_areaFormacao($_POST); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
				<img align="middle" border="0" src="../imagens/ajuda.png" width="13px" style="margin-left:15px;margin-bottom:5px;cursor:help" title="Obrigatório somente para docente.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Disciplina(s) que leciona&nbsp;
			</td>
			<td>
				<?php combo_popup_disciplina($_POST); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
				<img align="middle" border="0" src="../imagens/ajuda.png" width="13px" style="margin-left:15px;margin-bottom:5px;cursor:help" title="Obrigatório somente para docente.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Etapa de Ensino em que Leciona&nbsp;
			</td>
			<td>
				<?php combo_popup_etapaEnsino( $_POST ); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
				<img align="middle" border="0" src="../imagens/ajuda.png" width="13px" style="margin-left:15px;margin-bottom:5px;cursor:help" title="Obrigatório somente para docente.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Modalidade em que leciona&nbsp;
			</td>
			<td>
				<?=monta_checkbox_modalidade( $dadosPublico ); ?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<img align="middle" border="0" src="../imagens/ajuda.png" width="13px" style="margin-left:15px;margin-bottom:5px;cursor:help" title="Obrigatório somente para docente.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Outras Exigências&nbsp;
			</td>
			<td>
				<?=campo_textarea('curpaoutrasexig', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Outras Exigências', '75', '4', '5000'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Curso disponivel para demanda social?&nbsp;
			</td>
			<td>
				<input type="radio" name="curpademsocial" value="S" <?=($curpademsocial == 't' ? 'checked="checked"' : '') ?> <?=($permissoes['gravar'] ? '' : 'disabled') ?>/>&nbsp; Sim &nbsp;
				<input type="radio" name="curpademsocial" value="N" <?=($curpademsocial == 'f' ? 'checked="checked"' : '') ?> <?=($permissoes['gravar'] ? '' : 'disabled') ?>/>&nbsp; Não &nbsp;
				<img border="0" src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr class="tr_demsoc">
			<td class="SubTituloDireita">
				Percentual máximo de participantes na demanda social&nbsp;
			</td>
			<td>
				<?=campo_texto('curpademsocialpercmax', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Público-alvo da demanda social', '3', '3','###', '', '', '', '', 'id="curpademsocialpercmax"'); ?>
				%
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
			</td>
		</tr>
		<tr class="tr_demsoc">
			<td class="SubTituloDireita">
				Público-alvo da demanda social&nbsp;
			</td>
			<td>
				<?php combo_popup_publicoAlvoDemandaSocial( $_POST ); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<center>
					<input type="button" id="voltar"  value="Voltar"/>
					<input type="button" id="salvar" value="Salvar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
					<input type="button" id="salvarC" value="Salvar e Continuar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
					<input type="button" id="proximo" value="Próximo"/>
				</center>
			</td>
		</tr>
	</table>
</form>