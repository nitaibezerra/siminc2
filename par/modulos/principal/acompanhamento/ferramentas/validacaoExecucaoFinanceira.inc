<?php
function download(){

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
}

function listaNotasSubacaoDetalhe( $sbdid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT 
				ico.icodescricao,
				ntf.ntfdescricao,
				'<input type=button class=downloadArqid id='|| ntf.arqid ||' value=Download />' as download
			FROM par.notasfiscais ntf 
			INNER JOIN par.subacaoitenscomposicaonotasfiscais	snf ON snf.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
			INNER JOIN par.subacaoitenscomposicao			ico ON ico.icoid = snf.icoid AND ico.icostatus = 'A' 
			INNER JOIN par.subacaodetalhe				sbd ON sbd.sbaid = ico.sbaid AND sbd.sbdano = ico.icoano
			WHERE
				sbd.sbdid =  $sbdid";
	
	$dados = $db->carregar( $sql );
	
	include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
	
	$param['nome'] 				= 'listaNotas';
	$param['titulo'] 			= '';
	$param['descricaoBusca']	= '';
	$param['arrDados']			= $dados;
	$param['arrCabecalho']		= Array( 'Item', 'Contrato', 'Download' );
	$param['arrSemSpan']		= Array( '', '', 'N' );
	
	$lista = new listaDT( false );
	echo $lista->lista($param);
}

function listaContratosSubacaoDetalhe( $sbdid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT 
				ico.icodescricao,
				con.condescricao,
				'<input type=button class=downloadArqid id='|| con.arqid ||' value=Download />' as download
			FROM 
				par.contratos con
			INNER JOIN par.subacaoitenscomposicaoContratos 	sic ON sic.conid = con.conid AND con.constatus = 'A'
			INNER JOIN par.subacaoitenscomposicao		ico ON ico.icoid = sic.icoid AND ico.icostatus = 'A'
			INNER JOIN par.subacaodetalhe			sbd ON sbd.sbaid = ico.sbaid AND sbd.sbdano = ico.icoano
			WHERE
				sbd.sbdid = $sbdid";
	
	$dados = $db->carregar( $sql );
	
	include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
	
	$param['nome'] 				= 'listaContratos';
	$param['titulo'] 			= '';
	$param['descricaoBusca']	= '';
	$param['arrDados']			= $dados;
	$param['arrCabecalho']		= Array( 'Item', 'Contrato', 'Download' );
	$param['arrSemSpan']		= Array( '', '', 'N' );
	
	$lista = new listaDT( false );
	echo $lista->lista($param);
}

