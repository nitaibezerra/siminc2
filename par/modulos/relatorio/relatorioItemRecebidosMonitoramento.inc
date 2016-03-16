<?php
ini_set( "memory_limit", "1024M" );
global $db;

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo( $titulo_modulo, '' ); 

function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
			FROM territorios.municipio
			WHERE estuf = '$estuf'
			ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}
?>
<script type="text/javascript">
jQuery(document).ready(function(){

	jQuery('.pesquisar').click(function(){
		if( jQuery('[name="estuf"]').val() == '' ){
			alert('Escolha uma UF.');
			return false;
		}
		jQuery('input[type="button"]').attr('disabled', true);
		jQuery('#formulario').submit();
	});
	
	jQuery('.xls').click(function(){
		if( jQuery('[name="estuf"]').val() == '' ){
			alert('Escolha uma UF.');
			return false;
		}
		jQuery('input[type="button"]').attr('disabled', true);
		jQuery('#exibirxls').val('exibe');
		jQuery('#formulario').submit();
	});
	
	jQuery('[name="itrid"]').change(function(){
		jQuery('[name="estuf"]').val('');
		if( jQuery('[name="itrid"]').val() != '1' ){
			jQuery('#td_muncod').parent().show();
		}else{
			jQuery('#td_muncod').parent().hide();
			jQuery('#td_muncod').html('Escolha uma UF.');
		}
	});
	
	jQuery('[name="estuf"]').change(function(){
		if( jQuery('[name="itrid"]').val() != '1' ){
			if( jQuery(this).val() != '' ){
				jQuery('#aguardando').show();
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "requisicao=carregaMunicipios&estuf="+jQuery(this).val(),
			   		async: false,
			   		success: function(resp){
			   			jQuery('#td_muncod').html(resp);
			   			jQuery('#aguardando').hide();
			   		}
			 	});
			}else{
				jQuery('#td_muncod').html('Escolha uma UF.');
			}
		}
	});
});
</script>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="exibirxls" name="exibirxls" value="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="30%">Esfera:</td>
			<td>
				<?php
					$itrid = $_POST['itrid'];
					$sql = Array(
						Array( 'codigo' => 1, 'descricao' => 'Estadual' ),
						Array( 'codigo' => 2, 'descricao' => 'Municipal' )
					);
					$db->monta_combo( "itrid", $sql, 'S', 'Todas as Esferas', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">UF:</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município:</td>
			<td id="td_muncod">
				<?php
				if( $estuf ){
					$muncod = $_POST['muncod'];
					$sql = "SELECT muncod as codigo, mundescricao as descricao 
							FROM territorios.municipio 
							WHERE estuf = '$estuf'
							ORDER BY 2 ASC";
					$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
				}else{
					echo "Escolha uma UF.";
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número do Termo:</td>
			<td colspan="3">
			<?php
				$ternumero = $_REQUEST['ternumero'];
				echo campo_texto( 'ternumero', 'N', 'S', '', 20, 50, '[#]', '');
			?>
			</td>
	
		</tr>
		<tr>
			<td class="SubTituloDireita">Item Recebido:</td>
			<td colspan="3">
				<input type="radio" name="item_recebido" value="TRUE" <?=($_REQUEST['item_recebido'] == 'TRUE' ? 'checked="checked"' : '' ) ?> /> Possui 
				<input type="radio" name="item_recebido" value="FALSE" <?=($_REQUEST['item_recebido'] != 'TRUE' ? 'checked="checked"' : '' ) ?> /> Todos 
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td align="center" colspan="2">
				<input type="button" name="buscar" value="Buscar" class="pesquisar" />
				<input type="button" name="buscar" value="Gerar Relatório XLS"  class="xls" />
			</td>
		</tr>
	</table>
</form>
<?php 
if( $_POST ){
	
	$innerJoin = '';
	$arWere = array('1=1');
	if( !empty($_REQUEST['muncod']) ){
		array_push($arWere, "inu.muncod = '{$_REQUEST['muncod']}'");
	}
	
	if( !empty($_REQUEST['estuf']) ){
		array_push($arWere, "(inu.estuf = '{$_REQUEST['estuf']}' or inu.mun_estuf = '{$_REQUEST['estuf']}')");
	}
	
	if( !empty($_REQUEST['itrid']) ){
		array_push($arWere, "inu.itrid = {$_REQUEST['itrid']}");
	}
	
	if($_REQUEST['ternumero']){
		array_push($arWere, "tc.dopid in (select dopid from par.documentopar where (dopnumerodocumento = '{$_REQUEST['ternumero']}' or dopid = '{$_REQUEST['ternumero']}') and dopstatus = 'A') ");
		$innerJoin .= "INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid";
	}
	
	if( $_REQUEST['item_recebido'] == 'TRUE' ){
		array_push($arWere, "(CASE WHEN sbacronograma = 1 
									THEN sic.icoquantidaderecebida
									ELSE mie.mieqtdrecebida 
								END) > 0");
	}
	
	$sql = "SELECT DISTINCT
				COALESCE(inu.estuf,inu.mun_estuf) AS uf,
				inu.muncod,
				(SELECT mundescricao FROM territorios.municipio mun WHERE mun.muncod = inu.muncod AND mun.estuf = inu.mun_estuf ) as municipio,
				(
				SELECT DISTINCT dopnumerodocumento 
				FROM par.termocomposicao tc 
				INNER JOIN par.documentopar dop ON dop.dopid = tc.dopid	
				WHERE tc.sbdid = sd.sbdid AND dop.dopstatus = 'A' 
				LIMIT 1
				) as dopnumerodocumento,
				substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
				substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
				substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
				substring(prp.prpnumeroprocesso from 16 for 2) as numeroprocesso,
				'('||par.retornacodigosubacao(s.sbaid)||')' as codigo, 
				s.sbadsc,
				sic.icodescricao,
				e.escnome,
				e.entcodent,
				CASE WHEN sbacronograma = 1 
					THEN coalesce(sic.icoquantidadetecnico,0) 
					ELSE 
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) 
							THEN coalesce(se.sesquantidadetecnico,0)
							ELSE coalesce(ssi.seiqtdtecnico,0) 
						END                                                                 
				END as qtdsubacao,
				CASE WHEN sbacronograma = 1 
					THEN sic.icoquantidaderecebida
					ELSE mie.mieqtdrecebida 
				END as qtdrecebida
			FROM par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe 					sd  ON ppc.sbdid = sd.sbdid AND ppc.ppcstatus = 'A'
			$innerJoin
			INNER JOIN par.subacao 							s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
			INNER JOIN par.processopar 						prp ON prp.prpid = ppc.prpid 
			INNER JOIN par.acao 							aca ON aca.aciid = s.aciid AND acistatus = 'A'
			INNER JOIN par.pontuacao 						pon ON pon.ptoid = aca.ptoid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade 				inu ON inu.inuid = pon.inuid
			INNER JOIN par.subacaoitenscomposicao 			sic ON sic.sbaid = s.sbaid AND sic.icostatus = 'A' AND sic.icovalidatecnico = 'S' AND sic.icoano = sd.sbdano
			LEFT  JOIN par.subacaoescolas 					se  ON se.sbaid = s.sbaid AND sesstatus = 'A'
			LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
			LEFT  JOIN par.escolas 							e   ON e.escid = se.escid			    
			LEFT  JOIN par.subacaoescolas 					sec ON sec.sesid = ssi.sesid and sec.sesstatus = 'A' and sec.sbaid = s.sbaid
			LEFT  JOIN par.monitoramentoitensescolas 		mie ON mie.icoid = sic.icoid and sec.escid = mie.escid
			WHERE 
				".($arWere ? implode(' and ', $arWere) : '')."
			ORDER BY
				1,2";

	$cabecalho = array('UF', 'IBGE', 'Municípios', 'Número Documento', 'N° Processo', 'Localizador', 'Subação', 'Itens', 'Escolas', 'Cod INEP', 'QTD planejada', 'QTD recebida');
	
	if( $_REQUEST['exibirxls'] == 'exibe' ){
		ob_clean();
		
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatorioItemRecebidosMonitoramento_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		
		$db->monta_lista_tabulado($sql,$cabecalho,10000000000,5,'N','100%', '');
		die();
	} else {
		$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','S', true, false, false, true);
	}
}
?>