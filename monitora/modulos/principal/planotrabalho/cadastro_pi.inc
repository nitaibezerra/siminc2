<?php

function validaacessopi() {
	global $db;
	$perfiscomacesso = array(112=>"Gestor da Unidade de Planejamento",
							 113=>"Equipe de Apoio ao Gestor da Unidade de Planejamento",
							 1=>"Coordenador de Ação",
							 8=>"Equipe de Apoio a Coordenadores de Ação",
							 18=>"Unidade de Monitoramento e Avaliação",
							 6=>"Super Usuário");
							 
	$sql = "SELECT peu.pflcod FROM seguranca.perfilusuario peu
			INNER JOIN seguranca.perfil per ON peu.pflcod = per.pflcod 
			WHERE per.sisid='1' AND peu.usucpf='".$_SESSION['usucpf']."'";
	
	$prfs = $db->carregar($sql);
	$acc = false;
	if($prfs[0]) {
		foreach($prfs as $pr) {
			if($perfiscomacesso[$pr['pflcod']]) {
				$acc = true;
			} 
		}
	}
	return $acc;
}


/*
 * Função de validação do código do PI
 */
function validaCodPi($pi){
	global $db;
	$sql = "SELECT plicod FROM monitora.planointerno WHERE plistatus='A' AND plicod = '{$pi}'"; 
	$plicod = $db->PegaUm($sql);
	if(!$plicod){
		$piaux = substr($pi, 0, 6);
		$sql = "SELECT DISTINCT plicod, plititulo FROM monitora.planointerno WHERE plistatus='A' AND plicod like '%{$piaux}%' ORDER BY plititulo";
		$dados = (array) $db->carregar($sql);
		if($dados[0]){
			$cabecalho = array('Cód PI','Título');
			$retorno = $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
			echo $retorno;
			exit;
		}
		else{
			$retorno = "";
			echo $retorno;
			exit;
		}
	}else{
		$retorno = "pijaexiste";
		echo $retorno;
		$sql = "SELECT 
				p.plicod as plicod,
				coalesce(p.plititulo,'Não preenchido') as titulo,
				coalesce(SUM(p.plivalor),0) as total,
							CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
								 WHEN p.plisituacao = 'C' THEN ' <font color=\"green\">Aprovado</font> '
								 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
								 WHEN p.plisituacao = 'V' THEN	' <font color=\"#3F85FF\">Revisado</font> '
								 WHEN p.plisituacao = 'S' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
								 WHEN p.plisituacao = 'R' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
				u.usunome ||' por '||to_char(p.plidata, 'dd/mm/YYYY hh24:mi'),
				a._atinumero||' - '||a.atidescricao as atividade
				FROM monitora.planointerno p
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				inner join monitora.planointernoatividade pa on pa.pliid = p.pliid
				inner join pde.atividade a on a.atiid = pa.atiid
				where 
				p.plicod='".$plicod."' AND
				p.plistatus = 'A' 
				GROUP BY p.plicod,p.plititulo,u.usunome,p.plidata,p.plisituacao,atividade 
				ORDER BY p.plidata DESC";

		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
		exit;
	}
}

/*
 * Validando as variaveis de sessão obrigatorias
 */
if(!$_SESSION['monitora_var']['entid'] || !$_SESSION['monitora_var']['tipo']) {
	echo "<script>alert('Parâmetros não encontrados');window.location='?modulo=principal/planotrabalho/plano&acao=A';</script>";
	exit;
}

