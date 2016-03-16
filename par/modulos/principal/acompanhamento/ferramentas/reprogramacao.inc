<?php
$_SESSION['reprogramacao_par']['filtro']['tipo'] 					= $_POST['tipo'] ? $_POST['tipo'] : $_SESSION['reprogramacao_par']['filtro']['tipo'];
$_SESSION['reprogramacao_par']['filtro']['estado'] 					= $_POST['estado'] ? $_POST['estado'] : $_SESSION['reprogramacao_par']['filtro']['estado'];
$_SESSION['reprogramacao_par']['filtro']['numeroprocesso'] 			= $_POST['numeroprocesso'] ? $_POST['numeroprocesso'] : $_SESSION['reprogramacao_par']['filtro']['numeroprocesso'];
$_SESSION['reprogramacao_par']['filtro']['dopid'] 					= $_POST['dopid'] ? $_POST['dopid'] : $_SESSION['reprogramacao_par']['filtro']['dopid'];
$_SESSION['reprogramacao_par']['filtro']['esfera'] 					= $_POST['esfera'] ? $_POST['esfera'] : $_SESSION['reprogramacao_par']['filtro']['esfera'];
$_SESSION['reprogramacao_par']['filtro']['estuf'] 					= $_POST['estuf'] ? $_POST['estuf'] : $_SESSION['reprogramacao_par']['filtro']['estuf'];
$_SESSION['reprogramacao_par']['filtro']['muncod'] 					= $_POST['muncod'] ? $_POST['muncod'] : $_SESSION['reprogramacao_par']['filtro']['muncod'];
$_SESSION['reprogramacao_par']['filtro']['dt_fim_vigencia_inicio']	= $_POST['dt_fim_vigencia_inicio'] ? $_POST['dt_fim_vigencia_inicio'] : $_SESSION['reprogramacao_par']['filtro']['dt_fim_vigencia_inicio'];
$_SESSION['reprogramacao_par']['filtro']['dt_fim_vigencia_fim']		= $_POST['dt_fim_vigencia_fim'] ? $_POST['dt_fim_vigencia_fim'] : $_SESSION['reprogramacao_par']['filtro']['dt_fim_vigencia_fim'];
if( $_POST['pagsituacaopagamento'][0] != '' ){
	unset($_SESSION['reprogramacao_par']['filtro']['pagsituacaopagamento']);
	$_SESSION['reprogramacao_par']['filtro']['pagsituacaopagamento'] = Array();
	foreach( $_POST['pagsituacaopagamento'] as $pagsituacao ){
		$_SESSION['reprogramacao_par']['filtro']['pagsituacaopagamento'][] = Array('codigo' => utf8_decode($pagsituacao), 'descricao' => utf8_decode($pagsituacao) );
	}
}else{
	$_SESSION['reprogramacao_par']['filtro']['pagsituacaopagamento'] = $_SESSION['reprogramacao_par']['filtro']['pagsituacaopagamento'];
}

if( $_POST['limpar_filtros'] ){
	unset($_SESSION['reprogramacao_par']['filtro']);
	$_SESSION['reprogramacao_par']['filtro'] = Array();
}

extract($_SESSION['reprogramacao_par']['filtro']);
/* finalizaProcesso - Ainda não está em uso, mas achamos que será pedido, logo, adiantei o código.*/
function finalizaProcesso(){
	global $db;

	// PAR
	if( $_POST['tipo'] == 1 ){
		$sql = "UPDATE par.processopar SET prpfinalizado = true WHERE prpid = ".$_POST['processo'];
	}
	// Obras do PAR
	if( $_POST['tipo'] == 2 ){
		$sql = "UPDATE par.processoobraspar SET profinalizado = true WHERE proid = ".$_POST['processo'];
	}
	// Obras do PAC
	if( $_POST['tipo'] == 3 ){
		$sql = "UPDATE par.processoobra SET profinalizado = true WHERE proid = ".$_POST['processo'];
	}

	$db->executar($sql);
	$db->commit();

	echo "<script>
				alert('Processo finalizado com sucesso');
				window.location.href = window.location;
		</script>";
	die();
}

function voltarParaAprocavaoFNDE(){
	
    global $db;
    
    $sql = "UPDATE par.subacaodetalhe SET ssuid = 21 WHERE sbdid = ". $_POST['sbdid'];
    $db->executar($sql);
    $db->commit();
    
    echo 1;
    
    die();
}

function voltarParaAguardandoGeracaoTermo(){
    global $db;

    try {
        $sql = "DELETE FROM par.termocomposicao WHERE dopid = {$_POST['dopid']}";
        $db->executar($sql);

        $sql = "DELETE FROM par.termocomposicaoitens WHERE dopid = {$_POST['dopid']}";
        $db->executar($sql);

        $sql = "DELETE FROM par.documentopar WHERE dopid = {$_POST['dopid']}";
        $db->executar($sql);

        $sql = "SELECT dopidoriginal FROM par.reprogramacao WHERE dopidreprogramado = {$_POST['dopid']}";
        $dopidOriginal = $db->pegaUm($sql);

        $sql = "UPDATE par.reprogramacao SET dopidreprogramado = null WHERE dopidoriginal = {$dopidOriginal}";
        $db->executar($sql);

        $sql = "UPDATE par.documentopar SET dopstatus = 'A' WHERE dopid = {$dopidOriginal}";
        $db->executar($sql);

        $db->commit();
        echo 'Enviado para Aguardando Geração de Termo com sucesso!';
    } catch (Exception $exc) {
        echo $exc->getTraceAsString();
        $db->rollback();
    } 
    die();
}

function voltarParaAguardandoAprovacaoFnde(){
    global $db;

    try {
        $sql = "UPDATE par.subacaodetalhe SET ssuid = 21 WHERE prpid = {$_POST['prpid']}";
        $db->executar($sql);

        $db->commit();
        echo 'Enviado para Aguardando do Aprovação FNDE com sucesso!';
    } catch (Exception $exc) {
        echo $exc->getTraceAsString();
        $db->rollback();
    }
    die();
}

ini_set( "memory_limit", "1024M" );
set_time_limit(0);