function validarSubacao(){
	
	global $db;
	
	if( !$_POST['sbdid'] ){
		$erro .= "- Erro ao identificar a subação.<br>";
	}
	
	if( !$_POST['tcpid'] ){
		$erro .= "- Favor escolher a \"Execução\".<br>";
	}
	
	if( !$_POST['cpajustificativa'] ){
		$erro .= "- Favor preencher a justificativa.<br>";
	}
	
	if( $erro != '' ){
		echo $erro;
		return false;
	}
	
	$sql = "UPDATE par.controlepagamento SET cpastatus = 'I' WHERE sbdid = {$_POST['sbdid']};
			INSERT INTO par.controlepagamento(sbdid, cpajustificativa, tcpid, usucpf)
			VALUES({$_POST['sbdid']}, '".str_replace("'", "''", $_POST['cpajustificativa'])."', {$_POST['tcpid']}, '{$_SESSION['usucpf']}');";
	
	$db->executar( $sql );
	$db->commit();
	
	return true;
}

function formValidarSubacao(){
	
	global $db;
	
	$sql = "SELECT tcpid, cpajustificativa, to_char(cpadata, 'DD/MM/YYYY HH12:MI:SS')||' - '||usu.usunome as ultima_validacao
			FROM par.controlepagamento cpa
			INNER JOIN seguranca.usuario usu ON usu.usucpf = cpa.usucpf
			WHERE cpa.cpastatus = 'A' AND sbdid = ".$_POST['sbdid'];
	
	$validacao = $db->pegaLinha( $sql );
?>
<script>
$(document).ready(function(){

	$('.downloadArqid').live('click',function(){

		var arqid = $(this).attr('id');

		window.open("par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=validacaoExecucaoFinanceira&req=download&arqid="+arqid,"ValidacaoPar","height=40,width=40,scrollbars=yes,top=50,left=200");

		return false;
	});

	$('.notas').click(function(){
		var display = $('#tr_nota').css('display');
		if( display == 'none' ){
			$('#tr_nota').show();
		}else{
			$('#tr_nota').hide();
		}
	});

	$('.contratos').click(function(){
		var display = $('#tr_contrato').css('display');
		if( display == 'none' ){
			$('#tr_contrato').show();
		}else{
			$('#tr_contrato').hide();
		}
	});
	
	$('.validarSubacao').click(function(){
		
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formValidarSubacao&"+$('#formValidacao').serialize(),
	   		async: false,
	   		success: function(msg){
		   		if( msg != '' ){
		   			$( "#dialog_erro" ).html(msg);
					$( "#dialog_erro" ).dialog({
						resizable: true,
						height:200,
						width:500,
						modal: true,
						buttons: {
							"Fechar": function() {
								$( this ).dialog( "close" );
							}
						}
					});
		   		}else{
			   		alert('Operação realizada com sucesso!');
			   		window.location.href = window.location.href;
			   	}
	   		}
		});
	});
});
</script>
<form method="post" name="formValidacao" id="formValidacao" >
	<input type="hidden" name="req" value="validarSubacao" >
	<input type="hidden" name="sbdid" value="<?=$_POST['sbdid'] ?>" >
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3>
		<?php if( $validacao['ultima_validacao'] != '' ){?>
		<tr>
			<td class="subtituloDireita">Ultima validação:</td>
			<td><?=$validacao['ultima_validacao'] ?></td>
		</tr>
		<?php }?>
		<tr>
			<td class="subtituloDireita">Execução:</td>
			<td>
			<?php 
			
			$sql = "SELECT
						tcpid as codigo,
						tcpdescricao as descricao
					FROM
						par.tipocontrolepagamento
					WHERE
						tcpstatus = 'A'";
			
			$dados = $db->carregar( $sql );
			
			foreach( $dados as $dado ){ ?>
				<input type=radio name=tcpid value=<?=$dado['codigo'] ?> <?=( ($dado['codigo'] == $validacao['tcpid']) ? "checked=checked" : "" ) ?> > <?=$dado['descricao'] ?> <br>
			<?php 
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Justificativa:</td>
			<td>
			<?php
			echo campo_textarea(
				"cpajustificativa",		// nome do campo
				"S",					// obrigatorio
				"S",					// habilitado
				"Comentário",			// label
				180,					// colunas
				10,						// linhas
				5000,					// quantidade maximo de caracteres
				'',
				'',
				'',
				'',
				'',
				$validacao['cpajustificativa']
			)
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" colspan=2 ><center><input type=button class=validarSubacao value=Validar /></center></td>
		</tr>
		<tr style="cursor:pointer" title="Mostrar Contratos" class=contratos >
			<td class="subtituloDireita" colspan=2 ><center>Contratos:</center></td>
		</tr>
		<tr id=tr_contrato style="display:none">
			<td colspan=2 ><? listaContratosSubacaoDetalhe( $_POST['sbdid'] ) ?></td>
		</tr>
		<tr style="cursor:pointer" title="Mostrar Notas" class=notas >
			<td class="subtituloDireita" colspan=2 ><center>Notas:</center></td>
		</tr>
		<tr id=tr_nota style="display:none">
			<td colspan=2 ><? listaNotasSubacaoDetalhe( $_POST['sbdid'] ) ?></td>
		</tr>
	</table>
</form>
<?php 
}

