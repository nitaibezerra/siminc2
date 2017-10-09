<?php
header('content-type: text/html; charset=ISO-8859-1');

include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_novasturmas.php";

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 

$obNovasTurmas = new NovasTurmas( $_REQUEST );

if( $_REQUEST['requisicao'] == 'mostraTurmaAnalise' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	if( $_POST['tipo'] == 'A' ) $arrEsdid = array(WF_NOVASTURMAS_EM_DILIGENCIA, WF_NOVASTURMAS_EM_ANALISE); //APROVADO
	if( $_POST['tipo'] == 'I' ) $arrEsdid = array(WF_NOVASTURMAS_EM_ANALISE); //INDEFERIDO
	if( $_POST['tipo'] == 'D' ) $arrEsdid = array(WF_NOVASTURMAS_EM_ANALISE); //DILIGENCIADO
	if( $_POST['tipo'] == 'C' ) $arrEsdid = array(WF_NOVASTURMAS_EM_ANALISE); //DILIGENCIADO
	
	if( $_POST['tipo'] == 'D' )
	{
		$arTurma = $obNovasTurmas->carregaTurmaPorMunicipio('abaAnalisados');
	}
	else
	{
		$arTurma = $obNovasTurmas->carregaTurmaPorMunicipio();		
	}
	
	$arrRegistro = array();
	foreach ($arTurma as $key => $v) {
		
		if( in_array($v['esdid'], $arrEsdid) ){
			
			$arrDados = array(
										'acao' => "<center><input type=\"checkbox\" name=\"chk[".$v['docid']."]\" class=\"check\" id=\"chk\" value=\"".$v['turid']."\"></center>",
										'turdsc' => $v['turdsc'],
										'tirdescricao' => $v['tirdescricao'],
										'tipoatendimento' => $v['tipoatendimento'],
										'entcodent' => $v['entcodent'],
										'turdtinicio' => $v['turdtinicio'],
										'dtanalise' => $v['dtanalise'],
										'periodo' => $v['periodo'],
										'situacao' => $v['esddsc']
										);
										
			array_push($arrRegistro, $arrDados);
		}
	}
	
	$cabecalho = array("Ação","Nome da Turma","Tipo de Rede","Atendimento","Código INEP","Data Início da Turma", "Data de Envio Análise", "Período de Repasse", "Situação");
	$db->monta_lista_simples($arrRegistro, $cabecalho, 10000000, 30, 'N', '100%', '', true, '', '', true);
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraTurmaAnaliseParecer' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$arTurma = $obNovasTurmas->carregaTurmaPorMunicipio();
	
	$disabled = '';
	$habilita = 'S';
	if( $arTurma[0]['esdid'] != WF_NOVASTURMAS_EM_ANALISE ){
		$disabled = 'disabled="disabled"';
		$habilita = 'N';
	}
	?>
	<table border="0" class="tabela" style="width: 100%" align="center"  bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<th>Dados Análise/Parecer da Turma</th>
	</tr>
	<tr>
		<td>
		<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" style="width: 100%" cellspacing="1" cellpadding="3">
			<tr>
				<td class="subtitulodireita" width="25%"><b>Turma:</b></td>
				<td width="25%"><?=$arTurma[0]['turdsc'] ?></td>
				<td class="subtitulodireita"><b>Data Início:</b></td>
				<td><?=$arTurma[0]['turdtinicio'] ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="25%"><b>Tipo de Rede:</b></td>
				<td width="25%"><?=$arTurma[0]['tirdescricao'] ?></td>
				<td class="subtitulodireita"><b>Atendimento:</b></td>
				<td><?=$arTurma[0]['tipoatendimento'] ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="25%"><b>Data Envio para análise:</b></td>
				<td width="25%" colspan="2"><?=$arTurma[0]['dtanalise'] ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="25%"><b>Valor de Repasse:</b></td>
				<td colspan="3">
					<?
					
					$arrRepasse = $obNovasTurmas->carregaRepassePorTurma();
					
					$arrRegistro = array();
					foreach ($arrRepasse as $key => $v) {
						$arrRegistro[$key] = array(
													'nome' => $v['nome'],
													'total_alunos' => '&nbsp;'.$v['total_alunos'],
													'vaavalor' => $v['vaavalor'],
													'periodo' => $v['periodo'].'&nbsp;',
													'valor_total' => $v['valor_total']
												);
					}
					
					print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';
					print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
								<b>Valor de Repasse para o Município</b></td></tr></table>';
					
					$cabecalho = array('Etapa', "Qtd Alunos", "Valor Unitário", 'Qtd de meses para Repasse dos recursos', 'Valor Total');
					$db->monta_lista_simples($arrRegistro, $cabecalho, 60000, 1, 'S', '100%', 'S', true);
					?>
					</td>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Análise</b></td>
				<td colspan="3">
					<input type="radio" name="anatipo" value="A" <?=($arTurma[0]['anatipo'] == 'A' ? 'checked="checked"' : ''); echo $disabled; ?> id="anatipo_A" /> Aprovada &nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="anatipo" value="I" <?=($arTurma[0]['anatipo'] == 'I' ? 'checked="checked"' : ''); echo $disabled; ?> id="anatipo_I" /> Indeferida &nbsp;&nbsp;&nbsp;&nbsp; 
					<input type="radio" name="anatipo" value="D" <?=($arTurma[0]['anatipo'] == 'D' ? 'checked="checked"' : ''); echo $disabled; ?> id="anatipo_D" /> Diligenciada
				</td>
			</tr>
			<?php 
			if( $habilita == 'S' && $arTurma[0]['anaparecer'] ){
				if( $_POST['btn'] == 'E' ){
					$habilita = 'S';
				} elseif( $_POST['btn'] == 'N' ){
					$habilita = 'S';
					$arTurma[0]['anaparecer'] = '';
				} else {
					$habilita = 'N';
				}
			?>
			<tr>
				<td class="subtitulodireita">&nbsp;</td>
				<td colspan="3">
					<input type="hidden" name="anaid" id="anaid" value="<?php echo $arTurma[0]['anaid'];?>">
					<input type="hidden" name="tipobtn" id="tipobtn" value="<?php echo $_POST['btn'];?>">
					<input type="button" name="tbnNovoParecer" id="tbnNovoParecer" value="Novo Parecer" onclick="habilitaParecer('N', <?php echo $_POST['turid'];?>);">
					<input type="button" name="tbnEditarParecer" id="tbnEditarParecer" value="Editar Parecer" onclick="habilitaParecer('E', <?php echo $_POST['turid'];?>);">
				</td>
			</tr>
			<?php }?>
			<tr>
				<td class="subtitulodireita"><b>Parecer</b></td>
				<td colspan="3"><?=campo_textarea('anaparecer', 'N', $habilita, 'Parecer', 90, 15, 4000, '', '', '', '', 'Parecer', $arTurma[0]['anaparecer']);?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td>
		<input type="button" name="btnGuiaParecer" id="btnGuiaParecer" value="Carregar Guia de Parecer" onclick="guiaParecerAnalise('2', 'anaparecer')"></td>
	</tr>
	<?
	exit();
}

