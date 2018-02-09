<?php

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

if( empty($_SESSION["emenda"]["ptridAnalise"]) ){
	echo "<script>
			alert('Faltam dados na sessão sobre este PTA.');
			window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
		  </script>";
	die;
} else {
	$ptridAnalise = $_SESSION["emenda"]["ptridAnalise"];
}

$perfil = pegaPerfilArray( $_SESSION['usucpf'] );

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = ( $boReformulacao ? $boReformulacao : pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] ) );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $ptridAnalise );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Geração de Minuta de Termo Aditivo', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

echo cabecalhoPlanoTrabalho($ptridAnalise, true);

montaVisualizarPTA( $ptridAnalise, false, $boReformulacao );

$obConvenio = new MinutaConvenio($ptridAnalise);

include_once APPRAIZ . 'includes/workflow.php';

if( $_POST['salvar1'] == 'alterar' ){
	$ptminutaTermo = $obConvenio->carregaMinutaTermoAditivo( $ptridAnalise, $_POST['refid'] );
} else {
	$ptminutaTermo = $obConvenio->carregaMinutaTermoAditivo( $ptridAnalise );
}

$arrTipos = pegaTipoReformulacao( $ptridAnalise );
$ptrcod = pegaNumPTA( $ptridAnalise );

$arRrefid = verificaTiposReformulacao( $ptridAnalise, 'codigo', false );

#caso a reformulação for Alteração de Cláusula de Convênio pega a ultima data de vigencia valida
if( (empty($ptminutaTermo['vigid']) && $arrTipos[0]['codigo'] == 1) || in_array(APOSTILAMENTO, $arRrefid) || in_array(ALTERACAO_DO_VALOR, $arRrefid) ){
	$sql = "select v.vigdatainicio, v.vigdatafim, v.vigdias, v.vigid 
			from emenda.ptvigencia v where
				v.ptrid in (select ptrid from emenda.planotrabalho where ptrcod = $ptrcod)
				and v.vigdatafim is not null
				order by v.vigid desc limit 1";
	$arrVigencia = $db->pegaLinha( $sql );
	
	$ptminutaTermo['vigdatainicio'] = $arrVigencia['vigdatainicio'];
	$ptminutaTermo['vigdatafim'] = $arrVigencia['vigdatafim'];
	$ptminutaTermo['vigdias'] = $arrVigencia['vigdias'];
	$ptminutaTermo['vigid'] = $arrVigencia['vigid'];
	
}

if($_REQUEST["submetido"]){
	
	if( $_REQUEST['salvar1'] != "" ){
		$boTermoAditivo = ( Object ) $_POST;
		$obConvenio->salvarMinutaTermoAditivo( $boTermoAditivo );
	}
	if($_REQUEST['mdoid'] != "") {
		if( !empty($_POST['obcid'][0]) && !empty($_POST['pmcid']) ){
			$sql = "DELETE FROM emenda.objetominutaconvenio WHERE pmcid = ".$_POST['pmcid'];
			$db->executar( $sql );
			foreach ($_POST['obcid'] as $obcid) {
				$sql = "INSERT INTO emenda.objetominutaconvenio( obcid, pmcid) 
						VALUES ( $obcid, {$_POST['pmcid']})";
				$db->executar( $sql );	
			}
			$db->commit();	
		}
		
		
		$sql = "SELECT mdoconteudo FROM emenda.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
		$imitexto = $db->pegaUm($sql);
		if($imitexto){
			$obConvenio->alteraMacrosConvenio($imitexto, $_REQUEST['pmcid']);
		}
		else{
			$imitexto = "Não existem informações sobre esta minuta.";
		}
	}
	if($_REQUEST['pmcid'] != "" && !$_REQUEST['mdoid']) {
		$imitexto = $ptminutaTermo["reftexto"];
	}

} else {
	if($ptminutaTermo) {
		$imitexto = $ptminutaTermo["reftexto"];
	}
}

if( !empty($ptminutaTermo['pmcid']) ){
	$sql = "SELECT omcid FROM emenda.objetominutaconvenio a WHERE pmcid = ".$ptminutaTermo['pmcid'];
	$arOmcid = $db->carregarColuna( $sql );
}
$arOmcid = ( $arOmcid ? $arOmcid : array() );

