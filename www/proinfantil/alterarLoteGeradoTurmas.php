<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once "_constantes.php";
include_once "_funcoes_novasturmas.php";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
 
session_start();

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$lotnumportaria 	= $_REQUEST['portaria'];
$lotdataportaria 	= $_REQUEST['data'];
$lote			 	= $_REQUEST['lote'];
$exercicio		 	= $_REQUEST['ano'];

$sql = "SELECT distinct 
		            mun.estuf,
		            mun.muncod,
		            mun.mundescricao,
                    array_to_string(array(select turid from proinfantil.turma where muncod = mun.muncod and lotid = tur.lotid), ',') as turma
			FROM territorios.municipio mun
				inner join proinfantil.turma tur on tur.muncod = mun.muncod
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			WHERE
				tur.lotid = $lote
				--and tur.muncod = '3535408'
			ORDER BY 	mun.estuf, mun.mundescricao";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

	$dataPortaria = explode('/', $lotdataportaria);
	$dia = $dataPortaria[0];
	$mes = $dataPortaria[1];
	$ano = $dataPortaria[2];
	$mes = mes_extenso($mes);
	
	$texto = '<p style="text-align: justify;"><strong>PORTARIA N&ordm;&nbsp;&nbsp;'.$lotnumportaria.', &nbsp;&nbsp;&nbsp;&nbsp;DE&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dia.'&nbsp;&nbsp;&nbsp;&nbsp; DE &nbsp;&nbsp;&nbsp;&nbsp;'.$mes.'&nbsp;&nbsp;&nbsp;&nbsp;DE '.$ano.'.</strong></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="padding-left: 300px; text-align: justify;">Autoriza o Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o - FNDE a realizar a transfer&ecirc;ncia de recurso financeiro para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil aos munic&iacute;pios e ao Distrito Federal que pleitearam e est&atilde;o aptos para pagamento, conforme Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 16, de 16 de maio de 2013.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>O SECRET&Aacute;RIO DE EDUCA&Ccedil;&Atilde;O B&Aacute;SICA</strong>, no uso das atribui&ccedil;&otilde;es, resolve:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 1&ordm; Divulgar os munic&iacute;pios e o Distrito Federal que est&atilde;o aptos a receber o pagamento do recurso financeiro para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil oferecidas em estabelecimentos educacionais p&uacute;blicos ou em institui&ccedil;&otilde;es comunit&aacute;rias, confessionais ou filantr&oacute;picas sem fins lucrativos conveniadas com o poder p&uacute;blico que tenham cadastradas novas matr&iacute;culas em novas turmas e que ainda n&atilde;o foram contempladas com recursos do Fundo de Manuten&ccedil;&atilde;o e Desenvolvimento da Educa&ccedil;&atilde;o B&aacute;sica e de Valoriza&ccedil;&atilde;o dos Profissionais da Educa&ccedil;&atilde;o (Fundeb), de que trata a Lei n&ordm; 12.722 de 3 de outubro de 2012, e conforme informa&ccedil;&otilde;es declaradas pelos munic&iacute;pios e Distrito Federal no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o &ndash; Novas Turmas de Educa&ccedil;&atilde;o Infantil.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 2&ordm; Autorizar o FNDE/MEC a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e Distrito Federal para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil, conforme destinat&aacute;rios e valores constantes da listagem anexa.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 3&ordm; Esta Portaria entra em vigor na data de sua publica&ccedil;&atilde;o.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: center;"><strong>MANUEL FERNANDO PALÁCIOS DA CUNHA E MELO</strong></p>
