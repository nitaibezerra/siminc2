<?php
if( $_REQUEST['pdf'] ){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($_GET['arqid']);
	echo "<script>window.location.href = 'par/par.php?modulo=principal/teladeassinatura&acao=A&dopid=".$_GET['dopid']."';</script>";
	exit;
}

if( $_REQUEST['requisicao'] == 'aceita' ){
	
	/*
	$sql = "UPDATE par.documentopar SET 
			  dopdatavalidacaogestor = now(),
			  dopusucpfvalidacaogestor = '".$_SESSION['usucpf']."' 
			WHERE 
			  dopid = ".$_REQUEST['dopid'];
	*/
	
	
	if($_REQUEST['dopid'])
	{
		$sql = "SELECT
			prpnumeroprocesso
		FROM
			par.processopar prp
		INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
		WHERE
			dopid = {$_REQUEST['dopid']}";
	
		$prpnumeroprocesso = $db->pegaUm($sql);
	
		if($prpnumeroprocesso)
		{
			$db->executar("UPDATE par.emailreprogramacaobloqueada SET erbstatus = 'I' where erbnumeroprocesso = '{$prpnumeroprocesso}' AND erbstatus = 'A'");
			$db->commit();
		}
	}
		
		
	
	if( testaProcessoTermoFinalizadoPAR( $_REQUEST['dopid'] ) ){
		echo "<script>
				alert('Termo não pode ser validado pois o processo já foi finalizado.');
				window.location.href = window.location;
			</script>";
		exit();
	}
	
	if( $_SESSION['usucpf'] != $_SESSION['usucpforigem'] ){
		echo "<script>
				alert('Ação não permitida para simulação de usuarios.');
				window.location.href = window.location;
			</script>";
		exit();
	}
	
	if($_SESSION['usucpf'] == '' ){
		echo "<script>
				alert('O CPF do usuario carregado na sessão é inválido!');
				window.location.href = window.location;
			</script>";
		exit();
	}
	
	$sql = "SELECT dopid FROM par.documentoparvalidacao WHERE dpvcpf = '".$_SESSION['usucpf']."' AND dopid = ".$_REQUEST['dopid']." AND dpvstatus = 'A'";
	$dopidIns = $db->pegaUm($sql);
	
	if(!$dopidIns){
		$sql = "INSERT INTO par.documentoparvalidacao ( dopid, dpvcpf, dpvdatavalidacao, dpvstatus ) VALUES ( ".$_REQUEST['dopid'].", '".$_SESSION['usucpf']."', NOW(), 'A' )";
		$db->executar( $sql );
		if($db->commit()){
			
			// Cria PDF
			$sql = "SELECT
						mdoqtdvalidacao
					FROM
						par.documentopar dop
					INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
					WHERE 	
						dopid = {$_REQUEST['dopid']}";

			$qtd_validado = $db->pegaUm( $sql );
			
			$sql = "SELECT count(dpvid) FROM par.documentoparvalidacao WHERE dopid = {$_REQUEST['dopid']}";
			
			$qtd_validar = $db->pegaUm( $sql );
			
			// Transforma em PDF somente se o numero de validações for igual ao numero requerido pelo modelo do documento.
			if( $qtd_validado == $qtd_validar ){
				
				ob_clean();
				
				require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
								
				$html = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '');
				
				
				$sql = "SELECT
							dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome,
							us.usucpf
						FROM par.documentopar  dp
						INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
						INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
						WHERE
							dpv.dpvstatus = 'A' and
							dp.dopid = ".$_REQUEST['dopid'];
				
				$dadosValidacao = $db->carregar($sql);
				
				$textoValidacao = "";
				
				if(is_array($dadosValidacao)){
					foreach( $dadosValidacao as $dv ){
						$cpfvalidacao[] = $dv['cpfgestor'];
						$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
					}
				}
				
				$html = trim(html_entity_decode( $html ))."
						<table id=termo align=center border=0 cellpadding=3 cellspacing=1 >
							<tr style=\"text-align: center;\">
								<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>$textoValidacao</td>
							</tr>
						</table>";
				
				$html = str_replace("'", '', $html);
				
				$pdf = pdf( utf8_encode( $html ) );
		
				$descricaoArquivo = 'PAR'.str_pad($_REQUEST['dopid'],8,0, STR_PAD_LEFT).'/'.date('d-m-Y');
		
				$campos = Array();
		
				$file = New FilesSimec("documentopar", $campos, 'par');
		
				$file->arquivo['name'] = $descricaoArquivo.".pdf";
		
				$arquivoSalvo = $file->setStream( $descricaoArquivo, $pdf, "text/pdf", ".pdf", false);
		
				$sql = "UPDATE par.documentopar SET
							arqid = $arquivoSalvo
						WHERE
							dopid = {$_REQUEST['dopid']}";
		
				$db->executar($sql);
		
				$db->commit();
		
// 				if($arquivoSalvo){
				
// 					pdf( utf8_encode($html) , 1 );
// 				}
			}
			// FIM - Cria PDF
			
			$proid = $db->pegaUm("select proid from par.documentopar where dopid = {$_REQUEST['dopid']}");
			$dadoDocu = $db->pegaLinha("select md.tpdcod as tpdcod, dp.dopvalortermo as valortermo from par.documentopar dp inner join par.modelosdocumentos md on md.mdoid = dp.mdoid where dp.dopid = ".$_REQUEST['dopid']);
			include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
			$oWSSigarp = new WSSigarp();
			if( empty($proid) ){ // Se for PAR
				if( $dadoDocu['tpdcod'] == 21 ){ //Aditivo
					$valorAntigo = $db->pegaUm("SELECT dp.dopvalortermo FROM par.documentopar dp WHERE dopid IN ( SELECT dopidpai FROM par.documentopar WHERE dopid = {$_REQUEST['dopid']} )");
					if( number_format($dadoDocu['valortermo'], 2, ',', '.') != number_format($valorAntigo, 2, ',', '.') ){ // Se o valor mudou eu reformulo
//						$sigarp = enviarItensSigarpReformulacao();
						$sigarp = $oWSSigarp->enviarItensSigarpReformulacao($_REQUEST['dopid']);
					}
				} else { // Comum
//					$sigarp = enviarItensSigarp();
					$sigarp = $oWSSigarp->enviarItensSigarp($_REQUEST['dopid']);
				}
			} else { // Se for Obras do PAR
				$sigarp = $oWSSigarp->solicitarItemObra($_REQUEST['dopid'], $proid, 'par');
			}
			
			if( $proid ){
				$sql = "SELECT DISTINCT 
							po.preid
						FROM par.empenhoobrapar  p
							INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
							INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
							INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
						WHERE
							pro.proid = $proid";
				
				$arrObra = $db->carregarColuna($sql);
				$arrObra = $arrObra ? $arrObra : array();
				
				$preObra = new PreObra();
				foreach ($arrObra as $preid) {					
					$preObra->importarPreobraParaObras2( $preid );
				}
			}
			
			if( $_REQUEST['validacao'] == 'S' ){
				echo "<script>
							alert('Operação realizada com sucesso!');
							window.opener.location.href = 'par.php?modulo=principal/documentosParaValidar&acao=A';
							window.close();
						</script>";
			} else {
				echo "<script>
							alert('Operação realizada com sucesso!');
							window.opener.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
							window.close();
						</script>";
			}
			exit();
		} else{
			echo "<script>
						alert('Falha na operação!');
					</script>";
		}
	} else {
		if( $_REQUEST['validacao'] == 'S' ){
			echo "<script>
						alert('Aceite já confirmado!');
						window.opener.location.href = 'par.php?modulo=principal/documentosParaValidar&acao=A';
						window.close();
					</script>";
		} else {
			echo "<script>
						alert('Aceite já confirmado!');
						window.opener.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
						window.close();
					</script>";
		}
	}
}