function salvarRecusarValidacaoPrazo(){
	global $db;
	
	if( $_POST['dprparecer'] && $_POST['dopid'] ){
	
		$teste = enviaEmailRecusaReprogramacao($_POST['dopid'], 'prazo', $_POST['dprparecer'] );
	
		$sql = "UPDATE par.documentoparreprogramacao SET dprstatus = 'I', dprdatavalidacao = null, dprdataprazoaprovado = null, dprvalidacao = 't', dprparecer = '".$_POST['dprparecer']."' WHERE dopid = ".$_POST['dopid']." AND dprstatus = 'P'";
		$db->executar($sql);
		$db->commit();
	
		if( $teste ){
			echo '<script>alert("Solicitação recusada com sucesso! O e-mail foi enviado ao dirigente."); window.location.href = window.location;</script>';
		} else {
			echo '<script>alert("Solicitação recusada com sucesso!"); window.location.href = window.location;</script>';
		}
	}
	exit;
}
function recusarValidacaoPrazo(){
	global $db;
	
	$dados = $db->pegaLinha("SELECT dprjustificativa FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dopid = ".$_POST['dopid']);
	$dados = ($dados ? $dados : array());
	
	$dprjustificativa = $dados['dprjustificativa'];
	
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<th colspan="3" align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
				<b>Recusar a Validação da Reprogramação</b>
			</th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Justificativa:</td>
			<td><?php echo $dprjustificativa; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Parecer:</td>
			<td>
				<?php echo campo_textarea('dprparecer', 'N', 'S', 'Justificativa', 94, 10, 500); ?>
			</td>
		</tr>
	</table>
	<?php 
	
	exit();
}
function formVizualizaDocumento(){
	
	global $db;
	
	$sql = "SELECT * FROM (
				SELECT 
					dpv.dpvcpf as cpfgestor, 
					to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
					us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, 
					( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
				FROM par.documentopar  dp
				INNER JOIN par.modelosdocumentos   		d   ON d.mdoid = dp.mdoid
				INNER JOIN par.processopar 				pp  ON pp.prpid = dp.prpid
				INNER JOIN par.instrumentounidade 		iu  ON iu.inuid = pp.inuid
				LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
				LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
				UNION 
				SELECT 
					dpv.dpvcpf as cpfgestor, 
					to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
					us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
				FROM par.documentopar  dp
				INNER JOIN par.modelosdocumentos   		d   ON d.mdoid = dp.mdoid
				INNER JOIN par.processopar 				pp  ON pp.prpid = dp.prpid AND pp.prpstatus = 'A'
				INNER JOIN par.instrumentounidade 		iu  ON iu.inuid = pp.inuid
				LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
				LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
			) as foo
			WHERE dopid = ".$_REQUEST['dopid']; 
	
	$html = $db->pegaLinha($sql);

	$sql = "SELECT 
				dpv.dpvcpf as cpfgestor, 
				to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
				us.usunome, 
				us.usucpf
			FROM par.documentopar  dp
			INNER JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid
			INNER JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
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

	function monta_cabecalho_relatorio_par( $data = '', $largura ){
		
		global $db;
		
		$data = $data ? $data : date( 'd/m/Y' );
		
		$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">'
					.'	<tr bgcolor="#ffffff">' 	
					.'		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">'			
					.'			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>'				
					.'			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />'
					.'		</td>'
					.'	</tr>'					
					.'</table><br><br>';					
									
			return $cabecalho;						
	}
	
	echo "<div style=\"background-color:white;\">";
	
	$cabecalhoBrasao .= "	<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
	$cabecalhoBrasao .= "		<tr>" .
						"			<td colspan=\"100\">" .($html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ).
						"			</td>" .
						"		</tr>
						  	</table>";
	echo $cabecalhoBrasao;
	
	$documento = pegaTermoCompromissoArquivo($_REQUEST['dopid'], '');
?>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<div>
			<?php 
				echo html_entity_decode ($documento);
			?>
			</div>
		</td>
	</tr>
</table>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
	<tr <?=$displayValidado; ?> style="text-align: center;">
		<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
			<?=$textoValidacao; ?>
		</td>
	</tr>
</table>
</div>
<?php 	
}

/* Função Ajax para listar municípios do estado
 * */
function carregaMunicipios(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "SELECT muncod as codigo, mundescricao as descricao
			FROM territorios.municipio
			WHERE estuf = '$estuf'
			ORDER BY 2 ASC";
	
	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}
/* Função Ajax para listar municípios do estado - FRIM
 * */

function montaFiltroGenerico(){
	
	extract($_POST);
	
	$where = Array();
	
	/* Filtra estado em estado e/ou municipio para n  interferir no filtro de esfera
	 * */
	if( $estuf ){
		$where[] = "( est.estuf = '$estuf' OR mun.estuf = '$estuf' )";
	}
	
	if( $esfera ){
		if( $esfera == 'M' ){
			$where[] = "inu.muncod IS NOT NULL";
		}else{
			$where[] = "inu.estuf IS NOT NULL";
		}
	}
	
	/* Só filtra por Município se for de esfera Municipal - Função abstraida, n mostra para o usuario.
	 * */
	if( $muncod && $esfera != 'E' ){
		$where[] = "inu.muncod = '$muncod'";
	}
	
	if( $numeroprocesso ){
		$numeroprocesso = str_replace(".","", $numeroprocesso);
		$numeroprocesso = str_replace("/","", $numeroprocesso);
		$numeroprocesso = str_replace("-","", $numeroprocesso);
		
		$where[] = "prp.prpnumeroprocesso = '$numeroprocesso'";
	}
	
	if( $dopid ){
		$where[] = "dop.dopnumerodocumento = $dopid";
	}
	
	if( $pagsituacaopagamento[0] != '' ){
		$where[] = "prp.prpnumeroprocesso IN (SELECT 
												empnumeroprocesso
											FROM
												par.empenho emp
											INNER JOIN par.pagamento pag ON pag.empid = emp.empid
											WHERE
												pagsituacaopagamento IN ( '".implode("','",$pagsituacaopagamento)."' ))";
	}
	
	return $where;
}

/* Funções Reprogramação Prazo
 * INICIO
 * */

function listaReprogramacoes(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "SELECT DISTINCT
				'<center>'||CASE WHEN dop.dopidpai IS NOT NULL
				THEN
					'<img border=0 src=/imagens/mais.gif title=\"Lista Histórico de Reprogramação\" id='|| dop.dopid ||' style=cursor:pointer class=listaReprogramacoes >&nbsp'
				ELSE
					''
				END ||
				'<img class=vizualizarDocumento id='|| dop.dopid ||' style=\"cursor:pointer;\" src=../imagens/icone_lupa.png></center>' as ver,
				mdo.mdonome||' - '||coalesce(dop.dopnumerodocumento, dop.dopid) as processo,
				dop.dopdatafimvigencia as vigencia
			FROM par.documentopar dop
			LEFT  JOIN par.modelosdocumentos mdo on mdo.mdoid = dop.mdoid
			INNER JOIN seguranca.usuario us on us.usucpf = dop.usucpfinclusao
			LEFT  JOIN par.documentoparvalidacao dv on dv.dopid = dop.dopid and dv.dpvstatus = 'A'
			WHERE 
				(dop.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = $dopid )
				OR dop.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = $dopid ))
				AND dop.dopstatus IN ('I')
			ORDER BY
				2";
// ver($sql,d);
	unset($_POST['req']); // Limpa request pra n dar merda... (Em homenagem ao grande McBenzi)
	$cabecalho = array("&nbsp;","Documento", "Vigência");
	$db->monta_lista_simples($sql,$cabecalho, 2000, 1,'N','95%', 'N', true,false,false,true);
}