//valida cod pi AJAX
if ($_REQUEST['piAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	validaCodPi($_POST['piAjax']);
	exit;
}

/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número
 */
define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

/*
 * Controle de edição do formulario
 */
$acessoeditar = true;

$acessoeditar = validaacessopi();

/*
 * Constantes das opções da combobox
 */
define('COMBO_REGRAGERAL', 5);
define('COMBO_REGRAPDEEPPA', 3);
/*
 * Constantes das opções da combobox
 */

/*
 * FIM SCRIPT que controle as regras de combobox
 */

include  APPRAIZ."includes/cabecalho.inc";

// carrega as funções de integração
include "planotrabalho/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";
include "planotrabalho/_componentes.php";

/*
 * Coletando dados da UNIDADE
 */
$unidade = $db->pegaLinha("SELECT ent.entunicod, ent.entungcod, ent.entnome FROM entidade.entidade ent 
				   	   	   WHERE ent.entid='".$_SESSION['monitora_var']['entid']."'");
/*
 * Verificando se existem dados da UNIDADE
 */
if(!$unidade) {
	echo "<script>alert('Não existe unidade');window.location = '?modulo=principal/planotrabalho/plano&acao=A';</script>";
	exit;
}

/*
 * Verificando se é UNIDADE ORÇAMENTARIA(UO) ou UNIDADE GESTORA(UG)
 */
switch($_SESSION['monitora_var']['tipo']) {
	case 'un':
	$unidsc    = $unidade['entunicod']." - ".$unidade['unidsc'];
	$unicod    = $unidade['entunicod'];
	break;
	case 'ug':
	$unicod       = $unidade['entunicod'];
	$entungcod    = $unidade['entungcod'];
	break;
}

//flag para setar todos para FNDE
if($unicod == AD || $unicod == CAPES || $unicod == INEP) $unicod = FNDE;


/*
 * Verfica se a UNIDADE possui programas institucionais
 */
switch($unicod) {
	case AD:
		break;
	case CAPES:
		break;
	case INEP:
		break;
	case FNDE:
		$possuirprogramainstitucional = true;
		break;
	default:
		$possuirprogramainstitucional = true;
		break;
}

$_POST['sbaid'] = $db->pegaUm("SELECT sbaid FROM monitora.subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");
if(!$_POST['sbaid']) {
	
	die("<script>
			alert('É obrigatorio a vinculação da subação numa atividade');
			window.location='monitora.php?modulo=principal/planotrabalho/subatividades&acao=A&atiid=".$_REQUEST['atiid']."';
		 </script>");
	
}


$atividade = atividade_pegar($_REQUEST['atiid']);
// verficando se existe a atividade
if (!$atividade) {
	echo "<script>alert('Não existe atividade');window.location = '?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$_REQUEST['atiid']."';</script>";
	exit;
}

echo "<br>";

// criando as abas
$db->cria_aba($abacod_tela,$url,'&atiid=' . $_REQUEST['atiid']);

// definindo variaveis
$atiid  = $_POST['atiid'] ? $_POST['atiid'] : $_GET['atiid']; 
$plicod = $_POST['plicod'] ? $_POST['plicod'] : $_GET['plicod']; 
$acaid  = $_POST['acaid'] ? $_POST['acaid'] : 'null'; 
$sbaid = $_POST['sbaid'] ? $_POST['sbaid'] : $_GET['sbaid'];

// verifica se ocorre algum evento
if(isset($_POST['evento']) && ($_POST['evento'] != '') ) {
	switch($_POST['evento']) {
		// atualizar os dados do plano interno
		case "G":
			
			//modalidade
			if($unidade['entunicod'] != INEP) {
				$modid = $_POST['modensino'];
				$modcod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$modid."'");
			}
			
			if(($_REQUEST['dpiid_']!=$_REQUEST['dpiid']) || 
			   ($_REQUEST['cpiid_']!=$_REQUEST['cpiid']) || 
			   ($_REQUEST['plilivre']!=$_REQUEST['plilivre_']) || 
			   ($_REQUEST['modensino_']!=$_REQUEST['modensino'])) {
				
				//categoria
				$cpiid = $_POST['cpiid'];
				$cpicod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$cpiid."'");
			
				//enquadramento
				$eqdid = $_POST['eqdid'];
				$eqdcod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$eqdid."'");
			
				//nivel
				$dpiid = $_POST['dpiid'];
				$dpicod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$dpiid."'");
				
			   	if($_SESSION['monitora_var']['tipo'] == "un") {
			   		$plicodnew = $eqdcod.$_REQUEST['geradorcompleto'].$dpicod.$cpicod.strtoupper($_POST['plilivre'].$modcod);
			   	} else {
					//executor
					$valor = explode("||",$_POST['campo3']);
					$gerador1 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
							
					//gestor
					$valor = explode("||",$_POST['campo4']);
					$gerador2 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
				   	
					//$gerador3 = controlegeradorsequencialpi($eqdcod.$gerador1.$gerador2.$dpicod.$cpicod.strtoupper($_POST['plilivre']),2);
					$gerador3aux = split(" - ",$_REQUEST['sbaid_dsc']);
					$gerador3 = substr($gerador3aux[0],2);
					
					$plicodnew = $eqdcod.$gerador1.$gerador2.$gerador3.$dpicod.$cpicod.strtoupper($_POST['plilivre'].$modcod);
			   	}
				
				
				
				$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.planointerno WHERE plistatus='A' AND plicod='".$plicodnew."'");
				if($existe_plicod) {
					echo "<script>
							alert('O Código do PI ja existe.');
							window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';
						  </script>";
					exit;
				}
				$db->executar("UPDATE monitora.planointerno SET plicod='".$plicodnew."' WHERE plicod='".$_POST['plicod']."'");
			   	
			   	$qry = "INSERT INTO monitora.planointernohistorico(
            	 plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			 VALUES ('".$plicodnew."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$_POST['plicod']."')";
			   	$db->executar($qry);
			   	
				$_POST['plicod'] = $plicodnew;
			   	
			}
			
			if($_REQUEST['atiid_move'] && $_REQUEST['atiid_move']!=$atiid) {
				$atiid = $_REQUEST['atiid_move'];
			}
			
			// removendo os PIs
			$sql = "UPDATE monitora.planointerno SET plistatus='I' WHERE plicod='".$_POST['plicod']."'";
			$db->executar($sql);
			
			
			$dataAtualizacao = date("Y-m-d H:i:s");
			$sql = "UPDATE monitora.planointerno SET modid=".(($_POST['modensino'])?"'".$_POST['modensino']."'":"NULL").", plilivre='".$_POST['plilivre']."', plititulo='".$_POST['txtsubacao'].$_POST['plititulo']."', plidsc='".$_POST['plidsc']."', usucpf='".$_SESSION['usucpf']."', plidata='".$dataAtualizacao."', cpiid='".$_POST['cpiid']."', eqdid='".$_POST['eqdid']."', dpiid='".$_POST['dpiid']."' ".(($_REQUEST['plisituacao'])?", plisituacao='".$_REQUEST['plisituacao']."'":"")." WHERE plicod='".$_POST['plicod']."'";
			$db->executar($sql);
			
			if(is_array($_POST['plivalored'])) {
				foreach($_POST['plivalored'] as $pliid => $valor) {
					$valor = $valor ? $valor : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
						
					$sql = "UPDATE monitora.planointerno SET plistatus='A', plivalor='".$valor."' WHERE pliid='".$pliid."'";
					$db->executar($sql);
					$sql = "UPDATE monitora.planointernoatividade SET atiid='".$atiid."' WHERE pliid='".$pliid."'";
					$db->executar($sql);
				}
			}
			
			$situacaopadrao = $db->pegaUm("SELECT plisituacao FROM monitora.planointerno WHERE plicod='".$_POST['plicod']."'");
			
			if(is_array($_POST['acaid'])) {
				
				foreach($_POST['acaid'] as $ptres => $acaid) {
					
					$valor = $_POST['plivalor'][$ptres][$acaid] ? $_POST['plivalor'][$ptres][$acaid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					
					$sql_I = "INSERT INTO monitora.planointerno 
						  		(plicod, modid, eqdid, dpiid, cpiid, plilivre, plidsc, plivalor, pliacao, acaid, usucpf, plidata, plititulo, pliptres, plisituacao, sbaid) 
		     			  	  VALUES ('".substr($_POST['plicod'],0,(strlen($_POST['plicod'])-3)).$_POST['plilivre'].$modcod."', '".$_POST['modensino']."', '".$_POST['eqdid']."', '".$_POST['dpiid']."','".$_POST['cpiid']."',upper('".$_POST['plilivre']."'), '".$_POST['plidsc']."', ".$valor.",'".(($_POST['gerador'])?$_POST['gerador']:substr($_POST['plicod'],1,4))."',".$acaid.",'".$_SESSION['usucpf']."','".$dataAtualizacao."','".$_POST['txtsubacao'].$_POST['plititulo']."','".(string) $ptres."','".(($_POST['plisituacao'])?$_POST['plisituacao']:$situacaopadrao)."',".(($_POST['sbaid'])?"'".$_POST['sbaid']."'":"NULL").")
		     			  	  RETURNING pliid";
					$pliid = $db->pegaUm($sql_I);
					
					$sql = "INSERT INTO monitora.planointernoatividade (pliid, atiid) VALUES (".$pliid.", ".$atiid.") ";
					$db->executar($sql);
				}
			}
			$db->commit();
			echo "<form method='POST' name='formulario'><input type='hidden' name='atiid' value='".$atiid."'><input type='hidden' name='evento' value='A'><input type='hidden' name='plicod' value='".$_POST['plicod']."'></form>
				  <script>alert('Dados gravados com sucesso.');document.formulario.submit();</script>";
			exit;
		break;
		
		// carregar os dados do pi
		case 'A':
			// pegar os dados comuns dos PI
			$planointerno = $db->pegaLinha("SELECT modid,plititulo,plisituacao,plidsc,plivalor,acaid,plicod,eqdid,cpiid,dpiid,pliacao,plilivre,sbaid FROM monitora.planointerno WHERE plicod = '".$_POST['plicod']."'");
			if($planointerno) {
				$plititulo = $planointerno['plititulo'];
				$plisituacao = $planointerno['plisituacao'];
				$plidsc = $planointerno['plidsc'];
				$acaid = $planointerno['acaid'];
				$sbaid = $planointerno['sbaid'];
				$modid = $planointerno['modid'];
				
				/*
				 * Verifica se a situação é PENDENTE
				 */
				if($plisituacao!='P' &&
				   $plisituacao!='R') {
					$acessoeditar = false;
				}
				
				
				$enquadramento = $db->pegaUm("SELECT cdtcod ||' - '|| cdtdsc FROM public.combodadostabela WHERE cdtid='".$planointerno['eqdid']."'");
				$categoria 	   = $db->pegaUm("SELECT cdtcod ||' - '|| cdtdsc FROM public.combodadostabela WHERE cdtid='".$planointerno['cpiid']."'");
				$nivel  	   = $db->pegaUm("SELECT cdtcod ||' - '|| cdtdsc FROM public.combodadostabela WHERE cdtid='".$planointerno['dpiid']."'");
				$gerador       = $planointerno['pliacao'];
				$plilivre      = $planointerno['plilivre'];
				// Verifica todos os ptres contidos no código do PI
				if($_REQUEST['plicod']) {
					if($possuirprogramainstitucional){
						/*
						 * Validando se existe uma SUBAÇÃO, nesta situação é obrigatorio
						 * a presença do $sbaid
						 */
						if(!$sbaid){
							echo "<script>
									alert('Plano interno não possui subação (Possivelmente é antigo). Apague este e crie um novo plano interno.');
									window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$_REQUEST['atiid']."';
								  </script>";
							exit;
						}
						/*
						 * FIM Validando se existe uma SUBAÇÃO, nesta situação é obrigatorio
						 * a presença do $sbaid
						 */
						
						$sql = "
						SELECT  
							pl.pliid,
							pl.pliptres,
							pl.plivalor, 
							pl.acaid,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							sum(ptr.ptrdotacao) as dotacaoinicial,
							round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
							coalesce((SELECT SUM(plivalor) as valor FROM monitora.planointerno WHERE pl.pliptres = pliptres AND plistatus='A'),0) as detalhamento
						FROM monitora.planointerno pl
						INNER JOIN monitora.acao ac ON ac.acaid = pl.acaid  and ac.prgano='2009'
						left JOIN monitora.ptres ptr ON ac.acaid = ptr.acaid
						left JOIN monitora.subacaodotacao sad ON ptr.ptrid = sad.ptrid
						WHERE
								pl.plicod = '".$plicod."' AND
								ac.acastatus = 'A' AND 
								ac.acasnrap = false AND
								pl.plistatus='A'
								{$flagsbaid}
				    	GROUP BY pl.pliid, pl.pliptres, pl.plivalor, ac.prgcod, pl.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
				    	ORDER BY pl.pliptres;";
					} else {
						$sql = "SELECT  pl.pliid,
					   			pl.pliptres,
					   			pl.plivalor, 
					   			pl.acaid,
					   			trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
					   			round(sum( coalesce(rofdot_ini,0) ),2) as dotacaoinicial,
					   			coalesce((SELECT SUM(plivalor) as valor FROM monitora.planointerno WHERE pl.pliptres = pliptres AND plistatus='A'),0) as detalhamento
						FROM monitora.planointerno pl
						INNER JOIN monitora.acao ac ON ac.acaid = pl.acaid  and ac.prgano='".$_SESSION['exercicio']."'
						LEFT JOIN financeiro.execucao e ON trim(pl.pliptres)=trim(e.ptres) and e.rofano='".$_SESSION['exercicio']."'
						WHERE
								pl.plicod = '".$plicod."' AND
								ac.acastatus = 'A' AND 
								ac.acasnrap = false AND
								pl.plistatus='A' 
				    	GROUP BY pl.pliid, pl.pliptres, pl.plivalor, ac.prgcod, pl.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
				    	ORDER BY pl.pliptres;";
					}
					$acoespl = $db->carregar($sql);
				}
			} else {
				echo "<script>alert('Plano interno não encontrado');window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';</script>";
				exit;
			}
		break;
		// inserir plano interno
		case 'I':
			if($_REQUEST['atiid_move'] && $_REQUEST['atiid_move']!=$atiid) {
				$atiid = $_REQUEST['atiid_move'];
			}
			//categoria
			$valor = explode("||",$_POST['campo7']);
			$cpiid = $valor[0];
			$cpicod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
			
			//enquadramento
			$valor = explode("||",$_POST['campo5']);
			$eqdid = $valor[0];
			$eqdcod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
			
			//nivel
			$valor = explode("||",$_POST['campo6']);
			$dpiid = $valor[0];
			$dpicod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");

			//modalidade
			if($unidade['entunicod'] != INEP) {
				$valor = explode("||",$_POST['modensino']);
				$modid = $valor[0];
				$modcod = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
			}
			
			// se for as unidades especiais, os 2 digitos seram selecionados
			switch($unicod) {
				case AD:
					break;
				case CAPES:
					break;
				case INEP:
					break;
				case FNDE:
					
					if($_POST['regraespecial1']) {
						
						$gerador = (string) $_POST['regraespecial1'];
						$plicod = $eqdcod.$gerador.$dpicod.$cpicod.strtoupper($_POST['plilivre'].$modcod);
						
					} else {
						
						//executor
						$valor = explode("||",$_POST['campo3']);
						$gerador1 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
						
						//gestor
						$valor = explode("||",$_POST['campo4']);
						$gerador2 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
						
						//$gerador3 = controlegeradorsequencialpi($eqdcod.$gerador1.$gerador2.$dpicod.$cpicod.strtoupper($_POST['plilivre']),2);
						$gerador3aux = split(" - ",$_REQUEST['sbaid_dsc']);
						$gerador3 = substr($gerador3aux[0],2);
						$plicod = $eqdcod.$gerador1.$gerador2.$gerador3.$dpicod.$cpicod.strtoupper($_POST['plilivre'].$modcod);
					
					}
					
					break;
					
				default:
					
					$plicod = $eqdcod.$_REQUEST['geradorcompleto'].$dpicod.$cpicod.strtoupper($_POST['plilivre'].$modcod);
					
				break;
			}
		
		
			// verificando a se existe algum plicod no BD
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.planointerno WHERE plistatus='A' AND plicod='".$plicod."'");
			if($existe_plicod) {
				echo "<script>alert('Não foi possível criar o Plano Interno. Código já existente.');window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';</script>";
				exit;
			}
			
			$qry = "INSERT INTO monitora.planointernohistorico(
            	    plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			    VALUES ('".$plicod."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$plicod."')";
			$db->executar($qry);
			

			if(is_array($_POST['acaid'])) {
				$dataInsercao = date("Y-m-d H:i:s");
				foreach($_POST['acaid'] as $ptres => $acaid) {
					
					$valor = $_POST['plivalor'][$ptres][$acaid] ? $_POST['plivalor'][$ptres][$acaid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					
					$sql_I = "INSERT INTO monitora.planointerno 
						  		(plicod, modid, eqdid, dpiid, cpiid, plilivre, plidsc, plivalor, pliacao, acaid, usucpf, plidata, plititulo, pliptres, sbaid) 
		     			  	  VALUES ('".$plicod."', ".(($modid)?"'".$modid."'":"NULL").", '".$eqdid."', '".$dpiid."','".$cpiid."',upper('".$_POST['plilivre']."'), '".$_POST['plidsc']."', ".$valor.",'".substr($plicod,1,4)."',".$acaid.",'".$_SESSION['usucpf']."','".$dataInsercao."','".$_POST['txtsubacao'].$_POST['plititulo']."','".(string) $ptres."',".(($sbaid)?"'".$sbaid."'":"NULL").")
		     			  	  RETURNING pliid";	
					
					$pliid = $db->pegaUm($sql_I);
					
					$sql = "INSERT INTO monitora.planointernoatividade (pliid, atiid) VALUES (".$pliid.", ".$atiid.") ";
					$db->executar($sql);
				}
			}
			
			$db->commit();
			echo "<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';</script>";
			exit;
		break;
		
		case 'E':
			$valorempenhado = false;
			if(!$valorempenhado) {
				$sql_D = "UPDATE monitora.planointerno SET plistatus = 'I' where plicod = '".$plicod."'";
				$db->executar($sql_D);
				$db->commit();
				echo "<script>alert('Registro removido com sucesso.');window.location = '?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';</script>";
			} else {
				echo "<script>alert('Registro não pode ser removido. PI possui valor emprenha.');window.location = '?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$atiid."';</script>";
			}
			exit;
		break;
	}
}

monta_titulo("Subação/PI",'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

/*
 * Verifica se o usuário selecionou a SUBAÇÃO
 * para coletar Enquadramento / Gestor / Executor
 */
if($sbaid){
	if($_SESSION['monitora_var']['tipo'] == "un") {
		
		$sql = "SELECT sbatitulo, sbacod FROM monitora.subacao sba WHERE sba.sbaid='".$sbaid."'";
		$subacao = $db->pegaLinha($sql);
		
	} else {
		
		$sql = sprintf("SELECT cg.cdtid as gstid,
							   cg.cdtcod ||' - '|| cg.cdtdsc as gstdsc,
							   ce.cdtid as exeid,
							   ce.cdtcod ||' - '|| ce.cdtdsc as exedsc,
							   sba.eqdid as eqdid 
						FROM monitora.subacao sba
						LEFT JOIN public.combodadostabela cg ON cg.cdtid = sba.gstid
						LEFT JOIN public.combodadostabela ce ON ce.cdtid = sba.exeid
						WHERE sba.sbaid = %d LIMIT 1", $sbaid);
		
		$cdados = $db->carregar($sql);
		$eqdid = $cdados[0]['eqdid'];
		$gstid = $cdados[0]['gstid'];
		$exeid = $cdados[0]['exeid'];
		$gstdsc = $cdados[0]['gstdsc'];
		$exedsc = $cdados[0]['exedsc'];
		
	}
}
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
//coloca tabindex no campo valor
function tabindexcampo(){
	var x = document.getElementsByTagName("input");
	var y = 1;
	for(i=0;i<x.length;i++) {
		if(x[i].type=="text"){
			if(x[i].name.substr(0,8) == 'plivalor'){
				x[i].tabIndex=y;
				y++;
			}
		}
	}
}
</script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="<? echo (($plicod)?"G":"I"); ?>">
<input type="hidden" name="atiid"  id="atiid" value="<?=$atiid?>">
<input type="hidden" name="plicod"  id="plicod" value="<?=$plicod;?>">


<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2"><?	echo montar_resumo_atividade( $atividade, $numeracao_relativa = true ); ?></td>
</tr>
<? if($possuirprogramainstitucional){ ?>
<tr>
	<td colspan="2" class="SubTituloCentro">
		<?
		if($sbaid){
			$sql = "SELECT sba.sbacod ||' - '|| sba.sbatitulo as descricao FROM monitora.subacao sba 
				   	LEFT JOIN monitora.subacaounidade ppe ON ppe.sbaid = sba.sbaid WHERE sbastatus='A'  and sba.sbaid = ".$sbaid;
			$subacaodsc = $db->PegaUm($sql);
			echo "Subação: ".strtoupper($subacaodsc);
		} else {
			echo "Selecione uma subação para inserir novo plano interno";	
		}
		?>
	</td>
</tr>
<? } ?>

<?


$mostra = true;
if($possuirprogramainstitucional) $mostra = false;
if($sbaid) $mostra = true;

/*
 * Imprimindo cabeçalho com o Detalhamento Orçamentário
 */
if($mostra){
	if($possuirprogramainstitucional){
		?>
			<tr>
			    <td colspan="2">
			        <table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" onmouseover="tabindexcampo();">
					<tr>
					<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário</b></td>
					</tr>
			        <tr>
						<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
						<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Inicial</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Subação</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado no PI</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
					    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
					</tr>
					<? 
					if($acoespl[0]) {
						$valortotalpi = 0;
						$cor = 0;
						foreach($acoespl as $acpl) { 
					?>
			        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['pliptres']; ?>">
						<td align="center"><? echo $acpl['pliptres']; ?></td>
						<td align="left"><? echo $acpl['descricao']; ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
					    <td align="right"><a href="javascript:detfin('<?=$acpl['pliptres']?>')"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></a></td>
					    <td align="right"><? echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); ?></td>
					    <td align="center"><input type="text" name="plivalored[<? echo $acpl['pliid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['plivalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['pliptres']; ?>','<? echo number_format($acpl['plivalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' <? echo ((!$acessoeditar)?'disabled':''); ?> /></td>
					</tr>
					<? 
							$cor++;
							$valortotalpi = $valortotalpi + $acpl['plivalor']; 
						}
					} 
					?>
			        <tr style="height: 30px;">
						<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
						<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
					</tr>
			        <tr>
						<td align="right" colspan="7">
						<? if($acessoeditar) { ?>
						<input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES">
						<? } ?>
						</td>
					</tr>
			        </table>
			    </td>
			</tr>
	<? } else { ?>
			<tr>
			    <td colspan="2">
			        <table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" onmouseover="tabindexcampo();">
					<tr>
					<td style="background-color: #C9C9C9;" colspan="6" align="center"><b>Detalhamento Orçamentário</b></td>
					</tr>
			        <tr>
						<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
						<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Inicial</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado no PI</b></td>
					    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
					    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
					</tr>
					<? 
					if($acoespl[0]) {
						$valortotalpi = 0;
						$cor = 0;
						foreach($acoespl as $acpl) { 
					?>
			        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['pliptres']; ?>">
						<td align="center"><? echo $acpl['pliptres']; ?></td>
						<td align="left"><? echo $acpl['descricao']; ?></td>
					    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
					    <td align="right"><a href="javascript:detfin('<?=$acpl['pliptres']?>')"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></a></td>
					    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
					    <td align="center"><input type="text" name="plivalored[<? echo $acpl['pliid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['plivalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['pliptres']; ?>','<? echo number_format($acpl['plivalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
					</tr>
					<? 
							$cor++;
							$valortotalpi = $valortotalpi + $acpl['plivalor']; 
						}
					} 
					?>
			        <tr style="height: 30px;">
						<td align="right" valign="top" colspan="5"><b>TOTAL :</b></td>
						<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
					</tr>
			        <tr>
						<td align="right" colspan="6">
						<? if($acessoeditar) { ?>
						<input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES">
						<? } ?>
						</td>
					</tr>
			        </table>
			    </td>
			</tr>
		<?
	}
}
/*
 * FIM Imprimindo cabeçalho com o Detalhamento Orçamentário
 */

?>

<?
/*
 * Se tiver programa, mostrar os programas existentes
 * Somente no momento em que o usuário selecionar um programa, ira mostrar o botão 
 * de adicionar "Ações"
 */

/*
 * Gatilho para não mostrar Atividade quando não for edição
 */
if($mostra) {
?>
	<tr>
				<td class="SubTituloDireita">Atividade do plano de trabalho:</td>
				<td>
				<?
				$atiid_move = $_REQUEST['atiid'];
				$sql = "SELECT atiid as codigo, substr(_atinumero||' - '||atidescricao, (strpos(_atinumero,'.')+1)) as descricao
						    FROM pde.atividade where entid='".$_SESSION['monitora_var']['entid']."' AND atistatus='A' AND _atiprofundidade > 1 ORDER BY _atiordem";
				$db->monta_combo('atiid_move', $sql, (($acessoeditar)?'S':'N'), 'Selecione', '', '', '', '340', 'S', 'atiid_move');
				?>
		</td>
	</tr>
<?
}

if($possuirprogramainstitucional) {
	if($plicod) {
		?>
			<tr>
				<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
			    <td><? echo $enquadramento; ?><input type="hidden" name="eqdid" value="<? echo $planointerno['eqdid']; ?>"></td>
			</tr>

		<tr>
			<td align='right' class="SubTituloDireita">Subação:</td>
			<td><?=$subacaodsc?><input type="hidden" name="sbaid_dsc" value="<?=$subacaodsc?>"><input type="hidden" name="sbaid" value="<?=$sbaid?>"></td>
		</tr>
		
<?
} else { 
	
	if($_POST['sbaid']) { 
		if($_SESSION['monitora_var']['tipo'] == "un") {
?>
	<tr>
		<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
		<td>
			<?
			$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
				    FROM public.combodadostabela where ctbid=5 and cdtstatus='A' order by cdtcod";
			$db->monta_combo('campo5', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', '000000005_0'); 
			?>
		</td>
	</tr>
<? 
			
		} else {
?>
	<tr>
		<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
		<td>
			<?
			$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
				    FROM public.combodadostabela cd 
				    LEFT JOIN monitora.subacaoenquadramento se ON se.eqdid=cd.cdtid 
				    WHERE ctbid=5 and cdtstatus='A' and sbaid = ".$sbaid." order by cdtcod";
			$db->monta_combo('campo5', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', '000000005_0');
			?>
		</td>
	</tr>
<? 
		}	
	} 
?>
	
		<tr>
			<td width="40%" align='right' class="SubTituloDireita">Subação:</td>
			<td>
			<table>
			<tr>
			<td>
			<?
			$sql = "SELECT sba.sbaid as value, UPPER(sba.sbacod) as codigo, sba.sbatitulo as descricao  FROM monitora.subacao sba 
			   		LEFT JOIN monitora.subacaounidade ppe ON ppe.sbaid = sba.sbaid WHERE sbastatus='A' and ppe.entid='".$_SESSION['monitora_var']['entid']."'";
			
				campo_popup(
                               'sbaid',
                               $sql,
                               'Selecione a Subação',
                               'selecionarprograma',
                               '400x400',
                               '60',
                               array(array("codigo" => "sba.sbatitulo",
                                           "descricao" => "Subação:",
                                           "numeric"        => "0"),
                               ),
                               1,
                               false
                       		);          
                       		
			?>
			<script>
				<?if($sbaid){?>
				document.formulario.sbaid_dsc.value = "<?=$subacaodsc?>";
				document.formulario.sbaid.value = "<?=$sbaid?>";
				<?}?>
			</script>
			</td>
			<?
			if($_SESSION['monitora_var']['tipo'] == "un") {
			?>
			<td>
			<input type="button" name="inserir" value="Novo" onclick="janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalho/programainstitucionalunidade&acao=A', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=400' ); janela.focus();">
			</td>
			<?
			}
			?>
			</tr>
			</table>
			</td>
		</tr>
		<?
		if($sbaid) {
			if($_SESSION['monitora_var']['tipo'] == "un") {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Código da subação:</td>
				<td><? echo strtoupper($subacao['sbacod']); ?></td>
			</tr>
			<?
			} else {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
				<td><?= $exedsc ?><input type="hidden" name="campo3" value="<?=$exeid?>"> <select name="00000000<?=COMBO_REGRAPDEEPPA;?>_0" id="00000000<?=COMBO_REGRAPDEEPPA;?>_0" style="display:none"><option value="<? echo $exedsc ?>" selected></option></select> </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
				<td><?= $gstdsc ?><input type="hidden" name="campo4" value="<?=$gstid?>"> <select name="00000000<?=COMBO_REGRAPDEEPPA;?>_1" id="00000000<?=COMBO_REGRAPDEEPPA;?>_1" style="display:none"><option value="<? echo $gstdsc ?>" selected></option></select> </td>
			</tr>
			<?
			}
		}
		?>
<?
	}
}
?>


<?
if($plicod) {
?>
	<? 
	if($possuirprogramainstitucional) {
		if($_SESSION['monitora_var']['tipo'] == "un") { 
		?>
		<tr>
			<td align='right' class="SubTituloDireita">Código da subação:</td>
			<td><? echo strtoupper($subacao['sbacod']); ?></td>
		</tr>
		<?
		} else {
		?>
		<tr>
			<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
			<td><?= $exedsc ?><input type="hidden" name="campo3" value="<?=$exeid?>"> <select name="00000000<?=COMBO_REGRAPDEEPPA;?>_0" id="00000000<?=COMBO_REGRAPDEEPPA;?>_0" style="display:none"><option value="<? echo $exedsc ?>" selected></option></select> </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
			<td><?= $gstdsc ?><input type="hidden" name="campo4" value="<?=$gstid?>"> <select name="00000000<?=COMBO_REGRAPDEEPPA;?>_1" id="00000000<?=COMBO_REGRAPDEEPPA;?>_1" style="display:none"><option value="<? echo $gstdsc ?>" selected></option></select> </td>
		</tr>
		<?
		}
		
	} else {
		?>
			<tr>
				<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
			    <td><? echo $enquadramento; ?><input type="hidden" name="eqdid" value="<? echo $planointerno['eqdid']; ?>"></td>
			</tr>
		<?
	
	}?>
	
	<tr>
		<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
		<td>
		<input type="hidden" name="dpiid_" value="<? echo $planointerno['dpiid']; ?>">
		<?
		$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
			    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
		$dpiid = $planointerno['dpiid'];
		$db->monta_combo('dpiid', $sql, (($acessoeditar)?'S':'N'), 'Selecione', 'atualizarPrevisaoPIParcial1', '', '', '240', 'S', '000000005_1');
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Categoria de Apropriação:</td>
		<td>
		<input type="hidden" name="cpiid_" value="<? echo $planointerno['cpiid']; ?>">
		<?
		$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
			    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
		$cpiid = $planointerno['cpiid'];
		$db->monta_combo('cpiid', $sql, (($acessoeditar)?'S':'N'), 'Selecione', 'atualizarPrevisaoPIParcial1', '', '', '340', 'S', '000000005_2');
		?>
		</td>
	</tr>
<?
} else {
	if($possuirprogramainstitucional) {
		if($sbaid){
			?>
				 
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
						$db->monta_combo('campo6', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
						$db->monta_combo('campo7', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '340', 'S', '000000005_2');
						?>
					</td>
				</tr>	
				 		
			<?
		}
	}
	else{
		?>
				<tr>
					<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=5 and cdtstatus='A' order by cdtcod";
						$db->monta_combo('campo5', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', '000000005_0'); 
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo6', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', '000000005_1');
						?>
					</td>
				</tr>
				<tr>
					<td class='SubTituloDireita'>Categoria de Apropriação:</td>
					<td>
						<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
						
						$db->monta_combo('campo7', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '340', 'S', '000000005_2');
						?>
					</td>
				</tr>	
		<?
	}
	switch($unicod) {
		case AD:
			break;
		case CAPES:
			break;
		case INEP:
			break;
		case FNDE:
			/* regra especifica, a combo surgira somente quando for escolhida a acacod
			 * for 0282 e 0283
			 */
			?>
			<tr style='display:none' id='geradorcompleto'>
				<td align='right' class="SubTituloDireita">Gerador:</td>
	    		<td><? $db->monta_combo('regraespecial1', array(), 'S', 'Selecione', 'regraespecial_1', '', '', '', 'S', 'regraespecial1'); ?></td>
			</tr>
			<?
			break;
	}
	
}

if($possuirprogramainstitucional) {
	if($sbaid){
	
		if(!$plicod) {
			if($unidade['entunicod'] == INEP) {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S','S','',4,3,'','',null,null,null,'id="idCodificacao"','atualizarPrevisaoPI();');?></td>
			</tr>
			<?
			} else {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S','S','',3,2,'','',null,null,null,'id="idCodificacao"','atualizarPrevisaoPI();');?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
			    <td>
				<?
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
					    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
				$modensino = $modid;
				$db->monta_combo('modensino', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'idModensino');
	    	 	?>
			    </td>
			</tr>
			<?
			}
			?>
		<? } else {
				if($unidade['entunicod'] == INEP) {
		?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S',(($acessoeditar)?'S':'N'),'',4,3,'','',null,null,null,'id="idCodificacao"','atualizarCodigoLivre();');?><input type="hidden" name="plilivre_" value="<? echo $plilivre; ?>"></td>
			</tr>
		<?
				} else {
		?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S',(($acessoeditar)?'S':'N'),'',3,2,'','',null,null,null,'id="idCodificacao"','atualizarCodigoLivre();');?><input type="hidden" name="plilivre_" value="<? echo $plilivre; ?>"></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
			    <td>
				<input type="hidden" name="modensino_" value="<? echo $modid; ?>">
				<?
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
						    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
					$modensino = $modid;
					$db->monta_combo('modensino', $sql, (($acessoeditar)?'S':'N'), 'Selecione', 'atualizarMod', '', '', '240', 'S', 'idModensino');
		    	 ?>
			    </td>
			</tr>
		<?
				}
		   }
		?>
			<tr>
				<td align='right' class="SubTituloDireita">Título:</td>
			    <td>
			    	<input type="hidden" name="txtsubacao" id="txtsubacao" value="<?=$subacaodsc?>">
			    	<?
			    		$subacaodsc2 = split(' - ', $subacaodsc);
			    		$subacaodsc = $subacaodsc2[1]." - ";
			    		$ctsubacao = strlen($subacaodsc2[1]);
			    		echo $subacaodsc;
			    		$plititulo = str_replace($subacaodsc, "", $plititulo);
			    		
			    		echo "<input type='hidden' name='txtsubacao' value='".$subacaodsc."'>";
			    		
			    		if(!$plicod) {
			    			echo campo_texto('plititulo','S','S','',50,45,'','',null,null,null,'',"limitaTitulo('$ctsubacao');"); //campo_texto('plititulo','S','S','',50,45,'',''); obrigatorio();  
			    		}
			    		else{
							echo campo_texto('plititulo','S',(($acessoeditar)?'S':'N'),'',50,45,'','',null,null,null,'',"limitaTitulo('$ctsubacao');"); //campo_texto('plititulo','S','S','',50,45,'',''); obrigatorio();
			    		}
			    		
			    	?>
			    </td>
			</tr>
			<tr>
			    <td align='right' class="SubTituloDireita" valign="top">Descrição / Finalidade:</td>
			    <td><?= campo_textarea( 'plidsc', 'S', (($acessoeditar)?'S':'N'), '', 60, 2, 250 );?></td>
			</tr>
			<?if($plisituacao){?>
			<tr>
			    <td align='right' class="SubTituloDireita">Situação:</td>
			    <td>
			    	<?
			    	$sit = recuperaCorSituacao($plisituacao);
			    	echo "<font color='".$sit['cor']."'>".$sit['nome']."</font>";
			    	?>
			    </td>
			</tr>
			<?}?>
			
			<tr>
			    <td align='right' class="SubTituloDireita">Previsão PI:</td>
			    <td align="left">
			    
			        <table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
			        		<tr>
				        		<td>
				        		<table cellpadding="0" border="0" width="98%" >
					        		<tr>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
					        		<td align="center" colspan="3"><b>Subação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
									<? if($unidade['entunicod'] != INEP) { ?>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Modalidade</b></td>
					        		<? } ?>
					        		</tr>
					        		<tr>
					        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo substr($planointerno['plicod'],0,1); ?></span></td>
					        			<?
										if($_SESSION['monitora_var']['tipo'] == "un") {
						        			?>
						        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="3"><? echo strtoupper($subacao['sbacod']); ?></td>
							        		<?
										} else {
							        		?>
							        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="3"><span id="gerador1"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],1,1):"X"); ?></span><span id="gerador2"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],2,1):"X"); ?></span><span id="gerador3"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],3,2):"XX"); ?></span></td>
							        		<?
										}
						        		?>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo substr($planointerno['plicod'],5,1); ?></span></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo substr($planointerno['plicod'],6,2); ?></span></td>
										<? if($unidade['entunicod'] == INEP) { ?>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'],8,3); ?></span></td>
										<? } else { ?>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'],8,2); ?></span></td>		        			        			  
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="modalidade"><? echo substr($planointerno['plicod'],10,1); ?></span></td>
										<? } ?>
					        		</tr>
					        		<tr>
					        			<td colspan="7" align="center"><b>Código PI / SIAFI: <span id="enquadramento_i"><? echo substr($planointerno['plicod'],0,1); ?></span><?
					        			if($_SESSION['monitora_var']['tipo'] == "un") {
					        				echo "<input type='hidden' name='geradorcompleto' id='geradorcompleto' value='".strtoupper($subacao['sbacod'])."'>";
					        				echo strtoupper($subacao['sbacod']);
										} else {
					        			?>
						        			<span id="gerador1_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],1,1):"X"); ?></span><span id="gerador2_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],2,1):"X"); ?></span><span id="gerador3_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],3,2):"XX"); ?></span>
					        			<?
										}
					        			?><span id="nivel_i"><? echo substr($planointerno['plicod'],5,1); ?></span><span id="apropriacao_i"><? echo substr($planointerno['plicod'],6,2); ?></span><span id="codificacao_i"><? echo substr($planointerno['plicod'],8,2); ?></span><span id="modalidade_i"><? echo substr($planointerno['plicod'],10,1); ?></span></b></td>
					        		</tr>
					        	</table>	        		
				        		</td>
			        		</tr>
			        </table>
			        
				</td>
			</tr>
		<?
	}
}
else{
		if(!$plicod) {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S','S','',3,2,'','',null,null,null,'id="idCodificacao"','atualizarPrevisaoPI();');?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
			    <td>
					<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
						$modensino = $modid;
						$db->monta_combo('modensino', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'idModensino');
			    	 ?>
			    </td>
			</tr>
		<?}else{?>
			<tr>
				<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
			    <td><?=campo_texto('plilivre','S','S','',3,2,'','',null,null,null,'id="idCodificacao"','atualizarCodigoLivre();');?><input type="hidden" name="plilivre_" value="<? echo $plilivre; ?>"></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Modalidade de Ensino / Tema / Público:</td>
			    <td>
			    	<?//=campo_texto('modensino','S','S','',3,1,'','',null,null,null,'id="idModensino"','atualizarCodigoLivre();');?>
					<?
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
							    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
						$modensino = $modid;
						$db->monta_combo('modensino', $sql, 'S', 'Selecione', 'atualizarMod', '', '', '240', 'S', 'idModensino');
			    	 ?>
			    </td>
			</tr>
		<?}?>
		<tr>
			<td align='right' class="SubTituloDireita">Título:</td>
		    <td><? 
		    		echo campo_texto('plititulo','S','S','',50,45,'','',null,null,null,''); //campo_texto('plititulo','S','S','',50,45,'',''); obrigatorio();
		    	?>
		    </td>
		</tr>
		<tr>
		    <td align='right' class="SubTituloDireita">Descrição:</td>
		    <td><?= campo_textarea( 'plidsc', 'S', 'S', '', 60, 2, 250 );?></td>
		</tr>
		<?if($plisituacao){?>
		<tr>
		    <td align='right' class="SubTituloDireita">Situação:</td>
		    <td>
		    	<?
		    	if($plisituacao == "P") $plisituacaodsc = "<font color='#AF7817'>Pendente</font>";
		    	if($plisituacao == "A") $plisituacaodsc = "<font color='green'>Aprovado</font>";
		    	if($plisituacao == "H") $plisituacaodsc = "<font color='blue'>Homologado</font>";
		    	if($plisituacao == "R") $plisituacaodsc = "<font color='#EAC117'>Revisado</font>";
		    	echo $plisituacaodsc;
		    	?>
		    </td>
		</tr>
		<?}?>
		
		<tr>
		    <td align='right' class="SubTituloDireita">Previsão PI:</td>
		    <td align="left">
		    
		        <table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
		        		<tr>
			        		<td>
			        		<table cellpadding="0" border="0" width="98%" >
				        		<tr>
				        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
				        		<td align="center" colspan="3"><b>Gerador</b></td>
				        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
				        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
				        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
				        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Modalidade</b></td>
				        		</tr>
				        		<tr>
				        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo substr($planointerno['plicod'],0,1); ?></span></td>
					        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="gerador1"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],1,1):"X"); ?></span></td>
					        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="gerador2"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],2,1):"X"); ?></span></td>
					        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="gerador3"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],3,2):"XX"); ?></span></td>
									<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo substr($planointerno['plicod'],5,1); ?></span></td>
									<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo substr($planointerno['plicod'],6,2); ?></span></td>
									<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'],8,2); ?></span></td>	
									<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="modalidade"><? echo substr($planointerno['plicod'],10,1); ?></span></td>	        			        			  
				        		</tr>
				        		<tr>
				        			<td colspan="7" align="center"><b>Código PI / SIAFI: <span id="enquadramento_i"><? echo substr($planointerno['plicod'],0,1); ?></span><span id="gerador1_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],1,1):"X"); ?></span><span id="gerador2_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],2,1):"X"); ?></span><span id="gerador3_i"><? echo (($planointerno['plicod'])?substr($planointerno['plicod'],3,2):"XX"); ?></span><span id="nivel_i"><? echo substr($planointerno['plicod'],5,1); ?></span><span id="apropriacao_i"><? echo substr($planointerno['plicod'],6,2); ?></span><span id="codificacao_i"><? echo substr($planointerno['plicod'],8,2); ?></span><span id="modalidade_i"><? echo substr($planointerno['plicod'],10,1); ?></span></b></td>
				        		</tr>
		
				        	</table>	        		
			        		</td>
		        		</tr>
		        </table>
		        
			</td>
		</tr> 
		
	
	<?
}

	if($possuirprogramainstitucional){
		if($sbaid){
			?>
				<tr bgcolor="#cccccc">
			  	  <td colspan="2" align="right">
			  	  <? if($acessoeditar) { ?>
			  	  <input type="button" class="botao" name="btg" value="Salvar" onclick="submeter('<?=$plicod?>');">
			  	  <input type="button" class="botao" name="btg" value="Salvar como revisado" onclick="submeterComSituacao('<?=$plicod?>','V');">
			  	  <input type="hidden" name="plisituacao" value="">
			  	  <? } ?>
			  	  <input type="button" class="botao" name="btn" value="Voltar" onclick="window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=<? echo $atiid;?>';">
			  	  </td>
			    </tr>
			<?
		}
		else{
			?>
				<tr bgcolor="#cccccc">
			      <td>&nbsp;</td>
			  	  <td>&nbsp;</td>
			    </tr>
			<?
		}
	}
	else{
		 ?>
			<tr bgcolor="#cccccc">
		  	  <td colspan="2" align="right">
		  	  <input type="button" class="botao" name="btg" value="Salvar" onclick="submeter('<?=$plicod?>');">
		  	  <input type="button" class="botao" name="btn" value="Novo" onclick="window.location='?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=<? echo $atiid;?>';"></td>
		    </tr>
		 <?
	}