function listaSubacaoVEF(){
	
	global $db;
	
	$sql = "SELECT 
				par.retornacodigosubacao( sbd.sbaid ) as codigo,
				sba.sbadsc,
				sbd.sbdano,
				coalesce(tcp.tcpdescricao, 'Nenhuma') as validacao,
				'<input type=button value=Validar class=btnVesquisar id=' || sbd.sbdid || ' />' as valida 
			FROM 
				par.subacaodetalhe sbd 
			INNER JOIN par.subacao 					sba ON sba.sbaid = sbd.sbaid
			LEFT  JOIN par.controlepagamento 		cpa ON cpa.sbdid = sbd.sbdid AND cpa.cpastatus = 'A'
			LEFT  JOIN par.tipocontrolepagamento	tcp ON tcp.tcpid = cpa.tcpid
			WHERE 
				sbd.prpid = {$_POST['prpid']}
				AND sba.sbastatus = 'A'";
	
	$dados = $db->carregar( $sql );
	
	$param = Array('ordena' => false, 'totalLinhas' => false );
	
	$cabecalhos = array("Localização", "Subação", "Ano", "Validação", "Validar Execução Financeira" );
	
	echo "<tr id=processo_subacao_{$_POST['prpid']} ><td colspan=8 >
			<table width=95% cellspacing=0 cellpadding=2 border=0 align=center class=listagem >
				<thead>
					<tr>";
	
	foreach( $cabecalhos as $cabecalho ){
					echo "<td valign=top bgcolor=\"cfcfcf\" align=95% style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" class=title>
							<strong>$cabecalho</strong>
						</td>";
	}
	
	echo 			"</tr>
				</thead>
				<tbody>";
	foreach( $dados as $k => $linha ){
		
		$cor = ( ($k % 2) == 1 ) ? "" : "#F7F7F7";
		echo	 	"<tr bgcolor=\"$cor\" onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\" >";
		
		foreach( $linha as $coluna ){
			echo		"<td align=left >$coluna</td>";
		}
		echo		"</tr>";
	}
	echo		"</tbody>
			</table>";
		
	echo "</td></tr>";
}

