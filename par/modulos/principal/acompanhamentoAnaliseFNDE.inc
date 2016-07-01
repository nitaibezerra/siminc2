<?php
$dopid = $_REQUEST['dopid'];
function salvar(){
	
	global $db;
	
	extract($_POST);
	
	if( verificaTermoEmReformulacao( $_SESSION['dopid'] ) ){
		echo "<script>alert('Não foi possivel gerar o parecer. Termo se encontra em reformulação.'); 
				window.location.href = window.location.href; </script>";
		die();
	}
	
	if( is_array($icoids) && is_array($atpids) && $acpdsc != '' ){
		
		$acprestritiva = $acprestritiva ? $acprestritiva : 'FALSE';
		
		$acpdsc = strlen($acpdsc) > 5000 ? substr($acpdsc, 0, 5000) : $acpdsc;
		$sql = "INSERT INTO par.acompanhamentoparecer( atpid, usucpf, acpdsc, acprestritiva ) VALUES ( $atpid, '{$_SESSION['usucpf']}', '$acpdsc', $acprestritiva) RETURNING acpid;";
		$acpid = $db->pegaUm($sql);
		
		$sql = "";
		foreach( $atpids as $atpid ){
			$sql .= "INSERT INTO par.acompanhamentotipopendencia_acompanhamentoparecer( acpid, atpid ) VALUES( $acpid, $atpid );";
		}
		$sql .= "UPDATE par.subacaoitenscomposicao_acompanhamentoparecer SET siastatus = 'I' WHERE icoid IN (".implode(' , ', $icoids).");";
		foreach( $icoids as $icoid ){
			$sql .= "INSERT INTO par.subacaoitenscomposicao_acompanhamentoparecer( icoid, acpid ) VALUES( $icoid, $acpid );";
		}
		if($db->executar($sql)){
			$db->commit();
			echo "<script>alert('Parecer gerado.'); window.location.href = window.location.href; </script>";
		}else{
			$db->rollback();
			echo "<script>alert('Não foi possivel gerar o parecer.'); window.location.href = window.location.href; </script>";
		}
	}else{
		echo "<script>alert('Não foi possivel gerar o parecer.'); window.location.href = window.location.href; </script>";
	}
}