?>
<tr>
	<td colspan="2" class="SubTituloCentro">Lista de Planos Internos cadastrados</td>
</tr>
<tr>
	<td colspan="2">
	<?
	$sql = "SELECT 
				'<center><a style=\"cursor:pointer;\" onclick=\"alterarpi(\''||p.plicod||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a>'|| CASE p.plisituacao 
																																										WHEN 'P' THEN ' <a style=\"cursor:pointer;\" onclick=\"removerpi(\''||p.plicod||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>' 
																																										WHEN 'R' THEN ' <a style=\"cursor:pointer;\" onclick=\"removerpi(\''||p.plicod||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>' 
																																										ELSE ''
																																										END ||'</center>' as acao, 
				p.plicod as plicod,
				coalesce(p.plititulo,'Não preenchido') as titulo,
				coalesce(SUM(p.plivalor),0) as total,
							CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
								 WHEN p.plisituacao = 'C' THEN ' <font color=\"green\">Aprovado</font> '
								 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
								 WHEN p.plisituacao = 'V' THEN	' <font color=\"#3F85FF\">Revisado</font> '
								 WHEN p.plisituacao = 'S' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
								 WHEN p.plisituacao = 'R' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
				(SELECT usunome ||' por '||to_char(pihdata, 'dd/mm/YYYY hh24:mi') FROM monitora.planointernohistorico p1 LEFT JOIN seguranca.usuario u1 ON u1.usucpf = p1.usucpf WHERE p1.plicod=p.plicod ORDER BY p1.pihdata DESC LIMIT 1) as hst
				FROM monitora.planointerno p
				inner join monitora.planointernoatividade pa on pa.pliid = p.pliid
				inner join pde.atividade a on a.atiid = pa.atiid
				where 
				a.atiid = ".$atiid." AND
				p.plistatus = 'A' 
				GROUP BY p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";
	
	$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