if( $_REQUEST['requisicao'] == 'enviaranalise' ){
	
	if( $_POST['chk'] ){
		
		if( $_POST['tipo'] == 'A' ) $esdiddestino = WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE; //APROVADO
		if( $_POST['tipo'] == 'I' ) $esdiddestino = WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO; //INDEFERIDO
		if( $_POST['tipo'] == 'D' ) $esdiddestino = WF_NOVASTURMAS_EM_DILIGENCIA; //DILIGENCIADO
		if( $_POST['tipo'] == 'C' ) $esdiddestino = WF_NOVASTURMAS_EM_CADASTRAMENTO; //DILIGENCIADO
		
		foreach ($_POST['chk'] as $docid => $turid) {
			$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$docid}";								
			$esdid = $db->pegaUm( $sql );
			
			$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
			$aedid = $db->pegaUm( $sql );
			
			if( $aedid && $docid ){
				$arDados = array('muncod' => $_SESSION['proinfantil']['muncod']);
				
				if(wf_alterarEstado( $docid, $aedid, 'Fluxo Novas Turmas', $arDados )){
					if( $_POST['tipo'] == 'D' ){
						$sql = "select distinct us.usuemail from proinfantil.usuarioresponsabilidade ur
									inner join seguranca.usuario us on us.usucpf = ur.usucpf 
								where ur.muncod = '{$_SESSION['proinfantil']['muncod']}'
									and ur.usucpf = '{$_SESSION['usucpf']}'
									and ur.rpustatus = 'A'
                        			and us.usustatus = 'A'";
						
						$arEmail = $db->carregarColuna($sql);
						
						$resolucao = verificaResolucao();
						
						//$arEmail	= array($_SESSION['email_sistema']);
						$assunto	= 'Turmas em diligência';
						$conteudo 	= '<p>Senhor Prefeito,</p>
<p>&nbsp;</p>
<p>Seu processo referente a&nbsp;&nbsp;"Novas Turmas de Educa&ccedil;&atilde;o Infantil"&nbsp;encontra-se em dilig&ecirc;ncia 
no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o. Para que o processo seja encaminhado para pagamento,&nbsp;&eacute; necess&aacute;rio corrigir as 
informa&ccedil;&otilde;es e enviar novamente para an&aacute;lise.</p>
<p>De acordo com o estabelecido na '.$resolucao['2'].'&nbsp;o 
munic&iacute;pio (ou o DF) ter&aacute; o prazo m&aacute;ximo de 90 (noventa) dias para esclarecer SEB/MEC sobre os estabelecimentos cuja situa&ccedil;&atilde;o esteja 
apresentada no Simec como "em dilig&ecirc;ncia".</p>
<p>&nbsp;</p>
<p>Atenciosamente,&nbsp;</p>
<p>Minist&eacute;rio da Educa&ccedil;&atilde;o</p>';
						
						enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $arEmail, $assunto, $conteudo, $cc, $cco );
					}
					$retorno = '1';
				} else {
					$retorno = 'erro';
				}
			} else {
				$retorno = 'acao';
			}
		}
	} else {
		$retorno = 'erro';
	}
	
	echo $retorno;
	exit();
}

if($_POST['historico']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql_historico = "SELECT 		u.usunome, n.ntapareceraprovacao, to_char(n.ntadata,'DD/MM/YYYY') as data, CASE WHEN n.ntastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END ntastatus
				 	  FROM 			proinfantil.novasturmasanalise n
				  	  LEFT JOIN 	seguranca.usuario u ON u.usucpf = n.usucpf 
				  	  WHERE 		n.muncod = '{$_SESSION['proinfantil']['muncod']}'
				  	  ORDER BY 		n.ntaid DESC";
	$historico = $db->carregar($sql_historico);
	$cab = array('Analista','Parecer','Data','Status');
	$db->monta_lista($sql_historico, $cab, 1000, 10, 'N', '', '', '', '', ''); 
	exit();
}