function pegaDadosLista(){

	global $db;
	
	extract($_REQUEST);

	$where = Array("prp.prpstatus = 'A'");
	
	if( $estuf != '' ){
		$where[] = "(inu.estuf = '$estuf' OR inu.mun_estuf = '$estuf')";
	}
	
	if( $mundescricao != '' ){
		$where[] = "public.removeacento(mun.mundescricao) ilike public.removeacento('%".str_replace("'", "''", $mundescricao)."%')";
	}
	
	if( $numero_termo != '' ){
		$where[] = "(CASE WHEN dop2.dopano::boolean
		THEN dop.dopnumerodocumento::text || dop2.dopano::text ilike '%$numero_termo%'
		ELSE dop.dopnumerodocumento::text ilike '%$numero_termo%'
		END)";
	}
	
	if( $empnumeroprocesso != '' ){
		$where[] = "prp.prpnumeroprocesso ilike '%".str_replace(Array(".","/","-"),"",$empnumeroprocesso)."%'";
	}

	if( $convinculado != '' ){
	$where[] = "(sbd.sbaid, sbd.sbdano) NOT IN (SELECT DISTINCT ico.sbaid, ico.icoano FROM par.contratos con
													INNER JOIN par.subacaoitenscomposicaoContratos 	sic ON sic.conid = con.conid AND sccstatus = 'A' AND constatus = 'A'
													INNER JOIN par.subacaoitenscomposicao			ico ON ico.icoid = sic.icoid AND ico.icostatus = 'A')";
	}else{
		$where[] = "(sbd.sbaid, sbd.sbdano) IN (SELECT DISTINCT ico.sbaid, ico.icoano FROM par.contratos con
												INNER JOIN par.subacaoitenscomposicaoContratos 	sic ON sic.conid = con.conid AND sccstatus = 'A' AND constatus = 'A'
												INNER JOIN par.subacaoitenscomposicao			ico ON ico.icoid = sic.icoid AND ico.icostatus = 'A')";
	}

	if( $notavinculado != '' ){
		$where[] = "(sbd.sbaid, sbd.sbdano) NOT IN (SELECT DISTINCT ico.sbaid, ico.icoano FROM par.notasfiscais ntf
													INNER JOIN par.subacaoitenscomposicaonotasfiscais 	snf ON ntf.ntfid = snf.ntfid AND ntfstatus = 'A' AND scnstatus = 'A'
													INNER JOIN par.subacaoitenscomposicao				ico ON ico.icoid = snf.icoid AND ico.icostatus = 'A')";
	}else{
		$where[] = "(sbd.sbaid, sbd.sbdano) IN (SELECT DISTINCT ico.sbaid, ico.icoano FROM par.notasfiscais ntf
												INNER JOIN par.subacaoitenscomposicaonotasfiscais 	snf ON ntf.ntfid = snf.ntfid AND ntfstatus = 'A' AND scnstatus = 'A'
												INNER JOIN par.subacaoitenscomposicao				ico ON ico.icoid = snf.icoid AND ico.icostatus = 'A')";
	}

	$mais = "";
	if( $_REQUEST['req'] != 'listaProcessosVEFXLS' ){
		$mais = "'<center> <img style=\"cursor:pointer\" id=' || prp.prpid || ' class=mais_processo src=/imagens/mais.gif /> </center>' as acao,";
	}
	
	$sql = "SELECT DISTINCT
				$mais
				coalesce(inu.estuf, inu.mun_estuf) as estuf,
				coalesce(mun.mundescricao, ' - ') as municipio,
				CASE WHEN inu.estuf IS NOT NULL
					THEN 'Estadual'
					ELSE 'Municipal'
				END as esfera,
				CASE WHEN dop2.dopano::boolean
					THEN dop.dopnumerodocumento::text || '/' || dop2.dopano::text
					ELSE dop.dopnumerodocumento::text
				END as ndocumento,
				substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
				substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
				substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
				substring(prp.prpnumeroprocesso from 16 for 2) as numeroprocesso,
				CASE WHEN (SELECT DISTINCT TRUE FROM par.subacaoitenscomposicaoContratos sic
					   INNER JOIN par.subacaoitenscomposicao		ico ON ico.icoid = sic.icoid AND ico.icostatus = 'A'
					   WHERE ico.sbaid = sbd.sbaid AND ico.icoano = sbd.sbdano )
					THEN 'SIM'
					ELSE 'Não'
				END as possui_contrato,
				CASE WHEN (SELECT DISTINCT TRUE FROM par.subacaoitenscomposicaonotasfiscais 	snf
					   INNER JOIN par.subacaoitenscomposicao		ico ON ico.icoid = snf.icoid AND ico.icostatus = 'A'
					   WHERE ico.sbaid = sbd.sbaid AND ico.icoano = sbd.sbdano )
					THEN 'SIM'
					ELSE 'Não'
				END as possui_nota
			FROM
				par.processopar prp
			INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = prp.prpid AND ppcstatus = 'A'
			INNER JOIN par.subacaodetalhe			sbd ON sbd.sbdid = ppc.sbdid
			INNER JOIN par.vm_documentopar_ativos 	dop ON dop.prpid = prp.prpid
			LEFT  JOIN par.documentopar 			dop2 ON dop2.dopid = dop.dopnumerodocumento
			INNER JOIN par.instrumentounidade 		inu ON inu.inuid = prp.inuid
			LEFT  JOIN territorios.municipio 		mun ON mun.muncod = inu.muncod
			WHERE ".implode(' AND ', $where)."";

	$dados = $db->carregar( $sql );
	
	return is_array($dados) ? $dados : Array();
}

function listaProcessosVEFXLS(){
	
	global $db;

	$_REQUEST['mundescricao'] = utf8_decode($_REQUEST['mundescricao']);
	
	$dados = pegaDadosLista();
	
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
	header("Content-Disposition: attachment; filename=SIMEC_ValidacaoExecucaoFinanceira" . date("Ymdhis") . ".xls");
	$cabecalho = Array( 'UF', 'Município', 'Esfera', 'N° Termo', 'N° Processo', 'Contrato', 'Nota' );
	$db->monta_lista_tabulado($dados, $cabecalho, 1000000, 5, 'N', '100%');
}

function listaProcessosVEF(){
	
	global $db;
	
	$dados = pegaDadosLista();
	
	include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
	
	$param['nome'] 				= 'listaCombo'.$nome;
	$param['titulo'] 			= $titulo;
	$param['descricaoBusca']	= 'Escreva aqui sua busca rápida';
	$param['arrDados']			= $dados;
	$param['arrCabecalho']		= Array( '&nbsp;', 'UF', 'Município', 'Esfera', 'N° Termo', 'N° Processo', 'Contrato', 'Nota' );
	$param['arrSemSpan']		= Array( 'N' );
	
	$lista = new listaDT();
	echo $lista->lista($param);
}

