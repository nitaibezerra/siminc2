<?

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/pdeescola/_constantes.php";
include_once APPRAIZ . "www/pdeescola/_funcoes.php";

$db = new cls_banco();

$retorno = "";

if(isset($_GET['tipo'])) {
	switch($_GET['tipo']) {
		case 'carrega_atividades':
			if($_GET['id'] != "") {
				/*
				 * Correção por Alexandre Dourado 17/11/09 
				 * - Validando se a sessão com a variavel existe
				 */
				if(!$_SESSION["memid"]) {
					return "Modalidade de ensino não encontrada. <a href='pdeescola.php?modulo=melista&acao=E&requisicao=cadastra'>Clique aqui e refaça o procedimento.</a>";
					exit;
				}
				$modalidadeEscola = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]);
				
				$tipoLocalizacao = $db->pegaUm("SELECT tplid FROM entidade.entidade ent
												INNER JOIN pdeescola.memaiseducacao mem ON mem.entid = ent.entid
												WHERE mem.memid = ".$_SESSION["memid"]);
				
				$idMacrocampo 	 = $_GET['id'];
				
				$selVazio = false;
				
				if($_GET['mtaatividadepst'] == 'false') {
					//$mtaatividadepst = " AND mta.mtaatividadepst = 'f' ";
					$mtaatividadepst = " AND mta.mtaid not in(730,1072,837) ";
				} else {
					if($_SESSION["boAtivNaoPagas2009"]) $selVazio = true;
					//$mtaatividadepst = " AND mta.mtapst = 'f' ";
					//$mtaatividadepst = "";
					//$mtaatividadepst = " AND mta.mtapst = 't' ";
				}
				
				//dbg($_GET['aderiu'],1);
				
				if($_GET['aderiu']) {
					$mtapst 	= " AND mta.mtaid in (730,1072,837)";
					$disabled 	= "disabled=\"disabled\"";
					$selected 	= "selected=\"selected\"";
				}
				else{
					//$mtapst 	= " AND mta.mtaid not in(730,1072)";
				}
				
				if($_GET['classificacaoEscola'] == 'U'){
					$mtclassificacao = " AND (mta.mtaurbana = 'U' OR mta.mtaurbana = 'T') ";
				}
				if($_GET['classificacaoEscola'] == 'R'){
					
					$mtclassificacao = " AND (mta.mtaurbana = 'R' OR mta.mtaurbana = 'T') ";
					
					//42 = ACOMPANHAMENTO PEDAGÓGICO (OBRIGATÓRIA PELO MENOS UMA ATIVIDADE)
					if($idMacrocampo == 42){
						//$mtclassificacao = " AND mta.mtaurbana = 'f' ";
					}
					
					/*
					if($_GET['maisDeCem'] == 'true'){
						$mtclassificacao .= " AND mta.mtacampomaiscem = 't' ";
					}
					else{
						$mtclassificacao .= " AND mta.mtacampomenoscem = 't' ";
					}
					*/
					
				}
				
				//verifica se a escola participou no ano anterior e se é urbana
				if($_SESSION["exercicio"] == 2013){
					$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'";
					$possuiAnoAnterior = $db->pegaUm($sql);
					
					if($_GET['classificacaoEscola'] == 'U'){
						if( (integer)$possuiAnoAnterior > 0 ) {
							if($idMacrocampo == 55){
								$andAno2013 = " AND mta.mtaid in(877) ";
							}
							if($idMacrocampo == 57){
								$andAno2013 = " AND mta.mtaid not in(812) ";
							}
							if($idMacrocampo == 59){
								$andAno2013 = " AND mta.mtaid not in(837, 824, 825, 826, 830, 831, 833, 843, 844, 845) ";
							}
						}
						else{
							if($idMacrocampo == 55){
								$andAno2013 = " AND mta.mtaid < 877 ";
							}
						}
					}
					else{
						if( (integer)$possuiAnoAnterior > 0 ) {
							if($idMacrocampo == 59){
								$andAno2013 = " AND mta.mtaid not in(824, 825, 830, 831, 833, 843, 844) ";
							}
						}
						else{
							/*
							if($idMacrocampo == 59){
								$andAno2013 = " AND mta.mtaid not in(884) ";
							}
							*/
						}
					}
				}				

				$sql = "SELECT
							mta.mtaid AS codigo, 
							mta.mtadescricao AS descricao
						FROM
							pdeescola.metipoatividade mta
						WHERE
							mta.mtasituacao = 'A' AND
							mta.mtaanoreferencia = " . $_SESSION["exercicio"] . " AND
							mta.mtmid = ".$idMacrocampo." AND 
							(mta.mtamodalidadeensino = '".$modalidadeEscola."' OR mta.mtamodalidadeensino = 'T')   
							".$mtaatividadepst." 
							".$mtapst."
							".$mtclassificacao."
							".$andAno2013."
							".$union2013."
						ORDER BY 2";
				//dbg($sql);
				$selAtividade = $db->carregar($sql);
				
				$retorno .= "<select name=\"atividade\" id=\"atividade\" style=\"width:200px;\" class=\"CampoEstilo\" onchange=\"alertAtividade();\" ".$disabled.">
								<option value=\"\">-- Selecione a Atividade --</option>";
				
				if(!$selVazio) {
					if($selAtividade) {
						for($i=0; $i<count($selAtividade); $i++) {
							// A atividade de 'ciclismo' só fica disponível para escolas rurais. (cod de ciclismo de 2010)
							if(($selAtividade[$i]["codigo"] == 261 || $selAtividade[$i]["codigo"] == 658) && $tipoLocalizacao != 2) continue;
							 
							$retorno .= "<option value=\"".$selAtividade[$i]["codigo"]."\" ".$selected.">".$selAtividade[$i]["descricao"]."</option>";
						}
					}
				}
				
				$retorno .= "</select>";
			}
			else {
				$retorno .= "<select name=\"atividade\" id=\"atividade\" style=\"width:200px;\" class=\"CampoEstilo\">
								<option value=\"\">-- Selecione a Atividade --</option>
							 </select>";
			}
			
			echo $retorno;
			break;
			
		case 'redirecioname':
			$entid = $_GET['entid'];
			
			if($entid)
			{
				$_SESSION["meentid"] = $entid;

				// para controlar o erro com acesso de alguns usuários
				$_SESSION["exercicio"] = ($_SESSION["exercicio"]) ? $_SESSION["exercicio"] : date('Y');
				
				// Quando for perfil de cadastrador, verifica em quais anos de exercício que a entidade existe.
				if( in_array( PDEESC_PERFIL_CAD_MAIS_EDUCACAO, arrayPerfil() ) )
				{
					$sql = "SELECT
								memanoreferencia
							FROM
								pdeescola.memaiseducacao
							WHERE
								entid = ".$entid." AND 
								memstatus = 'A'";
					$anoReferencia = $db->carregar($sql);
					
					if(count($anoReferencia) == 1)
					{
						$_SESSION["exercicio"] = $anoReferencia[0]["memanoreferencia"];
					}
					else
					{
						echo "melista_ano_exercicio";
						die;
					}
				}
				
				if( $_SESSION["exercicio"] )
				{
					$sql = "SELECT
								memid
							FROM
								pdeescola.memaiseducacao
							WHERE
								memanoreferencia = " . $_SESSION["exercicio"] . " AND
								entid = ".$entid." AND memstatus = 'A'";
					$memid = $db->pegaUm($sql);
				}
				
				// Seta sessão do MEMID
				if($memid)
					$_SESSION["memid"] = $memid;
				else
					unset($_SESSION["memid"]);
			}
			else
			{
				unset($_SESSION["meentid"]);
				unset($_SESSION["memid"]);
			}
			
			break;
			
		case 'verifica_censo':
			$serie = $_GET['serie'];
			$valor = $_GET['valor'];
			
			$sql = "SELECT
						mecquantidadealunos
					FROM
						pdeescola.mecenso
					WHERE
						entid = '".$_SESSION["meentid"]."' AND
						mecanoreferencia = " . $_SESSION["exercicio"] . " AND
						mecserie = ".$serie;
			$quantidade = $db->pegaUm($sql);
			
			$quantidade = ($quantidade != NULL) ? $quantidade : 0;
			
			//if($quantidade)
				$retorno = ((integer)$valor > (integer)$quantidade) ? "erro" : "ok";
			//else
				//$retorno = "ok";
			
			$retorno .= "@".$serie;
			echo $retorno;
			break;
			
		case 'testa_requisitos':
			
			/*
			 * Correção Alexandre Dourado 17/11/09
			 */
			if(!$_SESSION["memid"]) {
				return "ERRO";
				exit;
			}
			
			$sql = "SELECT
						mem.memmodalidadeensino as modalidade,
						coalesce(count(mem2.*), 0) as existe_ano_anterior
					FROM
						pdeescola.memaiseducacao mem
					LEFT JOIN
						pdeescola.memaiseducacao mem2 ON mem2.entid = mem.entid 
													 AND mem2.memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." 
													 AND mem2.memstatus = 'A'
					WHERE
						mem.memid = ".$_SESSION["memid"]." AND 
						mem.memstatus = 'A'
					GROUP BY
						mem.memmodalidadeensino";
			$dados = $db->carregar($sql);
			
			$qtdAlunos = $_GET["qtd_alunos"];
			
			//$retorno = ($dados[0]["modalidade"] == 'F' && (integer)$dados[0]["existe_ano_anterior"] > 0 && (integer)$qtdAlunos >= 150) ? 'true' : 'false';
			$retorno = ($dados[0]["modalidade"] == 'F' && (integer)$dados[0]["existe_ano_anterior"] > 0 ) ? 'true' : 'false';
			 
			echo $retorno;
			break;
			
		case 'testa_requisitos2':
			
			/*
			 * Correção Alexandre Dourado 17/11/09
			 */
			if(!$_SESSION["memid"]) {
				return "ERRO";
				exit;
			}
			
			$sql = "SELECT memadesaopst FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1);
			$dados = $db->carregar($sql);
			
			$retorno = ($dados[0]["memadesaopst"] == 'S') ? 'true' : 'false';
			 
			echo $retorno;
			break;
			
		case 'aderir_pst':
			$sql = "UPDATE
						pdeescola.memaiseducacao
					SET
						memadesaopst = 'S',
						memdataadesaopst = now(),
						memcpfadesaopst = '".$_SESSION["usucpf"]."'
					WHERE
						memid = ".$_SESSION["memid"];
			$db->executar($sql);
			$db->commit();
			
			break;
		
		case 'nao_aderir_pst':
			$sql = "UPDATE
						pdeescola.memaiseducacao
					SET
						memadesaopst = 'N',
						memdataadesaopst = now(),
						memcpfadesaopst = '".$_SESSION["usucpf"]."'
					WHERE
						memid = ".$_SESSION["memid"];
			$db->executar($sql);
			$db->commit();
			
			break;
			
		case 'desfazer_escolha_adesao':
			// retira a escolha de adesao ao pst 
			$sql = "UPDATE
						pdeescola.memaiseducacao
					SET
						memadesaopst = NULL,
						memdataadesaopst = NULL,
						memcpfadesaopst = NULL
					WHERE
						memid = ".$_SESSION["memid"];
			$db->executar($sql);
			
			// exclui as atividades
			$meaid = $db->pegaUm("SELECT meaid FROM pdeescola.meatividade WHERE memid = ".$_SESSION['memid']." AND meaano = ".$_SESSION["exercicio"]); 
			
			if($meaid) {
				$sql = "DELETE FROM pdeescola.mealunoparticipanteatividade WHERE meaid = ".$meaid;
				$db->executar($sql);
			}
			
			$sql = "DELETE FROM pdeescola.meatividade WHERE memid = ".$_SESSION['memid']." AND meaano = ".$_SESSION["exercicio"];
			$db->executar($sql);
			
			if($db->commit())
				echo "true";
			else
				echo "false";
			
			break;
	}
}

?>