if( $_REQUEST['requisicao_parecer'] == 'salvarParecerTurma' ){
	$_REQUEST['turid'] = $_REQUEST['turid_parecer'];
	
	$obNovas = new NovasTurmas( $_REQUEST );	
	$docid = $obNovas->criaDocumentoNovasTurmas();
	$esdid = $obNovas->pegaEstadoAtualNovasTurmas($docid);
	
	$arrRepasse = $obNovas->carregaRepassePorTurma();
	$vrlTotal = 0;
	foreach ($arrRepasse as $key => $v) {		
		$valor = str_replace(".","", $v['valor_total']);
		$valor = str_replace(",",".", $valor);
		$vrlTotal += $valor;
	}
	
	if( $esdid == WF_NOVASTURMAS_EM_ANALISE ){
		
		if( $_POST['tipobtn'] != 'N' ){
			$sql = "select turid from proinfantil.analisenovasturmasaprovacao where turid = {$_REQUEST['turid_parecer']}";
			$temturma = $db->pegaUm($sql);
		}
		
		if($temturma){
			
			if( $_REQUEST['anaparecer'] ){
				$filtroPa = ", anaparecer = '{$_REQUEST['anaparecer']}'";
			}
			$sql_turma = "UPDATE proinfantil.analisenovasturmasaprovacao SET anatipo = '{$_REQUEST['anatipo']}' $filtroPa
					  	  	,anaanalisado = TRUE
							WHERE turid = {$_REQUEST['turid_parecer']} and anaid = {$_POST['anaid']}"; 
		} else {
			
			if( $_POST['anaid'] && $_POST['tipobtn'] == 'N' ){
				
				$sql = "UPDATE proinfantil.analisenovasturmasaprovacao SET anastatus = 'I' ,anaanalisado = TRUE WHERE anaid = {$_POST['anaid']}	";
				$db->executar($sql);
			}
			
			$sql_turma = "INSERT INTO proinfantil.analisenovasturmasaprovacao (anatipo, anaparecer, turid, anastatus, usucpf,anaanalisado )
					  		VALUES ('{$_REQUEST['anatipo']}', '{$_REQUEST['anaparecer']}', {$_REQUEST['turid_parecer']}, 'A', '{$_SESSION['usucpf']}', TRUE)";
		}
	
		if(	$db->executar($sql_turma)){
			$sql = "UPDATE proinfantil.turma SET turvalorrepasse = $vrlTotal WHERE  turid = {$_REQUEST['turid']}";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('novasturmas/analiseTurmas');
		} else {
			$db->rollback();
			echo "<script>
					alert('Falha na operação');
					window.location.href = window.location; 
				</script>";
		}
	} else {
		echo "<script>
					alert('Não e possível salvar dados da turma, pois a turma não está em analise.');
					window.location.href = window.location; 
				</script>";
	}
	exit();
}

if( $_REQUEST['requisicao'] == 'parecerDevolverAnalise' ){
	
	$sql = "select
				tur.turid,
				doc.docid
			from 
				proinfantil.turma tur 
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			    inner join workflow.documento doc on doc.docid = ntw.docid
			where tur.muncod = '{$_REQUEST['muncod']}'
				and doc.esdid = ".WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE;
	
	$arrTurma = $db->carregar($sql);
	
	foreach ($arrTurma as $turma) {
										
		$esdid 			= WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE;
		$esdiddestino 	= WF_NOVASTURMAS_EM_ANALISE;
		$docid 			= $turma['docid'];
		$turid 			= $turma['turid'];
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
		$aedid = $db->pegaUm( $sql );
		
		if( $aedid && $docid ){
			$arDados = array('muncod' => $_SESSION['proinfantil']['muncod']);
			
			if(wf_alterarEstado( $docid, $aedid, 'Fluxo Novas Turmas', $arDados )){
				$sql = "INSERT INTO proinfantil.novasturmasanaliseaprovacao(turid, usucpf, ntuapareceraprovacao, ntuastatus, esdid) 
						VALUES ($turid, '{$_SESSION['usucpf']}', '{$_POST['ntuapareceraprovacao']}', 'A', $esdiddestino)";
				$db->executar($sql);
			} else {
				$retorno = 'erro';
			}
		}
		
	}
	
	$db->commit();
	$db->sucesso('novasturmas/analiseTurmas');
	exit();
}

if( $_REQUEST['requisicao'] == 'parecerAprovacao' ){
	
	$sql = "select
				tur.turid,
				doc.docid
			from 
				proinfantil.turma tur 
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			    inner join workflow.documento doc on doc.docid = ntw.docid
			where tur.muncod = '{$_REQUEST['muncod']}'
				and doc.esdid = ".WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE;
	
	$arrTurma = $db->carregar($sql);
	
	foreach ($arrTurma as $turma) {
										
		$esdid 			= WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE;
		$esdiddestino 	= WF_NOVASTURMAS_EM_APROVADA;
		$docid 			= $turma['docid'];
		$turid 			= $turma['turid'];
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
		$aedid = $db->pegaUm( $sql );
		
		if( $aedid && $docid ){
			$arDados = array('muncod' => $_SESSION['proinfantil']['muncod']);
			
			if(wf_alterarEstado( $docid, $aedid, 'Fluxo Novas Turmas', $arDados )){
				$sql = "INSERT INTO proinfantil.novasturmasanaliseaprovacao(turid, usucpf, ntuapareceraprovacao, ntuastatus, esdid) 
						VALUES ($turid, '{$_SESSION['usucpf']}', '{$_POST['ntuapareceraprovacao']}', 'A', $esdiddestino )";
				$db->executar($sql);
			} else {
				$retorno = 'erro';
			}
		}
		
	}
	
	$db->commit();
	$db->sucesso('novasturmas/analiseTurmas');
	exit();
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$perfis = pegaPerfil($_SESSION['usucpf']);

// Monta abas
$db->cria_aba( $abacod_tela, $url, '' );

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Análise', $titulo2);
cabecalhoTurma();

if($_SESSION['proinfantil']['muncod']){
	$sql = "SELECT 	ntaid, muncod, ntamesesrepasse, ntapareceraprovacao 
			FROM 	proinfantil.novasturmasanalise 
			WHERE	muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND 	ntastatus = 'A'";
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);	
}