$sql = "select * from (
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, d.mdoid, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, dp.iueid,
						( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado,
						arqid_documento
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   		d   ON d.mdoid   = dp.mdoid
					INNER JOIN par.processopar 				pp  ON pp.prpid  = dp.prpid
					INNER JOIN par.instrumentounidade 		iu  ON iu.inuid  = pp.inuid
					LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
					LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
			UNION 
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, d.mdoid, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, dp.iueid, 
						( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado,
						arqid_documento
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   		d   ON d.mdoid   = dp.mdoid
					INNER JOIN par.processoobraspar 		pp  ON pp.proid  = dp.proid and pp.prostatus = 'A'
					INNER JOIN par.instrumentounidade 		iu  ON iu.inuid  = pp.inuid
					LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
					LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
			) as foo
		 WHERE dopid = ".$_REQUEST['dopid']; 

$html = $db->pegaLinha($sql);

$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );

$sql = "SELECT 
			dpv.dpvcpf as cpfgestor, 
			to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
			us.usunome, 
			us.usucpf
		FROM par.documentopar  dp
		INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
		INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
		WHERE 
			dpv.dpvstatus = 'A' and
			dp.dopid = ".$_REQUEST['dopid'];

$dadosValidacao = $db->carregar($sql);

$textoValidacao = "";