</table>

</form>


<script type="text/javascript">

function submeter(pliid){
	var validado = true;
	if(validar()){
		if(document.formulario.atiid_move.value=='') {
			alert("Selecione uma atividade");
			return false;
		}
		<? if($unidade['entunicod'] != INEP) { ?>
		if(document.formulario.modensino.value=='') {
			alert("Selecione uma modalidade");
			return false;
		}
		<? } ?>
		if(validado) {
			document.formulario.submit();
		}
	}

}

function submeterComSituacao(pliid,situacao) {
	var validado = true;
	
	if(!validar()){
		return false;
	}
	if(document.formulario.atiid_move.value=='') {
		alert("Selecione uma atividade");
		return false;
	}
	<? if($unidade['entunicod'] != INEP) { ?>
	if(document.formulario.modensino.value=='') {
		alert("Selecione uma modalidade");
		return false;
	}
	<? } ?>
	if(validado) {
		document.formulario.plisituacao.value=situacao;
		document.formulario.submit();
	}
}


function removerpi(plicod){
	var conf = confirm("Você realmente deseja excluir este PI?");	
	if(conf) {
		document.getElementById('evento').value = 'E';
		document.getElementById('plicod').value = plicod;
		document.formulario.submit();	
	}
}

function alterarpi(plicod){
	document.getElementById('evento').value = 'A';
	document.getElementById('plicod').value = plicod;
	document.formulario.submit();	
}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function validar(x){	

	var msg = "";
	
	<?if($mostra){
		if($possuirprogramainstitucional){?>
			var tabela = document.getElementById('orcamento');
			// validando se existe ação selecionado/ valor
			if(tabela.rows.length == 4) {
				msg+="A escolha das ações é obrigatório.\n";
			} else {
				for(i=2;i<(tabela.rows.length-2);i++) {
					if(!tabela.rows[i].cells[6].firstChild.value) {
						msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
					}
				}
			}
		<?}else{?>
			var tabela = document.getElementById('orcamento');
			// validando se existe ação selecionado/ valor
			if(tabela.rows.length == 4) {
				msg+="A escolha das ações é obrigatório.\n";
			} else {
				for(i=2;i<(tabela.rows.length-2);i++) {
					if(!tabela.rows[i].cells[5].firstChild.value) {
						msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
					}
				}
			}
	<?}}?>
	<?
	// se for unidades especiais, deve ser verificado os digitos do gerador
	switch($unicod) {
		case AD:
			break;
		case CAPES:
			break;
		case INEP:
			break;
		case FNDE:
			if(!$plicod){
				?>
				if(document.formulario.sbaid.value == '') {
					msg+="O preenchimento do campo Subação é obrigatório.\n";
				}
				<?
			}	
			if(!$plicod && $sbaid){
				?>
				if(document.formulario.atiid_move.value == '') {
					msg+="O preenchimento da atividade do plano de trabalho.\n";
				}
				if(document.formulario.plititulo.value == '') {
					msg+="O preenchimento do campo Título é obrigatório.\n";
				}
				if(document.formulario.plidsc.value == ''){
					msg+="O preenchimento do campo Descrição é obrigatório.\n";
				}
				
				// testa se foi selecionada o enquadramento
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").value == ''){
					msg+="O preenchimento do campo Enquadramento da Despesa é obrigatório.\n";
				}
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").value == ''){
					msg+="O preenchimento do campo Nível/Etapa de Ensino é obrigatório.\n";
				}
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").value == ''){
					msg+="O preenchimento do campo Categoria de Apropriação é obrigatório.\n";
				}
				<? if($unidade['entunicod'] == INEP) { ?>
				if(document.formulario.plilivre.value.length != 3){
					msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 3(dois dígitos).\n";
				}
				<? } else { ?>
				if(document.formulario.plilivre.value.length != 2){
					msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 2(dois dígitos).\n";
				}
				<? } ?>
				
				if(document.getElementById("regraespecial1").value == '') {
					
					if(document.getElementById("000000003_1").value == ''){
						msg+="O preenchimento do campo Executor Orçamentário é obrigatório.\n";
					}
					if(document.getElementById("000000003_0").value == ''){
						msg+="O preenchimento do campo Gestor da Subação é obrigatório.\n";
					}
		
				}
				<?
			}
			break;
			
		default:
			if(!$plicod) { ?>
				if(document.formulario.plititulo.value == '') {
					msg+="O preenchimento do campo Título é obrigatório.\n";
				}
				if(document.formulario.plidsc.value == ''){
					msg+="O preenchimento do campo Descrição é obrigatório.\n";
				}
				 
				// testa se foi selecionada o enquadramento
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").value == ''){
					msg+="O preenchimento do campo Enquadramento da Despesa é obrigatório.\n";
				}
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").value == ''){
					msg+="O preenchimento do campo Nível/Etapa de Ensino é obrigatório.\n";
				}
				if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").value == ''){
					msg+="O preenchimento do campo Categoria de Apropriação é obrigatório.\n";
				}
				<? if($unidade['entunicod'] == INEP) { ?>
				if(document.formulario.plilivre.value.length != 3){
					msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 3(dois dígitos).\n";
				}
				<? } else { ?>
				if(document.formulario.plilivre.value.length != 2){
					msg+="O preenchimento do campo Codifição da Unidade é obrigatório e igual a 2(dois dígitos).\n";
				}
				<?
				   }
		
			}
		break;
	
	} ?>
	
	
	if(msg != ""){
		alert(msg);
		return false;
	}else{
		<?if(!$plicod && $sbaid){?>
		 document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").disabled = false;
		 <?}?>
		 
		 <?
		 if($possuirprogramainstitucional){
		 	if(!$plicod){
		   		if($unidade['entunicod'] == INEP) {		 	
		 ?>
				 var pi = (document.getElementById("enquadramento").innerHTML + 
				 		  document.getElementById("gerador1").innerHTML + 
				 		  document.getElementById("gerador2").innerHTML +
				 		  document.getElementById("gerador3").innerHTML +
				 		  document.getElementById("nivel").innerHTML +
				 		  document.getElementById("apropriacao").innerHTML +
				 		  document.getElementById("codificacao").innerHTML);
		<?
				} elseif($_SESSION['monitora_var']['tipo'] == "un") {
		?>
				 var pi = (document.getElementById("enquadramento").innerHTML + 
				 		  document.getElementById("geradorcompleto").innerHTML + 
				 		  document.getElementById("nivel").innerHTML +
				 		  document.getElementById("apropriacao").innerHTML +
				 		  document.getElementById("codificacao").innerHTML +
				 		  document.getElementById("modalidade").innerHTML);
		<?
				} else {
					
		?>
				 var pi = (document.getElementById("enquadramento").innerHTML + 
				 		  document.getElementById("gerador1").innerHTML + 
				 		  document.getElementById("gerador2").innerHTML +
				 		  document.getElementById("gerador3").innerHTML +
				 		  document.getElementById("nivel").innerHTML +
				 		  document.getElementById("apropriacao").innerHTML +
				 		  document.getElementById("codificacao").innerHTML +
				 		  document.getElementById("modalidade").innerHTML);

		<?
				}
		?>
				 if(validapi(pi)) {
				 	return true;
				 } else {
				 	return false;
				 }
		 <?
			}else{
		 		echo "return true;";
		 	  }
		   }
		 	else{
		 		echo "return true;";
	 	   }
	 	 ?>
		 
		 
	}	
}