unset( $_SESSION['proinfantil']['popup'] );

/*$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);*/

$acesso = ($acesso['analise'] = 'S'); //verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$arMes = array(
			'Janeiro',
			'Fevereiro',
			'Março',
			'Abril',
			'Maio',
			'Junho',
			'Julho',
			'Agosto',
			'Setembro',
			'Outubro',
			'Novembro',
			'Dezembro'
			);
?>
<style type="text/css">
.div_historico {
	background: #white;
}
.div_historico:hover {
	background: #eeeeaa;
}

.div_turma {
	background: #white;
}
.div_turma:hover {
	background: #eeeeaa;
}

.tr_Titulo{
	background-color: #ADD8E6;
}

.tr_Titulo1{
	background-color: #FFA500;
	width: 12.5%;
}

.tr_Mes{
	background-color: #7BA2B6;
	width: 10%;
}

.tr_Matricula{
	background-color: #FABF40;
	width: 12%;
}

</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />

<form name="formulario" id="formulario_parecer" method="post" action="">
<input type="hidden" name="requisicao" value="salvarParecer" />
<table border="0" align="center" class="tabela" cellspacing="1" cellpadding="3">	
<tr>
	<td colspan="2">
	<table align="center" style="width: 100%"  cellspacing="0" cellpadding="3">
	<tr>
		<td style="width: 50%">
			<table class="tabela" align="center" style="width: 100%"  cellspacing="1" cellpadding="3">
				<tr>
					<th colspan="10" class="tr_Titulo">Matrículas - Cálculos dos Dados do Município</th>
				</tr>
				<tr>
					<th rowspan="2" class="tr_Mes" width="10%">Meses</th>
					<th colspan="2" class="tr_Titulo1" style="width: 22.5%">Creche Integral</th>
					<th colspan="2" class="tr_Titulo1" style="width: 22.5%">Creche Parcial</th>
					<th colspan="2" class="tr_Titulo1" style="width: 22.5%">Pré-Escola Integral</th>
					<th colspan="2" class="tr_Titulo1" style="width: 22.5%">Pré-Escola Parcial</th>
					<th rowspan="2" class="tr_Titulo1">Total Ampliado</th>
				</tr>
				<tr>
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
					
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
					
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
					
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
				</tr>
<?
$arrDados = $obNovasTurmas->carregaDadosMatriculaTurmaMunicipio();

