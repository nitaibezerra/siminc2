<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);

header('content-type: text/html; charset=ISO-8859-1');

include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_novasturmas.php";

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

$mesAtual = ($_REQUEST['ntmmmes1'] ? $_REQUEST['ntmmmes1'] : date('m'));

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 

$obNovasTurmas = new NovasTurmas( $_REQUEST );

if( $_POST['requisicao'] == 'salvar' ){
	
	$qtdTurmaInformada = (int)$_POST['ntmmqtdturmacrecheconveniada'] + (int)$_POST['ntmmqtdturmapreescolapublica'] + (int)$_POST['ntmmqtdturmapreescolaconveniada'] +
						(int)$_POST['ntmmqtdturmaunificadapublica'] + (int)$_POST['ntmmqtdturmaunificadaconveniada'] + (int)$_POST['ntmmqtdturmacrechepublica'];
						
	$totCrecPublicaM = (int)$_POST['ntmmqtdmatriculacrecheintegralpublica'] + (int)$_POST['ntmmqtdmatriculacrecheparcialpublica'];
	$totCrecConveniM = (int)$_POST['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$_POST['ntmmqtdmatriculacrecheparcialconveniada'];
	
	$totPreEPublicaM = (int)$_POST['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$_POST['ntmmqtdmatriculapreescolaparcialpublica'];
	$totPreEConveniM = (int)$_POST['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$_POST['ntmmqtdmatriculapreescolaparcialconveniada'];
		
	if($qtdTurmaInformada == 0 && $totCrecPublicaM == 0 && $totCrecConveniM == 0 && $totPreEPublicaM == 0 && $totPreEConveniM == 0){
		echo "<script>
					alert('Antes de salvar e prosseguir é necessário confirmar se realmente não existe mais nenhuma matrícula e nenhuma turma na educação infantil.');
					//window.location.href = window.location; 
				</script>";
	} else {
		
		$arrMatriculaMaior = $obNovasTurmas->carregaDadosMatriculaMaiorPorMes();
		$arrTurmaMaior = $obNovasTurmas->carregaDadosTurmaMaiorPorMes();
		
		#Quantidade de Matricula
		$totCrecPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialpublica'];
		$totCrecConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialconveniada'];
		
		$totPreEPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialpublica'];
		$totPreEConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialconveniada'];	
		
		#quantidade de Turmas
		$qtdTurmaCenso = (int)$arrTurmaMaior['ntmmqtdturmacrechepublica'] + (int)$arrTurmaMaior['ntmmqtdturmacrecheconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmapreescolapublica'] +
						(int)$arrTurmaMaior['ntmmqtdturmapreescolaconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadapublica'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadaconveniada'];
				
		$totGeralCenso 		= (int)$totCrecPublicaC + (int)$totCrecConveniC + (int)$totPreEPublicaC + (int)$totPreEConveniC;
		$totGeralMatricula 	= (int)$totCrecPublicaM + (int)$totCrecConveniM + (int)$totPreEPublicaM + (int)$totPreEConveniM;
		
		$totTurmaCadastro = ((int)$qtdTurmaInformada - (int)$qtdTurmaCenso);
		
		//ver($totCrecPublicaC, $totCrecConveniC, $totPreEPublicaC, $totPreEConveniC);
		
		$boTemDireito = true;
		#quantidade de Turmas
		$qtdTurmaCrecPub = 0;
		$qtdTurmaCrecConv = 0;		
		$qtdTurmaPreEPub = 0;	
		$qtdTurmaPreEConv = 0;		
		$qtdTurmaUnifPub = 0;	
		$qtdTurmaUnifConv = 0;
		
		if( ((int)$totGeralMatricula > (int)$totGeralCenso) && ( (int)$qtdTurmaInformada > (int)$qtdTurmaCenso ) ){
		
			$totUnifPublicaC = (int)$totCrecPublicaC + (int)$totPreEPublicaC;
			$totUnifPublicaM = (int)$totCrecPublicaM + (int)$totPreEPublicaM;
			
			$totUnifConvenC = (int)$totCrecConveniC + (int)$totPreEConveniC;
			$totUnifConvenM = (int)$totCrecConveniM + (int)$totPreEConveniM;
			
			
			$qtdMatriCreche =  (int)$totCrecPublicaM + (int)$totCrecConveniM;
			$qtdMatriPreEscola =  (int)$totPreEPublicaM + (int)$totPreEConveniM;
			
			$qtdMatriCrecheCenso =  (int)$totCrecPublicaC + (int)$totCrecConveniC;
			$qtdMatriPreEscolaCenso =  (int)$totPreEPublicaC + (int)$totPreEConveniC;
			
			$qtdMatriUnif = (int)$qtdMatriCreche + (int)$qtdMatriPreEscola; 
			$qtdMatriUnifCenso = (int)$qtdMatriCrecheCenso + (int)$qtdMatriPreEscolaCenso; 
						
			#Verifica a quantidade de matricula informada
			$qtdTotalCreche = (int)$qtdMatriCreche - (int)$qtdMatriCrecheCenso;
			$qtdTotalPreEscola = (int)$qtdMatriPreEscola - (int)$qtdMatriPreEscolaCenso;
			$qtdTotalUnificada = (int)$qtdMatriUnif - (int)$qtdMatriUnifCenso;
										
			$boCrechePublica = 'N';
			$boPreEsPublica = 'N';
			$boCrecheCoveniada = 'N';
			$boPreEsCoveniada = 'N';
			$boUnificadaPublica = 'N';
			$boUnificadaCoven = 'N';			
			
			if( (int)$totCrecPublicaM > (int)$totCrecPublicaC && $qtdTotalCreche > 0 ){
				$boCrechePublica = 'S';
			}
			if( (int)$totPreEPublicaM > (int)$totPreEPublicaC && $qtdTotalPreEscola > 0 ){
				$boPreEsPublica = 'S';
			}
			if( (int)$totCrecConveniM > (int)$totCrecConveniC && $qtdTotalCreche > 0 ){
				$boCrecheCoveniada = 'S';
			}
			if( (int)$totPreEConveniM > (int)$totPreEConveniC && $qtdTotalPreEscola > 0 ){
				$boPreEsCoveniada = 'S';
			}
			if( (int)$totUnifPublicaM > (int)$totUnifPublicaC && $qtdTotalCreche > 0 && $qtdTotalPreEscola > 0 ){
				$boUnificadaPublica = 'S';
			}
			if( (int)$totUnifConvenM > (int)$totUnifConvenC && $qtdTotalCreche > 0 && $qtdTotalPreEscola > 0 ){
				$boUnificadaCoven = 'S';
			}
			
			
			#quantidade de Turmas
			$qtdTurmaCrecPub = (int)$_POST['ntmmqtdturmacrechepublica'] - (int)$arrTurmaMaior['ntmmqtdturmacrechepublica'];
			$qtdTurmaCrecConv = (int)$_POST['ntmmqtdturmacrecheconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmacrecheconveniada'];
			
			$qtdTurmaPreEPub = (int)$_POST['ntmmqtdturmapreescolapublica'] - (int)$arrTurmaMaior['ntmmqtdturmapreescolapublica'];	
			$qtdTurmaPreEConv = (int)$_POST['ntmmqtdturmapreescolaconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmapreescolaconveniada'];
			
			$qtdTurmaUnifPub = (int)$_POST['ntmmqtdturmaunificadapublica'] - (int)$arrTurmaMaior['ntmmqtdturmaunificadapublica'];	
			$qtdTurmaUnifConv = (int)$_POST['ntmmqtdturmaunificadaconveniada'] - (int)$arrTurmaMaior['ntmmqtdturmaunificadaconveniada'];
						
			//ver($totCrecPublicaC, $totCrecConveniC, $totPreEPublicaC, $totPreEConveniC);
			
			/* $arrQTD = array(
							'qtdTurmaCrecPub' 	=> $qtdTurmaCrecPub, 
							'qtdTurmaCrecConv' 	=> $qtdTurmaCrecConv, 
							'qtdTurmaPreEPub' 	=> $qtdTurmaPreEPub, 
							'qtdTurmaPreEConv' 	=> $qtdTurmaPreEConv, 
							'qtdTurmaUnifPub' 	=> $qtdTurmaUnifPub, 
							'qtdTurmaUnifConv'	=> $qtdTurmaUnifConv);
			asort($arrQTD);
			$arrQTDInt = array(); */
			
			/* 
			 * Verifica se a quantidade de turma informada bate com o total que relamente tem direito
			 * caso as proximas turmas seja maior que o total informado elas irão herdar o valor 0 
			 */
			/* foreach ($arrQTD as $key => $valor) {
				if( $valor > 0 ){
					if( $valor <= $totTurmaCadastro){
						$totTurmaCadastro = ((int)$totTurmaCadastro - (int)$valor);
					} else {
						$$key = $totTurmaCadastro;
						$totTurmaCadastro = 0;
					}
					//array_push($arrQTDInt, $valor);
				}
			} */
			//ver($arrQTD, $arrQTDInt, $totTurmaCadastro, $qtdTurmaCenso, $qtdTurmaInformada, $qtdTurmaCrecPub, $qtdTurmaCrecConv, $qtdTurmaPreEPub, $qtdTurmaPreEConv, $qtdTurmaUnifPub, $qtdTurmaUnifConv,d);
			
			$msgErro = '';
			if( ((int)$qtdTurmaCrecPub > 0) && $boCrechePublica == 'N' ){
				$msgErro .= 'Não pode inserir turmas para creche publica.\n';
			}
			if( ((int)$qtdTurmaCrecConv > 0) && $boCrecheCoveniada == 'N' ){
				$msgErro .= 'Não pode inserir turmas para creche conveniada.\n';
			}
			if( ((int)$qtdTurmaPreEPub > 0) && $boPreEsPublica == 'N' ){
				$msgErro .= 'Não pode inserir turmas para pre-escola publica.\n';
			}
			if( ((int)$qtdTurmaPreEConv > 0) && $boPreEsCoveniada == 'N' ){
				$msgErro .= 'Não pode inserir turmas para pre-escola conveniada.\n';
			}
			if( ((int)$qtdTurmaUnifPub > 0) && $boUnificadaPublica == 'N' ){
				$msgErro .= 'Não pode inserir turmas para unificada publica.\n';
			}
			if( ((int)$qtdTurmaUnifConv > 0) && $boUnificadaCoven == 'N' ){
				$msgErro .= 'Não pode inserir turmas para unificada conveniada.\n';
			}
		} else {
			$boTemDireito = false;
			$msgErro = 'Atenção! De acordo com as informações inseridas no "Questionário de Atendimento" o seu município não tem direito ao recurso. Para cadastrar as novas turmas  é necessário que, além de criar novas turmas com todas as matrículas não computadas no âmbito do Fundeb, o município demonstre a ampliação do acesso à educação infantil.';
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
													'QtdCreche' => $qtdTotalCreche,
												),
							'CrecheConveniada' => array(
													'qtdTurma' => ( ((int)$qtdTurmaCrecPub < 0) ? ((int)$qtdTurmaCrecConv - abs($qtdTurmaCrecPub)) : $qtdTurmaCrecConv ),
													'boCadastra' => ( (int)$qtdTurmaCrecConv <= 0 ? 'N' : $boCrecheCoveniada),
													'tipoTurma' => '1',
													'tipoRede' => '2',
													'descricao' => 'Creche Conveniada',
													'QtdCreche' => $qtdTotalCreche,
												),
							'PreEscolaPublica' => array(
													'qtdTurma' => ( (int)$qtdTurmaPreEConv < 0 ? (int)$qtdTurmaPreEPub - abs($qtdTurmaPreEConv) : $qtdTurmaPreEPub),
													'boCadastra' => ( (int)$qtdTurmaPreEPub <= 0 ? 'N' : $boPreEsPublica),
													'tipoTurma' => '2',
													'tipoRede' => '1',
													'descricao' => 'Pré-Escola Pública',
													'QtdPreEscola' => $qtdTotalPreEscola,
												),
							'PreEscolaConveniada' => array(
													'qtdTurma' => ( (int)$qtdTurmaPreEPub < 0 ? (int)$qtdTurmaPreEConv - abs($qtdTurmaPreEPub) : $qtdTurmaPreEConv ),
													'boCadastra' => ( (int)$qtdTurmaPreEConv <= 0 ? 'N' : $boPreEsCoveniada),
													'tipoTurma' => '2',
													'tipoRede' => '2',
													'descricao' => 'Pré-Escola Conveniada',
													'QtdPreEscola' => $qtdTotalPreEscola,
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
		//ver($msgErro, $arrCadTurma,d);
		
		if( !empty($msgErro) ){
			echo "<script>
						alert('".$msgErro."');
					</script>";
		}
		
		$retorno = $obNovasTurmas->salvarMatriculaTurmaMunicipio( $_REQUEST, $boCadastraTurma, $boTemDireito, $arrCadTurma );
	
		if( $retorno ){
			echo "<script>
					alert('Operação Realizada com Sucesso!');
					window.location.href = window.location; 
				</script>";
			exit();
		} else {
			echo "<script>
					alert('Falha na operação');
				</script>";
		}
	}
}
/*$docid = $obNovasTurmas->pegaDocidNovasTurmas();
$esdid = $obNovasTurmas->pegaEstadoAtualNovasTurmas($docid);*/

//$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$obDate = new Data();
$arrMatTurma		= $obNovasTurmas->carregaDadosMatriculaTurmaMes();
$rsMatriculasTurmas = $obNovasTurmas->recuperarDadosMatriculasTurmasMes();

$arrMatTurma['ntmmqtdmatriculacrecheparcialpublica'] 		= ($_POST['ntmmqtdmatriculacrecheparcialpublica'] 			? (string)$_POST['ntmmqtdmatriculacrecheparcialpublica'] 		: (string)$arrMatTurma['ntmmqtdmatriculacrecheparcialpublica']);
$arrMatTurma['ntmmqtdmatriculacrecheintegralpublica'] 		= ($_POST['ntmmqtdmatriculacrecheintegralpublica'] 			? (string)$_POST['ntmmqtdmatriculacrecheintegralpublica'] 		: (string)$arrMatTurma['ntmmqtdmatriculacrecheintegralpublica']);
$arrMatTurma['ntmmqtdmatriculapreescolaparcialpublica'] 	= ($_POST['ntmmqtdmatriculapreescolaparcialpublica'] 		? (string)$_POST['ntmmqtdmatriculapreescolaparcialpublica'] 	: (string)$arrMatTurma['ntmmqtdmatriculapreescolaparcialpublica']);
$arrMatTurma['ntmmqtdmatriculapreescolaintegralpublica'] 	= ($_POST['ntmmqtdmatriculapreescolaintegralpublica'] 		? (string)$_POST['ntmmqtdmatriculapreescolaintegralpublica'] 	: (string)$arrMatTurma['ntmmqtdmatriculapreescolaintegralpublica']);
$arrMatTurma['ntmmqtdmatriculacrecheparcialconveniada'] 	= ($_POST['ntmmqtdmatriculacrecheparcialconveniada'] 		? (string)$_POST['ntmmqtdmatriculacrecheparcialconveniada'] 	: (string)$arrMatTurma['ntmmqtdmatriculacrecheparcialconveniada']);
$arrMatTurma['ntmmqtdmatriculacrecheintegralconveniada'] 	= ($_POST['ntmmqtdmatriculacrecheintegralconveniada'] 		? (string)$_POST['ntmmqtdmatriculacrecheintegralconveniada'] 	: (string)$arrMatTurma['ntmmqtdmatriculacrecheintegralconveniada']);
$arrMatTurma['ntmmqtdmatriculapreescolaparcialconveniada'] 	= ($_POST['ntmmqtdmatriculapreescolaparcialconveniada'] 	? (string)$_POST['ntmmqtdmatriculapreescolaparcialconveniada'] 	: (string)$arrMatTurma['ntmmqtdmatriculapreescolaparcialconveniada']);
$arrMatTurma['ntmmqtdmatriculapreescolaintegralconveniada'] = ($_POST['ntmmqtdmatriculapreescolaintegralconveniada'] 	? (string)$_POST['ntmmqtdmatriculapreescolaintegralconveniada'] : (string)$arrMatTurma['ntmmqtdmatriculapreescolaintegralconveniada']);

$arrMatTurma['ntmmqtdturmacrechepublica'] 		= ($_POST['ntmmqtdturmacrechepublica'] 			? (string)$_POST['ntmmqtdturmacrechepublica'] 		: (string)$arrMatTurma['ntmmqtdturmacrechepublica']);
$arrMatTurma['ntmmqtdturmapreescolapublica'] 	= ($_POST['ntmmqtdturmapreescolapublica'] 		? (string)$_POST['ntmmqtdturmapreescolapublica'] 	: (string)$arrMatTurma['ntmmqtdturmapreescolapublica']);
$arrMatTurma['ntmmqtdturmaunificadapublica'] 	= ($_POST['ntmmqtdturmaunificadapublica'] 		? (string)$_POST['ntmmqtdturmaunificadapublica'] 	: (string)$arrMatTurma['ntmmqtdturmaunificadapublica']);
$arrMatTurma['ntmmqtdturmacrecheconveniada'] 	= ($_POST['ntmmqtdturmacrecheconveniada'] 		? (string)$_POST['ntmmqtdturmacrecheconveniada'] 	: (string)$arrMatTurma['ntmmqtdturmacrecheconveniada']);
$arrMatTurma['ntmmqtdturmapreescolaconveniada'] = ($_POST['ntmmqtdturmapreescolaconveniada'] 	? (string)$_POST['ntmmqtdturmapreescolaconveniada'] : (string)$arrMatTurma['ntmmqtdturmapreescolaconveniada']);
$arrMatTurma['ntmmqtdturmaunificadaconveniada'] = ($_POST['ntmmqtdturmaunificadaconveniada'] 	? (string)$_POST['ntmmqtdturmaunificadaconveniada'] : (string)$arrMatTurma['ntmmqtdturmaunificadaconveniada']);

if($rsMatriculasTurmas){
	$qtdCreche    = $rsMatriculasTurmas['ntmmqtdturmacrechepublica']-$rsMatriculasTurmas['ntcqtdturmacrechepublica'];
	$qtdCrecheC   = $rsMatriculasTurmas['ntmmqtdturmacrecheconveniada']-$rsMatriculasTurmas['ntcqtdturmacrecheconveniada'];
	$totalCreche = $qtdCreche + $qtdCrecheC;
	
	$qtdPreEscola  = $rsMatriculasTurmas['ntmmqtdturmapreescolapublica']-$rsMatriculasTurmas['ntcqtdturmapreescolapublica'];
	$qtdPreEscolaC = $rsMatriculasTurmas['ntmmqtdturmapreescolaconveniada']-$rsMatriculasTurmas['ntcqtdturmapreescolaconveniada'];
	$totalPreEscola = $qtdPreEscola + $qtdPreEscolaC;			
	
	$qtdUnificada  = $rsMatriculasTurmas['ntmmqtdturmaunificadapublica']-$rsMatriculasTurmas['ntcqtdturmaunificadapublica'];
	$qtdUnificadaC = $rsMatriculasTurmas['ntmmqtdturmaunificadaconveniada']-$rsMatriculasTurmas['ntcqtdturmaunificadaconveniada'];
	$totalUnificada = $qtdUnificada + $qtdUnificadaC;			
} else {
	$totalCreche = "";
	$totalPreEscola = "";
	$totalUnificada = "";
}

verificaPermissaoAcesso();
$urlBlq = substr($_SESSION['favurl'], 0, strpos($_SESSION['favurl'], '&'));
$arrAcesso = $_SESSION['proinfantil']['acesso'][$urlBlq];
$arrAcesso = ($arrAcesso ? $arrAcesso : array());
$arBloqueio = array();
foreach ($arrAcesso as $key => $acesso) {
	if($acesso == 'N'){
		array_push($arBloqueio, $key);
	}
}
$arBloqueio = '"'.implode('", "',$arBloqueio).'"';

$sql = "SELECT adpresposta FROM proinfantil.adesaoprogramanovasturmas WHERE muncod = '".$_REQUEST['muncod']."' and adpano = ".$_SESSION['exercicio'];
$adpresposta = $db->pegaUm($sql);

if( $adpresposta != 'S' ){
	echo "<script>
			//alert('Operação Realizada co Sucesso!');
			window.location.href = 'proinfantil.php?modulo=novasturmas/termoNovasTurmas&acao=A'; 
		</script>";
	exit();
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

//$arMnuid = array('13223');

// Monta abas
$db->cria_aba( $abacod_tela, $url, '' );

$titulo2 = '';
monta_titulo('Matrículas/Turmas por Municipios', $titulo2);
cabecalhoTurma();

$sql = "select count(t.turid) from 
			proinfantil.novasturmasdadosmunicipiospormes ntm
		    inner join proinfantil.turma t on t.muncod = ntm.muncod and ntm.ntmmmes = t.turmes and cast(ntm.ntmmano as integer) = t.turano
		    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid
		    inner join workflow.documento doc on doc.docid = ntw.docid
		where
			/*ntm.ntmmid = {$arrMatTurma['ntmmid']}
		    and*/ doc.esdid = ".WF_NOVASTURMAS_EM_ANALISE."
		    and ntm.muncod = '{$_SESSION['proinfantil']['muncod']}'
		    and ntm.ntmmmes = $mesAtual
		    and t.turmes = $mesAtual
		    and ntmmstatus = 'A'
		    and t.turano = '{$_SESSION['exercicio']}'";

$boAnalise = $db->pegaUm($sql);

?>
<style type="text/css">
.tituloListagem{
		border-bottom: none;
		background-color: #C4C4C4;
		text-align: left;
		width: 100%;
	}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />

<form name="formulario" id="formulario" method="post" action="" autocomplete="off">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="ntmmid" id="ntmmid" value="<?=$arrMatTurma['ntmmid'] ?>">
	<input type="hidden" name="boAnalise" id="boAnalise" value="<?=$boAnalise ?>">
	
	<input type="hidden" name="ntmmmes" id="ntmmmes" value="<?=($arrMatTurma['ntmmmes'] ? $arrMatTurma['ntmmmes'] : $mesAtual) ?>">
	
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td style="color: red; font-weight: bold;"><?//=$msgErro; ?></td>
		</tr>
		<tr>
			<td>
				<table border="0" width="100%" class="listagem" cellpadding="3" cellspacing="2">
				<tr>
					<td class="SubTitulodireita" width="50%"><b>O seu município/DF atende crianças da educação infantil em estabelecimentos educacionais públicos ou em instituições comunitárias, 
								confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público, em tempo parcial ou integral?</b></td>
					<td width="50%">
							<input name="ntmmatendeedinf" value="S" class="habilitado" type="radio" <?=(($arrMatTurma['ntmmatendeedinf'] == 'S' || empty($arrMatTurma['ntmmatendeedinf']) ) ? 'checked="checked"' : ''); ?>> Sim
							<input name="ntmmatendeedinf" value="N" class="habilitado" type="radio" <?=($arrMatTurma['ntmmatendeedinf'] == 'N' ? 'checked="checked"' : ''); ?>> Não</td>
				</tr>
				<? 
				$cpf = array('', '');
				if( in_array($_SESSION['usucpforigem'], $cpf) ){ ?>
				<tr>
					<td class="SubTitulodireita" width="50%"><b>Mês a ser Simulado:</b></td>
					<td width="50%"><?
					$arrMes = array(
								array('codigo' => '01', 'descricao' => 'Janeiro'),
								array('codigo' => '02', 'descricao' => 'Fevereiro'),
								array('codigo' => '03', 'descricao' => 'Março'),
								array('codigo' => '04', 'descricao' => 'Abril'),
								array('codigo' => '05', 'descricao' => 'Maio'),
								array('codigo' => '06', 'descricao' => 'Junho'),
								array('codigo' => '07', 'descricao' => 'Julho'),
								array('codigo' => '08', 'descricao' => 'Agosto'),
								array('codigo' => '09', 'descricao' => 'Setembro'),
								array('codigo' => '10', 'descricao' => 'Outubro'),
								array('codigo' => '11', 'descricao' => 'Novembro'),
								array('codigo' => '12', 'descricao' => 'Dezembro')
								);
						$ntmmmes1 = $_REQUEST['ntmmmes1'];
								
						$db->monta_combo("ntmmmes1", $arrMes, 'S','-- Selecione um mês --','simulaMesAtual', '', '',150,'N','ntmmmes1', '', '', 'Mês', '');
					?></td>
				</tr>
				<?} ?>
				</table>
				<table border="0" width="100%" class="listagem" cellpadding="3" cellspacing="2">
					<tr>
						<td class="SubTituloCentro" style="background-color: #ADD8E6" colspan="5">Matrículas</td>
					</tr>
					<tr>
						<td class="SubTituloEsquerda" style="background-color: #ADD8E6" colspan="5">Na tabela abaixo são apresentadas as matrículas de educação infantil informadas no Educacenso de <?=((int)$_SESSION['exercicio'] - 1) ?>. Ao lado informe o 
									número ATUAL de matrículas existentes na educação infantil. </td>
					</tr>
					<tr>
						<td rowspan="2" width="10%" bgcolor="#7BA2B6">&nbsp;</td>
						<td colspan="2" width="45%" bgcolor="#FFA500" align="center"><b>Educacenso <?=$arrMatTurma['ntcanocenso']?></b></td>
						<td colspan="2" width="45%" bgcolor="#2E8B57" align="center"><b>Matrículas em <?=$obDate->mesTextual($mesAtual).' de '.$_SESSION['exercicio'] ?> - SITUAÇÃO ATUAL</b></td>
					</tr>
					<tr>
						<td bgcolor="#FFA500" align="center"><b>Rede pública municipal</b></td>
						<td bgcolor="#FFA500" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
						<td bgcolor="#2E8B57" align="center"><b>Rede pública municipal</b></td>
						<td bgcolor="#2E8B57" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
					</tr>	
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Creche Integral</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunocrecheintegralpublica'] ?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunocrecheintegralconveniada'] ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculacrecheintegralpublica', 'N', 'S', '', 10, 6, '[#]', '', 'right','','','class="habilitado"',"",$arrMatTurma['ntmmqtdmatriculacrecheintegralpublica']) ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculacrecheintegralconveniada', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"',"",$arrMatTurma['ntmmqtdmatriculacrecheintegralconveniada']) ?></td>
					</tr>
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Creche Parcial</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunocrecheparcialpublica'] ?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunocrecheparcialconveniada'] ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculacrecheparcialpublica', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculacrecheparcialpublica']) ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculacrecheparcialconveniada', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculacrecheparcialconveniada']) ?></td>
					</tr>
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Pré-escola Integral</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunopreescolaintegralpublica'] ?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunopreescolaintegralconveniada'] ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculapreescolaintegralpublica', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculapreescolaintegralpublica']) ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculapreescolaintegralconveniada', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculapreescolaintegralconveniada']) ?></td>
					</tr>
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Pré-escola Parcial</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunopreescolaparcialpublica'] ?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdalunopreescolaparcialconveniada'] ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculapreescolaparcialpublica', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculapreescolaparcialpublica']) ?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdmatriculapreescolaparcialconveniada', 'N', 'S', '', 10, 6, '######', '',    'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdmatriculapreescolaparcialconveniada']) ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="5" style="height: 5px;"></td>
		</tr>
		<tr>
			<td>
				<table border="0" width="100%" class="listagem" cellpadding="3" cellspacing="2">
					<tr>
						<td class="SubTituloCentro" style="background-color: #ADD8E6" colspan="5">Turmas</td>
					</tr>
					<tr>
						<td class="SubTituloEsquerda" style="background-color: #ADD8E6" colspan="5">As crianças são organizadas em turmas. No Educacenso, o município define o tipo de atendimento de cada turma de educação infantil. 
									Na tabela abaixo, informe o número ATUAL de turmas.</td>
					</tr>
					<tr>
						<td width="10%" rowspan="2" bgcolor="#7BA2B6">&nbsp;</td>
						<td width="45%" colspan="2" bgcolor="#FFA500" align="center"><b>Educacenso <?=$arrMatTurma['ntcanocenso']?> - Número de Turmas</b></td>
						<td width="45%" colspan="2" bgcolor="#2E8B57" align="center"><b>Turmas em <?=$obDate->mesTextual($mesAtual).' de '.$_SESSION['exercicio'] ?> - SITUAÇÃO ATUAL</b></td>
					</tr>
					<tr>
						<td bgcolor="#FFA500" align="center"><b>Rede pública municipal</b></td>
						<td bgcolor="#FFA500" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
						<td bgcolor="#2E8B57" align="center"><b>Rede pública municipal</b></td>
						<td bgcolor="#2E8B57" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
					</tr>	
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Creche</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmacrechepublica']?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmacrecheconveniada']?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmacrechepublica', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmacrechepublica'])?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmacrecheconveniada', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmacrecheconveniada'])?></td>
					</tr>
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Pré-escola</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmapreescolapublica']?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmapreescolaconveniada']?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmapreescolapublica', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmapreescolapublica'])?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmapreescolaconveniada', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmapreescolaconveniada'])?></td>
					</tr>
					<tr>
						<td align="right" bgcolor="#7BA2B6"><b>Unificada (matrículas de creche e pré-escola na mesma turma)</b></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmaunificadapublica']?></td>
						<td align="center" bgcolor="#FABF40"><?=$arrMatTurma['ntcqtdturmaunificadaconveniada']?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmaunificadapublica', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmaunificadapublica'])?></td>
						<td align="center" bgcolor="#50A56C"><?=campo_texto('ntmmqtdturmaunificadaconveniada', 'N', 'S', '', 10, 6, '######', '', 'right','','','class="habilitado"','',$arrMatTurma['ntmmqtdturmaunificadaconveniada'])?></td>
					</tr>
				</table>
			</td>
		</tr>
		<?if($totalCreche == 0 && $totalPreEscola == 0 && $totalUnificada == 0){ ?>
		<tr>
			<td>
			<table border="0" align="center" width="100%" class="tabela" cellpadding="3" cellspacing="2">
			  	<thead>
					<tr>
						<td align="justify" style="color: red;"><font size="2"><b>"Atenção! De acordo com as informações inseridas no <b>Questionário de Atendimento</b> o 
							seu município não tem direito ao recurso. Para cadastrar as novas turmas  é necessário que, além de criar novas turmas com todas as matrículas não 
							computadas no âmbito do Fundeb, o município demonstre a <b>ampliação do acesso à educação infantil</b>."</b></font></td>
					</tr>
				</thead>
			 </table>
			</td>
		</tr>
		<?}?>
		<? if( !$boAnalise && $_SESSION['exercicio'] == date('Y') ){ ?>
		<tr bgcolor="#cccccc" align="center">
			<td>
				<?
				//if( in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(EQUIPE_MUNICIPAL, $perfis) ){ ?>
					<input onclick="salvarMatricula()" class="habilitado" <?php echo $_SESSION['proinfantil']['acesso'][$url]['button']?> value="Salvar" type="button" id="btn_qst_salvar">
				<?/*}else{ ?>
					<input disabled="disabled" class="habilitado" value="Salvar" type="button" id="btn_qst_salvar">
				<?} */?>
			</td>
		</tr>
		<? } ?>
	</table>
	<?
	$arrMatricula = $obNovasTurmas->carregaDadosMatriculaTurmaMunicipio();
	if( $arrMatricula ){
	
	?>
	<table border="0" align="center" width="100%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<th>Lista de Matrículas/Turmas por Municípios</th>
		</tr>
		<tr>
			<td> 
			<div id="accordion">
			<?			
			foreach ($arrMatricula as $key => $matricula) {
				echo '
					<div style="margin-top: 5px">
							<div class="tituloListagem" style="background-color: #DCDCDC; " >
								<table border="0" width="100%">
									<tr>
									<td style="padding-left: 20px; font-weight: bold; text-transform:capitalize;">Matrículas/Turmas em '.$obDate->mesTextual($matricula['ntmmmes']).' de '.$matricula['ntmmano'].'</td>
									<!--<td style="text-align: right">Analisadas:<?php echo $contAnalisadas; ?> &nbsp; | &nbsp;  Pendentes: <?php echo $contNaoAnalisadas; ?></td>-->
								</tr>
							</table>
						</div>
					</div>
					  <div>
					    <p>
					    <table border="0" width="100%" cellpadding="0" cellspacing="0">
					    	<tr>
					    		<td width="49.5%"><table border="0" width="100%" class="listagem" cellpadding="3" cellspacing="2">
					    				<tr>
											<td rowspan="2" width="10%" bgcolor="#7BA2B6">&nbsp;</td>
											<td colspan="2" width="45%" bgcolor="#2E8B57" align="center"><b>Matrículas em '.$obDate->mesTextual($matricula['ntmmmes']).' de '.$matricula['ntmmano'].' - SITUAÇÃO ATUAL</b></td>
										</tr>
										<tr>
											<td bgcolor="#2E8B57" align="center"><b>Rede pública municipal</b></td>
											<td bgcolor="#2E8B57" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
										</tr>	
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Creche Integral</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculacrecheintegralpublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculacrecheintegralconveniada'].'</td>
										</tr>
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Creche Parcial</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculacrecheparcialpublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculacrecheparcialconveniada'].'</td>
										</tr>
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Pré-escola Integral</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculapreescolaintegralpublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculapreescolaintegralconveniada'].'</td>
										</tr>
										<tr>
											<td align="right" bgcolor="#7BA2B6" height="43px;"><b>Pré-escola Parcial</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculapreescolaparcialpublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdmatriculapreescolaparcialconveniada'].'</td>
										</tr>
					    			</table></td>
					    		<td width="1%">&nbsp;</td>
					    		<td width="49.5%"><table border="0" width="100%" class="listagem" cellpadding="3" cellspacing="2">
					    				<tr>
											<td rowspan="2" width="10%" bgcolor="#7BA2B6">&nbsp;</td>
											<td colspan="2" width="45%" bgcolor="#2E8B57" align="center"><b>Turmas em '.$obDate->mesTextual($matricula['ntmmmes']).' de '.$matricula['ntmmano'].' - SITUAÇÃO ATUAL</b></td>
										</tr>
										<tr>
											<td bgcolor="#2E8B57" align="center"><b>Rede pública municipal</b></td>
											<td bgcolor="#2E8B57" align="center"><b>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</b></td>
										</tr>	
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Creche</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmacrechepublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmacrecheconveniada'].'</td>
										</tr>
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Pré-escola</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmapreescolapublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmapreescolaconveniada'].'</td>
										</tr>
										<tr>
											<td align="right" bgcolor="#7BA2B6"><b>Unificada (matrículas de creche e pré-escola na mesma turma)</b></td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmaunificadapublica'].'</td>
											<td align="center" bgcolor="#50A56C">'.$matricula['ntmmqtdturmaunificadaconveniada'].'</td>
										</tr>
					    			</table></td>
					    	</tr>
						</table>
					    </p>
					  </div>';
			}			
			?>
			</div>
			</td>
		</tr>
	</table>
	<?} ?>
