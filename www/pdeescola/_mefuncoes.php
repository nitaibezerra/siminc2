<?php

/**
 * Função para validar se a escola pode fazer o plano de atividades em 2011.
 */
function podeFazerPlano2011()
{
	$retorno = true;
	
	if( $_SESSION["meentid"] )
	{
		global $db;
		
		$existeAnoAnterior = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
		
		if($existeAnoAnterior)
		{
			$retorno = false;
			$atividadesAnoAnterior = $db->carregarColuna("SELECT meacomecounoano FROM pdeescola.meatividade WHERE memid = ".$existeAnoAnterior[0]["memid"]." AND meaano = ".((integer)$_SESSION["exercicio"] - 1)." AND meacomecounoano is not null");
			
			if( count($atividadesAnoAnterior) > 0 )
			{
				$flagAnoAnterior = false;
				foreach($atividadesAnoAnterior as $comecouano)
				{
					if( $comecouano == 't' )
					{
						$flagAnoAnterior = true;
						break;
					}
				}
				
				if( $flagAnoAnterior )
				{
					//$memvlrpago = $existeAnoAnterior[0]["memvlrpago"];
					//if( $memvlrpago && (float)$memvlrpago > 0 )
					/*
					if( $existeAnoAnterior[0]["mempagofnde"] == 't' )
					{
						$retorno = true;
					}
					*/
					$retorno = true;
				}
				else
				{
					if( $existeAnoAnterior[0]["mempagofnde"] == 'f' )
					{
						$retorno = true;
					}
				}
			}
			else
			{
				$retorno = true;
			}
		}
	}
	
	return $retorno;
}

/**
 * Verifica se as variáveis de sessão do Mais Educação
 * estão adequadamente setadas.
 * 
 * @return void
 */