if(is_array($dadosValidacao)){
	foreach( $dadosValidacao as $dv ){
		$cpfvalidacao[] = $dv['cpfgestor'];
		$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
	}
}

$cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

//if( !empty($html['cpfgestor']) ){
if( $html['mdoqtdvalidacao'] <= $html['qtdvalidado'] || in_array($_SESSION['usucpf'], $cpfvalidacao) ){
	$display = 'style="display: none;"';
	$displayValidado = '';
} else { 
	if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
//        in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || # Removido devido solicitacao dos administradores
		in_array(PAR_PERFIL_PREFEITO, $perfil) || 
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || 
		( in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) && in_array( $html['mdoid'], Array(34, 40) ) )  ||
		( in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) )
	){
		$display = '';
	} else {
		$display = 'style="display: none;"';
	}
	$displayValidado = 'style="display: none;"';
	if( $html['qtdvalidado'] >= 1 ){
		$displayValidado = '';
	}
}

#Verifica se o termo é EX-OFICIO, caso retorne registro para ex-oficio o botao de aceite eh removido.
$sql = "SELECT
            count(d.dopid)
        FROM par.documentopar d
        WHERE d.mdoid IN (
            SELECT
                md.mdoid
            FROM par.modelosdocumentos md
            WHERE md.mdonome ilike '%_EX'
        ) AND d.dopid = {$_REQUEST['dopid']}";

$arrTermo = $db->pegaUm($sql);

if ((int)$arrTermo > (int)0) {
    $display = 'style="display: none;"';
}

if( !in_array($html['tpdcod'], array(21, 102, 103)) ) $display = 'style="display: none;"';

if( $_REQUEST['edita'] == 1 ){
	$display = 'style="display: none;"';
}

$validaReprogramacao = $db->pegaUm("SELECT repstatus FROM par.reprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']);
if( $validaReprogramacao == 'A' ){
	$display = 'style="display: none;"';
}

if( testaProcessoTermoFinalizadoPAR( $_REQUEST['dopid'] ) ){
	$display = 'style="display: none;"';
}

function monta_cabecalho_relatorio_par( $data = '', $largura ){
	
	global $db;
	
	$data = $data ? $data : date( 'd/m/Y' );
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">'			
				//.'		<td nowrap align="center" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>'				
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />'
				.'		</td>'
				//.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				//.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				//.'			Hora da Impressão: '.$data .' - ' . date( 'H:i:s' ) . '<br />'					
				//.'		</td>'	
				.'	</tr>'					
				.'</table><br><br>';					
								
		return $cabecalho;						
						
}
?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<style>

@media print {.notprint { display: none } .div_rolagem{display: none} }	
@media screen {.notscreen { display: none; }
.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
 
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function aceitarTermo(dopid){
		if(confirm('Eu li, e estou de acordo com o termo de compromisso.')){
			$('#aceitar').attr('disabled',true);
			$('#requisicao').val('aceita');
			
			$('#formulario').submit();
			
			<?php /*if( $_REQUEST['validacao'] == 'S' ){ ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&validacao=S&dopid='+dopid+'&aceita=S';
			<?php } else { ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&dopid='+dopid+'&aceita=S';
			<?php }*/ ?>
		}
	}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="dopid" id="dopid" value="<?=$_REQUEST['dopid'] ?>">
	<input type="hidden" name="validacao" id="validacao" value="<?=$_REQUEST['validacao']; ?>">
	
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
                            <?php if(empty($display)): ?>
				<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$_REQUEST['dopid'] ?>)" />
                            <?php endif; ?>
				<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	</table>
	<table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
		<tr>
			<td colspan="100"><?=($html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ) ?> </td>
		</tr>
	</table>
	<?php echo $cabecalhoBrasao; ?>
	<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td style="font-size: 12px; font-family:arial;">
				<div>
				<?php echo simec_html_entity_decode($doptexto); ?>
				</div>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr <?=$displayValidado; ?> style="text-align: center;">
			<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
				<?=$textoValidacao; ?>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
				<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$_REQUEST['dopid'] ?>)" />
				<?php if( $db->testa_superuser() ){?>
				<!--  <input type="button" name="pdf" id="pdf" value="Baixar PDF" onclick="window.location.href=window.location.href+'&pdf=true&arqid=<?=$html['arqid_documento'] ?>'" /> -->
				<?php }?>
				<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	</table>
</form>