<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

/* Tratamento de segurança */
$permissoes = verificaPerfilProfeinep();
/* FIM - Tratamento de segurança */

echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']), "/profeinep/profeinep.php?modulo=principal/expediente&acao=A");
monta_cabecalho_profeinep($_SESSION['profeinep_var']['prcid']);
if($_REQUEST['expid']) {
	$sql = "SELECT * FROM profeinep.expediente WHERE expid='".$_REQUEST['expid']."'";
	$expediente = $db->pegaLinha($sql);
	if($expediente) {
		$requisicao = 'atualizarexpediente';
		$tpeid = $expediente['tpeid'];
		$expdtinclusaoadvogado = $expediente['expdtinclusaoadvogado'];
		$expdscadvogado = $expediente['expdscadvogado'];
		$expdtinclusaoprofeinep = $expediente['expdtinclusaoprofeinep'];
		$expdscprofeinep = $expediente['expdscprofeinep'];
	} else {
		direcionar('?modulo=principal/expediente&acao=A','Não existe o expediente solicitado. Entre em contato com a equipe técnica.');
	}
} else {
	$requisicao = 'inserirexpediente';	
}
?>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
		});
</script>
<script src="./js/profeinep.js"></script>
<script src="../includes/calendario.js"></script>
<form name="formulario" id="formulario" method="post">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>" />
<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Parecerer</td>
</tr>
<tr>
	<td class="SubTituloDireita">Tipo:</td>
	<td>
	<?
	$sqlSituacao = "SELECT tpeid as codigo, tpedsc as descricao  from profeinep.tipoexpediente";
	$db->monta_combo('tpeid', $sqlSituacao, (($permissoes['gravar'])?'S':'N'), "", '', '', '', '150', 'N','tpeid');			
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Advogado</td>
</tr>
<tr>
	<td class="SubTituloDireita">Data :</td>
	<td><? echo campo_data('expdtinclusaoadvogado', 'N', (($permissoes['gravar'])?'S':'N'), '', 'S' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" >Parecer advogado :</td>
	<td><? echo campo_textarea('expdscadvogado', 'N', $somenteLeitura, '', '100', '10', '500'); ?></td>			
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">PROFE/INEP</td>
</tr>
<tr>
	<td class="SubTituloDireita">Data :</td>
	<td><? echo campo_data( 'expdtinclusaoprofeinep', 'N', (($permissoes['gravar'])?'S':'N'), '', 'S' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" >Parecer:</td>
	<td><? echo campo_textarea( 'expdscprofeinep', 'N', $somenteLeitura, '', '100', '10', '500'); ?></td>			
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
	<td>&nbsp;
	<? if($permissoes['gravar']) { ?>
	<input type="button" value="Salvar pareceres" id="botaosubmeter" onclick="return submeteFormExpediente();"/> 
	<? } ?>
	<input type="button" value="Voltar" onclick="this.disabled=true;window.location='?modulo=principal/expediente&acao=A';">
	</td>
</tr>
</form>
<? if($requisicao == 'atualizarexpediente') { ?>
<? if($permissoes['gravar']) { ?>
<form method="post" name="anexo" id="anexo" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirarquivoprofeinep" />
<input type="hidden" name="anxtipo" value="E"/>
<input type="hidden" name="expid" value="<? echo $_REQUEST['expid']; ?>"/>
<tr>
	<td class="SubTituloCentro" colspan="2">Anexar arquivo</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
	<td><input type="file" name="arquivo"/> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>
</tr>
<tr>
	<td class="SubTituloDireita">Tipo:</td>
	<td>
	<?
	$sql = "SELECT tpaid AS codigo, tpadsc AS descricao 
			FROM profeinep.tipoarquivo";
					
	$db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S');
		?>
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
	<td><?= campo_textarea( 'anxdesc', 'N', 'S', '', 70, 4, 250 ); ?></td>
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
	<td><input type="button" id="botaoanexosubmeter" value="Anexar arquivo" onclick="return submeteFormAnexo();"/> <input type="button" value="Voltar" onclick="this.disabled=true;window.location='?modulo=principal/expediente&acao=A';"></td>
</tr>
</form>
<? } ?>
<tr>
<td colspan='2'>
<?
$sql = "SELECT '<center><a href=\"#\" onclick=\"javascript:Excluir(\'?modulo=principal/documento&acao=A&requisicao=removerdocumento&anxid='||anx.anxid||'&arqid='||arq.arqid||'\',\'Deseja realmente excluir o documento?\');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,						
				to_char(anx.anxdtinclusao, 'dd/mm/yyyy'),
				tpa.tpadsc,
				arq.arqnome||'.'||arq.arqextensao,
				arq.arqtamanho,
				anx.anxdesc,
				usu.usunome
		FROM profeinep.anexos anx 
		LEFT JOIN profeinep.tipoarquivo tpa ON tpa.tpaid = anx.tpaid  
		LEFT JOIN public.arquivo arq ON arq.arqid = anx.arqid 
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf
		WHERE anx.anxstatus='A' AND anx.expid='".$_REQUEST['expid']."' AND anx.prcid='".$_SESSION['profeinep_var']['prcid']."'";
				
$cabecalho = array( "Ação", 
					"Data Inclusão",
					"Tipo Arquivo",
					"Nome Arquivo",
					"Tamanho (Mb)",
					"Descrição Arquivo",
					"Responsável");
$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%', '' );
} 
?>
</td>
</tr>
</table>

		