if( $_REQUEST['req'] ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

?>
<style>

#ToolTables_listaCombo_0{
	display:none;
}

</style>
<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript">

$(document).ready(function(){

	$('#btnLimpar').click(function(){
		window.locaton.href = window.location.href;
	});

	$('#btnExportarXLS').click(function(){

		$('#req').val('listaProcessosVEFXLS');

		window.open("par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=validacaoExecucaoFinanceira&"+$('#formulario').serialize(),"Exportar XLS","height=40,width=40,scrollbars=yes,top=50,left=200");
	});

	$('#btnPesquisar').click(function(){
		$('#formulario').submit();
	});

	$('.mais_processo').click(function(){

		var prpid = $(this).attr('id');

		var html_subacao = $('#processo_subacao_'+prpid).html();

		if( html_subacao != '' && html_subacao != undefined ){

			if( $('#processo_subacao_'+prpid).css('display') == 'none' ){
				
				$('#processo_subacao_'+prpid).show();
				$(this).attr('src', '/imagens/menos.gif');
			}else{
				
				$('#processo_subacao_'+prpid).hide();
				$(this).attr('src', '/imagens/mais.gif');
			}
		}else{
			
			$.ajax({
				type: "POST",
				url: window.location, 
				data: "req=listaSubacaoVEF&prpid="+prpid,
				async: false,
				success: function( retorno ){
					$('.mais_processo[id="'+prpid+'"]').parent().parent().parent().after( retorno );
				}
			});

			$(this).attr('src', '/imagens/menos.gif');
		}
	});

	$('.btnVesquisar').live( 'click', function(){

		var sbdid = $(this).attr('id');
		
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formValidarSubacao&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			$( "#dialog" ).html(msg);
				$( "#dialog" ).dialog({
					resizable: true,
					height:720,
					width:1280,
					modal: true,
					buttons: {
						"Fechar": function() {
							$( this ).dialog( "close" );
						}
					}
				});
	   		}
		});
	});
});

</script>
<div id=dialog ></div>
<div id=dialog_erro title="Não foi possivel realizar a validação! " ></div>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
			<form method="post" name="formulario" id="formulario" action="">
				<input type="hidden" name="req" id="req" >
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3>
					<tr>
						<td class="subtituloDireita">UF:</td>
						<td>
							<?php
							$estuf = $_POST['estuf'];
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
						<td class="subtituloDireita">Município:</td>
						<td>
							<?php
							$mundescricao = $_POST['mundescricao'];
							echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', '');
							?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Nº Termo:</td>
						<td>
							<?php
							$numero_termo = $_POST['numero_termo'];
							echo campo_texto('numero_termo', 'N', 'S', '', 10, 20, '[#]', '');
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Número de Processo:</td>
						<td>
							<?php
								$filtro = simec_htmlentities( $_POST['empnumeroprocesso'] );
								$empnumeroprocesso = $filtro;
								echo campo_texto( 'empnumeroprocesso', 'N', 'S', '', 30, 50, '#####.######/####-##', ''); 
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Sem contrato vinculado:</td>
						<td>
							<input type="checkbox" value="check" <?php if($_POST['convinculado']){ echo 'checked="checked"'; }  ?> name="convinculado" >
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Sem nota vinculada:</td>
						<td>
							<input type="checkbox" value="check" <?php if($_POST['notavinculado']){ echo 'checked="checked"'; }  ?> name="notavinculado" >
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">&nbsp;</td>
						<td class="subtituloEsquerda">
							<input type="button" 	value="Pesquisar" 		id="btnPesquisar" />
						 	<input type="button"	value="Limpar" 			id="btnLimpar" />
						 	<input type="button"	value="Exportar Excel" 	id="btnExportarXLS" />
						</td>
					</tr>
				</table>
			</form> 
		</td>
	</tr>
	<tr>
		<td>
			<?=listaProcessosVEF(); ?>
		</td>
	</tr>
</table>