foreach ($arrDados as $v) {
	$mes = $arMes[$v['ntmmmes'] - 1];
	
	$arrPost = array(
				'muncod' => $v['muncod'],
				'ntmmmes' => $v['ntmmmes'],
				); 
	
	$obMatricula = new NovasTurmas($arrPost);
	$arrMatricula = $obMatricula->carregaDadosMatriculaMaiorPorMes();
		
	$crecheIntegralPublica 			= ((int)$v['ntmmqtdmatriculacrecheintegralpublica'] - (int)$arrMatricula['ntmmqtdmatriculacrecheintegralpublica']);
	$crecheIntegralConveniada 		= ((int)$v['ntmmqtdmatriculacrecheintegralconveniada'] - (int)$arrMatricula['ntmmqtdmatriculacrecheintegralconveniada']);
	$crecheParcialPublica 			= ((int)$v['ntmmqtdmatriculacrecheparcialpublica'] - (int)$arrMatricula['ntmmqtdmatriculacrecheparcialpublica']);	
	$crecheParcialConveniada 		= ((int)$v['ntmmqtdmatriculacrecheparcialconveniada'] - (int)$arrMatricula['ntmmqtdmatriculacrecheparcialconveniada']);	
	$PreEscolaIntegralPublica 		= ((int)$v['ntmmqtdmatriculapreescolaintegralpublica'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaintegralpublica']);	
	$PreEscolaIntegralConveniada 	= ((int)$v['ntmmqtdmatriculapreescolaintegralconveniada'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaintegralconveniada']);	
	$PreEscolaParcialPublica 		= ((int)$v['ntmmqtdmatriculapreescolaparcialpublica'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaparcialpublica']);
	$PreEscolaParcialConveniada 	= ((int)$v['ntmmqtdmatriculapreescolaparcialconveniada'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaparcialconveniada']);
	
	$totalAmpliacaoMat = ($crecheIntegralPublica + $crecheIntegralConveniada + $crecheParcialPublica + $crecheParcialConveniada + $PreEscolaIntegralPublica + $PreEscolaIntegralConveniada + $PreEscolaParcialPublica + $PreEscolaParcialConveniada);
	
	if( (int)$totalAmpliacaoMat > 0 ) $totalAmpliacaoMat = '<span style="color:blue;">'.$totalAmpliacaoMat.'</span>';
	?>
				<tr>
					<td class="tr_Mes" align="right"><b><?=$mes; ?></b></td>
					<td class="tr_Matricula"><?=($crecheIntegralPublica) ?></td>
					<td class="tr_Matricula"><?=($crecheIntegralConveniada) ?></td>
					<td class="tr_Matricula"><?=($crecheParcialPublica) ?></td>
					<td class="tr_Matricula"><?=($crecheParcialConveniada) ?></td>
					<td class="tr_Matricula"><?=($PreEscolaIntegralPublica) ?></td>
					<td class="tr_Matricula"><?=($PreEscolaIntegralConveniada) ?></td>
					<td class="tr_Matricula"><?=($PreEscolaParcialPublica) ?></td>
					<td class="tr_Matricula"><?=($PreEscolaParcialConveniada) ?></td>
					<td class="tr_Matricula" style="text-align: center; font-weight: bold"><?=$totalAmpliacaoMat; ?></td>
				</tr>
<?} ?>
			</table>
		</td>
		<td style="width: 50%">
			<table class="tabela" align="center" style="width: 100%"  cellspacing="1" cellpadding="3">
				<tr>
					<th colspan="10" class="tr_Titulo">Turmas - Cálculos dos Dados do Município</th>
				</tr>
				<tr>
					<th rowspan="2" class="tr_Mes">Meses</th>
					<th colspan="2" class="tr_Titulo1">Creche</th>
					<th colspan="2" class="tr_Titulo1">Pré-Escola</th>
					<th colspan="2" class="tr_Titulo1">Unificada</th>
					<th rowspan="2" class="tr_Titulo1">Total Ampliado</th>
				</tr>
				<tr>
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
					
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
					
					<th class="tr_Titulo1">Pública</th>
					<th class="tr_Titulo1">Conveniadas</th>
				</tr>
<?
foreach ($arrDados as $v) {
	$mes = $arMes[$v['ntmmmes'] - 1];
	
	$arrPost = array(
				'muncod' => $v['muncod'],
				'ntmmmes' => $v['ntmmmes'],
				); 
	
	$obMatricula = new NovasTurmas($arrPost);
	$arrMatricula = $obMatricula->carregaDadosTurmaMaiorPorMes();
	//ver($arrDados, $arrMatricula,d);
	
	$turmaPublica 			= ((int)$v['ntmmqtdturmacrechepublica'] - (int)$arrMatricula['ntmmqtdturmacrechepublica']);
	$turmaConveniada 		= ((int)$v['ntmmqtdturmacrecheconveniada'] - (int)$arrMatricula['ntmmqtdturmacrecheconveniada']);	
	$preEscolaPublica 		= ((int)$v['ntmmqtdturmapreescolapublica'] - (int)$arrMatricula['ntmmqtdturmapreescolapublica']);	
	$preEscolaConveniada 	= ((int)$v['ntmmqtdturmapreescolaconveniada'] - (int)$arrMatricula['ntmmqtdturmapreescolaconveniada']);	
	$unificadaPublica 		= ((int)$v['ntmmqtdturmaunificadapublica'] - (int)$arrMatricula['ntmmqtdturmaunificadapublica']);
	$unificadaConveniada 	= ((int)$v['ntmmqtdturmaunificadaconveniada'] - (int)$arrMatricula['ntmmqtdturmaunificadaconveniada']);
	
	$totalAmpliacao = ((int)$turmaPublica + (int)$turmaConveniada + (int)$preEscolaPublica + (int)$preEscolaConveniada + (int)$unificadaPublica + (int)$unificadaConveniada);
	
	if( (int)$totalAmpliacao > 0 ) $totalAmpliacao = '<span style="color:blue;">'.$totalAmpliacao.'</span>';
	?>
				<tr>
					<td class="tr_Mes" align="right"><b><?=$mes; ?></b></td>
					<td class="tr_Matricula"><?=($turmaPublica); ?></td>
					<td class="tr_Matricula"><?=($turmaConveniada); ?></td>
					<td class="tr_Matricula"><?=($preEscolaPublica); ?></td>
					<td class="tr_Matricula"><?=($preEscolaConveniada); ?></td>
					<td class="tr_Matricula"><?=($unificadaPublica); ?></td>
					<td class="tr_Matricula"><?=($unificadaConveniada); ?></td>
					<td class="tr_Matricula" style="text-align: center; font-weight: bold"><?=$totalAmpliacao; ?></td>
				</tr>
<?} ?>
			</table>
		</td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td colspan="2">
	<table class="tabela" align="center" style="width: 100%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<tr>
		<td class="subtituloDireita">Situação workflow:</td>
		<td>
			<?php 
			$sql = "SELECT 		esdid as codigo,
								esddsc as descricao
					FROM		workflow.estadodocumento
					WHERE		tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS;
			$esdidfiltro = $_POST['esdidfiltro'];
			$db->monta_combo('esdidfiltro', $sql, 'S', 'Selecione..', '', '', '', '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Situação Analise:</td>
		<td>
			<?php
					
			$arrTipoAna = array(
							array('codigo' => 'A', 'descricao' => 'Aprovada'),
							array('codigo' => 'I', 'descricao' => 'Indeferida'),
							array('codigo' => 'D', 'descricao' => 'Diligenciada'),
							);
			$anatipofiltro = $_POST['anatipofiltro'];
			$db->monta_combo('anatipofiltro', $arrTipoAna, 'S', 'Selecione..', '', '', '', '');
			?>
		</td>
	</tr>
	<tr>
			<td class="subtituloesquerda" colspan="2" style="text-align: center;">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisaSituacao();"/>
				<input type="button" value="Limpar" id="btnLimpar" onclick="javascript: window.location.href = window.location" />
			</td>
		</tr>
	</table>
	</td>
</tr>

<tr>
	
		<?php
			$_REQUEST['abas'] = (empty($_REQUEST['abas']) ? 'listaAbaAguardandoAnalise' : $_REQUEST['abas']);
		
			include_once APPRAIZ . "proinfantil/modulos/novasturmas/{$_REQUEST['abas']}.inc";
		?>
	
		<tr>
			<th colspan="2">Valor de Repasse para o Município</th>
		</tr>
		<tr>
	    	<td class="subtitulodireita">Valor Total:</td>
	    	<td width="50%"><?php echo number_format($valor_total,2,",",".") ;?></td>
	    </tr>
	    <tr>
	    	<td colspan="2">
				<table border="0" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<th>Obras do Proinfância</th>
					</tr>
					<tr>
						<td>
						<?
						$sql = "SELECT
									'<div style=\"white-space: nowrap\" ><img class=\"link\" src=\"../imagens/alterar.gif\" onclick=\"editarProInfantil(\'' || oi.obrid || '\')\" /></div>' as acao,
							        upper(oi.obrnome) as Nome_Da_Obra,
							        tp.tobdesc as TipodeObra,
							        COALESCE(tpl.tpodsc,'N/A') as TipologiaObra,
							        edoc.esddsc as descricaowork
							    FROM
									obras2.obras AS oi
					                INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
									INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
									INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
					                INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
					                INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND
					                								 oo.orgstatus = 'A'                                                 
					                INNER JOIN workflow.documento d1 ON d1.docid = oi.docid
					                INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
					                INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
					                INNER JOIN obras2.tipoobra AS tp ON oi.tobid = tp.tobid
					                LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
					                LEFT JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid
					                LEFT JOIN (SELECT 		q.pinid, r.resdsc
					                           FROM 		proinfantil.questionario q
					                           INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
					                           WHERE	 	r.perid = 1587 ) as dt ON dt.pinid = pi.pinid				
					                LEFT JOIN workflow.documento d ON d.docid = pi.docid
					                LEFT JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
								WHERE
								    oi.obrstatus = 'A' AND
								    ee.entstatus = 'A' AND
								    pf.prfid = 41 AND
								    oi.obrpercentultvistoria >= 90 AND
					                esd1.esdid IN (690, 693) and
								    oi.obridpai IS NULL
								    and tm.muncod = '{$_SESSION['proinfantil']['muncod']}' 
					                and tpl.tpoid in (16,9,10)
								ORDER BY
								    oi.obrnome";
						
						$cabecalho = array("Ação", "Nome da Obra", "Tipo da Obra", "Tipologia", "Situação do Plano");	
						$db->monta_lista_simples($sql, $cabecalho, 50000, 10, 'N', '100%', '', false, false, false, true );
						?>
						</td>
					</tr>
				</table>
			</td>
	    </tr>
	    <tr>
	    	<td>&nbsp;</td>
	    </tr>
	    <?if( $boMostraParecer ){?>
		<tr>
			<th colspan="2">Parecer de Aprovação</th>
		</tr>
		<tr>
			<td colspan="2">
				<table border="0"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td align="center" >
				    		<?=campo_textarea('ntuapareceraprovacao', 'S', 'S', '', 150, 10, '', '', '', '', false, '', '', array('id'=>'ntuapareceraprovacao')); ?>
				    	</td>
				    	<td align="left" >
				    		<img src="../imagens/consultar.gif" style="cursor:pointer;vertical-align: top" onclick="guiaParecerNovasTurmas('2', '1', 'ntuapareceraprovacao')">
				    	</td>
				    </tr>
				    <tr>
				    	<td colspan="2" align="center">
				    		<input type="button" value="Devolver para Análise" class="btnDevolverAnalise" />
				    		<input type="button" value="Enviar para Aprovado" class="btnEnviarAprovado" />
				    	</td>
				    </tr>
				</table>
			</td>
		</tr>
		<?} ?>
	</table>
	</td>
	<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( PERFIL_ANALISTA, $perfis )) ){  ?>
	<td  align="center" valign="top">
			<br>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Análise de Turmas"><strong>Análise de Turmas</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="enviarTurmasAnalise('A');" title="Enviar Turmas para Aprovado">Enviar Turmas para Validação da Análise</a>
					</td>
				</tr>
			</table>
			<br><br>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Análise de Turmas"><strong>Análise de Turmas</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="enviarTurmasAnalise('I');" title="Enviar Turmas para Indeferida ">Enviar Turmas para Indeferido</a>
					</td>
				</tr>
			</table>
			<br><br>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Análise de Turmas"><strong>Análise de Turmas</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="enviarTurmasAnalise('D');" title="Enviar Turmas para Diligenciada">Enviar Turmas para Diligenciada</a>
					</td>
				</tr>
			</table>
			<br><br>
			<?if( $_SESSION['usucpforigem'] == '' || $_SESSION['usucpforigem'] == '' ){ ?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Retornar Turmas"><strong>Retornar Turmas</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="enviarTurmasAnalise('C');" title="Retornar Turmas para Cadastramento">Retornar Turmas para Cadastramento</a>
					</td>
				</tr>
			</table>
			<?} ?>
		</td>
		<?} ?>
</tr>
</table>
</form>
<div id="debug"></div>
<div id="dialog_turma" title="Lista de Turmas" style="display: none; text-align: center;">
	<form name="formTurmaCad" id="formTurmaCad" action="" enctype="multipart/form-data" method="post">
		<div style="padding:5px;text-align:justify;" align="left"><input class="marcar-todos" marcar="check" type="checkbox" name="todos" id="todos" value=""><b>Selecionar Todos</b></div>
		<div style="padding:5px;text-align:justify;" id="mostraTurma"></div>
	</form>
</div>
<div id="dialog_parecer" title="Dados do Parecer/Análise" style="display: none; text-align: center;">
	<form name="formTurmaAnalise" id="formTurmaAnalise" action="" enctype="multipart/form-data" method="post">
		<input type="hidden" name="turid_parecer" id="turid_parecer" value="">
		<input type="hidden" name="requisicao_parecer" id="requisicao_parecer" value="" />
		<div style="padding:5px;text-align:justify;" id="turmaAnalise"></div>
	</form>
</div>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<?php 
$sql_historico = "SELECT 		u.usunome, n.ntapareceraprovacao, to_char(n.ntadata,'DD/MM/YYYY') as data, CASE WHEN n.ntastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END ntastatus
				  FROM 			proinfantil.novasturmasanalise n
				  LEFT JOIN 	seguranca.usuario u ON u.usucpf = n.usucpf 
				  WHERE 		n.muncod = '{$_SESSION['proinfantil']['muncod']}'
				  ORDER BY 		n.ntaid DESC";

$historico = $db->pegaLinha($sql_historico);

if(!empty($historico) && $_SESSION['exercicio'] < 2013 ){?>
<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
	<tr>
		<td style="background-color: #CDCDCD;">
			<div id="hist" class="div_historico" style="float:left;cursor:pointer;width:100%">
				<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmaishist">
				<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenoshist">
				<center><b>Histórico de Pareceres</b></center>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div id="td_hist" style="display:none;"></div>
		</td>
	</tr>	
</table>

<br>
<?} ?>
<script type="text/javascript" src="/estrutura/js/funcoes.js"></script>
<script>
	function pesquisaSituacao(){
		$('[name="requisicao"]').val('filtrar');
		$('[name="formulario"]').submit();
	}
	function editarProInfantil(obrid){
		var url = "?modulo=principal/questionarioMunicipio&acao=A&popup=true&obrid="+obrid;
		w = window.open(url, 'popproinfantil','scrollbars=yes,location=no,toolbar=no,menubar=no,width=1000,height=700'); 
		w.focus();
	}

	$(document).ready(function(){
		$('.tr_Matricula').each(function(){
			if( Number($(this).html()) ){
				if( $(this).html() < 0 ){
					$(this).css('color', 'red');
				}
			}
		});
	});

	function guiaParecerAnalise(modid, campo){
		var tipid = '';
		if( $('[name="anatipo"]:checked').val() == 'A' ) var tipid = 1;
		if( $('[name="anatipo"]:checked').val() == 'I' ) var tipid = 3;
		if( $('[name="anatipo"]:checked').val() == 'D' ) var tipid = 2;
		
		if( tipid != '' ){
			guiaParecerNovasTurmas(modid, tipid, campo);
		} else {
			alert('Informe o tipo de análise');
			return false;
		}
	}
		
	function guiaParecerNovasTurmas(modid, tipid, campo){		
		w = window.open('?modulo=novasturmas/guiaParecerNovasTurmas&acao=A&modid='+modid+'&tipid='+tipid+'&campo='+campo,'guiaparecer','scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=400,left=250,top=125'); 
		w.focus();	
	}
	
	function abrirTurma(turid){
		var html = ('<tr id="linhaTurma'+turid+'"><td colspan="10" ><div id="listaTurma'+turid+'"></div></td></tr>');
		$.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=novasturmas/repasseTurma&acao=A",
			data: {turid:turid},
			async: false,
			success: function(data){
				
				//console.log(data);
				
				if($('#linhaTurma'+turid).html() == null){
					
				 	$('#imgmais'+turid).parent("center").parent("td").parent("tr").after(html);
				 }
				 $("#listaTurma"+turid).html(data);
				 $("#linhaTurma"+turid).show();
				 $('#imgmais'+turid).hide();
				 $('#imgmenos'+turid).show();
			}
		});
	}
	
	function fecharTurma(turid){
		$('#linhaTurma'+turid).hide();
		$('#imgmais'+turid).show();
		$('#imgmenos'+turid).hide();
	}

	$(function(){
		function aguarda(val){
			if(val){
				$('#aguardando').show();
			}else{
				$('#aguardando').hide();
			}
		}
		$('.div_historico').click(function(){
			aguarda(true);
			var id = $(this).attr('id');
			if($('#td_'+id).html() != ''){
				if( $('#td_'+id).css('display') == 'none' ){
					$('#td_'+id).show();
					$('#imgmais'+id).hide();
					$('#imgmenos'+id).show();
				} else {
					$('#td_'+id).hide();
					$('#imgmais'+id).show();
					$('#imgmenos'+id).hide();
				}
				aguarda(false);
			} else {
				$.ajax({
					type: "POST",
					url: 'proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A',
					data: "historico=true&id="+id,
					async: false,
					success: function(msg){
						$('#td_'+id).html(msg);
						$('#td_'+id).show();
						$('#imgmais'+id).hide();
						$('#imgmenos'+id).show();					
						aguarda(false);
					}
				});
			}
		});
		aguarda(false);
	});
	
	function abrirTurmaPopup(turid, muncod){
		return window.open('proinfantil.php?modulo=novasturmas/mostraDadosTurma&acao=A&turma='+turid+'&muncod='+muncod,'modelo',"fullscreen=yes,scrollbars=yes");
	}
	
	function abrirParecerTurma(turid, muncod){
		return window.open('proinfantil.php?modulo=novasturmas/mostraParecerTurma&acao=A&turma='+turid+'&muncod='+muncod,'modelo',"fullscreen=yes,scrollbars=yes");
	}

	$(function(){
		$('.btnDevolverAnalise').click(function(){
			if($('[name=ntuapareceraprovacao]').val() == ''){
				alert('O campo descrição da análise é obrigatório!');
				$('[name=ntuapareceraprovacao]').focus();
				return false;
			}
			$('[name=requisicao]').val('parecerDevolverAnalise');	
			$('#formulario_parecer').submit();
		});
		$('.btnEnviarAprovado').click(function(){
			
			if($('[name=ntuapareceraprovacao]').val() == ''){
				alert('O campo descrição da análise é obrigatório!');
				$('[name=ntuapareceraprovacao]').focus();
				return false;
			}	
			$('[name=requisicao]').val('parecerAprovacao');
			$('#formulario_parecer').submit();
		});
	});
	
	/*function salvarParecerTurma(turid){
	
		if($('[name=rdn_turid'+turid+']:checked').length == ''){
			alert('Informe a Análise.');
			//$('[name=rdn_turid'+turid+']:checked').focus();
			return false;
		}	
		
		if($('#parecer'+turid).val() == ''){
			alert('Informe o Parecer.');
			$('#parecer').focus();
			return false;
		}
		$.ajax({
			type: "POST",
			url: 'proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A',
			data: { salvar_parecer_turma: "true", turma: turid, parecer: $('#parecer'+turid).val(), analise: $('[name=rdn_turid'+turid+']:checked').val()},
			async: false,
			success: function(retornoajax) {
			if(retornoajax == 1){
				alert("Parecer cadastrado!");
			} else {
				alert("Parecer não cadastrado!");
			}	
		}});
	}*/
	
	var texto = "";
	function enviarTurmasAnalise( tipo ){
		
		if( tipo == 'A' ) texto = 'Enviar para Validação da Análise';
		if( tipo == 'I' ) texto = 'Enviar para Indeferido';
		if( tipo == 'D' ) texto = 'Enviar para Diligenciado';
		if( tipo == 'C' ) texto = 'Retornar para Cadastramento';
		
		$.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A",
			data: "requisicao=mostraTurmaAnalise&tipo="+tipo,
			async: false,
			success: function(msg){
				$( "#dialog_turma" ).show();
				$( "#mostraTurma" ).html(msg);
				$( '#dialog_turma' ).dialog({
						resizable: false,
						width: 900,
						modal: true,
						show: { effect: 'drop', direction: "up" }
				});
				
				$( "#dialog_turma" ).dialog({ buttons: [ 
					{ text: texto, click: function() { 
							if( Number($('#chk:checked').val())){
								enviaTurma(tipo);
								$( this ).dialog( 'close' );
							} else {
								alert('Selecione pelo menos uma turma!');
							}
							 
						} }, 
					{ text: 'Cancelar', click: function() { $( this ).dialog( "close" ); } }					
					] }
				);
			}
		});
	}
	
	function inserirAnaliseTurma( turid, tipo ){
		
		$.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A",
			data: "requisicao=mostraTurmaAnaliseParecer&turid="+turid+"&btn="+tipo,
			async: false,
			success: function(msg){
				$( "#dialog_parecer" ).show();
				$( "#turmaAnalise" ).html(msg);
				$( '#dialog_parecer' ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
						'Salvar': function() {
							
							if( !$('[name="anatipo"]:checked').val() ){
								alert('O campo "Tipo de Análise" é obrigatório.');
								return false;
							} else if( !$('[name="anaparecer"]').val() ){
								alert('O campo "Parecer" é obrigatório.');
								$('[name="anaparecer"]').focus();
								return false;
							} else {
								salvarParecerPorTurma( turid );
							}
						},
						'Cancelar': function() {
							$( this ).dialog( 'close' );
						}
					}
				});
			}
		});
	}
	
	function salvarParecerPorTurma( turid ){
		$('#requisicao_parecer').val('salvarParecerTurma');
		$('#turid_parecer').val(turid);
		$('#formTurmaAnalise').submit();
	}
	
	function enviaTurma( tipo ){
		var dados = $('#formTurmaCad').serialize();
		
		if( tipo == 'A' ) texto = 'APROVADO';
		if( tipo == 'I' ) texto = 'INDEFERIDO';
		if( tipo == 'D' ) texto = 'DILIGENCIADO';
		if( tipo == 'C' ) texto = 'CADASTRAMENTO';
		
		$.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A",
			data: "requisicao=enviaranalise&"+dados+"&tipo="+tipo,
			async: false,
			success: function(msg){
				//$( "#debug" ).html(msg);
				if(msg == 1){
					alert('Turmas em \"ANALISE\"enviadas para \"'+texto+'\" com sucesso!');
				} else {
					alert('Falha na operação. Turmas não enviadas para '+texto+'!');
				}
				window.location.href = window.location;
			}
		});
	}
	
	function carregaParecer(parid, tipid){
		if( tipid == 1 ){
			$('#anatipo_A').attr( 'checked', true );
			$('#anatipo_I').attr( 'checked', false );
			$('#anatipo_D').attr( 'checked', false );
		} else if( tipid == 2 ){
			$('#anatipo_D').attr( 'checked', true );
			$('#anatipo_I').attr( 'checked', false );
			$('#anatipo_A').attr( 'checked', false );
		} else {
			$('#anatipo_I').attr( 'checked', true );
			$('#anatipo_D').attr( 'checked', false );
			$('#anatipo_A').attr( 'checked', false );
		}
		
		$('#anaparecer').val( $('#div_'+parid).html() );
	}

	function habilitaParecer( tipo, turid){
		$( '#dialog_parecer' ).dialog( 'close' );		
		inserirAnaliseTurma(turid, tipo);
	}
	
</script>