function meVerificaSessao()
{
	/*** Se alguma das variáveis de sessão estiver nula ou vazia ***/ 
	if( empty($_SESSION['memid']) || is_null($_SESSION['memid']) || empty($_SESSION['meentid']) || is_null($_SESSION['meentid']) )
	{
		echo '<script>
		
				/*** Exibe o alerta de erro ***/
				alert("Ocorreu um erro interno.\n
				       O sistema irá redirecioná-lo à página inicial do módulo.");
				       
				/*** Redireciona o usuário ***/
				location.href = "pdeescola.php?modulo=inicio&acao=C";
				
			  </script>';
		die();
	}
}

/*** INICIO - FUNÇÕES DOS CÁLCULOS - INICIO ***/

// Cálculo dos Kits e Ressarcimento 
function calculoKitsRessarcimento($memid, $ano_exercicio, &$custeio, &$capital, &$ressarcimento, &$pagamentopst, $meses, $restricao = false, $nPagaKits = false) {
	global $db;
	
	$sql = "SELECT
				sum(mta.mtavlrcapital) as capital,
				sum(mta.mtavlrcusteio) as custeio,
				mea.meaid,
				mta.mtapst as pst,
				mem.memclassificacaoescola
				,mta.mtaid
				,(select count(*) from pdeescola.meatividade where meaano = '{$ano_exercicio}' and memid = ".(integer)$memid." and mtaid not in (877)) as tot_atividades
			FROM
				pdeescola.meatividade mea
			INNER JOIN
				pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid 
											 AND mta.mtasituacao = 'A'
											 AND mta.mtaanoreferencia = ".$ano_exercicio."
			INNER JOIN
				pdeescola.memaiseducacao mem ON mem.memid = mea.memid
			WHERE
				mea.memid = ".(integer)$memid." AND
				mea.meaano = ".$ano_exercicio."
			GROUP BY
				mea.meaid,
				mta.mtapst,
				mem.memclassificacaoescola
				,mta.mtaid";
	
	$kits = $db->carregar($sql);
// 	ver($sql);
	if($kits) {
		
		if($restricao) $contAtiv = 0;
		
		for($k=0; $k<count($kits); $k++) {
			
			if($ano_exercicio < 2012){
				$valorMonitorPorTurma = 60;
			}
			else{
				if($kits[$k]["memclassificacaoescola"] == 'R'){
					$valorMonitorPorTurma = 120;
				}else{
					if( (integer) $ano_exercicio == 2013 ){
						$valorMonitorPorTurma = 80;
					}else{
						$valorMonitorPorTurma = 60;
					}
				}
			}
			
			if(!$nPagaKits) {
				$custeio += $kits[$k]["custeio"];
				$capital += $kits[$k]["capital"];
// 				if(311714 == $memid) ver($custeio, $kits[$k]["custeio"], $kits[$k]["capital"]);
			}
			
			// Se for de 2009, permite somente o Ressarcimento para 6 atividades (máximo).
			if(!$restricao || ($restricao && $contAtiv < 6)) {
				$sql = "SELECT
							sum(mpa.mpaquantidade) as total_alunos
						FROM
							pdeescola.mealunoparticipanteatividade mpa
						WHERE
							mpa.meaid = ".$kits[$k]["meaid"];
				$totalAlunos = $db->pegaUm($sql);
				$totalAlunos = (integer)$totalAlunos;       

				if( (integer) $ano_exercicio == 2013 ){
					/*
					 * NOVA REGRA PARA CALCULO
					* Data: 05/08/2013
					* Calculo enviado por: Clarrissa Guedes Machado
					*/
					// ver($kits[$k]['mtaid'], trim($kits[$k]['mtaid']).' == 877', ($kits[$k]['mtaid'] == '877') );
					if( $kits[$k]['mtaid'] == '877' ){
						$ressarcimento += ( ( (ceil($totalAlunos / 15) * $meses ) * $valorMonitorPorTurma) );
						// 						ver($memid, $ressarcimento, ceil($totalAlunos / 15)."_turmas", "( ( (".$totalAlunos." / 15) * ".$meses.") * ".$valorMonitorPorTurma.")");
					}else{
						// 						$ressarcimento += ( ( ( (ceil($totalAlunos / 30) * $kits[$k]['tot_atividades'] )  * $meses ) * $valorMonitorPorTurma) );
						$ressarcimento += ( ( ( (ceil($totalAlunos / 30)  )  * $meses ) * $valorMonitorPorTurma) );
						// 						ver($memid, $ressarcimento, ceil($totalAlunos / 30)."_turmas", "( ( ( (".$totalAlunos." / 30) * ".$kits[$k]['tot_atividades'].") * ".$meses.") * ".$valorMonitorPorTurma.")");
					}
						

				}else{
					
					if($kits[$k]["pst"] != "t") {
						// Cálculo de 'Ressarcimento Monitores': (Total de Alunos / 25) X 60 X 11)
						//$ressarcimentoMonitores += ((ceil($totalAlunos / 30) * 60) * 6);
						$ressarcimento += ((ceil($totalAlunos / 30) * $valorMonitorPorTurma) * $meses);
							
						if($restricao) $contAtiv++;
					} else {
						//calculaPagamentoPST(&$pagamentopst, $totalAlunos);
						$pagamentopst += ((ceil($totalAlunos / 30) * $valorMonitorPorTurma) * $meses);
					}
				}
				
				if($restricao) $contAtiv++;

			}
		}
	}
}

// Cálculo do pagamento do PST
function calculaPagamentoPST($pst, $alunos) {
	switch($alunos) {
		case ($alunos <= 150):
			$qtdProf = 1;
			break;
		case (($alunos > 150) && ($alunos <= 300)):
			$qtdProf = 2;
			break;
		case (($alunos > 300) && ($alunos <= 450)):
			$qtdProf = 3;
			break;
		case (($alunos > 450) && ($alunos <= 600)):
			$qtdProf = 4;
			break;
		case (($alunos > 600) && ($alunos <= 750)):
			$qtdProf = 5;
			break;
		case (($alunos > 750) && ($alunos <= 900)):
			$qtdProf = 6;
			break;
		case (($alunos > 900) && ($alunos <= 1050)):
			$qtdProf = 7;
			break;
		case (($alunos > 1050) && ($alunos <= 1200)):
			$qtdProf = 8;
			break;
		case (($alunos > 1200) && ($alunos <= 1350)):
			$qtdProf = 9;
			break;
		case (($alunos > 1350) && ($alunos <= 1500)):
			$qtdProf = 10;
			break;
	}
	
	// atribui ao pagamento do PST o cálculo: quantidade de professores x R$900,00
	$pst = ($qtdProf * 900 * 10);
}

// Cálculo do Valor Limite
function calculoValorLimite($memid, $ano_exercicio, $numatividades, &$valorlimite, $meses) {
	global $db;
	
	$sql = "SELECT
				sum(mapquantidade) as alunado
			FROM
				pdeescola.mealunoparticipante
			WHERE
				memid = ".(integer)$memid." AND 
				mapano = ".$ano_exercicio."";
	$totalAlunadoParticipante = (integer)$db->pegaUm($sql);
	
	//$valorLimite = ((ceil(($totalAlunadoParticipante * (integer)$_REQUEST["num_atividades_calculadas"]) / 30) * 60) * 6);
	$valorlimite = ((ceil(($totalAlunadoParticipante * (integer)$numatividades) / 30) * 60) * $meses);
}

function calculoJovem15a17($memid, $ano_exercicio, $meses, $calculoJovem15a17)
{
	global $db;
	
	$sql = "select 
				majquantidadeai, 
				majquantidadeaf, 
				majtotalmatricula1517 
			from pdeescola.mealunojovemparticipante 
			where memid = {$memid} and majano = '{$ano_exercicio}'";
	
	$rs = $db->pegaLinha($sql);
	
	$valorMonitorPorTurma = 80;
	$totalJovem15a17 = $rs['majquantidadeai']+$rs['majquantidadeaf'];
	
	if($rs['majtotalmatricula1517']>0){
		$calculoJovem15a17 = ( ceil($totalJovem15a17 / 15) * $meses ) * $valorMonitorPorTurma;
	}
// 	ver($rs['majtotalmatricula1517'], $calculoJovem15a17, "( ({$totalJovem15a17} / 20) * {$meses} ) * {$valorMonitorPorTurma}");
}

function calculoPeif($memid, &$peifCusteio, &$peifCapital, $ano_exercicio, &$alunos = 0)
{
	global $db;
	
// 	$sql = "SELECT
// 				sum(mapquantidade) as alunado
// 			FROM
// 				pdeescola.mealunoparticipante
// 			WHERE
// 				memid = ".(integer)$memid." AND
// 				mapano = ".$ano_exercicio."";

	$sql = "
			SELECT
				sum(mecquantidadealunos) as total
			FROM
				pdeescola.mecenso
			WHERE
				entcodent = (select entcodent from pdeescola.memaiseducacao where memid = {$memid}) AND
				mecanoreferencia = '{$ano_exercicio}' AND
				mecserie in ( 1, 2, 3, 4, 5, 6, 7, 8, 9, 20, 21, 22 )
			";	
	$alunos = (integer)$db->pegaUm($sql);
	
	switch ($alunos){
		case ($alunos <= 300):
			$peifCusteio = 17000;
			$peifCapital = 3000;
			break;
		case (($alunos > 300) && ($alunos <= 600)):
			$peifCusteio = 19000;
			$peifCapital = 4000;
			break;
		case ($alunos > 600):
			$peifCusteio = 20000;
			$peifCapital = 5000;
			break;
	}
	
}


// Cálculo de Escola Aberta
function calculoEscolaAberta($memid, $ano_exercicio, &$escolaAbertaCapital, &$escolaAbertaCusteio, $meses) {
	global $db;
	
	$dados = $db->pegaLinha("select entid, eabqtduexatende from pdeescola.memaiseducacao where mamescolaaberta='t' and memid = ".$memid);
	$eabqtduexatende = $dados['eabqtduexatende'];
	$entid = $dados['entid'];
	
	if($entid){
		$sql = "SELECT
					sum(mecquantidadealunos) as total
				FROM
					pdeescola.mecenso
				WHERE
					entid = '".$entid."' AND
					mecanoreferencia = " . $ano_exercicio . " AND
					mecserie in (1,2,3,4,5,6,7,8,9)";
		$totalCenso = $db->pegaUm($sql);	
		
		
		//pega valor capital
		$escolaAbertaCapitalx = 0;
		if($eabqtduexatende == 1){
			$escolaAbertaCapitalx = 0;
		}
		else{
			$escolaAbertaCapitalx = 1000;
		}		
		
		//pega valor custeio
		$escolaAbertaCusteiox = 0;
		if($totalCenso <= 850){
			$escolaAbertaCusteiox = 1088.60;
		}elseif($totalCenso > 850 && $totalCenso <= 1700){
			$escolaAbertaCusteiox = 1217.20;
		}elseif($totalCenso > 1700){
			$escolaAbertaCusteiox = 1345.80;
		}
		
		// Cálculo de 'Escola Aberta Capital': Resultado do eabqtduexatende X 10
		//if($escolaAbertaCapitalx) {
			$escolaAbertaCapital = ( $escolaAbertaCapitalx );
		//}
		
		// Cálculo de 'Escola Aberta Custeio': Resultado do Total Alunado X 10
		//if($escolaAbertaCusteiox) {
			$escolaAbertaCusteio = ( $escolaAbertaCusteiox * $meses );
		//}
	}
	else{
		$escolaAbertaCapital = 0;
		$escolaAbertaCusteio = 0;
	}
	
}

// Cálculo de Serviços e Materiais
function calculoServicosMateriais($memid, $ano_exercicio, &$servicosmateriais, $meses, &$calculoServicosMateriaisCapital = 0, &$calculoServicosMateriaisCusteio = 0, $totalAlunado = 0) {
	global $db;
	
	$sql = "SELECT
				sum(map.mapquantidade) AS soma
			FROM
				pdeescola.mealunoparticipante map
			WHERE
				map.memid = ".(integer)$memid." AND
				map.mapano = ".$ano_exercicio;
	$totalAlunado = $db->pegaUm($sql);
	$totalAlunado = (integer)$totalAlunado;
	
	
	if( (integer) $ano_exercicio == 2013 ){
	
		switch($totalAlunado) {		
			case ($totalAlunado <= 500):
				$calculoServicosMateriaisCapital = 1000;
				$calculoServicosMateriaisCusteio = 3000;
				break;
			case (($totalAlunado > 500) && ($totalAlunado <= 1000)):
				$calculoServicosMateriaisCapital = 2000;
				$calculoServicosMateriaisCusteio = 6000;
				break;
			case ($totalAlunado > 1000):
				$calculoServicosMateriaisCapital = 2000;
				$calculoServicosMateriaisCusteio = 7000;
				break;
		}
		
	}
	
// 	else{
		
// 		if($totalAlunado) {			
// 			$servicosmateriais += ( escalonamento($totalAlunado) * $meses );			
// 		}
// 	}
	
	// Cálculo de 'Serviços/Materiais': Resultado do Escalonamento(Total Alunado) X 11(Custeio)
	//$servicosMateriais = ( escalonamento($totalAlunado) * 6 );
	if($totalAlunado) {
		$servicosmateriais += ( escalonamento($totalAlunado) * $meses );
	}
}

// Função para calcular o Escalonamento.
function escalonamento($numAlunos) {
	$numAlunos = (integer) $numAlunos;
	
	switch($numAlunos) {
		case ($numAlunos <= 500):
			$valorRetorno = 500; 
			break;
		case (($numAlunos > 500) && ($numAlunos <= 1000)):
			$valorRetorno = 1000; 
			break;
		case ($numAlunos > 1000):
			$valorRetorno = 1500;
			break;
	}
	
	return (integer)$valorRetorno;
}
/*** FIM - FUNÇÕES DOS CÁLCULOS - FIM ***/


function meMaxProgramacaoExercicio() {
	global $db;
	
	/*** GAMBI GO HORSE ***/
	//if( $_SESSION["exercicio"] == 2011 )
	//{
		//return 2011;
	//}
	//else
	//{
		$sql = "SELECT
					max(prsano)
				FROM
					pdeescola.programacaoexercicio
				WHERE
					prsstatus = 'A'
					AND prsexerccorrente = 't'";
		return (integer)$db->pegaUm($sql);
	//}
}



function me_verificaSessao(){
	if (!$_SESSION["meentid"]){
		echo "<script> window.location = '../logout.php';</script>";
		exit();
	}
}

/**
 * Recupera a escola, estado ou município
 * atribuído ao perfil do usuário no Mais Educação
 * 
 * @param string $resp
 * @return mixed
 * @author Felipe Carvalho
 */
function meRecuperaResponsabilidadePerfil($resp)
{
	global $db;

	$sql = "SELECT
				".$resp."
			FROM
				pdeescola.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION["usucpf"]."' 
				AND rpustatus = 'A'
				AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.")";

	return $db->pegaUm($sql);
}


/**
 * Função que monta as abas do 'Mais Educação'
 *
 * @return array
 * 
 * Since: 13/04/2009
 */
function carregaAbasMaisEducacao() {
	global $db;
	if(!$_SESSION['meentid']){

		$menu = array(
				  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=melista&acao=E"),
				  );
		
		$usuPerfil = arrayPerfil();
		//estadual
		if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) ||
		   in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || 
		   in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
		   	
			array_push($menu, array("id" => count($menu)+1, "descricao" => "Coordenador Estadual", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=E&acao=A") );
			 
		}
		
		//municipal
		if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) || 
		   in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || 
		   in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
		   	
		   	array_push($menu, array("id" => count($menu)+1, "descricao" => "Coordenador Municipal", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=M&acao=A") );
		   	
		}
		
	} else {

		//$memmodalidadeensino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']."");
		//$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memmodalidadeensino = '".$memmodalidadeensino."' AND memstatus = 'A'";
		$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'";
		
		$possuiAnoAnterior = $db->pegaUm($sql);
		
		/*** Aba de Quadras - Ficará desabilitada até que a funcionalidade seja aprovada ***/
		//7 => array("id" => 8, "descricao" => "Quadras", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/quadras&acao=A")
		
		$anoAnterior = $_SESSION['exercicio'] - 1;
		$entidResp = $db->pegaUm( "select entid from pdeescola.memaiseducacao where entid = ".$_SESSION['meentid']." AND memanoreferencia = ".$anoAnterior." AND memstatus = 'A'" );

		if($_SESSION['exercicio'] == 2013){
			$labelCoordenadorProfessor = "Professor Comunitário";
			$labelAnexoPME = "Espaços PME";
		}
		else{
			$labelCoordenadorProfessor = "Coordenador Mais Educação";
			$labelAnexoPME = "Anexos";
		}
		
		if($_SESSION['exercicio'] > 2011 && $entidResp ){
			
			if((integer)$possuiAnoAnterior > 0) {
				if($_SESSION['bo_cadastrador_mais_escola']){
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  1 => array("id" => 2, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  2 => array("id" => 3, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  3 => array("id" => 4, "descricao" => "Questionário de Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A"),
							  3 => array("id" => 5, "descricao" => "Atividades " . ((integer)$_SESSION["exercicio"] - 1), "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A"),
							  4 => array("id" => 6, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				} else {
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=melista&acao=E"),
							  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  2 => array("id" => 3, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  3 => array("id" => 4, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  4 => array("id" => 5, "descricao" => "Questionário de Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A"),
							  4 => array("id" => 6, "descricao" => "Atividades " . ((integer)$_SESSION["exercicio"] - 1), "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A"),
							  5 => array("id" => 7, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				}
			} else {
				if($_SESSION['bo_cadastrador_mais_escola']){
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  1 => array("id" => 2, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  2 => array("id" => 3, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  3 => array("id" => 4, "descricao" => "Questionário de Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A"),
							  3 => array("id" => 5, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
					
				} else {
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=melista&acao=E"),
							  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  2 => array("id" => 3, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  3 => array("id" => 4, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  4 => array("id" => 5, "descricao" => "Questionário de Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A"),
							  4 => array("id" => 6, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				}
			}
			
		}else{
			
			if((integer)$possuiAnoAnterior > 0) {
				if($_SESSION['bo_cadastrador_mais_escola']){
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  1 => array("id" => 2, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  2 => array("id" => 3, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  3 => array("id" => 4, "descricao" => "Atividades " . ((integer)$_SESSION["exercicio"] - 1), "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A"),
							  4 => array("id" => 5, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				} else {
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=melista&acao=E"),
							  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  2 => array("id" => 3, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  3 => array("id" => 4, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  4 => array("id" => 5, "descricao" => "Atividades " . ((integer)$_SESSION["exercicio"] - 1), "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A"),
							  5 => array("id" => 6, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				}
			} else {
				if($_SESSION['bo_cadastrador_mais_escola']){
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  1 => array("id" => 2, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  2 => array("id" => 3, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  3 => array("id" => 4, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
					
				} else {
					$menu = array(
							  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=melista&acao=E"),
							  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A"),
							  2 => array("id" => 3, "descricao" => "Diretor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
							  3 => array("id" => 4, "descricao" => "$labelCoordenadorProfessor", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
							  4 => array("id" => 5, "descricao" => "Atividades " . $_SESSION["exercicio"], "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A"),
							  );
				}
			}
			
		}		
		
		

		//verifica se a escola tem acesso a aba Escola Aberta
		if((integer)$possuiAnoAnterior > 0) {
			if($_SESSION["memid"]){
				$mamescolaaberta = $db->pegaUm("SELECT mamescolaaberta FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'");
			}
			if($mamescolaaberta == 't') {
				if($_SESSION["exercicio"] < 2013){
					$menu2 = array("id" => count($menu)+1, "descricao" => "Escola Aberta", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_programa&acao=A");
					array_push($menu,$menu2); 
				}
				else{
					
					$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'";
					$docid = $db->pegaUm($sql);
					$esdid = mePegarEstadoAtual( $docid );
					
					if(!in_array($esdid, array(CADASTRAMENTO_ME, false))){
						$menu2 = array("id" => count($menu)+1, "descricao" => "Relação Escola-comunidade", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/dados_programa&acao=A");
						array_push($menu,$menu2);
					}
				}
			}
		
		}
		//fim verifica se a escola tem acesso a aba Escola Aberta
		
		
		//nova aba para escolas de 2010
		/*
		$escolas2010 = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE memanoreferencia = 2010 and entid = ".$_SESSION["meentid"]." AND memstatus = 'A'");
		if( $escolas2010 && ($_SESSION["exercicio"] == 2013) ) {
			$menu3 = array("id" => count($menu)+1, "descricao" => "Jovens de 15 a 17", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/jovens15a17&acao=A");
			array_push($menu,$menu3);
		}
		*/
		if($_SESSION["memid"]){
			$memjovem1517 = $db->pegaUm("SELECT memjovem1517 FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'");
		}
		if($memjovem1517 == 't' && $_SESSION["exercicio"] == 2013) {
			$menu3 = array("id" => count($menu)+1, "descricao" => "Jovens de 15 a 17", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/jovens15a17&acao=A");
			array_push($menu,$menu3);
		}
		//fim nova aba para escolas de 2010
		
		
		//Anexos ou Espaços PME
		$menu4 = array("id" => count($menu)+1, "descricao" => "$labelAnexoPME", "link" => "pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A");
		array_push($menu,$menu4);
		
		/*
		$usuPerfil = arrayPerfilMaisEducacao();
		if(in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil)) {
			//coloca a aba do questionario
			$menu3 = array("id" => count($menu)+1, "descricao" => "Questionário", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A");
			array_push($menu,$menu3);
		} 
		*/

		//coloca a ultima aba
		//$menu4 = array("id" => count($menu)+1, "descricao" => "Parceiros", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A");
		//array_push($menu,$menu4); 
		
		
		#ABA QUESTIONARIO - É EXIBIDA APENAS PARA ESCOLAS COM EXERCICIO (MEMID REFERENTE A 2012) EM 2012.
		$dados_escolas_2012 = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE memanoreferencia = 2012 and entid = ".$_SESSION["meentid"]." AND memstatus = 'A'");
		if($dados_escolas_2012 && $_SESSION["exercicio"] == 2013){
			$menu5 = array("id" => count($menu)+1, "descricao" => "Questionário Monitoramento Físico-Financeiro", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A");
			array_push($menu,$menu5);
		}
		
		
		//coloca a ultima aba
		if($_SESSION["exercicio"] < 2013) {
			$menu6 = array("id" => count($menu)+1, "descricao" => "Parceiros", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A");
			array_push($menu,$menu6);
		}
		else{
			$menu6 = array("id" => count($menu)+1, "descricao" => "Verificar Pendências", "link" => "/pdeescola/pdeescola.php?modulo=meprincipal/verificaPendencias&acao=A");
			array_push($menu,$menu6);
		}
		
		
	}		
	$menu = $menu ? $menu : array();
	
	return $menu;
	
}

/**
 * Função para montar o cabeçalho usado nas páginas do 'Mais Educação'
 * 
 * @return string
 * 
 * Since: 15/04/2009
 */
function cabecalhoMaisEducacao() {

	global $db;
	
	$entid = $_SESSION['meentid'];
	
	$sql = "SELECT DISTINCT
				est.estdescricao as est,
				est.estuf,
				mun.mundescricao as mun,
				ent.entnome as esc
			FROM
				entidade.entidade ent 
			INNER JOIN 
				entidade.endereco ende ON ent.entid = ende.entid
			INNER JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN 
				territorios.estado est ON est.estuf = mun.estuf		
			WHERE
				--ent.funid = 3 and
			  	--ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$entid}')";
// 	ver($sql, d);
	$dados = $db->carregar($sql);
	
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
			 	<tr>
			 		<td colspan=\"2\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\">
			 			<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(".$_SESSION['meentid'].");\" ><img style=\"vertical-align:middle;\" src=\"/imagens/globo_terrestre.png\" border=\"0\" title=\"Exibir Mapa\"> Georeferenciamento: Itinerário Educativo</a>
			 		</td>
			 	</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Escola</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['esc']}</td>
				</tr>			 
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['mun']}</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Estado</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['est']}</td>
				</tr>
			 </tbody>
			</table>";
	
	return $cab;
}

/**
 * Recupera os perfis do usuário somente relacionados
 * ao Mais Educação
 */
function arrayPerfilMaisEducacao()
{
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 34
					WHERE
					 pu.usucpf = '%s'
					 AND pu.pflcod in (".PDEESC_PERFIL_SUPER_USUARIO.",
					 				   ".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.",
					 				   ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO.",
					 				   ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO.",
					 				   ".PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO.",
					 				   ".PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO.")
					ORDER BY
					 p.pflnivel",
	$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

/*
 * Monta lista de Escolas
 * Em conformidade com o filtro
 */
function melista(){
	global $db;
	$ano = $_SESSION["exercicio"];
	$anoAnterior = $ano -1;
	$anoAnt = $anoAnterior -1;
	
	$anoMaximo = $db->pegaUm("select max(prsano) from pdeescola.programacaoexercicio");
	if(!$_GET['memanoreferencia']) $_GET['memanoreferencia'] = $ano;

	/*
	 * Filtro
	 * Escola, Código, Estado, Municipio, Situação, Tipo
	 */
	if ($_POST['escola'])
		$where[] = " UPPER(e.entnome) LIKE UPPER('".tratarStrBusca($_POST['escola'])."')";

	if ($_POST['entcodent'])
		$where[] = " e.entcodent LIKE '%".$_POST['entcodent']."%'";	
		
	if ($_REQUEST['estuf'])
		$where[] = " m.estuf = '".$_REQUEST['estuf']."'";
	elseif($_SESSION['maiseducacao']['filtro']['estuf'])
		$where[] = " m.estuf = '".$_SESSION['maiseducacao']['filtro']['estuf']."'";

	if($_POST['muncod'])
		$where[] = " m.muncod = '".$_POST['muncod']."'";
	elseif($_SESSION['maiseducacao']['filtro']['muncod'])
		$where[] = " m.muncod = '".$_SESSION['maiseducacao']['filtro']['muncod']."'";
		
	if ($_REQUEST['esdid'] == '0'){
		$_REQUEST['esdid'] = "naoiniciado";
	}
 	if ($_REQUEST['esdid']) {
		$naoIniciado = "";
		
		if($_REQUEST['esdid'] != "naoiniciado")
			$where[] = " est.esdid = '".$_REQUEST['esdid']."'";
		else
			$naoIniciado = "maedu.docid is null and";
	} 

	if($_POST['tpcid'])
		$where[] = " e.tpcid IN (".$_POST['tpcid'].")";
	elseif($_SESSION['maiseducacao']['filtro']['tpcid'])
		$where[] = " e.tpcid IN (".$_SESSION['maiseducacao']['filtro']['tpcid'].")";
	//else
		//$where[] = " e.tpcid IN (1,3)";		
		
	if ( $_POST['usuativo'] ){
		$where1 = "WHERE ativo = 'Sim'"; 
	}elseif ( isset($_POST['usuativo']) ){
		$where1 = "WHERE ativo = 'Não'"; 
	}	

	if( $_REQUEST['modalidade'] == 'F') {
		$where[] = " maedu.memmodalidadeensino = 'F' ";
	}
	else if( $_REQUEST['modalidade'] == 'M') {
		$where[] = " maedu.memmodalidadeensino = 'M' ";
	}
	
	if( $_REQUEST['classificacao'] == 'U') {
		$where[] = " maedu.memclassificacaoescola = 'U' ";
	}
	else if( $_REQUEST['classificacao'] == 'R') {
		$where[] = " maedu.memclassificacaoescola = 'R' ";
	}
	else if( $_REQUEST['classificacao'] == 'A') {
		$where[] = " maedu.mamescolaaberta = 't' ";
	}else if( $_REQUEST['classificacao'] == 'J') {
		$where[] = " maedu.memjovem1517 = 't' ";
	}
	/*
	if( $_GET['memanoreferencia'] == $ano) {
		$where[] = " maedu.entcodent not in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')";
	}
	else if( $_GET['memanoreferencia'] == $anoAnterior) {
		$where[] = " maedu.entcodent in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')";
	}
	
	if( $_GET['memanoreferencia'] == $anoMaximo) {
		$where[] = " maedu.memid not in (select mem.memid from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')";
	}
	*/
	
	
	if( $_REQUEST['aderiupst'] == 'S' ) {
		
		$where[] = " maedu.memadesaopst = 'S' ";
	}
	elseif( $_REQUEST['aderiupst'] == 'N' ) {
		
		$where[] = " maedu.memadesaopst = 'N' ";
	}
	elseif( $_REQUEST['aderiupst'] == 'null' ) {
		
		$where[] = " maedu.memadesaopst is null ";
	}  
	
	if ( $_REQUEST['anoanterior'] == 1){
		$where[] = " maedu.entcodent in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')"; 
	}
	else{
		//$where[] = " maedu.entcodent not in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')";
	}
	
	

	if( $_REQUEST['escolasAnexo'] == 1) {
		$where[] = " aqb.arqid is not null ";
	}
	if( $_REQUEST['escolasPBF'] == 1) {
		$where[] = " maedu.memmaioriapbf = 't' ";
	}
	
	if($_REQUEST['tipoescola']){
		switch ($_REQUEST['tipoescola']){
			case 'proemi':
				$where[] = "mceproemi = 't' and mcepme = 'f'";
				break;
			case 'pme':
				$where[] = "mceproemi = 'f' and mcepme = 't'";
				break;
			case 'ambos':
				$where[] = "mceproemi = 't' and mcepme = 't'";
				break;
		}
	}	
	
	/*
	 * Carrega array com perfis do usuário
	 */	
	$perfil = arrayPerfilMaisEducacao();
	
	/*
	 * Caso não tenha acesso global
	 * vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade"
	 */
	$from = "";
	if (    in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) 
		 || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil)  
		 || in_array(PDEESC_PERFIL_CONSULTA, $perfil) 
		) {
		$from = "";
    } else {
    	if ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $perfil) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $perfil)){
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)"; 
		} elseif ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $perfil)) { //Perfil PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO só ver na sua escola ESTADUAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)";
		} elseif ( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $perfil)) { //Perfil PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO só ver na sua escola MUNICIPAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid
	 					)";
		}
    } 
    
    
    $memStatus = ( !$_REQUEST['status'] ) ? "AND maedu.memstatus = 'A'" : "AND maedu.memstatus = 'I'";
    //(($_GET['memanoreferencia']) ? $_GET['memanoreferencia'] : $_SESSION["exercicio"])
        
    /**
     * Verifica se o usuário tem perfil de 'Super Usuário' ou 'Administrador'
     * e habilita a opção de alterar o memstatus da escola
     */
    if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfil) )
    {
    	$acao = "CASE WHEN maedu.memstatus = 'A'
    			 THEN '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"inativarEscola(' || maedu.memid || ')\"><img src=\"/imagens/valida6.gif\" border=0 title=\"Inativar Escola\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\" ><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
    			 ELSE
    			 	  '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"ativarEscola(' || maedu.memid || ')\"><img src=\"/imagens/valida1.gif\" border=0 title=\"Ativar Escola\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\"><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
    			 END";
    }
    else
    {
    	$acao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\" ><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'";
    }
    
if($_POST['requisicao'] == 'excel'){
    	$acoes = "";
    } else {
    	$acoes = " {$acao} as acao, ";    	
    }
    
	$sql = sprintf("SELECT * FROM(
						SELECT DISTINCT
							 $acoes
							 e.entcodent,
							 e.entnome,
							 CASE
							  WHEN e.tpcid = 1 THEN 'Estadual'
							  ELSE 'Municipal'
							 END AS tipo,
							 m.estuf, 
							 m.mundescricao,
							 CASE
						  	  WHEN est.esdid IS NOT NULL THEN est.esddsc
						  	  ELSE 'Não Iniciado'
						 	 END AS situacao,
							 CASE WHEN
							 	ur1.entid is not null THEN 'Sim' ELSE 'Não' END as ativo,
							 CASE WHEN maedu.memmodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END,
							 CASE WHEN maedu.memadesaopst = 'S' THEN 'Sim'
							 WHEN maedu.memadesaopst = 'N' THEN 'Não'							 
							 ELSE '-' END AS pst	 
						FROM
							 entidade.entidade e
						INNER JOIN 
							 entidade.endereco endi ON endi.entid = e.entid
						LEFT JOIN 
							 territorios.municipio m ON m.muncod = endi.muncod
						LEFT JOIN	
							 pdeescola.usuarioresponsabilidade ur1 ON ur1.entid = e.entid AND ur1.rpustatus = 'A' AND ur1.pflcod = 383
							 %s
						INNER JOIN
							 pdeescola.memaiseducacao maedu ON %s maedu.entid = e.entid AND maedu.memanoreferencia = ".$_GET['memanoreferencia']." {$memStatus}
						LEFT JOIN 
							 pdeescola.mearquivos aqb ON aqb.memid = maedu.memid
						LEFT JOIN 
							 public.arquivo arq ON arq.arqid = aqb.arqid
						LEFT JOIN 
							 workflow.documento d ON d.docid = maedu.docid
						LEFT JOIN 
							 workflow.estadodocumento est ON est.esdid = d.esdid
							 %s %s ) as foo 
						%s",
				$from,
				$naoIniciado,
				$where ? " WHERE ".implode(' AND ', $where)." " : ' ',
				$and,
				$where1 ? $where1 : '');
				
// 				dbg($sql,1);
// 	ver(simec_htmlentities($sql));
	if($_POST['requisicao'] == 'excel'){
		$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Usuário Ativo", "Ensino", "Aderiu PST");
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%', 'S');
		exit;
	} else {
		$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Usuário Ativo", "Ensino", "Aderiu PST");
		$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', '', 3600);
	}
}

/**
 * Função para Verificar se existe a Entidade
 * 
 * @return string
 * 
 * Since: 29/04/2009
 */
function boExisteEntidade( $entid ){
	global $db;
	$entidade = "";
	
	if($entid){
		$entidade = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid = {$entid}");
	}	
	
	return $entidade;
}

/**
 * Função para Verificar se existe a Diretor ou Coordenador para Entidade escolhida
 * 
 * @return string
 * 
 * Since: 29/04/2009
 */
function existeDiretorCoordenadorPorCpf($funid){ // Função feita para atender necessidade do Cliente com urgência
	global $db;
	
	# Comentado por causa das modificações da entidade
	/*$sql = "SELECT mep.entid FROM pdeescola.memaiseducacao mee
			  inner join pdeescola.mepessoal mep on mee.memid = mep.memid
		      inner join entidade.entidade e on mep.entid = e.entid
		      inner join entidade.funcaoentidade fe on e.entid = fe.entid
			where mee.entid = $entid and fe.funid = $funid and mep.mepstatus = 'A' and fe.fuestatus = 'A' ";*/
	
	$entid = $_SESSION['meentid'];
	
	/*
	 * Correção por Alexandre Dourado 17/11/09
	 */
	if(!$entid) {
		echo "<script>
				alert('Entidade não encontrada. Refaça o procedimento.');
				window.location='pdeescola.php?modulo=melista&acao=E&requisicao=cadastra';
			  </script>";
		exit;
	}
	
	$sql = "SELECT e.entnumcpfcnpj FROM entidade.entidade e  
			INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
			INNER JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
			WHERE fea.entid = '".$entid."' AND fe.funid = '".$funid."'";
	$cpfDiretorCoord = $db->pegaUm($sql);
	return $cpfDiretorCoord;
}

/**
 * Função para Verificar se existe a algum parceiro cadastrado para a Entidade passada
 * 
 * @return string
 * 
 * Since: 29/04/2009
 */
function existeParceiro($entidDirCor = false){
	global $db;
	
	$entid = $_SESSION['meentid'];
	
	/*
	 * Correção por Alexandre Dourado 17/11/09
	 */
	if(!$entid) {
		echo "<script>
				alert('Entidade não encontrada. Refaça o procedimento.');
				window.location='pdeescola.php?modulo=melista&acao=E&requisicao=cadastra';
			  </script>";
		exit;
	}
	
	$sql = "SELECT e.entnumcpfcnpj FROM pdeescola.memaiseducacao mee
		  inner join pdeescola.meparceiro mep on mee.memid = mep.memid
	      inner join entidade.entidade e on mep.entid = e.entid
	      inner join entidade.funcaoentidade fe on e.entid = fe.entid
		where mee.entid = $entid and fe.funid = ". FUN_PARCEIRO_ME . "  and fe.fuestatus = 'A' ";
	if($entidDirCor){
		$sql .= "and mep.entid = ". $entidDirCor;
	}
		
	$sql .= " limit 1";
	
	$boExisteParceiro = $db->pegaUm($sql);
	
	return $boExisteParceiro;
}

function existeAtividadesAnoAtual($memid) { 
	global $db;
	
	$sql = "SELECT
				count(*)
			FROM
				pdeescola.meatividade
			WHERE
				memid = ".$memid." AND
				meaano = " . $_SESSION["exercicio"];
	$existe = $db->pegaUm($sql);
	
	if($existe)
		return true;
	else
		return false;
}

function existeAtividadesAnoAnterior($memid) {
	global $db;
	
	$memid = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'"); 
	
	$sql = "SELECT
				meacomecounoano,
				meaqtdefetivaaluno
			FROM
				pdeescola.meatividade
			WHERE
				memid = ".$memid." AND
				meaano = ".((integer)$_SESSION["exercicio"] - 1);
	
	$dados = $db->carregar($sql);
	
	$existe = true;
	
	for($i=0; $i<count($dados); $i++) {
		if(($dados[$i]["meacomecounoano"] == NULL) || ($dados[$i]["meacomecounoano"] == "") || 
		   ($dados[$i]["meaqtdefetivaaluno"] == NULL) || ($dados[$i]["meaqtdefetivaaluno"] == "")) {
		   		$existe = false;
		   }
	}
	
	if($existe)
		return true;
	else
		return false;
}

/*
 ********************************** FUNÇÕES WORKFLOW ***************************
 */
function mePegarDocid( $entid , $memid ) {
	global $db;
	
	$entid = (integer) $entid;
	$memid = (integer) $memid;
	
	$sql = "SELECT
			 docid
			FROM
			 pdeescola.memaiseducacao
			WHERE
			 memid = " . $memid . " AND 
			 memstatus = 'A'";
	return (integer) $db->pegaUm( $sql );
}

 

/*function pegarMemid( $entid ) {
	global $db;
	$entid = (integer) $entid;
	$sql = "SELECT
			 memid
			FROM
			 pdeescola.memaiseducacao
			WHERE
			 memanoreferencia = " .$_SESSION["exercicio"]. " AND
			 entid  = " . $entid;
	return (integer) $db->pegaUm( $sql );
}*/

function meCriarDocumento( $entid, $memid ) {
	global $db;
	
	if(!$entid) return false;
	
	$docid = mePegarDocid($entid, $memid);
	
	if( ! $docid ){
		
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */
		/*$sqlTpdid = "SELECT
					  t.tpdid 
					 FROM 
					  seguranca.sistema s					
					  INNER JOIN workflow.tipodocumento t ON s.sisid = t.sisid					
					 WHERE
					  s.sisid = '".$_SESSION['sisid']."'";
		$tpdid = $db->pegaUm( $sqlTpdid );*/
		$tpdid = TPDID_MAIS_EDUCACAO;
		
		/*
		 * Pega nome da entidade
		 */
		$sqlDescricao = "SELECT
						  entnome
						 FROM
						  entidade.entidade
						 WHERE
						  entid = '" . $entid . "'";
		
		$descricao = $db->pegaUm( $sqlDescricao );
		
		$docdsc = "Cadastramento maiseducacao - " . $descricao;
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		
		//if ($memid = pegarMemid($entid)){
		if($memid) {
			$sql = "UPDATE pdeescola.memaiseducacao SET 
					 docid = ".$docid." 
					WHERE
					 memid = ".$memid;	

			$db->executar( $sql );		
			$db->commit();
			return $docid;
		}else{
			return false;
		}
	}
	else {
		return $docid;
	}
}

function mePegarEstadoAtual( $docid ) {
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

function meVerificaPendencias( $memid ) {
	if (!$memid){
		$memid = $_SESSION["memid"];
	}
	
	global $db;
	
	$controlePendencias = true;
	
	if(!existeDiretorCoordenadorPorCpf(19)) $controlePendencias = false;
	if(!existeDiretorCoordenadorPorCpf(41)) $controlePendencias = false;
	
	/*$sql = "SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1);
	$possuiAnoAnterior = $db->carregar($sql);
	
	if($possuiAnoAnterior) {
		if(!existeAtividadesAnoAnterior($memid)) $controlePendencias = false;
	}*/
	
	if( podeFazerPlano2011() )
	{
		if( !existeAtividadesAnoAtual($memid) )
		{
			$controlePendencias = false;
		}
	}
	
	/*** Se a escola tiver atividades no ano anterior e atividades no ano atual cadastradas, 
	 *   e não informou se começou as atividades do ano anterior, bloqueia a tramitação.
	 **/
	/*
	$sql = "SELECT DISTINCT
				count(mem.*) as num
			FROM
				pdeescola.memaiseducacao mem
			INNER JOIN
				 pdeescola.memaiseducacao mem2 on mem2.entid = mem.entid and
				 mem2.memstatus = 'A' and
				 mem2.memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." and
				 mem2.memmodalidadeensino = mem.memmodalidadeensino
			INNER JOIN
				 pdeescola.meatividade mea on mea.memid = mem.memid and 
				 							  mea.meaano = ".$_SESSION["exercicio"]."
			INNER JOIN
				pdeescola.meatividade mea2 on mea2.memid = mem2.memid and 
											  mea2.meaano = ".((integer)$_SESSION["exercicio"] - 1)." and
			     							  mea2.meacomecounoano is null
			WHERE
				mem.memid = ".$memid;
	$num = $db->pegaUm($sql);
	
	if( (integer)$num > 0 )
	{
		$controlePendencias = false;
	}
	*/
	
	/*if(!existeAtividadesAnoAtual($memid)) {
		if($possuiAnoAnterior) {
			$memvlrpago = $db->pegaUm("SELECT memvlrpago FROM pdeescola.memaiseducacao WHERE memid = ".$memid);
			$atividadesAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$possuiAnoAnterior[0]["memid"]." AND meaano = ".((integer)$_SESSION["exercicio"] - 1)." AND meacomecounoano = 't'");
			
			if($memvlrpago || (integer)$atividadesAnoAnterior > 0) {
				$controlePendencias = false;
			}
		} else {
			$controlePendencias = false;
		}
	}*/
	
	//if(!existeParceiro()) $controlePendencias = false;
	
	//verifica outras pendencias
	$arPendencias = meVerificaPendencias2();
	if($arPendencias) $controlePendencias = false;
		
	
	return (boolean) $controlePendencias;
}


function meVerificaPendencias2() {
	global $db;
	
	$memid = $_SESSION['memid'];
	if (!$memid) return false;
	
	//array verifica pendencias
	/*
	$arPendencias = array(
	  					  'Atividades 2012' => 'Falta o preenchimento dos dados.',
						  'Atividades 2013' => 'Falta o preenchimento dos dados.',
						  'Relação Escola-comunidade' => 'Falta o preenchimento dos dados.',
						  'Jovens de 15 a 17' => 'Falta o preenchimento dos dados.',
						  'Espaços PME' => 'Falta o preenchimento dos dados.',
						  'Questionário Monitoramento Físico-Financeiro' => 'Falta o preenchimento dos dados.');
	 */
	$arPendencias = array();
	//array_push_associative($arPendencias, array('Atividades 2013' => 'Falta o preenchimento dos dados.'));
	
	//verifica se a escola participou no ano anterior
	$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'";

	$possuiAnoAnterior = $db->pegaUm($sql);
		
	if( (integer)$possuiAnoAnterior > 0 ) {
		
		//ABA ATIVIDADES 2012
		$memid_ano_anterior = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
 
		$sql = "SELECT
					count(mea.meaid) as total
				FROM
					pdeescola.meatividade mea
				INNER JOIN
					pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid AND mta.mtasituacao = 'A' 
				INNER JOIN
					pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid	AND mtm.mtmsituacao = 'A' 
				WHERE
					mea.memid = ".$memid_ano_anterior." AND
					mea.meaano = ".((integer)$_SESSION["exercicio"] - 1)." and
					mea.meacomecounoano is null";
	
		$totalAba2012 = $db->pegaUm($sql);
		
		if($totalAba2012 > 0){
			array_push_associative($arPendencias, array('Atividades 2012' => 'Falta o preenchimento dos dados.'));
		}
		
		//Aba Relação Escola-comunidade (escola aberta)
		$mamescolaaberta = $db->pegaUm("SELECT mamescolaaberta FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
		if($mamescolaaberta == 't'){
			$sql = "SELECT count(eaba.eatid) FROM pdeescola.meeabatividade eaba
					WHERE eaba.memid = ".$_SESSION["memid"];
			$totalEscolaAberta = $db->pegaUm($sql);
			
			$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'";
			$docid = $db->pegaUm($sql);
			$esdid = mePegarEstadoAtual( $docid );
			
// 			if($totalEscolaAberta == 0){
			if($totalEscolaAberta == 0 && !in_array($esdid, array(CADASTRAMENTO_ME, false))){
				array_push_associative($arPendencias, array('Relação Escola-comunidade' => 'Falta o preenchimento dos dados.'));
			}
		}
		
		//Aba jovens de 15 a 17
		$memjovem1517 = $db->pegaUm("SELECT memjovem1517 FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
		if($memjovem1517 == 't'){
			$sql = "SELECT
						coalesce(majquantidadeai,0)+coalesce(majquantidadeaf,0) as total
					FROM
						pdeescola.mealunojovemparticipante
					WHERE
						memid = ".$_SESSION["memid"]." AND
						majano = ".$_SESSION["exercicio"];
			$totalJovem1517 = $db->pegaUm($sql);
			if($totalJovem1517 == 0){
				array_push_associative($arPendencias, array('Jovens de 15 a 17' => 'Falta o preenchimento dos dados.'));
			}
		}
		
		//Aba Questionario
		#ABA QUESTIONARIO - É EXIBIDA APENAS PARA ESCOLAS COM EXERCICIO (MEMID REFERENTE A 2012) EM 2012.
		$dados_escolas_2012 = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE memanoreferencia = 2012 and entid = ".$_SESSION["meentid"]." AND memstatus = 'A'");
		if($dados_escolas_2012 && $_SESSION["exercicio"] == 2013){
			$queid = QUESTIONARIO_MONIT_FISICO_FINANC;
			$sql = "Select	qp.qrpid
					From  pdeescola.pdequestionario qp
					Join questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
					Where qp.memid = ".$_SESSION["memid"]." and qr.queid = ".$queid;

			$qrpid = $db->pegaUm( $sql );
			
			if($qrpid){
				$sql = "select case
       						when (select count(*) from pdeescola.meatividade where meaano = 2012 and memid = ".$memid_ano_anterior.") != 0 then 
	    						 (select cast('t' as text))
       						else
	    						 (select cast('f' as text))
						end";

				$gprg2 = $db->pegaUm( $sql );

				$sql = "select case
							when (select count(*) from pdeescola.memaiseducacao where memanoreferencia = 2012 and entid = ".$_SESSION["meentid"]." and mamescolaaberta = 't') != 0 then
							  	 (select cast('t' as text))
							else
								 (select cast('f' as text))
						end";

				$gprg14 = $db->pegaUm( $sql );
				

				$sql = "SELECT 
						      count(resid2) as total
						FROM (
							SELECT 
							      grpordem, gp.grptitulo, p.perordem, p.pertitulo, p.pertipo, ip.*, r.resid as resid1, r2.resid as resid2 
							FROM 
							      pdeescola.pdequestionario pq
							INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = pq.qrpid
							INNER JOIN questionario.grupopergunta gp ON gp.queid = qr.queid
							inner join questionario.pergunta p on p.grpid = gp.grpid
							left join questionario.itempergunta ip 
							                inner join questionario.resposta r ON r.itpid = ip.itpid AND r.qrpid = $qrpid
							ON ip.perid = p.perid
							left join questionario.resposta r2 ON p.perid = r2.perid  AND r2.qrpid = $qrpid
							WHERE 
				                qr.qrpid = $qrpid ";

						//Exclui a pergunta 2 para as escolas que não preencheram atividades em 2012				
						if ($gprg2 == 'f')		
							$sql = $sql." AND  gp.grpid != 651 ";							                

						//Exclui a pergunta 14 para as escolas que não são abertas				
						if ($gprg14 == 'f')		
							$sql = $sql." AND  gp.grpid != 663 ";							                

						$sql = $sql.") as foo

						GROUP BY
						                grpordem, grptitulo, perordem, pertitulo
						having count(foo.resid2) = 0
						ORDER BY
						                grpordem, perordem";				
				
				$totalQuestionario = $db->carregar($sql);
				
				if($totalQuestionario){
					array_push_associative($arPendencias, array('Questionário Monitoramento Físico-Financeiro' => 'Falta o preenchimento dos dados.'));
				}
			}
			else{
				array_push_associative($arPendencias, array('Questionário Monitoramento Físico-Financeiro' => 'Falta o preenchimento dos dados.'));				
			}
			
		}
	}


	//ABA ATIVIDADES 2013
	$sql = "SELECT 
									count(mtm.mtmid)
								FROM
									pdeescola.meatividade mea
								INNER JOIN
									pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
												AND mta.mtasituacao = 'A'
								INNER JOIN
									pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
												AND mtm.mtmsituacao = 'A'
								WHERE
									mea.memid = ".$_SESSION["memid"]." AND
									mea.meaano = ".$_SESSION["exercicio"];
	$totalAba2013 = $db->pegaUm($sql);
	if($totalAba2013 == 0){
		array_push_associative($arPendencias, array('Atividades 2013' => 'Falta o preenchimento dos dados.'));
	}
	
	
	return $arPendencias;
	
}

function array_push_associative(&$arr) {
   $args = func_get_args();
   foreach ($args as $arg) {
       if (is_array($arg)) {
           foreach ($arg as $key => $value) {
               $arr[$key] = $value;
               $ret++;
           }
       }else{
           $arr[$arg] = "";
       }
   }
   return $ret;
}



function meMudaSituacaoIndo(){
	global $db;
	
	$memid = $_SESSION['memid'];

	if ( !$memid )
		return false;
		
	$sql = "SELECT
				esdid
			FROM
				pdeescola.memaiseducacao m
				INNER JOIN workflow.documento d ON d.docid = m.docid
			WHERE
				memid = " . $memid;
	
	$esdid = $db->pegaUm($sql);
	
	
	switch ($esdid){
		
		case CADASTRAMENTO_ME:
			$mesid = ME_SIT_NAO_CADASTRADO;		
		break;
		
		case AVALIACAO_SECRETARIA_ME:
			$mesid = ME_SIT_CADASTRADO;
		break;
		
		case AVALIACAO_MEC_ME:
			$mesid = ME_SIT_APROVADO;
		break;
		
		case FINALIZADO_ME:
			$mesid = ME_SIT_FINALIZADO;
		break;
		
	}
	
	$sql = "UPDATE
				pdeescola.memaiseducacao
			SET
				mesid = " . $mesid . "
			WHERE
				memid = " . $memid;
	
	$db->executar($sql);
	$db->commit();
	
	return true;
}

function meMudaSituacaoVolta(){
	global $db;
	
	$memid = $_SESSION['memid'];
		
	if ( !$memid )
		return false;
	
	$sql = "SELECT
				esdid
			FROM
				pdeescola.memaiseducacao m
				INNER JOIN workflow.documento d ON d.docid = m.docid
			WHERE
				memid = " . $memid;
	
	$esdid = $db->pegaUm($sql);
	
	
	switch ($esdid){
		
		case CADASTRAMENTO_ME:
			$mesid = ME_SIT_NAO_APROVADO_SEC;		
		break;
		
		case AVALIACAO_SECRETARIA_ME:
			$mesid = ME_SIT_NAO_APROVADO_SECAD;
		break;
		
	}
		
	$sql = "UPDATE
				pdeescola.memaiseducacao
			SET
				mesid = " . $mesid . "
			WHERE
				memid = " . $memid;
	
	$db->executar($sql);
	$db->commit();
	
	return true;
	
}

/*
 * Função da Pós-Ação da opção de 'Retornar para finalizado' do workflow
 */
function excluirEscolaLote() {
	 global $db;
	 
	 $sql = "DELETE FROM pdeescola.meloteimpressao WHERE entid = ".$_SESSION["meentid"];
	 $db->executar($sql);
	 $db->commit();
	 return true;
}

//função será chamada por callback no workflow ao voltar-se o estado de "relatorio consolidado" para "finalizado".
function voltarParaFinalizado() {
	global $db;  
	$sql = "DELETE from pdeescola.meloteimpressao WHERE memid = ".$_SESSION['memid']; 
	$db->executar($sql );
	$db->commit(); 
	return true;
}

//função que retorna o id do questionário
function pegaQrpidME( $memid, $queid ){
    
	global $db;
    
    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
   
    $sql = "SELECT
            	mme.qrpid
            FROM
            	pdeescola.memaiseducacao mme
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = mme.qrpid
            WHERE
            	mme.memid = {$memid} 
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	$sql = "SELECT e.entnome FROM pdeescola.memaiseducacao mme INNER JOIN entidade.entidade e ON e.entid = mme.entid WHERE mme.memid = ".$memid;
        $titulo = $db->pegaUm( $sql );
        $arParam = array ( "queid" => $queid, "titulo" => "MAIS EDUCACAO (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE pdeescola.memaiseducacao SET qrpid = {$qrpid} WHERE memid = ".$memid;
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

//verifica se o questionário foi inteiramente preenchido!
function verificaQuestionarioME( $memid ){
	
	global $db;
	
	$sql = "SELECT
            	mme.qrpid
            FROM
            	pdeescola.memaiseducacao mme
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = mme.qrpid
            WHERE
            	mme.memid = {$memid} 
            	AND qr.queid = ".QUESTIONARIO_MAISEDUC;
    $qrpid = $db->pegaUm( $sql );
    
    if( !$qrpid ){
    	//erro 1000 = O usuário ainda não acessou o questionário.
    	return 1000;
    } else {
	/*
		$sql = "(SELECT
					p.perid AS perid,
					p.pertitulo AS pergunta
				FROM
					questionario.questionario q
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				WHERE
					q.queid = ".QUESTIONARIO_MAISEDUC."
				
				
				UNION ALL
				
				
				SELECT
					p.perid AS perid,
					p.pertitulo AS pergunta
				FROM
					questionario.questionario q
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp2 ON gp2.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp2.grpid
				WHERE
					q.queid = ".QUESTIONARIO_MAISEDUC.")
				ORDER BY
						perid";
		
		$arrPerid = $db->carregar( $sql );
	*/			
    	$arrRespondido = array();
    	
		$sql = "(SELECT
					p.perid AS perid,
					p.pertitulo,
					r.itpid::varchar as resposta
				FROM
					pdeescola.memaiseducacao qq
				JOIN entidade.entidade ent USING (entid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid AND q.queid = ".QUESTIONARIO_MAISEDUC."
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid = ip.itpid
				WHERE
					qq.qrpid = ".$qrpid."
				
				UNION ALL
				
				SELECT
					p.perid AS perid,
					p.pertitulo,
					r.resdsc as resposta
				FROM
					pdeescola.memaiseducacao qq
				JOIN entidade.entidade ent USING (entid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid AND q.queid = ".QUESTIONARIO_MAISEDUC."
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.resdsc IS NOT NULL
				WHERE
					qq.qrpid = ".$qrpid."
				
				UNION ALL

				SELECT
					p.perid AS perid,
					p.pertitulo,
					r.itpid::varchar as resposta
				FROM
					pdeescola.memaiseducacao qq
				JOIN entidade.entidade ent USING (entid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.grupopergunta gp ON gp.queid = ".QUESTIONARIO_MAISEDUC."
				JOIN questionario.grupopergunta gp2 ON gp2.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp2.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
												AND r.qrpid = qr.qrpid
												AND r.itpid = ip.itpid
				WHERE
					qq.qrpid = ".$qrpid."
				)
				ORDER BY
					perid";

		$arrRespondido = $db->carregar( $sql );

		$respond = array();
		$i = 0;
		
		if(is_array($arrRespondido) && $arrRespondido[0]){
			foreach($arrRespondido as $respondidos){
				if( !in_array($respondidos['perid'], $respond) ){
					$respond[] = $respondidos['perid'];
				}
			}
		}
/*
		foreach( $arrPerid as $dadoPerid ){
			if( !in_array( $dadoPerid['perid'], $respond ) ){
				// conta quantas perguntas não foram respondidas!
				$i++;
			}
		}
*/		
		$soma = 23; //quantidade de perguntas obrigatorias para todos.
		$p1 = 0;
		$p2 = 0;
		
		//se respondeu não na questão do ESCOLA ABERTA (perid 1786 itpid 3250) passa direto
		if(is_array($arrRespondido) && $arrRespondido[0]){
			foreach( $arrRespondido as $respondidos ){	
				if( $respondidos['perid'] == 1786 ){
					if( $respondidos['resposta'] == 3250 ){
						//respondeu não
						$soma = $soma;
					} else {
						//respondeu sim
						$soma = $soma + 6;
					}
					$p1++;
				}
				if( $respondidos['perid'] == 1763 ){
					if( $respondidos['resposta'] == 3191 ){
						//respondeu sim
						$soma = $soma + 5;
					}else{
						//respondeu não
						$soma = $soma;
					}
					$p2++;
				}
			}
		}
		
		$soma = $p1 == 0 ? $soma + 100 : $soma;
		$soma = $p2 == 0 ? $soma + 100 : $soma;

		if($soma > count($respond)){
			if( $soma > 100 ){
				return 1000;	
			} else {
				$total = $soma - count($respond);
				return $total;
			}
		} else {
			return 0;
		}
    }
}

function meVerificaCoordenador(){
	global $db;
	
	if(!$_SESSION['meentid']) return "Coordenador não encontrado. Refaça o procedimento.";
	
	
	$sql = "SELECT ed.muncod, ed.estuf FROM entidade.entidade e  
			LEFT JOIN entidade.endereco ed on ed.entid = e.entid 
			WHERE e.entid = ". $_SESSION['meentid'];
	$dados = $db->pegaLinha($sql);
	
	$muncod = $dados['muncod'];
	$estuf = $dados['estuf'];
	
	$existeRegistro = 0;
	
	$usuPerfil = arrayPerfil();
		
	//estadual
	if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
		
		$msg = "Estadual";
	   	if($estuf){
			$sql = "SELECT count(e.entid) as total FROM entidade.entidade e  
					INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
					LEFT JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
					LEFT JOIN entidade.endereco ed on ed.entid = e.entid 
					WHERE fe.funid = ". FUN_COORDENADOR_ME_ESTADUAL ."
					and ed.estuf = '".$estuf."'";
			
			$existeRegistro = $db->pegaUm($sql);
	   	}
	   	
	}
	
	//municipal
	if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
	   	
		$msg = "Municipal";
	   	if($muncod){	   	
		   	$sql = "SELECT count(e.entid) as total FROM entidade.entidade e  
					INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
					LEFT JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
					LEFT JOIN entidade.endereco ed on ed.entid = e.entid 
					WHERE fe.funid = ". FUN_COORDENADOR_ME_MUNICIPAL ."
					and ed.muncod = '".$muncod."'";
			$existeRegistro = $db->pegaUm($sql);
	   	}	   	
	}	
	
	if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
	   	
		$msg = "Estadual ou Municipal";
   	
	   	$sql = "SELECT count(e.entid) as total FROM entidade.entidade e  
				INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
				LEFT JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
				LEFT JOIN entidade.endereco ed on ed.entid = e.entid 
				WHERE fe.funid in (". FUN_COORDENADOR_ME_MUNICIPAL .",". FUN_COORDENADOR_ME_ESTADUAL .")
				and (ed.muncod = '".$muncod."' or ed.estuf = '".$estuf."') ";
		$existeRegistro = $db->pegaUm($sql);
	   		   	
	}	
	

	if($existeRegistro > 0){
		return true;
	}
	else{
		return "Cadastro do Coordenador {$msg} incompleto.";
	}
		
}

function recuperaRelacaoEscolaComunidade($memid = null)
{
	global $db;

	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
				e.entcodent,
				e.entnome,
				maedu.memanoreferencia
			FROM
				entidade.entidade e
			INNER JOIN
				pdeescola.memaiseducacao maedu 
					ON maedu.entid = e.entid
			WHERE
				maedu.memstatus = 'A' AND
				maedu.mamescolaaberta = 't' AND
				maedu.memid = '{$memid}'
			ORDER BY
				e.entnome";

	$entidadeEA = $db->pegaLinha($sql);

	if($entidadeEA){

		echo "<br>
				<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
					<tr>
						<td align=\"left\" class=\"tituloRelatorio2\" colspan=\"17\"><b>RELAÇÃO ESCOLA COMUNIDADE</b></td>
					</tr>
					<tr bgcolor=\"#f0f0f0\">
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">Nº</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">COD. INEP</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">NOME DA ESCOLA</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">Nº DE MATRÍCULAS</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">QUANTIDADES DE ATIVIDADES PLANEJADAS</td>
						<td align=\"center\" class=\"bordaBaixo\" colspan=\"2\">RECURSOS FINANCEIROS</td>
					</tr>
					<tr bgcolor=\"#f0f0f0\">
						<td align=\"center\" class=\"bordaDireitaBaixo\">(A) CULTURA E ARTE</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(B) ESPORTE / LAZER / RECREAÇÃO</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(C) QUALIFICAÇÃO PARA O TRABALHO / GERAÇÃO DE RENDA</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(D) FORMAÇÃO EDUCATIVA COMPLEMENTAR</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">TOTAL</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">CAPITAL****</td>
						<td align=\"center\" class=\"bordaBaixo\">CUSTEIO</td>
					</tr>
				";

			//pega total de matricula censo
			$sql = "SELECT
						sum(mecquantidadealunos) as total
					FROM
						pdeescola.mecenso
					WHERE
						entid = '".$entidadeEA['entid']."' AND
						mecanoreferencia = " . $entidadeEA['memanoreferencia'] . " AND
						mecserie in (1,2,3,4,5,6,7,8,9)";
		
			$qtdMatriculaCenso = $db->pegaUm($sql);
			if(!$qtdMatriculaCenso) $qtdMatriculaCenso = 0;
			$totalMatriculaCenso += $qtdMatriculaCenso;


			//pega total CULTURA E ARTE
			$sql = "SELECT
						count(eaaid) as total
					FROM
						pdeescola.meeabatividade eaa
					WHERE
						eatid = 1 AND
						eaa.memid = ". $entidadeEA['memid'];
		
			$qtdC = $db->pegaUm($sql);
			if(!$qtdC) $qtdC = 0;
			$totalC += $qtdC;

			//pega total ESPORTE / LAZER / RECREAÇÃO
			$sql = "SELECT
						count(eaaid) as total
					FROM
						pdeescola.meeabatividade eaa
					WHERE
						eatid = 2 AND
						eaa.memid = ". $entidadeEA['memid'];
		
			$qtdE = $db->pegaUm($sql);
			if(!$qtdE) $qtdE = 0;
			$totalE += $qtdE;

			//pega total QUALIFICAÇÃO PARA O TRABALHO
			$sql = "SELECT
						count(eaaid) as total
					FROM
						pdeescola.meeabatividade eaa
					WHERE
						eatid = 3 AND
						eaa.memid = ". $entidadeEA['memid'];
		
			$qtdQ = $db->pegaUm($sql);
			if(!$qtdQ) $qtdQ = 0;
			$totalQ += $qtdQ;

			//pega total FORMAÇÃO EDUCATIVA
			$sql = "SELECT
						count(eaaid) as total
					FROM
						pdeescola.meeabatividade eaa
					WHERE
						eatid = 4 AND
						eaa.memid = ". $entidadeEA['memid'];
				
			$qtdF = $db->pegaUm($sql);
			if(!$qtdF) $qtdF = 0;
			$totalF += $qtdF;

			$totalLinha = $qtdC + $qtdE + $qtdQ + $qtdF;

			//pega os valores capital e custeio
			$meses = 6;
			calculoEscolaAberta($entidadeEA['memid'], $entidadeEA['memanoreferencia'], $escolaAbertaCapital, $escolaAbertaCusteio, $meses);

			$totalEscolaAbertaCapital += $escolaAbertaCapital;
			$totalEscolaAbertaCusteio += $escolaAbertaCusteio;

			echo "
					<tr>
						<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">&nbsp;</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".$entidadeEA['entcodent']."</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" valign=\"top\">".$entidadeEA['entnome']."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".$qtdMatriculaCenso."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdC."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdE."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdQ."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdF."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".$totalLinha."</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" >".($escolaAbertaCapital ? number_format($escolaAbertaCapital,2,',','.') : '0,00' )."</td>
						<td align=\"center\" class=\"bordaBaixo\">".($escolaAbertaCusteio ? number_format($escolaAbertaCusteio,2,',','.') : '0,00' )."</td>
					</tr>
				</table>
			";
	}
}

function recuperaJovem15a17anos($memid = null)
{
	global $db;

	if($memid){

		$memjovem1517 = $db->pegaUm("select memjovem1517 from pdeescola.memaiseducacao  where memid = {$memid}");

		$calculoJovem15a17 = 0;
		calculoJovem15a17($memid, $_SESSION['exercicio'], 6, $calculoJovem15a17);

		if($memjovem1517 == 't'){
			
				$sql = "select * from pdeescola.mealunojovemparticipante where memid = {$memid}";
				$rs = $db->pegaLinha($sql);

				if($rs){
					echo '
						<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
							<tr>
								<td class="bordaDireitaBaixo tituloRelatorio2" rowspan="4" valign="middle" align="center" width="10%"><b>Alunado Participante:</b></td>
								<td class="bordaDireitaBaixo tituloRelatorio2" colspan="10" valign="middle" align="center" width="60%"><b>Mais Educação para Jovens de 15 a 17 anos no Ensino Fundamental</b></td>
							</tr>
							<tr bgcolor="#f0f0f0">
								<td class="bordaDireitaBaixo" valign="middle" align="center"><b>Anos Iniciais</b></td>
								<td class="bordaDireitaBaixo" valign="middle" align="center"><b>Anos Finais</b></td>
								<td class="bordaDireitaBaixo" valign="middle" align="center"><b>Tutor</b></td>
								<td colspan="2" class="bordaDireitaBaixo" valign="middle" align="center"><b>Recursos Financeiros</b></td>
							</tr>
							<tr style="background-color:#FAFAFA;">
								<td rowspan="2" class="bordaDireitaBaixo" valign="middle" align="center">'.$rs['majquantidadeai'].'</td>
								<td rowspan="2" class="bordaDireitaBaixo" valign="middle" align="center">'.$rs['majquantidadeaf'].'</td>
								<td rowspan="2" class="bordaDireitaBaixo" valign="middle" align="center">'.number_format($calculoJovem15a17,"2",",",".").'</td>
								<td class="bordaDireitaBaixo" valign="middle" align="center">Custeio</td>
								<td class="bordaDireitaBaixo" valign="middle" align="center">Capital</td>
							</tr>
							<tr style="background-color:#FAFAFA;">
								<td class="bordaDireitaBaixo" valign="middle" align="center">'.($calculoJovem15a17>0 ? '5.000,00' : '0,00').'</td>
								<td class="bordaDireitaBaixo" valign="middle" align="center">'.($calculoJovem15a17>0 ? '2.000,00' : '0,00').'</td>
							</tr>
						</table>
					';
			}
		}
	}
}

function recuperaPeif($memid = null, &$peifCapital = 0, &$peifCusteio = 0)
{
	global $db;
	
	if($memid){
		$mempeif = $db->pegaUm("select mempeif from pdeescola.memaiseducacao  where memid = {$memid}");
		
		if($mempeif == 't'){
			
			$peifCusteio = 0; 
			$peifCapital = 0;
			$totalAlunos = 0;
			
			if(isset($_REQUEST["ano"]) && $_REQUEST["ano"] != "") {
				$ano = (integer)$_REQUEST["ano"];
			}else{
				$ano = 	$_SESSION["exercicio"];
			}
 
			calculoPeif($memid, $peifCusteio, $peifCapital, $ano, $totalAlunos);
			
			echo '
				<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
					<tr>						
						<td class="bordaDireitaBaixo tituloRelatorio2" colspan="3" valign="middle" align="left">
							<b>PEIF - Programa Escola Intercultural de Fronteira</b>
						</td>
					</tr>					
					<tr bgcolor="#f0f0f0">
						<td class="bordaDireitaBaixo" valign="middle" align="center" width="10%">Total de Alunos</td>
						<td class="bordaDireitaBaixo" valign="middle" align="center" width="10%">Custeio</td>
						<td class="bordaDireitaBaixo" valign="middle" align="center" width="10%">Capital</td>
					</tr>
					<tr>
						<td class="bordaDireitaBaixo" valign="middle" align="center">'.$totalAlunos.'</td>
						<td class="bordaDireitaBaixo" valign="middle" align="center">'.number_format($peifCapital,"2",",",".").'</td>
						<td class="bordaDireitaBaixo" valign="middle" align="center">'.number_format($peifCusteio,"2",",",".").'</td>
					</tr>
				</table>
				';
		}
	}	
}

function verificaPodeGerarRelConsolidado()
{
	global $db;
	
	$usuPerfil = arrayPerfil();
	
	if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)){
		
		if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) && !in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)){
			$tpcid = 1;
			$pflcod = 385;
		}else if(!in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)){
			$tpcid = 3;
			$pflcod = 386;
		}else if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) ){			
			$tpcid = '1, 3';
			$pflcod = '385, 386';
		}
		
// 		$sql = "
// 				select 
// 					entnome, dc.esdid 
// 				from pdeescola.memaiseducacao me
// 				join entidade.entidade et on et.entid = me.entid
// 				join entidade.endereco ed on ed.entid = et.entid
// 				join workflow.documento dc on dc.docid = me.docid
// 				join pdeescola.usuarioresponsabilidade ur on ur.muncod = ed.muncod 
// 					and ur.usucpf = '{$_SESSION['usucpf']}' and ur.pflcod in ({$pflcod})
// 					and (
// 					 ur.muncod = ed.muncod OR ur.entid  = et.entid
// 					) 
// 				where me.memanoreferencia = '{$_SESSION['exercicio']}' 
// 				and me.memstatus = 'A'
// 				and et.tpcid in ( {$tpcid} )
// 				";	
		
		$sql = "
				SELECT DISTINCT	   
					 e.entcodent,
					 e.entnome,
					 doc.esdid
				FROM
					 entidade.entidade e
				INNER JOIN 
					 entidade.endereco endi ON endi.entid = e.entid
				INNER JOIN 
					pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' 
						AND 
						ur.pflcod IN ({$pflcod}) AND
						ur.usucpf = '{$_SESSION['usucpf']}' AND
						(
						 (ur.muncod = endi.muncod AND 
						  e.tpcid = {$tpcid}) OR
						 ur.entid  = e.entid
						)
				INNER JOIN
					 pdeescola.memaiseducacao maedu ON  maedu.entid = e.entid 
						AND maedu.memanoreferencia = {$_SESSION['exercicio']} 
						AND maedu.memstatus = 'A'
				LEFT JOIN 
					workflow.documento doc on doc.docid = maedu.docid
				WHERE
					e.entcodent NOT IN ( select entcodent from pdeescola.meescolasrestricao )
				";
		
		$rs = $db->carregar($sql);
		$rs = $rs ? $rs : array();
		
		$totalEscolas = count($rs);
		$totalEscolasFinalizadas = 0;
		
		foreach($rs as $escola){
			if($escola['esdid'] == 34){
				$totalEscolasFinalizadas++;
			}
		}
// 		ver($sql, $totalEscolas, $totalEscolasFinalizadas);
		if($totalEscolas == $totalEscolasFinalizadas){
			return true;
		}
	}
	return false;
}
?>