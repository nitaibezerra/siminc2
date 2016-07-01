<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_novasturmas.php";

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct
			mun.muncod,
		    mun.estuf,
			mun.mundescricao
		from territorios.municipio mun
			inner join proinfantil.novasturmasmunicipios ntm on ntm.muncod = mun.muncod
		where
			--mun.muncod in (select distinct muncod from proinfantil.novasturmasdadosmunicipiospormes)
		    mun.muncod = '2100055'
		order by mun.mundescricao";

$arrMunicipio = $db->carregar($sql);
$arrMunicipio = $arrMunicipio ? $arrMunicipio : array();

foreach ($arrMunicipio as $arMun) {
	
	$arrMes = $db->carregarColuna("select distinct ntmmmes from proinfantil.novasturmasdadosmunicipiospormes where muncod = '{$arMun['muncod']}' and ntmano = '2014' order by ntmmmes asc");
	
	foreach ($arrMes as $mes) {
		$arrPost = array(
						'muncod' => $arMun['muncod'],
						'ntmmmes' => ((int)$mes)
						);
		$obNovasTurmas = new NovasTurmas( $arrPost );
		
		$arrMatriculaMaior 		= $obNovasTurmas->carregaDadosMatriculaMaiorPorMes();
		$arrTurmaMaior 			= $obNovasTurmas->carregaDadosTurmaMaiorPorMes();
		$arrMatTurmaCadastrada 	= $obNovasTurmas->carregaDadosMatriculaTurmaMes();
		//ver($mes, $arrMatriculaMaior, $arrTurmaMaior, $arrMatTurmaCadastrada);
		
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
		$totalTurmaCadastrada = $db->pegaUm("select count(turid) from proinfantil.turma where muncod = '{$arMun['muncod']}'");
		
		ver($totalTurmaCadastrada, $totTurmaCadastro, $qtdTurmaInformada, $qtdTurmaCenso);
		
		$totalDeletar = ((int)$totalTurmaCadastrada - (int)$totTurmaCadastro);
				
		$boTemDireito = true;
		if( ((int)$totGeralMatricula > (int)$totGeralCenso) && ( (int)$qtdTurmaInformada > (int)$qtdTurmaCenso ) ){
		
			$totUnifPublicaC = (int)$totCrecPublicaC + (int)$totPreEPublicaC;
			$totUnifPublicaM = (int)$totCrecPublicaM + (int)$totPreEPublicaM;
			
			$totUnifConvenC = (int)$totCrecConveniC + (int)$totPreEConveniC;
			$totUnifConvenM = (int)$totCrecConveniM + (int)$totPreEConveniM;
				
			$boCrechePublica = 'N';
			$boPreEsPublica = 'N';
			$boCrecheCoveniada = 'N';
			$boPreEsCoveniada = 'N';
			$boUnificadaPublica = 'N';
			$boUnificadaCoven = 'N';
			
			if( (int)$totCrecPublicaM > (int)$totCrecPublicaC ){
				$boCrechePublica = 'S';
			}
			if( (int)$totPreEPublicaM > (int)$totPreEPublicaC ){
				$boPreEsPublica = 'S';
			}
			if( (int)$totCrecConveniM > (int)$totCrecConveniC ){
				$boCrecheCoveniada = 'S';
			}
			if( (int)$totPreEConveniM > (int)$totPreEConveniC ){
				$boPreEsCoveniada = 'S';
			}
			if( (int)$totUnifPublicaM > (int)$totUnifPublicaC ){
				$boUnificadaPublica = 'S';
			}
			if( (int)$totUnifConvenM > (int)$totUnifConvenC ){
				$boUnificadaCoven = 'S';
			}
			
			
			#quantidade de Turmas
			$qtdTurmaCrecPub = (int)$arrMatTurmaCadastrada['ntmmqtdturmacrechepublica'] - (int)$arrTurmaMaior['ntmmqtdturmacrechepublica'];
			$qtdTurmaCrecConv = (int)$arrMatTurmaCadastrada['ntmmqtdturmacrecheconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmacrecheconveniada'];
			
			$qtdTurmaPreEPub = (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolapublica'] - (int)$arrTurmaMaior['ntmmqtdturmapreescolapublica'];	
			$qtdTurmaPreEConv = (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolaconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmapreescolaconveniada'];
			
			$qtdTurmaUnifPub = (int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadapublica'] - (int)$arrTurmaMaior['ntmmqtdturmaunificadapublica'];	
			$qtdTurmaUnifConv = (int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadaconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmaunificadaconveniada'];
			
			$arrQTD = array(
							'CrechePublica' 	=> $qtdTurmaCrecPub, 
							'CrecheConveniada' 	=> $qtdTurmaCrecConv, 
							'PreEscolaPublica' 	=> $qtdTurmaPreEPub, 
							'PreEscolaConveniada' 	=> $qtdTurmaPreEConv, 
							'UnificadaPublica' 	=> $qtdTurmaUnifPub, 
							'UnificadaConveniada'	=> $qtdTurmaUnifConv);
			asort($arrQTD);
			$arrQTDInt = array();
			foreach ($arrQTD as $key => $valor) {
				if( $valor > 0 ){
					/*if( $valor <= $totTurmaCadastro){
						$totTurmaCadastro = ((int)$totTurmaCadastro - (int)$valor);
					} else {
						$$key = $totTurmaCadastro;
						$totTurmaCadastro = 0;
					}*/
					$arrQTDInt[$key] = $valor;
				}
			}
			arsort($arrQTDInt);
		}
		
		$boCadastraTurma = array('CrechePublica' => $boCrechePublica,
								'PreEscolaPublica' => $boPreEsPublica,
								'CrecheCoveniada' => $boCrecheCoveniada,
								'PreEscolaCoveniada' => $boPreEsCoveniada,
								'UnificadaPublica' => $boUnificadaPublica,
								'UnificadaCoveniada' => $boUnificadaCoven,
							);
		
		$arrCadTurma = array(
						'CrechePublica' => array(
												'qtdTurma' => ( ((int)$qtdTurmaCrecConv < 0) ? ((int)$qtdTurmaCrecPub - abs($qtdTurmaCrecConv)) : $qtdTurmaCrecPub ),
												'boCadastra' => ( (int)$qtdTurmaCrecPub <= 0 ? 'N' : $boCrechePublica ),
												'tipoTurma' => '1',
												'tipoRede' => '1',
												'descricao' => 'Creche Pública',
											),
						'CrecheConveniada' => array(
												'qtdTurma' => ( ((int)$qtdTurmaCrecPub < 0) ? ((int)$qtdTurmaCrecConv - abs($qtdTurmaCrecPub)) : $qtdTurmaCrecConv ),
												'boCadastra' => ( (int)$qtdTurmaCrecConv <= 0 ? 'N' : $boCrecheCoveniada),
												'tipoTurma' => '1',
												'tipoRede' => '2',
												'descricao' => 'Creche Conveniada',
											),
						'PreEscolaPublica' => array(
												'qtdTurma' => ( (int)$qtdTurmaPreEConv < 0 ? (int)$qtdTurmaPreEPub - abs($qtdTurmaPreEConv) : $qtdTurmaPreEPub),
												'boCadastra' => ( (int)$qtdTurmaPreEPub <= 0 ? 'N' : $boPreEsPublica),
												'tipoTurma' => '2',
												'tipoRede' => '1',
												'descricao' => 'Pré-Escola Pública',
											),
						'PreEscolaConveniada' => array(
												'qtdTurma' => ( (int)$qtdTurmaPreEPub < 0 ? (int)$qtdTurmaPreEConv - abs($qtdTurmaPreEPub) : $qtdTurmaPreEConv ),
												'boCadastra' => ( (int)$qtdTurmaPreEConv <= 0 ? 'N' : $boPreEsCoveniada),
												'tipoTurma' => '2',
												'tipoRede' => '2',
												'descricao' => 'Pré-Escola Conveniada',
											),
						'UnificadaPublica' => array(
												'qtdTurma' => ( (int)$qtdTurmaUnifConv < 0 ? $qtdTurmaUnifPub - abs($qtdTurmaPreEConv) : $qtdTurmaUnifPub ),
												'boCadastra' => ( (int)$qtdTurmaUnifPub <= 0 ? 'N' : $boUnificadaPublica),
												'tipoTurma' => '3',
												'tipoRede' => '1',
												'descricao' => 'Unificada Pública',
											),
						'UnificadaConveniada' => array(
												'qtdTurma' => ( (int)$qtdTurmaUnifPub < 0 ? (int)$qtdTurmaUnifConv - abs($qtdTurmaUnifPub) : $qtdTurmaUnifConv),
												'boCadastra' => ( (int)$qtdTurmaUnifConv <= 0 ? 'N' : $boUnificadaCoven),
												'tipoTurma' => '3',
												'tipoRede' => '2',
												'descricao' => 'Unificada Conveniada',
											)
						);
		//ver($arrCadTurma, $arrQTDInt,$totalDeletar,d);
		if( is_array($arrCadTurma) && is_array($arrQTDInt) && $totalDeletar > 0 ){
			foreach ($arrQTDInt as $tipoRedeEnsino => $valor) {
				//ver($arrCadTurma[$tipoRedeEnsino], $tipoRedeEnsino, $valor);
				
				$tipoTurma 	= $arrCadTurma[$tipoRedeEnsino]['tipoTurma'];
				$tipoRede 	= $arrCadTurma[$tipoRedeEnsino]['tipoRede'];
				
				$sql = "update proinfantil.turma set turstatus = 'E'
						where
						    turid in (select turid from proinfantil.turma where 
						    				muncod = '{$arMun['muncod']}'
						    				and ttuid 	= $tipoTurma
										    and tirid 	= $tipoRede
										    and turmes 	= $mes 
										    and turano 	= '2013' 
						    				and turdtinicio is null limit ".$totalDeletar.")";
				$db->executar($sql);
				
			}				
			$db->commit();
		}
	}
}
?>