function montaSQLProcessos1(){
	
	global $db;
	
	extract($_POST);
	
	$where = montaFiltroGenerico();
	
	$where[] = "dopstatus = 'A'";
// 	ver(formata_data_sql($dt_fim_vigencia));
	
	if( $dt_fim_vigencia_fim != '' ){
		$where[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric <= to_char('".formata_data_sql($dt_fim_vigencia_fim)."'::date, 'YYYYMM')::numeric";
	}
	
	if( $dt_fim_vigencia_inicio != '' ){
		$where[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric >= to_char('".formata_data_sql($dt_fim_vigencia_inicio)."'::date, 'YYYYMM')::numeric";
	}
	
	/* Legenda de Situação da reprogramação
	* A = Aguardando Validação do FNDE
	* B = Aguardando Geração de Termo
	* E = Reformulações Finalizadas
	* */
	$botao = "";
	$mais = "''";
	switch( $estado ){
		case 'A':
			$where[] = "dpr.dprstatus = 'P' AND dprdataprazoaprovado IS NULL";
			$botao = ", CASE WHEN prpstatus = 'A' 
							THEN '<center><input type=button id='|| dop.dopid ||' class=validarReprogramacaoPrazo value=\"Validar Reprogramação de Prazo\"></center>'
							ELSE '<span style=color:red; >Processo Inativo. Favor entrar em contato com a CGPES/DIGAP.</span>'
						END as botao";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "Reprogramação");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			$innerReprogramacao = "INNER JOIN par.documentoparreprogramacao 	dpr ON dpr.dopid = dop.dopid";
			break;
		case 'B':
			$where[] = "dpr.dprstatus = 'P' AND dprdataprazoaprovado IS NOT NULL";
			$botao = ",CASE WHEN prpstatus = 'A' 
							THEN '<center>
									<input type=button id='|| dop.dopid ||' prpid='|| dop.prpid ||' class=gerarReprogramacaoPrazo value=\"Gerar Termo\" >
									<input type=button id='|| dop.dopid ||' prpid='|| dop.prpid ||' class=recusarValidacaoPrazo value=\"Recusar Validação\" >
								</center>'
							ELSE '<span style=color:red; >Processo Inativo. Favor entrar em contato com a CGPES/DIGAP.</span>'
						END as botao";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "Gerar Termo");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			$innerReprogramacao = "INNER JOIN par.documentoparreprogramacao 	dpr ON dpr.dopid = dop.dopid";
			break;
		case 'E':
			$where[] = "dop.dopidpai IS NOT NULL";
			$innerReprogramacao = "INNER JOIN par.documentoparreprogramacao 	dpr ON dpr.dopid = dop.dopidpai";
			$mais = "'<img border=0 src=/imagens/mais.gif title=\"Lista Histórico de Reprogramação\" id='|| dop.dopid ||' style=cursor:pointer class=listaReprogramacoes >&nbsp;
					  <img class=vizualizarDocumento id='|| dop.dopid ||' style=\"cursor:pointer;\" src=../imagens/icone_lupa.png>&nbsp;'";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
		default:
        	$where[] = "dpr.dprstatus = 'P' AND dprdataprazoaprovado IS NULL";
			$botao = ", '<center><input type=button id='|| dop.dopid ||' class=validarReprogramacaoPrazo value=\"Validar Reprogramação de Prazo\"></center>' as botao";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Data de Solicitação", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			$innerReprogramacao = "INNER JOIN par.documentoparreprogramacao 	dpr ON dpr.dopid = dop.dopid";
			break;
	}
	
	$acoes = "'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" title=\"Visualizar Termo\" onclick=\"carregaTermoMinuta('||dop.dopid||');\" border=\"0\">'";
	if( $xls ){
		$acoes = "";
		$mais = "";
		$botao = '';
	}else{
		$acoes = "$acoes||'&nbsp;'||";
		$mais = "$mais||
				CASE WHEN prp.prpfinalizado IS NULL 
					THEN '<img  style=\"width:18px;height: 18px;\" border=\"0\" title=\"Processo Vigente\" src=\"../imagens/par/cadeado-aberto.png\">' 
					ELSE '<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>' 
				END  as mais,";
	}
	
	$sql = "SELECT DISTINCT
				$mais
				
				substring(prpnumeroprocesso from 1 for 5)||'.'||
		   		substring(prpnumeroprocesso from 6 for 6)||'/'||
		   		substring(prpnumeroprocesso from 12 for 4)||'-'||
		   		substring(prpnumeroprocesso from 16 for 2) 
				 as processo,
				coalesce(dop.dopnumerodocumento, dop.dopid) as termo,
				$acoes mdo.mdonome,
				coalesce(mun.estuf, est.estuf) as estado,
				mun.mundescricao as municipio,
				to_char(dprdata, 'DD/MM/YYYY') as dt_solicitacao,
				cast(dop.dopvalortermo as numeric(20,2)) as dopvalortermo,
				em.valor as valorempenho,
			    pgs.valor_pagamento as valorpagamentosolicitado,
			    pm.valor_pagamento as valorpagamento,
				dop.dopdatafimvigencia as vigencia,
				COALESCE((par.retornasaldoprocesso(prp.prpnumeroprocesso)),'Não informado') as saldo_bancario,
				(SELECT count(icoid) || '/' || sum(contrato)
					from (
				
					SELECT
							distinct sic.icoid as icoid,
							
							case when exists ( SELECT icoid from par.subacaoitenscomposicaocontratos sicc WHERE sicc.icoid = sic.icoid) then
								1
							else
								0
							end as contrato
							
					FROM
							par.subacaoitenscomposicao sic
							INNER JOIN par.subacao s ON s.sbaid = sic.sbaid AND s.sbastatus = 'A'
							INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
							INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid AND ppc.ppcstatus = 'A'
							INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
					WHERE
							pp.prpnumeroprocesso =  prp.prpnumeroprocesso
				    	and sic.icoano = sd.sbdano
						and icostatus = 'A'
			
			
				) as foo) as contrato
				$botao
			FROM 
				par.processopar prp
			INNER JOIN par.v_saldo_empenho_do_processo	vep ON vep.processo = prp.prpnumeroprocesso
			INNER JOIN par.documentopar 				dop ON dop.prpid = prp.prpid
			INNER JOIN par.modelosdocumentos  			mdo ON mdo.mdoid = dop.mdoid
			$innerReprogramacao
			INNER JOIN par.instrumentounidade 			inu ON inu.inuid = prp.inuid
			LEFT  JOIN territorios.municipio  			mun ON mun.muncod = inu.muncod
			LEFT  JOIN territorios.estado 				est ON est.estuf = inu.estuf
			LEFT JOIN ( 
		        SELECT
		            dopid,
		            sum(valor) as valor
		        FROM
		            (
		            SELECT DISTINCT
		                d.dopid, 
		                vve.saldo as valor
		            FROM 
		                par.documentopar d
		            INNER JOIN par.processopar prp on prp.prpid = d.prpid and prp.prpstatus = 'A'
		            INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                    inner join par.v_saldo_empenho_do_processo vve on vve.processo = prp.prpnumeroprocesso
		            ) as foo
		        GROUP BY dopid
		        ) em ON em.dopid = dop.dopid
		        LEFT JOIN (select 
		                        d.dopid, sum( ps.pobvalorpagamento ) as valor_pagamento
		                    from 
		                        par.documentopar d
		                    inner join par.processopar prp on prp.prpid = d.prpid
		                    inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
		                    inner join par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
		                    inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pm ON pm.dopid = dop.dopid
		        LEFT JOIN (select 
		                        d.dopid, sum( ps.pobvalorpagamento ) as valor_pagamento
		                    from 
		                        par.documentopar d
		                    inner join par.processopar prp on prp.prpid = d.prpid
		                    inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
		                    inner join par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
		                    inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pgs ON pgs.dopid = dop.dopid
			WHERE
				".implode(' AND ',$where)."
			ORDER BY
				3,4";
	
	return Array($sql, $cabecalho);
}

/* Funções Reprogramação Prazo
 * FIM
* */

/* Funções Reprogramação Subação
 * INICIO
* */

/* Esta função é a cópia fiel da requisição da página documentoPar.
 * Não está lá para não termos problemas de sessão.
 * */
function finalizaReformulacao(){
	
	global $db;
	
	$sql = "SELECT
				prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
				dp.mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
				dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
				dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
				dopreformulacao, dopidpai, tpdcod, dopdatainiciovigencia, dopdatafimvigencia
			FROM
				par.documentopar dp
			INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
			WHERE dp.dopid = {$_REQUEST['dopid']}
				AND dopreformulacao = true";

	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
	
	$testeI = explode('/',$arrDadosDoc['dopdatainiciovigencia']);
	$teste = explode('/',$arrDadosDoc['dopdatafimvigencia']);
	if( $teste[0] < 1 || $teste[1] < $testeI[1] ){
		$arrDadosDoc['dopdatafimvigencia'] = str_pad($teste[0],2,'0').'/'.date('Y');
		$sql = "SELECT
					TO_CHAR(('01/'||{$arrDadosDoc['dopdatafimvigencia']})::timestamp + INTERVAL '6 MONTHS', 'MM/YYYY') as dopdatafimvigencia
				FROM
					par.documentopar dp
				INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
				WHERE 
					dp.dopid = {$_REQUEST['dopid']}
					AND dopreformulacao = true";
	
		$arrDadosDoc['dopdatafimvigencia'] = $db->pegaUm($sql);
	}
	

	$sql = "SELECT  s.sbadsc as finalidade,
				to_char(now(), 'MM/YYYY') AS cronogramainicial,
				to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A' and empstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = ".$arrDadosDoc['prpid'];

	$arCronograma = $db->pegaLinha( $sql );

	/* Modificado dia 13/01/2014
	 * A pedido de: Isabel
	* Programador: Eduardo Dunice Neto
	* */
// 	if( $arrDadosDoc['tpdcod'] == 21 ){ // Aditivo
		$tipoDocumento = $arrDadosDoc['modelo'];
// 	} else {
// 		$sql = "SELECT mdoid FROM par.modelosdocumentos WHERE mdotipovinculado = ".$arrDadosDoc['modelo'];
// 		$tipoDocumento = $db->pegaUm($sql);
// 	}

	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$tipoDocumento;
	$arrModelo = $db->pegaLinha($sql);

	$mdoconteudo = $arrModelo['mdoconteudo'];
	$tpdcod = $arrModelo['tpdcod'];

	$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$_REQUEST['dopid'];
	$subacoes 		= $db->carregarColuna($sql);
	$_POST['chk'] 	= $subacoes;

	$post = array('mdoid' => $arrDadosDoc['modelo'], 'dopid' => $_REQUEST['dopid'], 'tpdcod' => $tpdcod, 'chk' => $subacoes);

	if( $_REQUEST['inicio'] != '' ){
		$_SESSION['par']['cronogramainicial'] 	= $_REQUEST['inicio'];
		$_SESSION['par']['cronogramaFinal'] 	= $_REQUEST['fim'];
		$arrDadosDoc['dopdatainiciovigencia'] 	= $_REQUEST['inicio'];
		$arrDadosDoc['dopdatafimvigencia'] 		= $_REQUEST['fim'];
	}else{
		$_SESSION['par']['cronogramainicial'] 	= $arrDadosDoc['dopdatainiciovigencia'];
		$_SESSION['par']['cronogramaFinal'] 	= $arrDadosDoc['dopdatafimvigencia'];
	}

	$doptexto = alteraMacrosMinuta($mdoconteudo, $arrDadosDoc['prpid'], $post);

	$arrDadosDoc['mdoid'] = $tipoDocumento;

	$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $_REQUEST['dopid']);

	$sql = "UPDATE par.subacao SET sbareformulacao = false WHERE sbaid in (SELECT s.sbaid FROM
				par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
			WHERE
				dop.dopid = {$_REQUEST['dopid']} and ppc.ppcstatus = 'A' and s.sbareformulacao = true
				AND sd.sbdid NOT IN (SELECT sbdid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus <> 'I'))";
	$db->executar( $sql );

	$sql = "UPDATE par.documentopar SET dopreformulacao = false WHERE prpid = {$arrDadosDoc['prpid']}";
	$db->executar( $sql );
	$db->commit();
	
	enviaEmailNovoTermo($_REQUEST['dopid'], 'reformulacao');

	echo 1;
	exit;
}

/* Esta função é a cópia fiel da requisição da página documentoPar.
 * Não está lá para não termos problemas de sessão.
* */
function verificaValorReformulacao(){

	global $db;
	
	$ano = $db->pegaUm( "select date_part('year',dopdatainclusao) from par.documentopar where dopid = ".$_REQUEST['dopid'] );

	$sql = "SELECT
				s.sbaid as sbaidpai, s2.sbaid as sbaidfilho
			FROM
				par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
			INNER JOIN par.subacao s2 ON s2.sbaidpai = s.sbaid
			WHERE
				dop.dopid = {$_REQUEST['dopid']} and ppc.ppcstatus = 'A' AND s.sbareformulacao = true";

	$sbaids = $db->pegaLinha( $sql );

	$sbaids = $sbaids ? $sbaids : array();

	if( count($sbaids) < 1  ){
		echo 1;
		exit;
	} else {

		$sql = "SELECT
					s.sbaid,
					CASE WHEN sbacronograma = 1
						THEN (  SELECT sum(foo.vlrsubacao) 
								FROM ( SELECT DISTINCT
										CASE WHEN sic.icovalidatecnico IS NULL -- antigos
										THEN sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										ELSE -- novos
											CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
											END
										END
										as vlrsubacao
									   FROM par.subacaoitenscomposicao sic
									WHERE sic.sbaid = s.sbaid
										AND sic.icoano = sd.sbdano
									GROUP BY sic.sbaid, sic.icovalidatecnico ) as foo )
						ELSE ( SELECT sum(foo.vlrsubacao) 
								FROM ( SELECT DISTINCT
								CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
								THEN -- escolas sem itens
									CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
									sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										END
									END
								END
					as vlrsubacao
				FROM entidade.entidade t
				inner join entidade.funcaoentidade f on f.entid = t.entid
				left join entidade.entidadedetalhe ed on t.entid = ed.entid
				inner join entidade.endereco d on t.entid = d.entid
				left join territorios.municipio m on m.muncod = d.muncod
				left join par.escolas e on e.entid = t.entid
				INNER JOIN par.subacaoescolas se ON se.escid = e.escid
				INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
				LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
				and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 and t.tpcid = 3
				GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico ) as foo )
				END AS valorsubacao
				FROM
				par.subacao s
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = {$ano}
				WHERE
				s.sbaid IN ( ".implode(',', $sbaids)." )";

		$dados = $db->carregar($sql);
	
		if( is_array($dados) ){
			if( $dados[0]['valorsubacao'] > $dados[1]['valorsubacao'] ){
				echo 2;
			} else {
				echo 1;
			}
		}
	}

	exit;

}

/* Esta função é a cópia fiel da requisição da página documentoPar.
 * Não está lá para não termos problemas de sessão.
* */
function formPeriodoFinalizarRef(){

	?>
<script>
$(document).ready(function(){
	$('.mes').blur(function(){
		if( $(this).val().length == 1 ){
			$(this).val('0'+jQuery(this).val());
		}
		var mes = $(this).val();
		if( parseFloat(mes) < 1 || parseFloat(mes) > 12 ){
			alert('Mês inválido.');
			$(this).val(1);
		}
	});
	$('.ano').blur(function(){
		var ano = $(this).val();
		if( parseFloat(ano) < 2013 || parseFloat(ano) < <?=date('Y')?>  ){
			alert('Ano inválido.');
			$(this).val(2013);
		}
	});
});

</script>
<center>
	<div id="aguardando2" style="display:none; position: absolute; background-color: white; height:90%; width:95%; opacity:0.4; filter:alpha(opacity=40); " >
		<div style="margin-top:20px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Início:</td>
		<td>
			<?=campo_texto('mes_inicio','S','S','',3,2,'##','', 'right', '',0, 'id="mes_inicio" class="mes"','', '', '', '' )?> Mês /
			<?=campo_texto('ano_inicio','S','S','',5,4,'####','', 'right', '',0, 'id="ano_inicio" class="ano"','', '', '', '' )?> Ano
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fim:</td>
		<td>
			<?=campo_texto('mes_fim','S','S','',3,2,'##','', 'right', '',0, 'id="mes_fim" class="mes"','', '', '', '' )?> Mês /
			<?=campo_texto('ano_fim','S','S','',5,4,'####','', 'right', '',0, 'id="ano_fim" class="ano"','', '', '', '' )?> Ano
		</td>
	</tr>
</table>
<?php

}

function listaSubacao(){
	
	global $db;
	
	extract($_POST);
	
	$where = Array();
	
	$where[] = "dopid = $dopid";
	$where[] = "dopreformulacao IS TRUE"; // se foi pedido a reformulação
	
	/* Legenda de Situação da reprogramação
	 * B = Aguardando Conlcusão do Municipio
	 * C = Aguardando Aprovação do FNDE
	 * */
	switch( $estado ){
		case 'B':
			$where[] = "(SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) WHERE dop.prpid = ppc.prpid and ppc.ppcstatus = 'A' AND sd2.sbaid = s.sbaid ) > 0";
			break;
		case 'C':
			$where[] = "((SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20) WHERE dop.prpid = ppc.prpid and ppc.ppcstatus = 'A' AND sd2.sbaid = s.sbaid) = 0 AND
						(SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (21) WHERE dop.prpid = ppc.prpid and ppc.ppcstatus = 'A' AND sd2.sbaid = s.sbaid) > 0 )";
			break;
	}
	
	$sql = "SELECT DISTINCT
				'<img class=abresubacao id='|| sd.sbaid ||'_'|| prp.inuid ||' style=\"cursor:pointer;\" src=../imagens/icone_lupa.png>' as vizualicao,
				sbadsc as subacao,
				sbdano as ano,
				TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), '999G999G999D99') as valor ";
        if($estado == "G")
        {
            $sql .= ", '<div width=\"100%\" align=\"center\"><input type=\"button\" onclick=\"voltarParaAprocavaoFNDE('|| sd.sbdid ||')\" value=\"Voltar para aprovação do FNDE\"></div>' as acoes ";
        }
	$sql .="FROM 
				par.documentopar dop
			INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = dop.prpid and ppc.ppcstatus = 'A'
			INNER JOIN par.subacaodetalhe 		 	sd  ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao 				 	s   ON s.sbaid = sd.sbaid
			INNER JOIN par.processopar 				prp ON prp.prpid = dop.prpid
			WHERE
				".implode(' AND ', $where)."
			ORDER BY
				3,4";
	
	unset($_POST['req']); // Vide comentario em função anterior
	
        if($estado != "G"){
            $cabecalho = array("&nbsp;", "Subação", "Ano", "Valor");
        }else{
            $cabecalho = array("&nbsp;", "Subação", "Ano", "Valor","Ações");
        }
        