function valida2(pi) {
	 if(!pi){
		return true;
	 }
	 else{
	 	if(pi.substr(0,10) == 'pijaexiste'){
	 		pi = pi.replace("pijaexiste","");
		 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
		 	displayStaticMessage(alertaDisplay,false);
	 		return false;
	 	}
	 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
	 	displayStaticMessage(alertaDisplay,false);
	 	return false;
	 }
	
}


/*
* Faz requisição via ajax
*/
function validapi(pi) {
	var retorno = true;
	
	var req = new Ajax.Request(window.location.href, {
							        method:     'post',
							        parameters: '&piAjax=' + pi,
							        asynchronous: false,
							        onComplete: function (res) {
    						        	x = res.responseText;
    									retorno = valida2(x);
							        }
							  });

	return retorno;	
}


function utilizaracacod(chk) {

	var tabelaorigem = document.getElementById('orcamento');
	var cod = "";
	var cod2 = "";
	for(i=2;i<tabelaorigem.rows.length-2;i++) {
		if(i==2){
			cod = tabelaorigem.rows[i].cells[1].innerHTML;
		}
	}

	if(i!=3){
		alert("Não é possível selecionar este item, pois não existe nenhuma ação/PTRES \nou existem mais de uma ação/PTRES.");
		document.getElementById("acacodpi").checked = false;
		cod="";
	}
	else{
		cod = cod.split(".");
		cod = cod[1];
		document.getElementById('acacodpidsc').innerHTML = cod;
		document.getElementById('acacodpi').value=cod;
	}
	

	if(chk.checked) {
		document.getElementById("gerador1").innerHTML = document.getElementById('acacodpidsc').innerHTML.substr(0,1);
		document.getElementById("gerador2").innerHTML = document.getElementById('acacodpidsc').innerHTML.substr(1,1);
		document.getElementById("gerador3").innerHTML = document.getElementById('acacodpidsc').innerHTML.substr(2,2);
	} else {
		document.getElementById("gerador1").innerHTML = "X";
		document.getElementById("gerador2").innerHTML = "X";
		document.getElementById("gerador3").innerHTML = "XX";
		document.getElementById('acacodpidsc').innerHTML = "";
		document.getElementById('acacodpi').value="";
	}
}