$imitexto = str_replace('"', "'", $imitexto);

$mdoid = ($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaTermo['mdoid']);

$vigdatafim = ($ptminutaTermo["vigdatafim"] ? formata_data($ptminutaTermo["vigdatafim"]) : "");

?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>


<form id="formMinutaConvenio" method="post" action="">
<input type="hidden" name="vigdatafim" id="vigdatafim" value="<?=$vigdatafim; ?>" />
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="modelo" id="modelo" value="<?=$mdoid; ?>" />
<input type="hidden" name="pmcid" id="pmcid" value="<?=$ptminutaTermo['pmcid'];?>" />
<input type="hidden" name="vigid" id="vigid" value="<?=$ptminutaTermo['vigid']; ?>" />
<input type="hidden" name="refid" id="refid" value="<?=$ptminutaTermo['refid']; ?>" />
<input type="hidden" name="ptridalterar" id="ptridalterar" value="" />
<input type="hidden" name="salvar1" id="salvar1" value="" />
<?php 
if( empty( $arOmcid ) ) echo '<input type="hidden" name="omcid[]" id="omcid[]" value="'.$omcid.'" />';

foreach ($arOmcid as $omcid) {
	echo '<input type="hidden" name="omcid[]" id="omcid[]" value="'.$omcid.'" />';
} 

if($ptminutaTermo){
	echo '<input type="hidden" id="imitexto" value="'. (($imitexto) ? $imitexto : 'Clique na Minuta desejada para recuperá-la.').'" />';
} else {
	echo '<input type="hidden" id="imitexto" value="" />';
}
?>