//         ver($sql,d);
	$db->monta_lista_simples($sql,$cabecalho, 200, 4,'N','95%', 'N', true,false,false,true);
}

function montaSQLProcessos2(){
	
	extract($_POST);

	$where = montaFiltroGenerico();
	
	if( $dt_fim_vigencia_inicio != '' ){
		$where[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric >= to_char('".formata_data_sql($dt_fim_vigencia_inicio)."'::date, 'YYYYMM')::numeric";
	}
	
	if( $dt_fim_vigencia_fim != '' ){
		$where[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric <= to_char('".formata_data_sql($dt_fim_vigencia_fim)."'::date, 'YYYYMM')::numeric";
	}
	
	$dpsstatus = 'A';
	$innerReprogramacao = "INNER JOIN par.documentoparreprogramacaosubacao 	dpr ON dpr.dopid = dop.dopid";
	$mais = "''";
	$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência");
	/* Legenda de Situação da reprogramação
	 * A = Aguardando Validação do FNDE
	* B = Aguardando COnlcusão do Municipio
	* C = Aguardando Aprovação do FNDE
	* G = Aguardando Empenho
	* D = Aguardando Geração de Termo
	* F = Aguardando Finalização da Reprogramação
	* E = Reprogramações Finalizadas
	* */
	switch( $estado ){
		case 'A': //A = Aguardando Validação do FNDE
			$dpsstatus = 'P';
			$botao = ", '<input type=button id='|| dop.dopid ||' class=validarReprogramacaoSubacao value=\"Validar Reprogramação de Subação\">' as botao";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "Validar");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
		case 'B': //B = Aguardando COnlcusão do Municipio
			$mais = "'<img border=0 src=/imagens/mais.gif title=\"Lista Subações\" id='|| dop.dopid ||' style=cursor:pointer class=listaSubacao >&nbsp;'";
			$where[] = "dopreformulacao IS TRUE";
			$where[] = "(SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) > 0";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
		case 'C': //C = Aguardando Aprovação do FNDE
			$mais = "'<img border=0 src=/imagens/mais.gif title=\"Lista Subações\" id='|| dop.dopid ||' style=cursor:pointer class=listaSubacao >&nbsp;'";
			$where[] = "dopreformulacao IS TRUE";
			$where[] = "((SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0 AND
				     (SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (21) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) > 0 )";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
		case 'G': //G = Aguardando Empenho
			$where[] = "(
							(
							SELECT DISTINCT
								(
									(par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))
									-
									sum(vve.vrlempenhocancelado)
								) > 0
							FROM
								par.empenhosubacao ems
							INNER join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A'
							inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
							inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
							left join par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
							left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = sd.sbdano
							left join (
								select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
								from par.empenho ep
									inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A' and eobstatus = 'A'
									inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
								where ep.empcodigoespecie in ('02')
								group by 
									ep.empnumeroprocesso,
									ep.empcodigoespecie,
									ep.empidpai,
									sd1.sbdid
							) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
							where
								ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
							GROUP BY
								sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado,emr.vrlreforco
							)
							)";
			$where[] = "dopreformulacao IS TRUE";
			$where[] = "(SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			$mais = "'<img border=0 src=/imagens/mais.gif title=\"Lista Subações\" id='|| dop.dopid ||' style=cursor:pointer class=listaSubacao >&nbsp;'";
			break;
		case 'D': //D = Aguardando Geração de Termo
			$cabecalho = array("&nbsp;", "Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "Gerar");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			//			$botao = ", '<input type=button id='|| dop.dopid ||' class=finalizaReprogramacaoSubacao value=\"Finalizar Reprogramação de Subação\">' as botao";
			$botao = ", '<input type=button id='|| dop.dopid ||' onclick=\"gerarTermoReprogramacao('||dop.dopid||','||dop.prpid||')\" value=\"Gerar Termo de reprogramação\">
						 <input type=button id='|| dop.dopid ||' class=acao_fluxo acao=voltarParaAguardandoAprovacaoFnde campo=prpid valor='||dop.prpid||' value=\"Voltar para Aprovação do FNDE\">' as botao";
			$where[] = "(
								(
								SELECT DISTINCT
									(
										(par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))::numeric
										-
										sum(des.saldo)
									) <= 0
								FROM
									par.empenhosubacao ems
								INNER JOIN par.v_dadosempenhosubacao 	des ON des.sbaid = ems.sbaid AND des.sbdano = ems.eobano
								INNER JOIN par.subacaodetalhe 			sbd ON ems.sbaid = sbd.sbaid and sbd.sbdano = ems.eobano
								WHERE
									ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
								GROUP BY
									sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado
								)
							)";
			$where[] = "dopreformulacao IS TRUE";
			$where[] = "(SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0";
			break;
		case 'F': //F = Aguardando Finalização da Reprogramação
			$where[] = "rep2.repdtfim IS NULL";
			$cabecalho = array("&nbsp;", "Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "&nbsp;");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			$innerReprogramacao = " INNER JOIN par.reprogramacao 	rep2 ON rep2.dopidreprogramado = dop.dopid AND rep2.repstatus = 'A'
										INNER JOIN par.documentoparreprogramacaosubacao 	dpr ON rep2.repid = dpr.repid";
			$botao = ", '<input type=button id='|| dop.dopid ||' class=acao_fluxo campo=dopid valor='||dop.dopid||' acao=finalizaReprogramacao value=\"Finalizar reprogramação\">&nbsp;
						 <input type=button id='|| dop.dopid ||' class=acao_fluxo campo=dopid valor='||dop.dopid||' acao=voltarParaAguardandoGeracaoTermo value=\"Voltar para Aguardando Geração de Termo\">' as botao";
			break;
		case 'E': //E = Reprogramações Finalizadas
			$where[] = "dop.dopidpai IS NOT NULL";
			$where[] = "rep2.repdtfim IS NOT NULL";
			$innerReprogramacao = "INNER JOIN par.documentoparreprogramacaosubacao 	dpr  ON dpr.dopid = dop.dopidpai
									   INNER JOIN par.reprogramacao 					rep2 ON rep2.dopidreprogramado = dop.dopid and rep2.repid = dpr.repid and rep2.repstatus = 'F'";
			$mais = "'<img border=0 src=/imagens/mais.gif title=\"Lista Histórico de Reprogramação\" id='|| dop.dopid ||' style=cursor:pointer class=listaReprogramacoes >&nbsp;
						  <img class=vizualizarDocumento id='|| dop.dopid ||' style=\"cursor:pointer;\" src=../imagens/icone_lupa.png>&nbsp;'";
			$cabecalho = array("&nbsp;","Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
		default:
			$dpsstatus = 'P';
			$botao = ", '<input type=button id='|| dop.dopid ||' class=validarReprogramacaoSubacao value=\"Validar Reprogramação de Subação\">' as botao";
			$cabecalho = array("&nbsp", "Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato", "Validar");
			if( $xls ){
				$cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário<br />(CC + CP + Fundo)", "QtdItem/<br>QtdContrato");
			}
			break;
	}

	$where[] = "dpr.dpsstatus = '$dpsstatus'";
	$where[] = "dopstatus = 'A'";
	$acoes = "'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" title=\"Visualizar Termo\" onclick=\"carregaTermoMinuta('||dop.dopid||');\" border=\"0\">'";
	if( $xls ){
		$acoes = "";
		$mais = "";
		$botao = '';
	}else{
		$acoes = "$acoes||'&nbsp;'||";
		$mais = "$mais ||
				CASE WHEN prp.prpfinalizado IS NULL
				THEN '<img  style=\"width:18px;height: 18px;\" border=\"0\" title=\"Processo Vigente\" src=\"../imagens/par/cadeado-aberto.png\">'
				ELSE '<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>'
				END  as mais,";
	}
	$sql = "SELECT DISTINCT
				$mais
				substring(prpnumeroprocesso from 1 for 5)||'.'||
   				substring(prpnumeroprocesso from 6 for 6)||'/'||
   				substring(prpnumeroprocesso from 12 for 4)||'-'||
   				substring(prpnumeroprocesso from 16 for 2) as processo,
				coalesce(dop.dopnumerodocumento, dop.dopid) as termo,
				$acoes mdo.mdonome,
				coalesce(mun.estuf, est.estuf) as estado,
				mun.mundescricao as municipio,
				cast(dop.dopvalortermo as numeric(20,2)) as dopvalortermo,
				em.valor as valorempenho,
			    pgs.valor_pagamento as valorpagamentosolicitado,
			    pm.valor_pagamento as valorpagamento, 
				dop.dopdatafimvigencia as vigencia,
				COALESCE(par.retornasaldoprocesso(prp.prpnumeroprocesso),'Não informado') as saldo_bancario,
				( 
					SELECT count(icoid) || '/' || sum(contrato)
			from (
		
					SELECT
							distinct sic.icoid as icoid,
							
							case when exists ( SELECT icoid from par.subacaoitenscomposicaocontratos sicc WHERE sicc.icoid = sic.icoid) then
								1
							else
								0
							end as contrato
							
					FROM
							par.subacaoitenscomposicao sic
							INNER JOIN par.subacao s ON s.sbaid = sic.sbaid AND s.sbastatus = 'A'
							INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
							INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid AND ppc.ppcstatus = 'A'
							INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
					WHERE
							pp.prpnumeroprocesso =  prp.prpnumeroprocesso
							and sic.icoano = sd.sbdano
							and icostatus = 'A'
				   and icostatus = 'A'
					) as foo ) as contrato
				$botao
			FROM
				par.processopar prp
			INNER JOIN par.processoparcomposicao 			ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			INNER JOIN par.subacaodetalhe 					sd  ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao 							s   ON s.sbaid = sd.sbaid
				
			INNER JOIN par.documentopar 					dop ON dop.prpid = prp.prpid
			INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
			$innerReprogramacao
			INNER JOIN par.instrumentounidade 				inu ON inu.inuid = prp.inuid
			LEFT  JOIN territorios.municipio  				mun ON mun.muncod = inu.muncod
			LEFT  JOIN territorios.estado 					est ON est.estuf = inu.estuf
			LEFT  JOIN ( 
		        SELECT
		            dopid,
		            sum(valor) as valor
		        FROM
		            (
		            SELECT DISTINCT
		                d.dopid, 
		                vve.saldo as valor
		            FROM 
		                par.documentopar d
		            INNER JOIN par.processopar prp on prp.prpid = d.prpid and prp.prpstatus = 'A'
		            INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                    inner join par.v_saldo_empenho_do_processo vve on vve.processo = prp.prpnumeroprocesso
		            ) as foo
		        GROUP BY dopid
		        ) em ON em.dopid = dop.dopid
	        LEFT JOIN (select 
	                        d.dopid, sum( ps.pobvalorpagamento ) as valor_pagamento
	                    from 
	                         par.documentopar d
	                    inner join par.processopar prp on prp.prpid = d.prpid
	                    inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
	                    inner join par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
	                    inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pm ON pm.dopid = dop.dopid
	        LEFT JOIN (select 
	                        d.dopid, sum( ps.pobvalorpagamento ) as valor_pagamento
	                    from 
	                        par.documentopar d
	                    inner join par.processopar prp on prp.prpid = d.prpid
	                    inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
	                    inner join par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
	                    inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pgs ON pgs.dopid = dop.dopid
			WHERE
				".implode(' AND ', $where)."
			ORDER BY
				3,4";
// 	ver($sql,d);
	return Array($sql, $cabecalho);
}

function listaProcessos(){
	
	global $db;
	
	unset($_POST['req']);
	$dados = call_user_func('montaSQLProcessos'.$_POST['tipo'] );

	$cabecalho = $dados[1];
	$db->monta_lista_simples($dados[0],$cabecalho, 2000, 1,'N','95%', 'N', true,false,false,true);
}

function listaProcessosXLS(){
	
	global $db;
	unset($_REQUEST['req']);
	$_POST = $_REQUEST;
	$dados = call_user_func('montaSQLProcessos'.$_REQUEST['tipo'] );
	$cabecalho 	= $dados[1];
	$arrDados 	= $db->carregar($dados[0]);

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Reprogramação".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Reprogramação".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_simples($dados[0],$cabecalho,100000000,5,'N','100%','S');
	echo "<script>window.location.href = 'par.php?modulo=principal/acompanhamento/ferramentas/reprogramacao&acao=A';</script>";
}

function finalizaReprogramacao(){

	global $db;
	
	finalizaReprogramacaoSubacao();
	
	exit;
}

/* Funções Reprogramação Subação
 * FIM
* */

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

include APPRAIZ."includes/cabecalho.inc";
?>
<!-- Div de aguardando -->
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:300%; width:100%; opacity:0.4; filter:alpha(opacity=40); " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<!-- Div de aguardando - FIM -->
<?php 
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery -->
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery - FIM -->
<script type="text/javascript">

/* Esta função é a cópia fiel da requisição da página documentoPar.
* */
function onOffCampo( campo ){
	var div_on 	= document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input 	= document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function carregaTermoMinuta(dopid){
	window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&dopid='+dopid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

// function finalizarProcesso(processo, tipo){
// 	if(confirm("Tem certeza que deseja finalizar esse processo?")){
// 		$('#req').val('finalizaProcesso');
// 		$('#processo').val(processo);
// 		$('#tipo').val(tipo);
// 		$('[name="formulario"]').submit();
// 	}
// }

function finaliza2( dopid ){
	$('#aguardando').show();
	if( confirm('Tem certeza que deseja finalizar essa reformulação?') ){
		var datas = '';
		if( jQuery('#mes_inicio').val() != '' ){
			var inicio	= jQuery('#mes_inicio').val()+'/'+jQuery('#ano_inicio').val();
			var fim		= jQuery('#mes_fim').val()+'/'+jQuery('#ano_fim').val();
			datas 		= "&inicio="+inicio+"&fim="+fim;
		}
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: '&req=finalizaReformulacao&dopid='+dopid+datas,
	   		async: false,
	   		success: function(resp){
	   			$('#aguardando').hide();
	   			if( resp = '1' ){
        			alert('Reformulação finalizada com sucesso!');
        			window.location.href = window.location.href;
        		} else {
        			alert('Falha na finalização da Reformulação!');
        		}
	   		}
	 	});
	}
	$('#aguardando').hide();
}

function voltarParaAprocavaoFNDE(sbdid){
	if( confirm('Tem certeza que gostaria de voltar essa subação para avaliação do FNDE?') ){
		$.ajax({
	        type: 'post',
	        url:  window.location.href,
	        data: '&req=voltarParaAprocavaoFNDE&sbdid='+sbdid,
	        async: false,
	        success: function (res){
	        	if( res == '1' ){
        			alert('Envio para o FNDE realizado com sucesso!');
//         			window.location.href = window.location.href;
        		} else {
        			alert('Falha no envio! Tente novamente');
        		}
	        }
	  });
	}
}

/* Esta função é a cópia fiel da requisição da página documentoPar.
* */
function finaliza( dopid ) {
	$('#aguardando').show();
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&req=verificaValorReformulacao&dopid='+dopid,
   		async: false,
   		success: function(resp){
   			$('#aguardando').hide();
        	if(resp == 1){
        		return finaliza2( dopid );
        	} else {
        		if( confirm("Valor divergente do Termo original. Continuar?") ){
        			return finaliza2( dopid );
        		}
        	}
   		}
 	});
}

