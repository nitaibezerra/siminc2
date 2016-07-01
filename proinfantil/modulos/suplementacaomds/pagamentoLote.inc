<?php 
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include  "_funcoes_mds.php";

if( $_REQUEST['req'] == 'geraLote' ){
	header('content-type: text/html; charset=ISO-8859-1');
	geraLoteMDS($_REQUEST);
	die();
}

if( $_REQUEST['req'] == 'listaMunicipiosComLote' ){
	header('content-type: text/html; charset=ISO-8859-1');
	listaMunicipiosComLoteMDS($_REQUEST);
	die();
}

if( $_REQUEST['req'] == 'confirmaPagamentoMunicipio' ){
	header('content-type: text/html; charset=ISO-8859-1');
	confirmaPagamentoMunicipioMDS($_REQUEST);
	die();
}

if( $_REQUEST['req'] == 'confirmaPagamentoLote' ){
	header('content-type: text/html; charset=ISO-8859-1');
	confirmaPagamentoLoteMDS($_REQUEST);
	die();
}

if( $_REQUEST['req'] == 'imprimirminutaexcel' ){
	ob_clean();
	
	$nomeArquivo = 'minuta_repasse_mds_'.date("Ymd").'_lote_'.$_REQUEST['lotid'];
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=".$nomeArquivo.".xls");
	header ( "Content-Disposition: attachment; filename=".$nomeArquivo.".xls");
	header ( "Content-Description: MID Gera excel" );
	
	$sql = "SELECT
				formata_cpf_cnpj(iue.iuecnpj) as cnpj, 
			  	lot.estuf,
			  	mun.mundescricao,
			  	lot.muncod,
			  	lot.crechepublicaparcial,
			  	lot.crechepublicaintegral,
			  	lot.crecheconveniadaparcial,
			  	lot.crecheconveniadaintegral,
				trim(to_char(lot.valorrepasse, '999G999G999G999G999D99')) as valorrepasse
			FROM 
			  proinfantil.loteminutamds lot
				inner join territorios.municipio mun on mun.muncod = lot.muncod
				left join par.instrumentounidade iu
					inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
				on iu.muncod = mun.muncod
			WHERE lot.lotid = {$_REQUEST['lotid']}
			order by
				lot.estuf,
			    mun.mundescricao";
	
	$cabecalho = array('CNPJ', 'UF', 'Municípo', 'Código IBGE', 'Creche Pública Parcial', 'Creche Pública Integral', 'Creche Conveniada Parcial', 'Creche Conveniada Integral', 'Valor do Repasse');
	$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%', 'S');
	exit;
}