function abrir_lista() {
	<?if($possuirprogramainstitucional){?>
		janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalho/listarPrograma&acao=A&atiid=<?php echo $_GET[atiid]; ?>&sbaid=<?=$sbaid?>&programainstitucional=S', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
	<?}else{?>
		janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalho/listarPrograma&acao=A&atiid=<?php echo $_GET[atiid]; ?>&sbaid=<?=$sbaid?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
	<?}?>
}

function atualizarPrevisaoPIParcial1() {
	if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").value) {
		var apropriacao = document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").options[document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").selectedIndex].text.split(" - ");
		document.getElementById("apropriacao").innerHTML = apropriacao[0]; 
		document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
	}
	if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").value) {
		var nivel = document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").options[document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").selectedIndex].text.split(" - ");
		document.getElementById("nivel").innerHTML = nivel[0]; 
		document.getElementById("nivel_i").innerHTML = nivel[0];
	}

}

function atualizarPrevisaoPI(){
	if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").value) {
		var apropriacao = document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").options[document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_2").selectedIndex].text.split(" - ");
		document.getElementById("apropriacao").innerHTML = apropriacao[0]; 
		document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
	}
	if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").value) {
		var enquadramento = document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").options[document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_0").selectedIndex].text.split(" - ");
		document.getElementById("enquadramento").innerHTML = enquadramento[0]; 
		document.getElementById("enquadramento_i").innerHTML = enquadramento[0];
	}
	if(document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").value) {
		var nivel = document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").options[document.getElementById("00000000<?=COMBO_REGRAGERAL;?>_1").selectedIndex].text.split(" - ");
		document.getElementById("nivel").innerHTML = nivel[0]; 
		document.getElementById("nivel_i").innerHTML = nivel[0];
	}
	<?
			switch($unicod) {
			case AD:
				break;
			case CAPES:
				break;
			case INEP:
				break;
			case FNDE:
			?>
			if(document.getElementById("000000003_0").value) {
				var gerador1 = document.getElementById("000000003_0").value.split(" - ");
				//var gerador1 = document.getElementById("000000003_0").options[document.getElementById("000000003_0").selectedIndex].text.split(" - ");
				document.getElementById("gerador1").innerHTML    = gerador1[0];
				document.getElementById("gerador1_i").innerHTML    = gerador1[0];
			}
			if(document.getElementById("000000003_1").value) {
				var gerador2 = document.getElementById("000000003_1").value.split(" - ");
				//var gerador2 = document.getElementById("000000003_1").options[document.getElementById("000000003_1").selectedIndex].text.split(" - ");
				document.getElementById("gerador2").innerHTML    = gerador2[0];
				document.getElementById("gerador2_i").innerHTML    = gerador2[0];
			}
			var gerador3 = document.getElementById("sbaid_dsc").value.split(" - ");
			document.getElementById("gerador3").innerHTML = gerador3[0].substr(2); 
			document.getElementById("gerador3_i").innerHTML = gerador3[0].substr(2);
			<?
			break;
			
		}
		
	?>

	document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
	document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
	document.getElementById("codificacao_i").innerHTML = document.getElementById("idCodificacao").value;
	if(document.getElementById("idModensino")) {
		if(document.getElementById("idModensino").value) {
			var modi = document.getElementById("idModensino").options[document.getElementById("idModensino").selectedIndex].text.split(" - ");
			document.getElementById("modalidade").innerHTML = modi[0]; 
			document.getElementById("modalidade_i").innerHTML = modi[0];
		}
	}

}