function gerarTermoReprogramacao(dopid, prpid){
    var popup = window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid+'&geraTermoReprogramacao=1&origem=2','minuta','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
    popup.onbeforeunload = function() { 
        $('.pesquisar').click();
    };
}

$(document).ready(function(){
    
    

	/* Só mostra Município se n for esfera municipal
	*/
	$('[name="esfera"]').change(function(){
		if( $(this).val() != 'E' ){
			$('#tr_muncod').show();
		}else{
			$('#tr_muncod').hide();
		}
	});

	/* Carrega Municpios de acordo com o estado
	*/
	$('[name="estuf"]').change(function(){
		if( $('[name="esfera"]').val() != 'E' ){
			if( $(this).val() != '' ){
				$('#aguardando').show();
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "req=carregaMunicipios&estuf="+$(this).val(),
			   		async: false,
			   		success: function(resp){
			   			$('#td_muncod').html(resp);
			   			$('#aguardando').hide();
			   		}
			 	});
			}else{
				$('#td_muncod').html('Escolha uma UF.');
			}
		}
	});

	/* Controla situações da reformulação de acordo com o tipo.
	*/
	$('#tipo').change(function(){
		if( $(this).val() != '' ){
			if( $(this).val() == 1 ){ // Prazo
				$('.tipo_subacao').hide();
				$('#escolha_tipo').hide();
				$('.tipo_data').show();
                                //$('#chechedDefault').attr('checked', 'checked');
			}else if( $(this).val() == 2 ){ // Subação
				$('#escolha_tipo').hide();
				$('.tipo_data').hide();
				$('.tipo_subacao').show();
				//$('#chechedDefault').attr('checked', 'checked');
			} else {
				alert("Não disponível no momento");
			}
		}else{
			$('.tipo_data').hide();
			$('.tipo_subacao').hide();
			$('#escolha_tipo').show();
		}
	});

	/* Carrega situações pela primeira vez
	*/
	$('#tipo').change();

	/* Lista DOcumentos com reprogramação de acordo com filtros
	*/
	$('.pesquisar').click(function(){
		$('#aguardando').show();
		$('#pagsituacaopagamento').children().attr('selected',true);
		
		if( $('#tipo').val() == '' ){
			alert('Escolha o tipo de reprogramação.');
			$('#aguardando').hide();
			return false;
		}
		if( $('[name="estado"]::checked').val() == '' ){
			alert('Escolha a situação da reprogramação.');
			$('#aguardando').hide();
			return false;
		}
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=listaProcessos&"+$('#form_reprogramacao_filtro').serialize(),
	   		async: false,
	   		success: function(resp){
	   			$('#lista').html(resp);
	   			$('#aguardando').hide();
	   		}
	 	});
		$('#aguardando').hide();
	});
	
	$('.limpar').click(function(){

		$('#aguardando').show();
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "limpar_filtros=true",
	   		async: false,
	   		success: function(resp){
	   			window.location.href = window.location.href;
	   		}
	 	});
		$('#aguardando').hide();
	});
	
	$('.xls').click(function(){
		
		$('#aguardando').show();
		$('#pagsituacaopagamento').children().attr('selected',true);
		
		if( $('#tipo').val() == '' ){
			alert('Escolha o tipo de reprogramação.');
			$('#aguardando').hide();
			return false;
		}
		if( $('[name="estado"]::checked').val() == '' ){
			alert('Escolha a situação da reprogramação.');
			$('#aguardando').hide();
			return false;
		}
		window.location.href = window.location.href+"&xls=1&req=listaProcessosXLS&"+$('#form_reprogramacao_filtro').serialize();
		$('#aguardando').hide();
	});

	$('.validarReprogramacaoPrazo').live('click',function(){
            var dopid = $(this).attr('id');
            var popup = window.open("par.php?modulo=principal/popupReprogramacaoPARAprovacao&acao=A&dopid="+dopid+"&valida=1","reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
            popup.onbeforeunload = function() { 
                $('.pesquisar').click();
            };
	});

	$('.gerarReprogramacaoPrazo').live('click',function(){

            var dopid = $(this).attr('id');
            var prpid = $(this).attr('prpid');

            var popup = window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid+'&geraTermoReprogramacaoPrazo=1&origem=2','minuta','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
            popup.onbeforeunload = function() { 
                $('.pesquisar').click();
            };
	});
	
	$('.recusarValidacaoPrazo').live('click',function(){

		var dopid = $(this).attr('id');
		var prpid = $(this).attr('prpid');
		var botao = $(this);

		 $.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=recusarValidacaoPrazo&dopid="+dopid,
		   		async: false,
		   		success: function(msg){
		   			$( "#dialog" ).html(msg);
					$( "#dialog" ).dialog({
						resizable: true,
						height:400,
						width:700,
						modal: true,
						buttons: {
							"Recusar": function() {
								if(confirm("Tem certeza que deseja recusar a solicitação?")){
									if( $('[name="dprparecer"]').val() == '' ){
										alert('Informe um Parecer!');
										return false;
									}
									$.ajax({
								   		type: "POST",
								   		url: window.location.href,
								   		data: "req=salvarRecusarValidacaoPrazo&dprparecer="+$('[name="dprparecer"]').val()+'&dopid='+dopid,
								   		async: false,
								   		success: function(resp){
								   			//$('#lista').html(resp);
								   			alert("Solicitação recusada com sucesso! O e-mail foi enviado ao dirigente.");
								   			$(botao).parent().parent().parent().remove();
								   		}
								 	});
									$( this ).dialog( "close" );
								}
							},
							"Fechar": function() {
								$( this ).dialog( "close" );
							}
						}
					});
		   		}
			});		
	});

	$('.listaReprogramacoes').live('click',function(){

		$('#aguardando').show();
		
		var imagem = $(this).attr('src');

		var dopid = $(this).attr('id');
		if( imagem == '/imagens/mais.gif' ){
			var next = $(this).parent().parent().parent();
			
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=listaReprogramacoes&dopid="+dopid,
		   		async: false,
		   		success: function(resp){
		   			$(next).after('<tr id="docPR_'+dopid+'"><td colspan=4>'+resp+'</td></tr>');
		   			$('#aguardando').hide();
		   		}
		 	});

			$(this).attr('src','/imagens/menos.gif');
		}else{
			$(this).attr('src','/imagens/mais.gif');
			$('#docPR_'+dopid).remove();
   			$('#aguardando').hide();
		}
		$('#aguardando').hide();
	});

	$('.listaSubacao').live('click',function(){

		$('#aguardando').show();
		
		var imagem = $(this).attr('src');

		var dopid = $(this).attr('id');
		if( imagem == '/imagens/mais.gif' ){
			var next = $(this).parent().parent();
			
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=listaSubacao&dopid="+dopid+"&estado="+$('[name="estado"]::checked').val(),
		   		async: false,
		   		success: function(resp){
		   			$(next).after('<tr id="docS_'+dopid+'"><td colspan=4>'+resp+'</td></tr>');
		   			$('#aguardando').hide();
		   		}
		 	});

			$(this).attr('src','/imagens/menos.gif');
		}else{
			$(this).attr('src','/imagens/mais.gif');
			$('#docS_'+dopid).remove();
   			$('#aguardando').hide();
		}
		$('#aguardando').hide();
	});

	$('.validarReprogramacaoSubacao').live('click',function(){
            var dopid = $(this).attr('id');
		
            var popup = window.open("par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid="+dopid+"&valida=1","reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
            popup.onbeforeunload = function() { 
                $('.pesquisar').click();
            };
	});

	$('.vizualizarDocumento').live('click', function(){

        var dopid = jQuery(this).attr('id');

        $.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formVizualizaDocumento&dopid="+dopid,
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
							jQuery( this ).dialog( "close" );
						}
					}
				});
	   		}
		});
	});

	/* Esta função é a cópia fiel da requisição da página documentoPar.
	* */
	$('.finalizaReprogramacaoSubacao').live('click', function(){

        var dopid = jQuery(this).attr('id');

        $.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formPeriodoFinalizarRef&dopid="+dopid,
	   		async: false,
	   		success: function(msg){
	   			$( "#dialog" ).html(msg);
				$( "#dialog" ).dialog({
					resizable: true,
					height:180,
					width:400,
					modal: true,
					buttons: {
						"Finalizar": function() {
							$('#aguardando2').show();
							if( $('#mes_inicio').val() == '' ){
								alert('Campo obrigatório.');
								$('#mes_inicio').focus();
								$('#aguardando2').hide();
								return false;
							}
							if( $('#mes_fim').val() == '' ){
								alert('Campo obrigatório.');
								$('#mes_fim').focus();
								$('#aguardando2').hide();
								return false;
							}
							if( $('#ano_inicio').val() == '' ){
								alert('Campo obrigatório.');
								$('#ano_inicio').focus();
								$('#aguardando2').hide();
								return false;
							}
							if( $('#ano_fim').val() == '' ){
								alert('Campo obrigatório.');
								$('#ano_fim').focus();
								$('#aguardando2').hide();
								return false;
							}
							if( parseFloat($('#ano_fim').val()+$('#mes_fim').val()) < parseFloat($('#ano_inicio').val()+$('#mes_inicio').val())){
								alert('Data de fim deve ser maior que a data de início.');
								$('#aguardando2').hide();
								return false;
							}
							finaliza( dopid );
							$('#aguardando2').hide();
							jQuery( this ).dialog( "close" );
						},
						"Atender Automaticamente": function() {
							$('#aguardando2').show();
							$('#mes_inicio').val('');
							$('#mes_fim').val('');
							$('#ano_inicio').val('');
							$('#ano_fim').val('');
							finaliza( dopid );
							$('#aguardando2').hide();
							$( this ).dialog( "close" );
						}

					}
				});
	   		}
		});
	});

	$('.abresubacao').live('click',function(){

		var dados = $(this).attr('id');
		var dados = dados.split('_');
		var sbaid = dados[0];
		var inuid = dados[1];
		
		janela('par.php?modulo=principal/subacao&acao=A&sbaid='+sbaid+'&inuid='+inuid,800,600,'Subação');
	});
        
	$("input[name='dopid']").bind("keyup blur focus", function(e) {
    	e.preventDefault();
        var expre = /[^0-9]/g;
        if ($(this).val().match(expre))
        	$(this).val($(this).val().replace(expre,''));
	});

	$('.acao_fluxo').live('click',function(){

		$('#aguardando').show();
		
		var acao 	= $(this).attr('acao');
		var campo 	= $(this).attr('campo');
		var valor 	= $(this).attr('valor');
		var texto 	= $(this).val();
		var botao 	= $(this);

		if( confirm('Tem certeza que gostaria de '+ texto +'?') ){
			$.ajax({
		        type: 'post',
		        url:  window.location.href,
		        data: '&req='+acao+'&'+campo+'='+valor,
		        async: false,
		        success: function (res){
		        	if( res ){
	        			alert(res);
	        			$(botao).parent().parent().remove();
	        		}else {
	        			alert('Falha no envio! Tente novamente');
	        		}
		        }
		  });
		} 
		$('#aguardando').hide();
	});
        
        <?php
        if (!is_null($tipo)) {
            echo "$('.pesquisar').click();";
        }
    ?>
});
</script>
<div id="dialog" title="Solicitações do Processo" style="display:none;">
</div>
<form name="form_reprogramacao_filtro" id="form_reprogramacao_filtro" method="post" action="" >
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtituloDireita" width="30%"> Tipo da Reprogramação</td>
			<td>
				<?php 
				$sql = "SELECT
							treid as codigo,
							tredsc as descricao
						FROM
							par.tiporeprogramacao
						WHERE
							trestatus = 'A'";
				echo $db->monta_combo('tipo', $sql, 'S', 'Selecione...', '', '', '', 150, 'S', 'tipo', '', $tipo);?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> Situação da Reprogramação</td>
			<td>
				<div id="escolha_tipo">Escolha um Tipo de Reprogramação.</div>
				<div class="tipo_data tipo_subacao">
					<input type="radio" id="chechedDefault" name="estado" value="A" <?=($estado == '' || $estado == 'A' ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Validação do FNDE 
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_data">
					<input type="radio" name="estado" value="B"  <?=(($estado == 'B' && $tipo == '1') ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Geração de Termo 
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_subacao">
					<input type="radio" name="estado" value="B"  <?=(($estado == 'B' && $tipo == '2') ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Conclusão do Município / Estado
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_subacao">
					<input type="radio" name="estado" value="C"  <?=($estado == 'C' ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Aprovação do FNDE 
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_subacao">
					<input type="radio" name="estado" value="G"  <?=($estado == 'G' ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Empenho
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_subacao">
					<input type="radio" name="estado" value="D"  <?=($estado == 'D' ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Geração de Termo 
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_subacao">
					<input type="radio" name="estado" value="F"  <?=($estado == 'F' ? 'checked="checked"' : '') ?>/> &nbsp; Aguardando Finalização de Reprogramação
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
				<div class="tipo_data tipo_subacao">
					<input type="radio" name="estado" value="E"  <?=($estado == 'E' ? 'checked="checked"' : '') ?>/> &nbsp; Reprogramações Finalizadas
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número de Processo:</td>
			<td colspan="3">
			<?php echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '', '', '', '', '', '', $numeroprocesso); ?>
			</td>
	
		</tr>
		<tr>
			<td class="SubTituloDireita">Número do Termo:</td>
			<td colspan="3">
			<?php echo campo_texto( 'dopid', 'N', 'S', '[#]', 20, 50, '', '', '', '', '', '', '', $dopid); ?>
			</td>
	
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> Esfera</td>
			<td>
				<?php
				$sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),Array('codigo'=>'E', 'descricao'=>'Estadual'));
				$db->monta_combo( "esfera", $sql, 'S', 'Todas as esferas', '', '', '', '', '', '', '', $esfera );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> UF</td>
			<td>
				<?php
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '', '', '', '', '', '', $estuf );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '', '', '', '', '', '', $muncod  );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
<!-- 		<tr class="tipo_data"> -->
		<tr>
			<td class="subtituloDireita" width="30%">Fim de Vigência</td>
			<td id="td_muncod">
				De: <?=campo_data2('dt_fim_vigencia_inicio', 'N', 'S', '', 'S', '', '',  $dt_fim_vigencia_inicio); ?>
				Até: <?=campo_data2('dt_fim_vigencia_fim', 'N', 'S', '', 'S' , '', '', $dt_fim_vigencia_fim); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Situação do Pagamento</td>
			<td>
			<?php
				//Ano subação
				$sql = "SELECT DISTINCT 
							pagsituacaopagamento as codigo,
							pagsituacaopagamento as descricao
						FROM par.pagamento 
						WHERE pagstatus = 'A' AND pagsituacaopagamento IS NOT NULL AND pagsituacaopagamento <> ''
						ORDER BY 1";
				combo_popup( 'pagsituacaopagamento', $sql, 'Situações de Pagamento', $tamanho_janela = '400x400', $maximo_itens = 0,
							$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
							$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
				<input type="button" class="xls" value="Exportar XLS"/>
				<input type="button" class="limpar" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>
<div id="lista">
</div>