if( $_REQUEST['req'] == 'imprimirminuta' ){
	$sql ="SELECT
			ar.arqnome,
			ar.arqextensao,
			ar.arqtipo
		FROM
			proinfantil.lote lp
		    inner join public.arquivo ar on ar.arqid = lp.arqid
		WHERE
			lp.lotid = ".$_REQUEST['lotid'];
		
	$arquivo = $db->pegaLinha($sql);
					
	$caminho = APPRAIZ."arquivos/proinfantil/minutaproinfantil/".$arquivo['arqnome'].'.pdf';

	$filename = $arquivo['arqnome'].'.'.$arquivo['arqextensao'];
	header( 'Content-type: '. $arquivo['arqtipo'] );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<style>
.div_lote {
background: #white;
}
.div_lote:hover {
background: #eeeeaa;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<tr>
		<td colspan="2" class="subtituloEsquerda">
			<input type="button" value="Voltar" id="btnVoltar"/>
		</td>
	</tr>
</table>
<br/>
<?php 
$perfil = pegaPerfil($_SESSION['usucpf']);

$arMnuid = array(11113, 11932, 11114, 11127);

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo_modulo = "Lista de Municípios";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
echo "<br>";
$abasPar = array( 
				0 => array( "descricao" => "Lotes",
							"link"	  	=> "proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A" ),
		  		1 => array( "descricao" => "Criar Lote",
							"link"	 	=> "proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A&criar=1" )
		 );
echo montarAbasArray($abasPar, $_SESSION['favurl']);
if($_POST){
	extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
$(function(){
	if( $.browser.msie ){
		$('.div_tramita').css('width', '200px');
		$('.div_minuta').css('width', '200px');
		$('.div_excel').css('width', '200px');
	}
	
	function aguarda(val){
		jQuery('#pesquisar').attr('disabled',val);
		jQuery('#limpar').attr('disabled',val);
		if(val){
			jQuery('#aguardando').show();
		}else{
			jQuery('#aguardando').hide();
		}
	}
	$('#btnPesquisar').click(function(){	
		$('#formularioFiltro').submit();
	});
	$('#btnLimpar').click(function(){
		window.location = window.location;
	});
	$('#btnVoltar').click(function(){
		window.location = 'proinfantil.php?modulo=inicio&acao=C';
	});
	$('#btnGerar').click(function(){
		if( $('[name="lotnumportaria"]').val() == '' ){
			alert('Informe o Número da Portaria');
			$('[name="lotnumportaria"]').focus();
			return false;
		}
		if( $('[name="lotdataportaria"]').val() == '' ){
			alert('Informe a Data da Portaria');
			$('[name="lotdataportaria"]').focus();
			return false;
		}
		
		if( $('[name="cpmid[]"]:checked').length > 0){
			$('#btnGerar').attr('disabled', true);
			$('#req').val('geraLote');
			$('[name="formulario"]').submit();
		}else{
			alert('Selecione pelo menos um município.');
			return false;
		}
	});
	$('[name="cpmid[]"]').click(function(){
		if( $('[name="cpmid[]"]').length == $('[name="cpmid[]"]::checked').length ){
			$('.todos').attr('checked',true);
		}else{
			$('.todos').attr('checked',false);
		}
	});
	$('.todos').click(function(){
		if( $('[name="cpmid[]"]').length!=$('[name="cpmid[]"]::checked').length ){
			$('[name="cpmid[]"]').attr('checked',true);
		}else{
			$('[name="cpmid[]"]').attr('checked',false);
		}
	});
	$('.tramitaDocid').live('click',function(){
		$('#req').val('confirmaPagamentoMunicipio');
		$('[name="formulario"]').submit();
	});
	$('.div_tramita').click(function(){
		if( $('[name="docid[]"]:checked').length > 0 ){
			$('#lotid').val($(this).attr('id'));
			$('#req').val('confirmaPagamentoLote');
			$('[name="formulario"]').submit();
		} else {
			alert('Selecione um município.');
		}
	});
	$('.div_minuta').click(function(){
		$('#lotid').val($(this).attr('id'));
		$('#req').val('imprimirminuta');
		$('[name="formulario"]').submit();
	});
	
	$('.div_excel').click(function(){
		$('#lotid').val($(this).attr('id'));
		$('#req').val('imprimirminutaexcel');
		$('[name="formulario"]').submit();
	});
	
	$('.div_lote').click(function(){
		aguarda(true);
		var lotid = $(this).attr('id');
		if( $('#td_'+lotid).html() != '' ){
			if( $('#td_'+lotid).css('display') == 'none' ){
				$('#td_'+lotid).show();
				$('#imgmais'+lotid).hide();
				$('#imgmenos'+lotid).show();
			}else{
				$('#td_'+lotid).hide();
				$('#imgmais'+lotid).show();
				$('#imgmenos'+lotid).hide();
			}
			aguarda(false);
		}else{
			$.ajax({
				type: "POST",
				url: 'proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A',
				data: "req=listaMunicipiosComLote&lotid="+lotid,
				async: false,
				success: function(msg){
					$('#imgmais'+lotid).hide();
					$('#imgmenos'+lotid).show();
					$('#td_'+lotid).show();
					jQuery('#td_'+lotid).html(msg);
					
					if( parseInt($('[name="docid[]"]').length) > 0 ){
						var tamanho = parseInt($('[name="docid[]"]').length) * 80;
					} else {
						var tamanho = 400;
					}
					
					
					if( tamanho > 400 ) tamanho = 400;
					
					jQuery('#td_'+lotid).css('height',tamanho+'px');
					jQuery('#td_'+lotid).css('overflow','scroll');
					aguarda(false);
				}
			});
		}
		if( $.browser.msie ) $('.tramitaDocid').css('width', '250px');
	});
	aguarda(false);
});
</script>
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<form method="post" name="formularioFiltro" id="formularioFiltro" action="">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<?php if( !$_REQUEST['criar'] ){ ?>
		<tr>
			<td class="subtituloDireita">Lote</td>
			<td>
				<?php
				$sql = "SELECT distinct 
							lot.lotid as codigo,
							lot.lotdsc as descricao
						FROM
							proinfantil.lote lot
                            inner join proinfantil.mdsdadoscriancapormunicipio dcm ON dcm.lotid = lot.lotid and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
						ORDER BY 
							lot.lotid desc";
				$db->monta_combo('lotid', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<?php } else { ?>
		<tr>
			<td class="subtituloDireita">UF</td>
			<td>
				<?php 
				$sql = "SELECT 
							estuf as codigo,
							estdescricao as descricao
						FROM
							territorios.estado
						ORDER BY 
							estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td><?php echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?></td>
		</tr>
		<?php } ?>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>

<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="req" id="req" value=""/>
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<?php 
	
	if( $_REQUEST['criar'] ){
	?>
		<tr>
			<td class="subtitulodireita" width="40%"><b>Número da Portaria:</b></td>
			<td><?php echo campo_texto('lotnumportaria', 'S', 'S', '', 15, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Data:</b></td>
			<td><?php echo campo_data2('lotdataportaria', 'S', 'S', '', 'S', '', '', $lotdataportaria); ?></td>
		</tr>
		<tr>
			<td colspan="2">
			<center>
				<div style="overflow:auto;height:400px;width:100%;background-color:white;">
			<?php 
				print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%">';
				print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Lista de obras para gerar lotes</label></td></tr><tr>';
				print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
				listaMunicipiosSemLoteMDS($_POST);
			?>
				</div>
			</center>
			</td>
		</tr>
	<?php 
		if( in_array(PERFIL_ANALISTA, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser() ){
	?>
	<tr>
		<td colspan="2">
		<table class="tabela" align="center" style="width: 100%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
			<tr>
				<td class="subtituloEsquerda">
					<center>
						<input type="button" value="Gerar Lote" id="btnGerar" />
					</center>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<?php
		}
	}else{
		if( $_REQUEST['lotid'] )  $filtro = " and lot.lotid = {$_REQUEST['lotid']}";
		$sql = "SELECT DISTINCT	lot.lotid, lotdsc, arqid
				FROM 			proinfantil.lote lot
				INNER JOIN		proinfantil.mdsdadoscriancapormunicipio dcm ON dcm.lotid = lot.lotid AND dcm.cpmano = ({$_SESSION['exercicio']} - 1)
				LEFT  JOIN 		workflow.documento doc ON doc.docid = dcm.docid AND doc.esdid = ".WF_MDS_ANALISADO."
				LEFT  JOIN 		workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE 			lotstatus = 'A'
					$filtro
				ORDER BY		1";
		$lotes = $db->carregar($sql);
	?>
	<tr>
		<td colspan="2">
	<input type="hidden" name="lotid" id="lotid" value=""/>
	<table class="tabela" align="center" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<?php 
		if( $lotes[0] != '' ){
			foreach( $lotes as $lote ){
	?>
		<tr>
			<td width="55%">
				<div id="<?=$lote['lotid']; ?>" class="div_lote" style="float:left;cursor:pointer;width:100%;">
					<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmais<?=$lote['lotid']; ?>" >
					<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenos<?=$lote['lotid']; ?>" >
					<center>
						<b><?=$lote['lotdsc'] ?></b>
					</center>
				</div>
			</td>
			<td align="center">
				<?php 
				$sql = "SELECT DISTINCT doc.esdid
						FROM 			proinfantil.lote lot
						INNER JOIN		proinfantil.mdsdadoscriancapormunicipio dcm ON dcm.lotid = lot.lotid and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
						LEFT  JOIN		workflow.documento	doc ON doc.docid = dcm.docid AND doc.esdid = ".WF_MDS_AGUARDANDO_PAGAMENTO."
						LEFT  JOIN 		workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE 			lotstatus = 'A' AND lot.lotid = ".$lote['lotid']."
						ORDER BY		1";
				$testaLote = $db->pegaUm($sql);
				
				$disab = '';
				if( empty($lote['arqid']) ) $disab = 'disabled="disabled"';
					
					if( $testaLote != '' && ( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser()  ) ){?>
						<input type="button" value="Confirmar Pagamento do Lote" class="div_tramita" id="<?=$lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
						<input type="button" value="Imprimir Minuta Portaria" <?=$disab ?> class="div_minuta" id="<?=$lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
						<input type="button" value="Imprimir Minuta Portaria em Excel" class="div_excel" id="<?=$lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
				<?php } elseif( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser()) { 
					?>	
						<input type="button" value="Confirmar Pagamento do Lote" disabled="disabled" style="cursor:pointer; width:160px; white-space: normal;"/>
						<input type="button" value="Imprimir Minuta Portaria" <?=$disab ?> class="div_minuta" id="<?=$lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
						<input type="button" value="Imprimir Minuta Portaria em Excel" <?=$disab ?> class="div_excel" id="<?=$lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
					<?
				}?>
			</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td colspan="2"><div id="td_<?=$lote['lotid'] ?>"></div></td>
		</tr>
	<?php 			
			}
		}
	?>
	</table>
	</td></tr>
	<?php 
	}
	?>
	</table>
</form>