<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="left" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="3"><b>Minuta de Convênio</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Tipo de Minuta:</b></td>
		<td>
			<label for="tipRadioC"><input type="radio" name="tipRadio[]" id="tipRadioC" <?php echo ($_REQUEST['tipo'] != 'T' ? 'checked="checked"' : '' ) ;?>  onclick="carregaTipoMinuta(this.value);" value="C">Convênio</label>
			<label for="tipRadioT"><input type="radio" name="tipRadio[]" id="tipRadioT" <?php echo ($_REQUEST['tipo'] == 'T' ? 'checked="checked"' : '' ) ;?> onclick="carregaTipoMinuta(this.value);" value="T">Termo Aditivo</label>
		</td>
	</tr>
	<?
	if( !$boReformulacao ){
	?>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Tipo de Minuta:</b></td>
		<td>
			<label for="tipRadioC"><input type="radio" name="tipRadio[]" id="tipRadioC" onclick="carregaTipoMinuta(this.value);" value="C">Convênio</label>
			<label for="tipRadioT"><input type="radio" name="tipRadio[]" id="tipRadioT" checked="checked" onclick="carregaTipoMinuta(this.value);" value="T">Termo Aditivo</label>
		</td>
	</tr>
	<?
	}
	?>
	<tr>
        <td class="SubTituloDireita" style="width: 19%;"><b>Data da Celebração:</b></td>
        <td>
		<?
			$vigdatainicio = $ptminutaTermo['vigdatainicio'];
			$vigdatainicio = ($vigdatainicio ? formata_data($vigdatainicio) : "");
			echo campo_texto( 'vigdatainicio', 'N', 'N', '', 11, 20, '', '', '', '', 0, 'id="vigdatainicio"','','','' );
			//echo campo_data2('vigdatainicio', 'N', 'S','Prazo Inicio', '', '', '');
		?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita"><b>Dias de Vigência:</b></td>
        <td>
		<?
			$vigdias = $ptminutaTermo["vigdias"];
			echo campo_texto( 'vigdias', 'N', 'N', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','','','' );
		?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita"><b>Data Final da Vigência:</b></td>
        <td>
		<?//prddiasvigencia
			$vigdatafim = $ptminutaTermo['vigdatafim'];
			$vigdatafim = ($vigdatafim ? formata_data($vigdatafim) : "");
			echo campo_texto( 'vigdatafim', 'N', 'N', '', 11, 20, '', '', '', '', 0, 'id="vigdatafim"','','','' );
		?>
        </td>
    </tr>
    <tr>
		<td class="SubTituloDireita" valign="middle"><b>Objeto do convênio:</b></td>
		<td >
			<?php
			$sql = "SELECT oc.obcid as codigo, oc.obcdsc as descricao
					FROM 
						emenda.objetoconvenio oc
					    inner join emenda.objetominutaconvenio omc on omc.obcid = oc.obcid
					WHERE
						omc.pmcid = ". $ptminutaTermo['pmcid']."
					    and oc.obcstatus = 'A'
					    and oc.obctpobj = 'A'";
			
			if(  !empty($ptminutaTermo['pmcid']) ) $obcid = $db->carregar( $sql );
			
			$sql = "SELECT oc.obcid as codigo, oc.obcdsc as descricao FROM emenda.objetoconvenio oc
					WHERE  obcstatus = 'A'
					and oc.obctpobj = 'A'
					ORDER BY oc.obcdsc";
						
			combo_popup( 'obcid', $sql, '', '400x400', 0, array(), '', 'S', false, false, 7, 600 );
			echo obrigatorio(); 
			?>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Modelo do termo aditivo utilizado:</b>
        </td> 
        <td>
	    <?php 
	    
    	$sql = "select 
    				mod.mdoid as codigo, 
    				mod.mdonome as descricao
					from emenda.modelosdocumentos mod
		     		where mod.mdostatus = 'A'
		     		and mod.tpdcod = 21 ";
    	
    	
    	$habil = ( $ptminutaTermo['pmcid'] ? 'S' : 'N' );
    		    	
	    $db->monta_combo("mdoid", $sql, $habil, 'Selecione...', 'carregaTexto', '', '', '', '', 'mdoid', '', '', 'Lista de modelo(s) vinculado(s) a termo aditivo' );
        ?>
        </select>
        </td>    
    </tr>
    <!-- <tr>
		<td class="SubTituloDireita" valign="middle"><b>Cronograma Desembolso:</b></td>
		<td >
			<a href="#" onclick="popUpIncluirCronogramaDesenbolso();" style="cursor:pointer">
				<label id="labelCronograma">Clique aqui para preencher o Cronograma Desembolso do Convenio</label>
			</a>
			<input type="hidden" name="boCronograma" id="boCronograma" value="<?=($prdid > 0 ? 'true' : 'false'); ?>" />
		</td>
	</tr>-->
	<?
	if($ptminutaTermo){?>
		<tr>
			<td colspan="3">
				<div>
					<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%" class="emendatinymce"></textarea>
				</div>
			</td>
		</tr>
	<?php
	} else {
		echo '<input type="hidden" name="texto" id="texto" value="" />';
	} 
	
	/*if(pegarEstadoAtual( $ptridAnalise ) != EM_GERACAO_DA_MINUTA_DE_CONVENIO){ //EM_CRIACAO_DO_COVENIO
		$disabled = 'disabled="disabled"'; 
	} else{
		if(!possuiPermissao()) {
			$disabled = 'disabled="disabled"';
		} else {
			$disabled = '';
		}
	}*/

	$possiveis = array( CONVENIO, SUPER_USUARIO );

	//if( !$ptminutaTermo || $estadoAtual != EM_GERACAO_TERMO_ADITIVO  ){
	if( ($ptminutaTermo || in_array( $possiveis, $perfil )) && !in_array( CONSULTA_GERAL, $perfil ) ){
		$disabled = '';
	} else {
		$disabled = 'disabled="disabled"';
	}
	
	if( !empty($_POST['ptridalterar']) ){
		$max = $db->pegaUm("SELECT max(ptrid) FROM emenda.planotrabalho WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = ".$ptridAnalise.")");
		if( $max != $_POST['ptridalterar'] ) $disabled = 'disabled="disabled"';
	}
	
	?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaTermoAditivo();" <?=(!empty($disabled) ? $disabled : verificaPermissaoPerfil('analise', '', '', true)); ?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
		</td>
	</tr>
	</table></td>
	<td valign="top" style="text-align: center; width: 20px;">
			<?php
				$docid = criarDocumento( $ptridAnalise );
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptridAnalise, 'tipo' => "" ) );
			?>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="js/minutaConvenio.js"></script>
<script language="javascript" type="text/javascript" src="js/emenda.js"></script>
<div id="erro"></div>
<?  
$_POST['texto'] = '';
echo $obConvenio->listaMinutaTermoAditivoPTA( $ptridAnalise, $perfil );
?>

<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
	});
});
</script>
</body>