if( $_REQUEST['req'] ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

header('content-type: text/html; charset=ISO-8859-1');

echo'<br>';

if( ! temPagamento($_SESSION['dopid']))
{
	echo "	<script>
					window.location=\"par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid= {$_SESSION['dopid']}\" ;
			</script>";
}

if( $perfilTipoAcompanhamento == 3 )
{
	$arrAbas =	array
	(	 
		0 => array( "descricao" => "Contratos", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
		1 => array( "descricao" => "Monitoramento", 				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
		2 => array( "descricao" => "Notas fiscais",					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
		3 => array( "descricao" => "Pendências/Finalizar",			"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
	);
}
else 
{
	$arrAbas =	array
	(	 
		0 => array( "descricao" => "Contratos", 						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
		1 => array( "descricao" => "Monitoramento", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
		2 => array( "descricao" => "Detalhamento do Serviço / Item",	"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=detalhamentoItem&dopid=" . $_SESSION['dopid']),
		3 => array( "descricao" => "Notas fiscais",						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
		4 => array( "descricao" => "Pendências/Finalizar",				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
	);
}
	
if( temPermissaoAnaliseFNDE() ){
	array_push($arrAbas, array( "descricao" => "Analise FNDE",				
								"link"	  	=> "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoAnaliseFNDE&dopid=" . $_SESSION['dopid']));
}
	
echo montarAbasArray( $arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoAnaliseFNDE&dopid=" . $_SESSION['dopid'] );

$habil = 'N';

if(  !verificaTermoEmReformulacao( $_SESSION['dopid'] ) ){
	$habil = 'S';
}else{
	$textTermo = '<label style="color:red">Este termo se encontra em reformulação.</label><br>';
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style type="">
.ui-dialog-titlebar{
	text-align: center;
}
</style>
<div id="debug"></div>
<script type="text/javascript">

$(document).ready(function(){

	<?php if( $habil == 'N' ){ ?>
	$('input[type="checkbox"]').attr('disabled','disabled');		
	<?php }?>

	$('.abre').live('click',function(){
		$('#aguardando').show();
		
		var sbdid = $(this).attr('sbdid');
		var linha = $(this);
		var html = '';
		
		if( $(linha).attr('src') == '/imagens/mais.gif' ){

			$(linha).attr('src','/imagens/menos.gif');
			
			if( $('#tr_'+sbdid).css('display') == 'none' ){
				$('#tr_'+sbdid).show();
			}else{
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "requisicao=carregaItensAnaliseFNDE&sbdid="+sbdid+"&dopid=<?php echo $dopid?>",
			   		async: false,
			   		success: function(msg){
				   		html = '<tr id=tr_'+sbdid+' bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">'+
				   					'<td align="left" title="&nbsp;Ações&nbsp;" colspan=2>'+msg+
				   					'</td>'+
				   				'</tr>';
			   			$(linha).parent().parent().parent().after(html);
			   			$('#aguardando').hide();
			   		}
				});
			}
		}else{
			$(linha).attr('src','/imagens/mais.gif');
			$('#tr_'+sbdid).hide();
		}
		$('#aguardando').hide();
	});

	$('#gravar').click(function(){

		if( $('[name="icoids[]"]::checked').length < 1 ){
			alert('Favor selecionar pelo menos 1(um) item para compor o parecer.');
			return false;
		}

		if( $('#acpdsc').val() == '' ){
			alert('Preencha os itens obrigatórios.');
			return false;
		}

		if( $('[name="atpids[]"]::checked').length < 1 ){
			alert('Favor selecionar pelo menos 1(um) tipo de pendência.');
			return false;
		}
		
		$('#formAnalise').submit();
	});

	$('.listaParecer').live('click',function(){
		var icoid = $(this).attr('icoid');
		window.open('par.php?modulo=principal/popupAcompanhamentoParecer&acao=A&icoid=' + icoid,  '' ,'width=800,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
	});

	$('#atpid').change(function(){
		if( $(this).val() != '1' ){
			$('#tr_restritiva').hide();
			$('#acprestritiva_N').attr('checked',true);
		}else{
			$('#tr_restritiva').show();
		}
	});
});

function voltar()
{
	location.href='par.php?modulo=principal/administracaoDocumentos&acao=A';
}

</script>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Analise FNDE</label>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> <?=$local ?> </b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta('<?=$dopid; ?>');" border="0">
			<?=getNumDoc( $dopid ) ?>
		</td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" >
		</td>
	</tr>
	<tr>
		<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<?=$textTermo ?>
			<b> Itens obrigatórios.</b> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
</table>
<?php 
function carregaDadoSubacao(){
	global $db;
	
	// PAR
	$sql = "SELECT DISTINCT  
				CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
					'<center>
						<img style=\"cursor:pointer\" src=\"/imagens/mais.gif\" class=abre border=0
							 sbdid='|| sd.sbdid ||' />
					</center>'
				END as acao,  
				s.sbadsc,
                                sd.sbdvaloraprovado as valor_subacao,
                                (SELECT sum(saldo) FROM par.v_saldo_empenho_por_subacao WHERE sbaid = s.sbaid) as valor_empenho,
                                (
                                    SELECT COALESCE(SUM(pg.pagvalorparcela),0.00)
                                     FROM par.pagamentosubacao pags
                                     INNER JOIN par.pagamento pg on pg.pagid = pags.pagid
                                     INNER JOIN par.empenho em ON em.empid = pg.empid AND empstatus = 'A' AND pg.pagstatus = 'A' AND trim(pg.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                                     WHERE pags.sbaid = s.sbaid AND pags.pobstatus = 'A'
                                ) as valor_pago,
                                (
                                    SELECT 
                                        COALESCE(SUM(convlrcontrato),0.00)
                                    FROM par.subacaoitenscomposicaocontratos sicc
                                    INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicc.icoid AND sic.icostatus = 'A'
                                    INNER JOIN par.contratos con ON con.conid=sicc.conid AND con.constatus = 'A'
                                    WHERE sic.sbaid = s.sbaid AND sicc.sccstatus = 'A'
                                ) as valor_contrato,
                                (
                                    SELECT 
                                        COALESCE(SUM(ntfvlrnota),0.00)
                                    FROM par.subacaoitenscomposicaonotasfiscais sicnf
                                    INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicnf.icoid AND sic.icostatus = 'A'
                                    INNER JOIN par.notasfiscais nf ON nf.ntfid=sicnf.ntfid AND nf.ntfstatus = 'A'
                                    WHERE sic.sbaid = s.sbaid AND sicnf.scnstatus = 'A'
                                ) as valor_nota
			FROM 
				par.subacao s
			INNER JOIN par.subacaodetalhe 	sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.termocomposicao 	tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar 	dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			INNER JOIN par.processopar 		prp on prp.prpid = dp.prpid AND prp.prpstatus = 'A'
			WHERE 
				dp.dopid = {$_SESSION['dopid']}
				AND sd.sbdid IN (
						SELECT  DISTINCT
							sd.sbdid
						FROM 
						par.termocomposicao tc
						INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
						INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
						INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
						INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
						LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
						WHERE   tc.dopid = {$_SESSION['dopid']} AND
							sd.sbdid IN(
								SELECT DISTINCT  
									CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
										sd.sbdid 
									END
								FROM 
									par.subacao s
								INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
								INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
								INNER JOIN par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
								WHERE 
									dp.dopid = {$_SESSION['dopid']}
							AND si.icoquantidaderecebida > 0
							)
						AND icomonitoramentocancelado = false
					)
                                
                            ";
                                                                        
                                                                    
	$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Valor Subação", "Valor Empenho", "Valor Pago", "Valor total dos Contratos", "Valor total das Notas");
	$db->monta_lista_simples($sql,$cabecalho,100,0,'N','95%','N');

}	
?>
<form method="post" name="formAnalise" id="formAnalise">
	<input type="hidden" name="req" id="req" value="salvar" />
	<?php 
	carregaDadoSubacao();
	?>
	<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
		<tr>
			<td class="subTituloDireita" width="30%">
				Parecer da Diligência:
			</td>
			<td width="100%" colspan="3" align="left">
				<?=campo_textarea('acpdsc', 'S', $habil, 'Descreva o parecer.', '150', '6', '5000') ?>
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita" width="30%">
				Tipo de Parecer:
			</td>
			<td width="100%" colspan="3" align="left">
				<?php
				$sql = "SELECT
							atpid as codigo,
	  						atpdsc as descricao
						FROM
							par.acompanhamentotipoparecer
	  					WHERE
							atpstatus = 'A'";
				
				$db->monta_combo('atpid',$sql,$habil,'Selecione...','','','','150','S','atpid');
				?>
			</td>
		</tr>
		<tr id="tr_restritiva" style="display:none;">
			<td class="subTituloDireita" width="30%">
				Restritiva ?
			</td>
			<td width="100%" colspan="3" align="left">
				<input type="radio" name="acprestritiva" value="TRUE" /> Sim &nbsp;
				<input type="radio" name="acprestritiva" id="acprestritiva_N" value="FALSE" checked="checked" /> Não
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita" width="30%">
				Pendência em:
			</td>
			<td width="100%" colspan="3" align="left">
				<?php
				$sql = "SELECT
							atpid as codigo,
	  						atpdsc as descricao
						FROM
							par.acompanhamentotipopendencia
	  					WHERE
							atpstatus = 'A'";
				
				$db->monta_checkbox('atpids[]', $sql, Array());
				?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<input type="hidden" name="atpid" value="1"/>
			</td>
		</tr>
		<tr>
			<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
				<?php if($habil=='S'){ ?>
				<input type="button" id="gravar" value="Gravar" >
				<?php } ?>
			</td>
		</tr>
	</table>
</form>