<p style="text-align: center;">Secret&aacute;rio da Educa&ccedil;&atilde;o B&aacute;sica</p>';
				
	$html = $texto.'
		<p style="page-break-before:always"><!-- pagebreak --></p>
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th rowspan="2" width="05%"><b>UF</b></th>
				<th rowspan="2" width="25%" style="text-align: center;"><b>Municípo</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Código IBGE</b></th>
				<th colspan="4" width="60%" style="text-align: justify;"><b>Quantidade de novas matrículas em novas turmas de educação infantil, declaradas pelos Municípios e o Distrito Federal, em   estabelecimentos públicos e /ou conveniados com o poder público</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Valor do Repasse</b></th>
			</tr>
			<tr>
				<th style="text-align: center;"><b>Creche Púb/Conv Parcial</b></th>
				<th style="text-align: center;"><b>Creche Púb/Conv Integral</b></th>
				
				<th style="text-align: center;"><b>Pré-Escola Púb/Conv Parcial</b></th>
				<th style="text-align: center;"><b>Pré-Escola Púb/Conv Integral</b></th>
			</tr>';
		
	$totMatricula = 0;
	$arrLote = array();
	$arrMuncod = array();
	$muncodAnterior	= '';
	$valorTotalGeral = 0;
	
	/*$sql = "delete from proinfantil.lotenovasturmas where lotid = $lote";
	$db->executar($sql);*/

	foreach ($arrDados as $v) {
		$sql = "SELECT ntmmid, muncod, ntmmstatus, ntmmano, ntmmmes, ntmmqtdmatriculacrecheparcialpublica, ntmmqtdmatriculacrecheintegralpublica,
		  			ntmmqtdmatriculapreescolaparcialpublica, ntmmqtdmatriculapreescolaintegralpublica, ntmmqtdmatriculacrecheparcialconveniada,
		  			ntmmqtdmatriculacrecheintegralconveniada, ntmmqtdmatriculapreescolaparcialconveniada, ntmmqtdmatriculapreescolaintegralconveniada,
		  			ntmmqtdturmacrechepublica, ntmmqtdturmapreescolapublica, ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
		  			ntmmqtdturmapreescolaconveniada, ntmmqtdturmaunificadaconveniada
				FROM proinfantil.novasturmasdadosmunicipiospormes WHERE ntmmstatus = 'A' and muncod = '{$v['muncod']}' and ntmmano = '2014' 
				order by ntmmmes desc";
				
		$arrMatricula = $db->carregar($sql);
		$arrMatricula = $arrMatricula ? $arrMatricula : array();
		
		$arrMat = matriculaCalculoDadosMunicipio( $arrMatricula );
		
		$arTurid = explode(',', $v['turma']);
		$valor_geral 						= 0;
		$qtdCrechepublicaparcial 			= 0;
		$qtdCrechepublicaintegral 			= 0;
		$qtdCrecheconveniadaparcial 		= 0;
		$qtdCrecheconveniadaintegral 		= 0;			
		$qtdPreescolapublicaparcial 		= 0;
		$qtdPreescolapublicaintegral 		= 0;
		$qtdPreescolaconveniadaparcial		= 0;
		$qtdPreescolaconveniadaintegral 	= 0;
		
		$arTurmas = array();
		foreach ($arTurid as $turid) {
			
			$arPost = array('turid' => $turid, 'muncod' => $v['muncod']);
			$obNovasTurmas = new NovasTurmas( $arPost );
			$aryRepasse = $obNovasTurmas->carregaRepassePorTurma();
			
			/*timid => 6 - Creche, 		7 - Pré-escola 	*/
			/*tatid => 1 - Integral, 	2 - Parcial 	*/
			/*tirid => 1 - Pública, 	2 - Privada 	*/
			#Creche
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 6, 'tatid' => 2, 'tirid' => 1, 'turma' => $turid);
			$qtdCrechepublicaparcial += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 6, 'tatid' => 1, 'tirid' => 1, 'turma' => $turid);
			$qtdCrechepublicaintegral += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 6, 'tatid' => 2, 'tirid' => 2, 'turma' => $turid);
			$qtdCrecheconveniadaparcial += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 6, 'tatid' => 1, 'tirid' => 2, 'turma' => $turid);
			$qtdCrecheconveniadaintegral += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			#Pre-Escola
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 7, 'tatid' => 2, 'tirid' => 1, 'turma' => $turid);
			$qtdPreescolapublicaparcial += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 7, 'tatid' => 1, 'tirid' => 1, 'turma' => $turid);
			$qtdPreescolapublicaintegral += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 7, 'tatid' => 2, 'tirid' => 2, 'turma' => $turid);
			$qtdPreescolaconveniadaparcial += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			$arParam = array('muncod' => $v['muncod'], 'ano' => $exercicio, 'timid' => 7, 'tatid' => 1, 'tirid' => 2, 'turma' => $turid);
			$qtdPreescolaconveniadaintegral += calculaQtdMatriculaAguardandoPagamento( $arParam );
			
			foreach($aryRepasse as $repasse){
				
				if( $repasse['anatipo'] == 'A' ){
					$totalGeral = str_replace(".","", $repasse['valor_total']);
					$totalGeral = str_replace(",",".", $totalGeral);
				    
				    $valor_geral += $totalGeral; 
				}
			}				
			array_push($arTurmas, $turid);
		}		
		/*$arrLote[]= array(
						'municipio' 					=> $v['mundescricao'],
						'ibge' 							=> $v['muncod'],
						'estuf' 						=> $v['estuf'],
						'crechepublicaparcial'			=> (int)$arrMat['crechepublicaparcial'].' - '.$crechepublicaparcial,
						'crechepublicaintegral' 		=> (int)$arrMat['crechepublicaintegral'].' - '.$crechepublicaintegral,
						'crecheconveniadaparcial' 		=> (int)$arrMat['crecheconveniadaparcial'].' - '.$crecheconveniadaparcial,
						'crecheconveniadaintegral' 		=> (int)$arrMat['crecheconveniadaintegral'].' - '.$crecheconveniadaintegral,
		
						'preescolapublicaparcial'		=> (int)$arrMat['preescolapublicaparcial'].' - '.$preescolapublicaparcial,
						'preescolapublicaintegral' 		=> (int)$arrMat['preescolapublicaintegral'].' - '.$preescolapublicaintegral,
						'preescolaconveniadaparcial'	=> (int)$arrMat['preescolaconveniadaparcial'].' - '.$preescolaconveniadaparcial,
						'preescolaconveniadaintegral' 	=> (int)$arrMat['preescolaconveniadaintegral'].' - '.$preescolaconveniadaintegral,
						'valorRepasse' 					=> (float)$valor_geral
					);*/
					
		$arrLote[]= array(
							'municipio' 					=> $v['mundescricao'],
							'ibge' 							=> $v['muncod'],
							'estuf' 						=> $v['estuf'],
							'crechepublicaparcial'			=> (int)$qtdCrechepublicaparcial,
							'crechepublicaintegral' 		=> (int)$qtdCrechepublicaintegral,
							'crecheconveniadaparcial' 		=> (int)$qtdCrecheconveniadaparcial,
							'crecheconveniadaintegral' 		=> (int)$qtdCrecheconveniadaintegral,
			
							'preescolapublicaparcial'		=> (int)$qtdPreescolapublicaparcial,
							'preescolapublicaintegral' 		=> (int)$qtdPreescolapublicaintegral,
							'preescolaconveniadaparcial'	=> (int)$qtdPreescolaconveniadaparcial,
							'preescolaconveniadaintegral' 	=> (int)$qtdPreescolaconveniadaintegral,
							'valorRepasse' 					=> (float)$valor_geral,
							'turmas'	 					=> $arTurmas
						);
	}
	//ver($arrLote,d);
	foreach ($arrLote as $v) {
			extract($v);
			$valorTotal = ($valorRepasse ? number_format($valorRepasse,2,",",".") : '0,00');
				
			$html.='
			<tr>
				<td>'.$estuf.'</td>
				<td style="text-align: left;">'.$municipio.'</td>
				<td style="text-align: center;">'.$ibge.'</td>
				<td style="text-align: center;">'.((int)$crechepublicaparcial + (int)$crecheconveniadaparcial).'</td>
				<td style="text-align: center;">'.((int)$crechepublicaintegral + (int)$crecheconveniadaintegral).'</td>
				
				<td style="text-align: center;">'.((int)$preescolapublicaparcial + (int)$preescolaconveniadaparcial).'</td>
				<td style="text-align: center;">'.((int)$preescolapublicaintegral + (int)$preescolaconveniadaintegral).'</td>
				<td style="text-align: right;">'.$valorTotal.'</td>
			</tr>';
			
			$valorTotal = str_replace(".","", $valorTotal);
			$valorTotal = str_replace(",",".", $valorTotal);
			
			$sql = "UPDATE proinfantil.loteminutanovasturmas SET
					  crechepublicaparcial = ".(int)$crechepublicaparcial.",
					  crechepublicaintegral = ".(int)$crechepublicaintegral.",
					  crecheconveniadaparcial = ".(int)$crecheconveniadaparcial.",
					  crecheconveniadaintegral = ".(int)$crecheconveniadaintegral.",
					  preescolapublicaparcial = ".(int)$preescolapublicaparcial.",
					  preescolapublicaintegral = ".(int)$preescolapublicaintegral.",
					  preescolaconveniadaparcial = ".(int)$preescolaconveniadaparcial.",
					  preescolaconveniadaintegral = ".(int)$preescolaconveniadaintegral.",
					  valorrepasse = ".$valorTotal." 
				WHERE 
				  lotid = $lote
				  and muncod = '{$v['ibge']}'
				  and estuf = '{$v['estuf']}'";
			
			$db->executar($sql);
		}
		
	$html.= '</table>';
	include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
	ob_clean();
		
	$nomeArquivo 		= 'minuta_repasse_'.date('Y-m-d').'_lote_'.$lote;
	$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutanovasturmas';
	$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutanovasturmas/'.$nomeArquivo.'.pdf';
	
	if( !is_dir($diretorio) ){
		mkdir($diretorio, 0777);
	}
	
	$http = new RequestHttp();
	$html = utf8_encode($html);
	$response = $http->toPdf( $html );

	$fp = fopen($diretorioArquivo, "w");
	if ($fp) {
	  stream_set_write_buffer($fp, 0);
	  fwrite($fp, $response);
	  fclose($fp);
	}
	
	$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
			VALUES( '".$nomeArquivo."',
					'pdf',
					'".$nomeArquivo."',
					'application/pdf',
					'".filesize($diretorioArquivo)."',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					{$_SESSION['sisid']},
					'A') RETURNING arqid";
	
	$arqid = $db->pegaUm($sql);

	$sql = "UPDATE proinfantil.lotenovasturmas SET arqid = $arqid, lotdsc = 'Lote: '||lotid WHERE lotid = $lote";
	$db->executar($sql);
	$db->commit();
		
	echo "alert('Lote criado com sucesso.');";

	function calculaQtdMatriculaAguardandoPagamento( $arParam = array() ){
		global $db;
		
		$muncod = $arParam['muncod'];
		$ano 	= $arParam['ano'];
		$timid 	= $arParam['timid'];
		$tatid 	= $arParam['tatid'];
		$tirid 	= $arParam['tirid'];
		$turma 	= $arParam['turma'];
		
		$filtro = '';
		if( $tirid ){
			$filtro = " and t.tirid = $tirid ";
		} else {
			$filtro = " and t.tirid in (1, 2) ";
		}
		
		if( $turma ) $filtro .= " and t.turid = $turma";
		
		$sql = "select coalesce(sum(nta.ntaquantidade),0) from proinfantil.turma t
					inner join proinfantil.novasturmasalunoatendido nta on nta.turid = t.turid
				        and nta.timid = $timid /* 6 - Creche, 7 - Pré-escola */
				        and nta.tatid = $tatid  /* 1 - Integral, 2 - Parcial */
				    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid
				    inner join workflow.documento doc on doc.docid = ntw.docid
				where
				    t.muncod = '$muncod'
				    and t.turano = '$ano'
				    $filtro /* 1 - Pública, 2 - Privada */
				    and doc.esdid = ".WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO;
		$total = $db->pegaUm($sql);
		
		return (int)$total;
	}
?>