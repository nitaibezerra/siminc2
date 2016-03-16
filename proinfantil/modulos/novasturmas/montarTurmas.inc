<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);

header('content-type: text/html; charset=ISO-8859-1');

include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once "_funcoes_novasturmas.php";

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 

$obNovasTurmas = new NovasTurmas( $_REQUEST );

$mesAtual = date('m');

include_once APPRAIZ . "includes/workflow.php";

if( $_REQUEST['requisicao'] == 'verificaTtdTurma' ){
	
	//$arrMes = $db->carregarColuna("select distinct ntmmmes from proinfantil.novasturmasdadosmunicipiospormes where muncod = '{$_REQUEST['muncod']}' order by ntmmmes asc");
	$arrMes = $db->carregar("SELECT distinct turmes as mes, turano, count(turid) as total FROM proinfantil.turma WHERE turid in (".implode(', ', $_REQUEST['chk']).") 
							group by turmes, turano 
							order by turmes desc");
	
	$sql = "SELECT ntmmid, muncod, ntmmstatus, ntmmano, ntmmmes, ntmmqtdmatriculacrecheparcialpublica, ntmmqtdmatriculacrecheintegralpublica,
  				ntmmqtdmatriculapreescolaparcialpublica, ntmmqtdmatriculapreescolaintegralpublica, ntmmqtdmatriculacrecheparcialconveniada,
  				ntmmqtdmatriculacrecheintegralconveniada, ntmmqtdmatriculapreescolaparcialconveniada, ntmmqtdmatriculapreescolaintegralconveniada,
  				ntmmqtdturmacrechepublica, ntmmqtdturmapreescolapublica, ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
  				ntmmqtdturmapreescolaconveniada, ntmmqtdturmaunificadaconveniada
			FROM proinfantil.novasturmasdadosmunicipiospormes WHERE ntmmano = '{$_SESSION['exercicio']}' and ntmmstatus = 'A' and muncod = '{$_REQUEST['muncod']}' and ntmmstatus = 'A'
			order by ntmmmes desc";
		
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	$temAmpliacao = 0;
	
	$arrTotTurmaMes = array();
	foreach ($arrDados as $v) {
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
		
		$arrTotTurmaMes[$v['ntmmmes']] = $totalAmpliacao;
		$temAmpliacao += $totalAmpliacao;
	}
	
	$totDisponivel = 0;
	$arrRetorno = array();
	if( $temAmpliacao > 0 ){
		foreach ($arrMes as $arMes) {
			$arrPost = array(
						'muncod' => $_REQUEST['muncod'],
						'ntmmmes' => ((int)$arMes['mes'])
						);
			$obNovasTurmas = new NovasTurmas( $arrPost );
		
			$arrMatriculaMaior 		= $obNovasTurmas->carregaDadosMatriculaMaiorPorMes();
			$arrTurmaMaior 			= $obNovasTurmas->carregaDadosTurmaMaiorPorMes();
			$arrMatTurmaCadastrada 	= $obNovasTurmas->carregaDadosMatriculaTurmaMes();
			
			$sql = "SELECT count(tur.turid)
					FROM proinfantil.turma tur
					    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
					    inner join workflow.documento doc on doc.docid = ntw.docid
					    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
					WHERE
					    tur.muncod = '".$_REQUEST['muncod']."'
					    and tur.turmes = '{$arMes['mes']}'
					    and tur.turano = '{$_SESSION['exercicio']}'
					    and tur.turstatus = 'A'
					    and doc.esdid not in (".WF_NOVASTURMAS_EM_CADASTRAMENTO.") ";
			$QtdTurmaEnviada = $db->pegaUm($sql);
			
			#Quantidade de Matricula
			$totCrecPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialpublica'];
			$totCrecConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialconveniada'];
		
			$totPreEPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialpublica'];
			$totPreEConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialconveniada'];
		
			$totCrecPublicaM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheintegralpublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheparcialpublica'];
			$totCrecConveniM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheparcialconveniada'];
		
			$totPreEPublicaM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaparcialpublica'];
			$totPreEConveniM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaparcialconveniada'];
		
			#quantidade de Turmas
			$qtdTurmaCenso = (int)$arrTurmaMaior['ntmmqtdturmacrechepublica'] + (int)$arrTurmaMaior['ntmmqtdturmacrecheconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmapreescolapublica'] +
								(int)$arrTurmaMaior['ntmmqtdturmapreescolaconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadapublica'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadaconveniada'];
		
			$qtdTurmaInformada = (int)$arrMatTurmaCadastrada['ntmmqtdturmacrecheconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolapublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolaconveniada'] +
								(int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadapublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadaconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmacrechepublica'];
			
			$totGeralCenso 		= (int)$totCrecPublicaC + (int)$totCrecConveniC + (int)$totPreEPublicaC + (int)$totPreEConveniC;
			$totGeralMatricula 	= (int)$totCrecPublicaM + (int)$totCrecConveniM + (int)$totPreEPublicaM + (int)$totPreEConveniM;
			
			$totTurmaCadastro = ((int)$qtdTurmaInformada - (int)$qtdTurmaCenso);
			
			$qtdTurma = ((int)$qtdTurmaInformada - (int)$qtdTurmaCenso);
			$qtdMatricula = ((int)$totGeralMatricula - (int)$totGeralCenso);
			if( $_POST['tipo'] == 'diligencia' ){
				
				$sql = "SELECT count(tur.turid)
						FROM proinfantil.turma tur
						    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
						    inner join workflow.documento doc on doc.docid = ntw.docid
						    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
						WHERE
						    tur.muncod = '".$_REQUEST['muncod']."'
						    and tur.turmes = '{$arMes['mes']}'
						    and tur.turano = '{$_SESSION['exercicio']}'
						    and tur.turstatus = 'A'
						    and doc.esdid not in (".WF_NOVASTURMAS_EM_DILIGENCIA.", ".WF_NOVASTURMAS_EM_CADASTRAMENTO.") ";
				$QtdTurmaDiligencia = $db->pegaUm($sql);
				
				$qtdRestante = (int)$qtdTurma - (int)$QtdTurmaDiligencia;
			} else {
				$qtdRestante = (int)$totTurmaCadastro - (int)$QtdTurmaEnviada;
			}
			
			//$arrTotTurmaMes
			if( $arrTotTurmaMes[(int)$arMes['mes'] - 1] < 0 ){
				$qtdRestante = (int)$qtdRestante + (int)$arrTotTurmaMes[(int)$arMes['mes'] - 1]; 
			}
			//ver($QtdTurmaEnviada, $totTurmaCadastro, $temAmpliacao, $qtdRestante);
			
			$qtdTurma = ($qtdTurma < 0 ? 0 : $qtdTurma);
			$qtdMatricula = ($qtdMatricula < 0 ? 0 : $qtdMatricula);
			$qtdRestante = ($qtdRestante < 0 ? 0 : $qtdRestante);
			
			//$qtdRestante = (($temAmpliacao < 1) ? 0 : ((int)$qtdRestante - (int)$temAmpliacao) );
			
			$msg = '<p>Prezado(a) Senhor(a) Prefeito(a),</p>
<p>&nbsp;</p>
<p>Os dados declarados indicam que houve um aumento de '.$qtdMatricula.' matr&iacute;culas e '.$qtdTurma.' turmas no m&ecirc;s '.simec_htmlentities(mes_extenso($arMes['mes'])).'/'.$arMes['turano'].'.</p>
<p>De acordo com essas informa&ccedil;&otilde;es, o munic&iacute;pio poder&aacute; enviar '.$qtdRestante.' novas turmas para an&aacute;lise.</p>';
			
			array_push($arrRetorno, array(
										'mes' => $arMes['mes'], 
										'qtdenviada' => ($QtdTurmaEnviada ? $QtdTurmaEnviada : 0), 
										'qtdselecionada' => $arMes['total'], 
										'qtdrestante' => $qtdRestante,
										'qtdTurmaInformada' => $qtdTurma,
										'totGeralMatricula' => $qtdMatricula,
										'totAmpliacao' => 1,
										'msg' => $msg
										)
									);
		}
	} else {
		$msg = '<p>Prezado(a) Senhor(a) Prefeito(a),</p>
<p>&nbsp;</p>
<p>Os dados declarados indicam que houve uma redu&ccedil;&atilde;o de matr&iacute;culas e de turmas.</p>
<p>De acordo com essas informa&ccedil;&otilde;es, o munic&iacute;pio n&atilde;o poder&aacute; enviar novas turmas para an&aacute;lise.</p>';

		array_push($arrRetorno, array(
									'mes' => 0, 
									'qtdenviada' => 0, 
									'qtdselecionada' => 0, 
									'qtdrestante' => 0,
									'qtdTurmaInformada' => 0,
									'totGeralMatricula' => 0,
									'totAmpliacao' => $temAmpliacao,
									'msg' => $msg
									)
								);
	}
	
	//ver($arrRetorno,d);
	echo simec_json_encode($arrRetorno);
	exit();
}
if( $_REQUEST['requisicao'] == 'mostraTurmaCadastrada' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$filtro = '';
	if( $_REQUEST['tipo'] == 'diligencia' ){
		$filtro = " and doc.esdid in (".WF_NOVASTURMAS_EM_DILIGENCIA.")
					and tur.turid in (select 
                                        ana.turid
                                    from proinfantil.analisenovasturmasaprovacao ana
                                        left join proinfantil.novasturmasdiligencia dil on dil.anaid = ana.anaid and dilstatus = 'A'
                                    where ana.anastatus = 'A' and dil.diltexto is not null) ";
	} else {
		if( $_SESSION['usucpf'] == '' ){
			$filtro = " and doc.esdid in (".WF_NOVASTURMAS_EM_CADASTRAMENTO.", ".WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO.")";
		} else {
			$filtro = " and doc.esdid in (".WF_NOVASTURMAS_EM_CADASTRAMENTO.")";
		}
		$filtro .= " and 0 = (select count(n.ntwid) from proinfantil.novasturmasworkflowturma n
                              inner join proinfantil.turma t on t.turid = n.turid
                          where ntwmesenvio = $mesAtual and n.muncod = tur.muncod and n.ntwanoenvio = '{$_SESSION['exercicio']}'
                              and t.turano = tur.turano and t.turstatus = 'A') ";
	}
	
	$sql = "SELECT distinct
				'<center><input type=\"checkbox\" class=\"check\" name=\"chk['||ntw.docid||']\" id=\"chk\" value=\"'||tur.turid||'\"></center>' as codigo,
			    tur.entcodent,
			    CASE WHEN ent.entnome IS NULL THEN tur.turnomeescola ELSE ent.entnome END AS estabelecimento,
			    tur.turdsc,
			    case when tur.turmes = 1 then 'Janeiro' 
			        when tur.turmes = 2 then 'Fevereiro'
			        when tur.turmes = 3 then 'Mar�o' 
			        when tur.turmes = 4 then 'Abril' 
			        when tur.turmes = 5 then 'Maio' 
			        when tur.turmes = 6 then 'Junho' 
			        when tur.turmes = 7 then 'Julho' 
			        when tur.turmes = 8 then 'Agosto' 
			        when tur.turmes = 9 then 'Setembro' 
			        when tur.turmes = 10 then 'Outubro' 
			        when tur.turmes = 11 then 'Novembro' 
			        when tur.turmes = 12 then 'Dezembro' end||'/'||tur.turano as mes,
			    esd.esddsc
			FROM proinfantil.turma tur
			    left join entidade.entidade ent ON ent.entcodent = tur.entcodent
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			    inner join workflow.documento doc on doc.docid = ntw.docid
			    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			WHERE
			    tur.muncod = '{$_REQUEST['muncod']}'
			    and tur.turstatus = 'A'
			    and tur.turano = '{$_SESSION['exercicio']}'
			    and tur.turdtinicio is not null
			    and tur.entcodent is not null
			    and tur.arqid is not null
			    $filtro
			    
			    and 0 < (select sum(ntaquantidade) from proinfantil.novasturmasalunoatendido where turid = tur.turid)
			    and 0 = (select count(*) from(
			                select
			                    (select count(fotid) from proinfantil.fotos where salid = s.salid and turid = tur.turid ) as fotos,
			                    count(s.salid) as sala
			                from
			                    proinfantil.tiposala tis
			                    inner join proinfantil.sala s on s.tisid = tis.tisid
			                where
			                    s.modid = 2
			                    and s.salstatus = 'A'
			                group by
			                    s.salid
			              ) as foo
			              where fotos < sala)
			ORDER BY 4";
	
	$cabecalho = array('Selecione', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'M�s', 'Situa��o');
	//$db->monta_lista($sql, $cabecalho, 10000000, 30, 'N', 'center', '', 'formTurmaCad');
	$db->monta_lista_simples($sql, $cabecalho, 10000000, 30, 'N', '100%', '', true, '', '', true);
	exit();
}

if( $_REQUEST['requisicao'] == 'enviaranalise' ){
	//@todo aqui modificar a flag
	
	if( $_POST['chk'] ){
		foreach ($_POST['chk'] as $docid => $turid) {
			$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$docid}";								
			$esdid 			= $db->pegaUm( $sql );
			$esdiddestino 	= WF_NOVASTURMAS_EM_ANALISE;
			
			$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
			$aedid = $db->pegaUm( $sql );
			
			if( $aedid && $docid ){
				$arDados = array('muncod' => $_SESSION['proinfantil']['muncod']);
				if(wf_alterarEstado( $docid, $aedid, 'Fluxo Novas Turmas', $arDados )){
					
					if( $_POST['tipo'] != 'diligencia' ){
						$sql = "UPDATE proinfantil.novasturmasworkflowturma SET
				  					ntwdataenvio = now(),
				  					ntwmesenvio = '{$mesAtual}',
				  					ntwanoenvio = '".$_SESSION['exercicio']."'
				  				WHERE turid = {$turid} 
				  					and muncod = '{$_REQUEST['muncod']}'";
						$db->executar($sql);
					}
					
					$sql2 = "UPDATE proinfantil.analisenovasturmasaprovacao SET
			  					anaanalisado = false
			  				WHERE turid = {$turid}";
					$db->executar($sql2);
					
					echo $db->commit();
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

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$perfis = pegaPerfil($_SESSION['usucpf']);
/*
$docid = $obNovasTurmas->pegaDocidNovasTurmas();
$esdid = $obNovasTurmas->pegaEstadoAtualNovasTurmas($docid);
*/

//$arMnuid = array('13223');

// Monta abas
$db->cria_aba( $abacod_tela, $url, '' );

//$titulo2 = '<img border="0" title="Indica campo obrigat�rio." src="../imagens/obrig.gif"> Indica campo obrigat�rio.';
monta_titulo('Cadastrar Turmas por Municipios', $titulo2);
cabecalhoTurma();

?>
<table class="tabela" align="center" cellSpacing="1" cellPadding=3 >
	<tr>
		<td valign="top">
				<form method="post" name="formulario" id="formulario" action="">
				<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:100%;">
					<tr>
						<td class="subtituloDireita">C�digo INEP</td>
						<td>
							<?php 
							$codinep = $_REQUEST['codinep'];
							echo campo_texto('codinep', 'N', 'S', '', 70, '', '', ''); ?>	
						</td>
					</tr>					
					<tr>
						<td class="subtituloDireita">Nome da Turma</td>
						<td>
							<?php 
							$turdsc = $_REQUEST['turdsc'];
							echo campo_texto('turdsc', 'N', 'S', '', 70, '', '', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Nome do Estabelecimento</td>
						<td>
							<?php 
							$descestabelecimento = $_REQUEST['descestabelecimento'];
							echo campo_texto('descestabelecimento', 'N', 'S', '', 70, '', '', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">&nbsp;</td>
						<td class="subtituloEsquerda">
							<input type="button" value="Pesquisar" id="btn_pesquisar" />
							<input type="button" value="Limpar" id="btn_limpar" />
						</td>
					</tr>
				</table>
			</form>
			<?php
			print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';
			print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Lista de Turmas por Municipios</label></td></tr><tr>';
			print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	
			$arWhere = array();
			if($_REQUEST['turdsc']){
				$arWhere[] = "(removeacento(turdsc) ilike '%{$_REQUEST['turdsc']}%' or turdsc ilike '%{$_REQUEST['turdsc']}%')";
			}
			
			if($_REQUEST['turdtinicio'] || $_REQUEST['turdtfim']){
				
				$_REQUEST['turdtinicio'] = $_REQUEST['turdtinicio'] ? formata_data_sql($_REQUEST['turdtinicio']) : '';
				$_REQUEST['turdtfim'] 	 = $_REQUEST['turdtfim'] ? formata_data_sql($_REQUEST['turdtfim']) : '';
				
				if($_REQUEST['turdtinicio'] && $_REQUEST['turdtfim']){
					$arWhere[] = "turdtinicio between '{$_REQUEST['turdtinicio']}' and '{$_REQUEST['turdtfim']}'";
				} elseif($_REQUEST['turdtinicio']){
					$arWhere[] = "turdtinicio = '{$_REQUEST['turdtinicio']}'";
				} elseif($_REQUEST['turdtfim']){
					$arWhere[] = "turdtinicio = '{$_REQUEST['turdtfim']}'";
				}
			}
			
			if($_REQUEST['codinep']){
				$arWhere[] = " ent.entcodent = '{$_REQUEST['codinep']}' ";
			}

			if($_SESSION['proinfantil']['muncod']){
				$arWhere[] = " tur.muncod = '{$_SESSION['proinfantil']['muncod']}' ";
			}
			
			if($_REQUEST['descestabelecimento']){
				$arWhere[] = "(ent.entnome ilike '%{$_REQUEST['descestabelecimento']}%' or turnomeescola ilike '%{$_REQUEST['descestabelecimento']}%' or ent.entcodent = '{$_REQUEST['descestabelecimento']}')";				
			}
			
			$stAcaoAlterar = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abrePopupTurma(' || tur.turid || ', ' || tur.muncod || ')\" alt=\"Editar\" title=\"Editar\"/>&nbsp;'";
			//$stAcaoExcluir = "'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirTurma(' || tur.turid || ')\" alt=\"Excluir\" title=\"Excluir\"/>'";
			$stAcaoExcluir = "''";
			$stAcaoFoto = "'&nbsp;<img src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"fotosTurma(' || tur.turid || ', ' || tur.muncod || ')\" />'";
			
			//$stAcaoExcluirInativo = "'<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" alt=\"Excluir\" title=\"Excluir\"/>'";
			$stAcaoExcluirInativo = "''";
			
			$cabecalho = array('A��o', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'Data in�cio', 'Situa��o', 'M�s');
			
			/*if($esdid == WF_NOVASTURMAS_EM_CADASTRAMENTO){
			
				$arSelect = ""; 
				$cabecalho = array('A��o', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'Data in�cio');
			
			} else {
				$stAcoes = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abrePopupTurma(' || tur.turid || ')\" alt=\"Editar\" title=\"Editar\"/>&nbsp;
							<img src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"fotosTurma(' || tur.turid || ')\" />'";
				$arSelect = ", CASE WHEN ana.anaparecer IS NULL OR ana.anaparecer = '' THEN ' - ' ELSE ana.anaparecer END, 
						   CASE WHEN ana.anatipo = 'A' THEN 'Aprovada' WHEN ana.anatipo = 'I' THEN 'Indeferida' WHEN ana.anatipo = 'D' THEN 'Diligenciada' ELSE ' - ' END as status";
				$cabecalho = array('A��o', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'Data in�cio', 'Parecer','Status');
			}*/
			
			$sql = "SELECT distinct
						case when esd.esdid = ".WF_NOVASTURMAS_EM_CADASTRAMENTO." then
							$stAcaoAlterar || $stAcaoExcluir
					    else 
					    	$stAcaoAlterar || $stAcaoExcluirInativo
					    end || 
					    case when tur.turdtinicio is not null then
					    	$stAcaoFoto
					    else '' end as acao,
								tur.turano||' - '||tur.entcodent as entcodent,
								CASE WHEN entnome IS NULL THEN turnomeescola ELSE entnome END AS estabelecimento,
								tur.turdsc,									
								to_char(turdtinicio, 'dd/MM/yyyy') as datainicio,
								esd.esddsc,
								esd.esdid,
								case when tur.turmes = 1 then 'Janeiro' 
									when tur.turmes = 2 then 'Fevereiro'
									when tur.turmes = 3 then 'Mar�o' 
									when tur.turmes = 4 then 'Abril' 
									when tur.turmes = 5 then 'Maio' 
									when tur.turmes = 6 then 'Junho' 
									when tur.turmes = 7 then 'Julho' 
									when tur.turmes = 8 then 'Agosto' 
									when tur.turmes = 9 then 'Setembro' 
									when tur.turmes = 10 then 'Outubro' 
									when tur.turmes = 11 then 'Novembro' 
									when tur.turmes = 12 then 'Dezembro' end||'/'||tur.turano as mes
					FROM		proinfantil.turma tur
					LEFT JOIN	entidade.entidade ent ON ent.entcodent = tur.entcodent
					LEFT JOIN 	proinfantil.analisenovasturmasaprovacao ana ON ana.turid = tur.turid
					left join proinfantil.novasturmasworkflowturma ntw
				    	inner join	workflow.documento doc on doc.docid = ntw.docid
						inner join	workflow.estadodocumento esd on esd.esdid = doc.esdid
				    on ntw.turid = tur.turid
				    $innerJoin
				    ".
								($arWhere ? ' WHERE '.implode(' AND ', $arWhere) : '')
								." AND tur.turstatus = 'A' and tur.turano = '{$_SESSION['exercicio']}' /*and tur.turmes = {$mesAtual} and dm.ntmmmes = {$mesAtual}*/
					ORDER BY 	7";
			
			$arrTurmasMun = $db->carregar($sql);
			$arrTurmasMun = $arrTurmasMun ? $arrTurmasMun : array();
			
			$arrRegistro = array();
			$boDiligencia = false;
			foreach ($arrTurmasMun as $key => $v) {
				
				if( $v['esdid'] == WF_NOVASTURMAS_EM_DILIGENCIA ) $boDiligencia = true;
				
				array_push($arrRegistro, array('acao' => $v['acao'],
												'entcodent' => $v['entcodent'],
												'estabelecimento' => $v['estabelecimento'],
												'turdsc' => $v['turdsc'],
												'datainicio' => $v['datainicio'],
												'esddsc' => $v['esddsc'],
												'mes' => $v['mes']
												)
							);
			}
			
			
			//ver( simec_htmlentities($sql),d );
			$db->monta_lista_array($arrRegistro, $cabecalho, 50, 20, 'N', 'center', '', '', 'formularioturma');
		?>
		</td>
		<?if( ( in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis ) ) ){ ?>
		<td width="100" valign="top" align="center">
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="An�lise de Turmas"><strong>An�lise de Turmas</strong></span> 
                    </td>
				</tr>
				<?if( $_SESSION['exercicio'] == date('Y') ){ ?>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="enviarTurmasAnalise();" title="Enviar Turmas para An�lise">Enviar Turmas para An�lise</a>
					</td>
				</tr>
				<?} else { ?>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						nenhuma a��o dispon�vel para o documento
					</td>
				</tr>
				<?} ?>
				<?if( $boDiligencia ){ ?>
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Dilig�ncia de Turmas"><strong>Dilig�ncia de Turmas</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="retornarTurmasAnalise('diligencia');" title="Retornar Turmas para An�lise">Retornar Turmas em Dilig�ncia para An�lise</a>
					</td>
				</tr>
				<?} ?>
			</table>
		</td>
		<?} ?>
	</tr>
</table>
<div id="debug"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> 
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<script type="text/javascript" src="/estrutura/js/funcoes.js"></script>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

/*messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(800, 600);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage(){
	messageObj.close();	
}*/

/*function abrePopupTurma(turid){
	var url = 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='+turid;
	displayMessage(url);
}*/

$(document).ready(function(){
	$('.listagem').css('width', '100%');
});

function abrePopupTurma(turid, muncod){
	return window.open('proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='+turid+'&muncod='+muncod,'modelo',"fullscreen=yes,scrollbars=yes");
}

function fotosTurma(turid, muncod){
	return window.open('proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid='+turid+'&muncod='+muncod+'&novasturma=true','modelo',"height=600,width=950,scrollbars=yes,top=50,left=200" );
}

$('#btn_pesquisar').click(function(){
	$('[name=formulario]').submit();
});	

$('#btn_limpar').click(function(){
	document.location.href = 'proinfantil.php?modulo=novasturmas/montarTurmas&acao=A';
});


function enviarTurmasAnalise(){
	
	$.ajax({
		type: "POST",
		url: "proinfantil.php?modulo=novasturmas/montarTurmas&acao=A",
		data: "requisicao=mostraTurmaCadastrada",
		async: false,
		success: function(msg){
			$('#marcartodos').css('display', '');
			
			$( "#dialog_turma" ).show();
			$( "#mostraTurma" ).html(msg);
			$( '#dialog_turma' ).dialog({
					resizable: false,
					width: 900,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Enviar para An�lise': function() {
							
							if( Number($('#chk:checked').val())){
								var retorno = verificaTtdTurma('');
								if( retorno == '1' ){
									enviaTurmaAnalise('');
									$( this ).dialog( 'close' );
								} else {
									$('#marcartodos').css('display', 'none');
									$( "#mostraTurma" ).html(retorno);
								}
								
							} else {
								alert('Selecione pelo menos uma turma!');
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

function retornarTurmasAnalise(tipo){
	
	$.ajax({
		type: "POST",
		url: "proinfantil.php?modulo=novasturmas/montarTurmas&acao=A",
		data: "requisicao=mostraTurmaCadastrada&tipo="+tipo,
		async: false,
		success: function(msg){
			$('#marcartodos').css('display', '');
			
			$( "#dialog_turma" ).show();
			$( "#mostraTurma" ).html(msg);
			$( '#dialog_turma' ).dialog({
					resizable: false,
					width: 900,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Enviar para An�lise': function() {
							
							if( Number($('#chk:checked').val())){
								var retorno = verificaTtdTurma(tipo);
								if( retorno == '1' ){
									enviaTurmaAnalise(tipo);
									$( this ).dialog( 'close' );
								} else {
									$('#marcartodos').css('display', 'none');
									$( "#mostraTurma" ).html(retorno);
								}								
							} else {
								alert('Selecione pelo menos uma turma!');
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

function verificaTtdTurma(tipo){
	var dados = $('#formTurmaCad').serialize();
	
	var retorno = '0';
	$.ajax({
		type: "POST",
		url: "proinfantil.php?modulo=novasturmas/montarTurmas&acao=A",
		data: "requisicao=verificaTtdTurma&tipo="+tipo+"&"+dados,
		async: false,
		success: function(msg){
			//$('#debug').html(msg);
			//return false;			
			var data =  $.parseJSON(msg);
			
			$(data).each(function(i, obj){
				if( obj.totAmpliacao > 0 ){
					if( obj.qtdselecionada > obj.qtdrestante ){
						retorno = obj.msg;
					} else {
						retorno = '1';
					}
				} else {
					retorno = obj.msg;
				}
			});
		}
	});
	return retorno;
}

function enviaTurmaAnalise( tipo ){
	var dados = $('#formTurmaCad').serialize();
	//console.log(dados);
	
	$.ajax({
		type: "POST",
		url: "proinfantil.php?modulo=novasturmas/montarTurmas&acao=A",
		data: "requisicao=enviaranalise&"+dados+"&tipo="+tipo,
		async: false,
		success: function(msg){
			//$('#debug').html(msg); 
			if( Number( msg ) ){
				alert('Turmas enviadas para \"AN�LISE\" com sucesso!');
			} else {
				alert('Falha na opera��o. Turmas n�o enviadas para an�lise!');
			}
			window.location.href = window.location;
		}
	});
}
</script>

<div id="dialog_turma" title="Lista de Turmas" style="display: none; text-align: center;">
	<form name="formTurmaCad" id="formTurmaCad" action="" enctype="multipart/form-data" method="post">
		<div style="padding:5px;text-align:justify;" align="left" id="marcartodos"><input type="checkbox" name="todos" id="todos" value="" class="marcar-todos" marcar="check"><b>Selecionar Todos</b></div>
		<div style="padding:5px;text-align:justify;" id="mostraTurma"></div>
	</form>
</div>