function atualizarMod(){
	if(document.getElementById("idModensino").value) {
		var modi = document.getElementById("idModensino").options[document.getElementById("idModensino").selectedIndex].text.split(" - ");
		document.getElementById("modalidade").innerHTML = modi[0]; 
		document.getElementById("modalidade_i").innerHTML = modi[0];
	}
}

function atualizarCodigoLivre() {
	document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
	document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
}

function calculovalorPI() {
	<?if($possuirprogramainstitucional){?>
		var tabela = document.getElementById('orcamento');
		var tot = 0;
		for(i=2;i<tabela.rows.length-2;i++) {
			if(tabela.rows[i].cells[6].firstChild.value != "") {
				tot = tot + parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[6].firstChild.value,".",""),",","."));
			}
		}
	<?}else{?>
		var tabela = document.getElementById('orcamento');
		var tot = 0;
		for(i=2;i<tabela.rows.length-2;i++) {
			if(tabela.rows[i].cells[5].firstChild.value != "") {
				tot = tot + parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[5].firstChild.value,".",""),",","."));
			}
		}
	<?}?>

	var c = tot.toString();
	if(c.indexOf('.') == -1) {
		document.getElementById('valortotalpi').value = tot.toFixed(2);
	} else {
		document.getElementById('valortotalpi').value = Arredonda(tot,2);
	}
	document.getElementById('valortotalpi').onkeyup();
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
	   	alert(novo);
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }
}