</form>
 <div id="debug"></div>
<script type="text/javascript">

$(document).ready( function() {

	var arBloqueio = new Array(<?=$arBloqueio ?>);
	
	for(var i=0; i<arBloqueio.length; i++){
		switch (arBloqueio[i]) {
			case ('select'):
				$('#formulario').find('select').attr('disabled', 'disabled');
				break;
			default:
				$('#formulario').find('input[type='+arBloqueio[i]+']').attr('disabled', 'disabled');
				break;
		}
	}
	
	if( $('#boAnalise').val() > 0 ){
		$('.habilitado').attr('disabled', 'disabled');
	}
	
	$('[type="text"]').focusin(function(){
		if( $(this).val() == 0 ){
			$(this).val('');
		}
	});
	
	$('[type="text"]').focusout(function(){
		if( $(this).val() == '' ){
			$(this).val('0');
		}
	});
	
	/*$('.normal').keyup(function(){
		
		$.ajax({
			type: "POST",
			url: 'proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio&acao=A',
			data: "calculaDisponivel=true&nome="+$(this).attr('name')+'&valor='+$(this).val(),
			async: false,
			success: function(msg){
				$('#debug').html(msg);
			}
		});
	});*/

	var icons = {
      header: "ui-icon-circle-arrow-e",
      activeHeader: "ui-icon-circle-arrow-s"
    };
    $( "#accordion" ).accordion({
    	collapsible: true,
		clearStyle: true,
    	//icons: icons,
    	active: -1
    });
    
   // $( "#accordion" ).accordion({ icons: { "header": "ui-icon-plus", "activeHeader": "ui-icon-minus" } });
 
});

/*function verificaValor(objeto, valor){
	var campo = objeto.id.substring(0, (objeto.id.length - 6) ) + valor;
	//alert(objeto.value+' - '+campo+' - '+$('[name='+campo+']').val());
	if( parseInt(objeto.value) > parseInt($('[name='+campo+']').val()) ){
		alert('Valor esta maior do que o permitido.');
		objeto.value = 0;
	}
}*/

function simulaMesAtual( valor ){
	$('#ntmmmes').val(valor);
	$('#formulario').submit();
}

function salvarMatricula(){
	if( $('#ntmmid').val() != '' ){
		if( confirm('As turmas em cadastramento serão excluidas.') ){
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		}
	} else {
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	}
}
</script>