function regraespecial_1(valor) {
	if(valor) {
		document.getElementById("gerador1").innerHTML = valor.substr(0,1);
		document.getElementById("gerador2").innerHTML = valor.substr(1,1);
		document.getElementById("gerador3").innerHTML = valor.substr(2,2);
	} else {
		document.getElementById("gerador1").innerHTML = "X";
		document.getElementById("gerador2").innerHTML = "X";
		document.getElementById("gerador3").innerHTML = "XX";
	}
}

function verificaDisponivel(campo, ptres, vlold) {}


function detfin(ptres){
	//alert(t);
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalho/detalhafinanceiropi&acao=A&ptres='+ptres, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
}

function limitaTitulo(tot){
	
	tot_geral = 45 - parseInt(tot);
	document.formulario.plititulo.maxLength = tot_geral;
	tit = document.formulario.plititulo.value;
	tot_tit = tit.length;
	if(tot_tit>tot_geral-1){
		alert('O campo Título possui o limite de '+tot_geral+' caracteres.');
		document.formulario.btg.focus();
	}
}

function selecionarprograma(sba) {
	document.formulario.evento.value = "SUBACAO";
	document.formulario.submit();
}

//seta a subação quando $sbaid for diferente de vazio
<?if(!$plicod && $sbaid){?>
	//enquadramento
	
	//document.getElementById("00000000<?//=COMBO_REGRAGERAL;?>_0").value = '<?// echo $eqdid; ?>'; 
	//executarChangeCombobox(document.getElementById("00000000<?//=COMBO_REGRAGERAL;?>_0"));
	//document.getElementById("00000000<?//=COMBO_REGRAGERAL;?>_0").disabled = true;
	atualizarPrevisaoPI();
	
	
	//executor
	//document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_0").value = '<?// echo $exeid; ?>'; 
//	executarChangeCombobox(document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_0"));
//	document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_0").disabled = true;

	//gestor
//	document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_1").value = '<?// echo $gstid; ?>'; 
	//executarChangeCombobox(document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_1"));
	//document.getElementById("00000000<?//=COMBO_REGRAPDEEPPA;?>_1").disabled = true;
	
<?}?>







messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow


function displayMessage(url)
{
	
	messageObj.setSource(url);

	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(400,200);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(600,500);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.

	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
	
	
}

function closeMessage()
{
	messageObj.close();	
}

</script>

