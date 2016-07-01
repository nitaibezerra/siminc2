<?php
// carrega as funções gerais
include_once "config.inc";
include_once "_constantes.php";
include_once '_funcoes.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/Modelo.class.inc";
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";
require_once APPRAIZ . "tarefa/classes/Restricao.class.inc";
// atualiza ação do usuário no sistema
include APPRAIZ . "includes/registraracesso.php";

$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

class Ajax {

	public $db;

	public function __construct(){
		$this->db = new cls_banco();
	}

	public function montaPai($post = array()){
		$where = $where2 = $whereBoFilhos = $sql2 = $boFiltro = "";
		$and = "";
		if($_POST['_tartarefa']){
			$where .= " t._tartarefa = {$_POST['_tartarefa']} ";
			$where2 .= " t._tartarefa = {$_POST['_tartarefa']} ";
			$whereBoFilhos .= " and t._tartarefa = {$_POST['_tartarefa']} ";
		}

		if($_POST['arFiltros']){
			$arFiltros = explode(",", $_POST['arFiltros']);
			//ver($arFiltros,d);
			if($arFiltros[0]){
				if($where || $where2){
					$and = " and ";
				}
				//Pesquisar pelo _tartarefa, ao invés de tarid
				//Desenvolvedor: Afonso Alves
				//Data: 12/04/2010
				$where .= " $and t._tartarefa = '{$arFiltros[0]}' ";
				//$whereBoFilhos .= " and t.unaidsetororigem = '{$arFiltros[0]}' ";
				$where2 .= " $and filhas._tartarefa = '{$arFiltros[0]}' ";
				$boFiltro = true;
			}
			if($arFiltros[1]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.tartitulo ilike '%{$arFiltros[1]}%' ";
				//$whereBoFilhos .= " and t.unaidsetororigem = '{$arFiltros[0]}' ";
				$where2 .= " $and filhas.tartitulo ilike '%{$arFiltros[1]}%' ";
				$boFiltro = true;
			}
			if($arFiltros[2]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.unaidsetororigem = '{$arFiltros[2]}' ";
				$whereBoFilhos .= " and t.unaidsetororigem = '{$arFiltros[2]}' ";
				$where2 .= " $and filhas.unaidsetororigem = '{$arFiltros[2]}' ";
				$boFiltro = true;
			}
			if($arFiltros[3]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.unaidsetorresponsavel = '{$arFiltros[3]}' ";
				$whereBoFilhos .= " and t.unaidsetorresponsavel = '{$arFiltros[3]}' ";
				$where2 .= " $and filhas.unaidsetorresponsavel = '{$arFiltros[3]}' ";
				$boFiltro = true;
			}
			if($arFiltros[4]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.usucpfresponsavel = '{$arFiltros[4]}' ";
				$whereBoFilhos .= " and t.usucpfresponsavel = '{$arFiltros[4]}' ";
				$where2 .= " $and filhas.usucpfresponsavel = '{$arFiltros[4]}' ";
				$boFiltro = true;
			}
			if($arFiltros[5]){
				if($where || $where2){
					$and = " and ";
				}
					
				$where .= " $and t.tarnumsidoc = '".str_replace( "-","", str_replace(".","", str_replace("/","",$arFiltros[5] ) ) )."' ";
				$where2 .= " $and filhas.tarnumsidoc = '".str_replace( "-","", str_replace(".","", str_replace("/","",$arFiltros[5] ) ) )."' ";
				$boFiltro = true;
			}
			if($arFiltros[6]){
				if($where || $where2){
					$and = " and ";
				}
				$situacao = str_replace('.',',',$arFiltros[6]);
				$situacao = substr($situacao,0,strlen(trim($situacao))-1);
				$where .= " $and t.sitid in ({$situacao}) ";
				$whereBoFilhos .= " and t.sitid in ({$situacao}) ";
				$where2 .= " $and filhas.sitid in ({$situacao}) ";
				$boFiltro = true;
			}
			if($arFiltros[7] && $arFiltros[8]){
				if($where || $where2){
					$and = " and ";
				}
				
				$where .= " $and t.tardataprazoatendimento between to_date('$arFiltros[7]','dd/mm/yyyy') and to_date('$arFiltros[8]','dd/mm/yyyy')";
				$where2 .= " $and filhas.tardataprazoatendimento between to_date('$arFiltros[7]','dd/mm/yyyy') and to_date('$arFiltros[8]','dd/mm/yyyy')";
				$boFiltro = true;
			} else if($arFiltros[7] && !$arFiltros[8]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.tardataprazoatendimento >= to_date('$arFiltros[7]','dd/mm/yyyy')";
				$where2 .= " $and filhas.tardataprazoatendimento >= to_date('$arFiltros[7]','dd/mm/yyyy')";
				$boFiltro = true;
			} else if($arFiltros[8] && !$arFiltros[7]){
				if($where || $where2){
					$and = " and ";
				}
				$where .= " $and t.tardataprazoatendimento <= to_date('$arFiltros[8]','dd/mm/yyyy')";
				$where2 .= " $and filhas.tardataprazoatendimento <= to_date('$arFiltros[8]','dd/mm/yyyy')";
				$boFiltro = true;
			}

			if($boFiltro){
				if(substr($where2,0,5) != '  and'){
					$where2 = " and $where2";
				}
				$sql2 = " and ($where or exists (Select 1 From tarefa.tarefa filhas Where filhas._tartarefa = t.tarid $where2 ))";
			}
		} elseif($where){
			$sql2 = " and $where ";
		}
		
		$sql = "SELECT t.tarid,
					t.tartitulo, 
					t._tarpai, 
					t._tartarefa, 
					to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') as tardataprazoatendimento,
					t.taraberto,
					t.usucpfresponsavel,
					case when u.usunome is null then 'Usuário Indefinido'
					else u.usunome
					end as nome,
					tu.unasigla as setorrespon,
					t.tardepexterna,
					t.tarprioridade as prioridade
			FROM tarefa.tarefa t
				left join seguranca.usuario u on t.usucpfresponsavel = u.usucpf			
				inner join tarefa.unidade tu on t.unaidsetorresponsavel = tu.unaid
			where 
				t._tarpai is null
				$sql2
			order by t._tarordem";
				$arDados = $this->db->carregar($sql);
					
				if(is_array($arDados) && $arDados[0] != "" ){
					$itens = array();
					foreach($arDados as $dados){
						//$img = $dados["taraberto"] == 't' ?  "menos.gif" : "mais.gif";
						$img = "mais.gif";
						/**
						 * Verifica se tem Filho
						 */
						$sql = "SELECT t.tarid FROM tarefa.tarefa t
										where t._tartarefa = {$dados['_tartarefa']} 
										and t._tarpai = {$dados['tarid']}
										order by t.tarid";
						$boFilho = $this->db->pegaUm($sql);
							
						/**
						 * Verifica se tem Anexo
						 */
						$sql = "SELECT arqid FROM tarefa.anexo
										where tarid = {$dados['tarid']} and arqid is not null";
						$boAnexo = $this->db->pegaUm($sql);

						/**
						 * Verifica se tem Restrição
						 */
						$sql = "SELECT resid FROM tarefa.restricao
										where tarid = {$dados['tarid']} and ressolucao = false";
						$boRestricao = $this->db->pegaUm($sql);

						/**
						 * Carrega Barra Situação e Data Prazo
						 */
						$barraExecucao = self::carregaBarraSituacao($dados);
						$dataPrazo     = self::carregaPrazoAtendimento($dados);

						/*
						 * BLOCO PARA FORMA CÓDIGO DA TAREFA
						 */
						$codTarefa = '';
						$arCodTarefa = array();
						self::formaCodTarefa( $dados['_tarordem'], $arCodTarefa );
						$arCodTarefa[0] = $dados['_tartarefa'];
						$codTarefa = implode(".", $arCodTarefa);

						$itens[] =
				  	  '{ \'tarpai\':\''     			. $dados['_tarpai'] . '\','
			  	  	. ' \'tarid\':\''     				. $dados['tarid'] . '\','
				  	  . ' \'tartarefa\':\'' 			. $dados['_tartarefa'] . '\','
				  	  . ' \'boFilho\':\''   			. $boFilho . '\','
				  	  . ' \'boAnexo\':\''   			. $boAnexo . '\','
				  	  . ' \'boRestricao\':\''   		. $boRestricao . '\','
				  	  . ' \'img\':\''   				. $img . '\','
				  	  . ' \'taraberto\':\'' 			. $dados['taraberto'] . '\','
				  	  . ' \'tardataprazoatendimento\':\''. $dados['tardataprazoatendimento'] . '\','
				  	  . ' \'nome\':\''					. $dados['nome'] . '\','
				  	  . ' \'barraExecucao\':\''			. $barraExecucao . '\','
				  	  . ' \'dataPrazo\':\''				. $dataPrazo . '\','
				  	  . ' \'setorrespon\':\''			. $dados['setorrespon'] . '\','
				  	  . ' \'tardepexterna\':\''			. trim($dados['tardepexterna']) . '\','
				  	  . ' \'codTarefa\':\''				. $codTarefa . '\','
				  	  . ' \'prioridade\':\''			. $dados['prioridade'] . '\','
				  	  . ' \'tartitulo\': "' 			. $dados['tartitulo']   . '"}';
					}
					header('content-type: application/json;charset=iso-8859-1');
					echo 'var arDados=[' , implode(',', $itens) , '];';
				} else {
					echo '';
				}
				die;
	}

	public function montaArvoreAberta($post = array()){
		$where = $where2 = $whereBoFilhos = $sql2 = $boFiltro = "";
		$and = "";
		if($_POST['_tartarefa']){
			$where .= " t._tartarefa = {$_POST['_tartarefa']} ";
			$where2 .= " t._tartarefa = {$_POST['_tartarefa']} ";
			$whereBoFilhos .= " and t._tartarefa = {$_POST['_tartarefa']} ";
		}

		/*if($_POST['arFiltros']){
			$arFiltros = explode(",", $_POST['arFiltros']);
			if($arFiltros[0]){
			if($where || $where2){
			$and = " and ";
			}
			$where .= " $and t.unaidsetororigem = '{$arFiltros[0]}' ";
			$whereBoFilhos .= " and t.unaidsetororigem = '{$arFiltros[0]}' ";
			$where2 .= " $and filhas.unaidsetororigem = '{$arFiltros[0]}' ";
			$boFiltro = true;
			}
			if($arFiltros[1]){
			if($where || $where2){
			$and = " and ";
			}
			$where .= " $and t.unaidsetorresponsavel = '{$arFiltros[1]}' ";
			$whereBoFilhos .= " and t.unaidsetorresponsavel = '{$arFiltros[1]}' ";
			$where2 .= " $and filhas.unaidsetorresponsavel = '{$arFiltros[1]}' ";
			$boFiltro = true;
			}
			if($arFiltros[2]){
			if($where || $where2){
			$and = " and ";
			}
			$where .= " $and t.usucpfresponsavel = '{$arFiltros[2]}' ";
			$whereBoFilhos .= " and t.usucpfresponsavel = '{$arFiltros[2]}' ";
			$where2 .= " $and filhas.usucpfresponsavel = '{$arFiltros[2]}' ";
			$boFiltro = true;
			}
			if($arFiltros[3]){
			if($where || $where2){
			$and = " and ";
			}
			$situacao = str_replace('.',',',$arFiltros[3]);
			$situacao = substr($situacao,0,strlen($situacao)-1);
			$where .= " $and t.sitid in ({$situacao}) ";
			$whereBoFilhos .= " and t.sitid in ({$situacao}) ";
			$where2 .= " $and filhas.sitid in ({$situacao}) ";
			$boFiltro = true;
			}
			if($boFiltro){
			if(substr($where2,0,5) != '  and'){
			$where2 = " and $where2";
			}
			$sql2 = " ($where or exists (Select 1 From tarefa.tarefa filhas Where filhas._tartarefa = t.tarid $where2 ))";
			}
			} elseif($where){
			$sql2 = " $where ";
			}*/

		$sql = "SELECT t.tarid,
					t.tartitulo, 
					t._tarpai, 
					t._tartarefa, 
					to_char(t.tardataprazoatendimento, 'DD/MM/YYYY'), 
					t.taraberto,
					t.usucpfresponsavel,
					case when u.usunome is null then 'Usuário Indefinido'
						else u.usunome
					end as nome,
					t._tarordem,
					tu.unasigla as setorrespon,
					t.tardepexterna,
					t.tarprioridade as prioridade
			FROM tarefa.tarefa t
				left join seguranca.usuario u on t.usucpfresponsavel = u.usucpf
				inner join tarefa.unidade tu on t.unaidsetorresponsavel = tu.unaid
			where 
			$where
			order by t._tarordem";
			$arDados = $this->db->carregar($sql);

			$count = 0;
			$count2 = 0;
			if(is_array($arDados) && $arDados[0] != "" ){
				$itens = array();
				$boCima = $boBaixo = false;
				foreach($arDados as $dados){
					//$img = $dados["taraberto"] == 't' ?  "menos.gif" : "mais.gif";
					$img = "menos.gif";
					/**
					 * Verifica se tem Filho
					 */
					$sql = "SELECT t.tarid FROM tarefa.tarefa t
										where t._tartarefa = {$dados['_tartarefa']} 
										and t._tarpai = {$dados['tarid']}
										order by t.tarid";
					$boFilho = $this->db->pegaUm($sql);

					/**
					 * Verifica se tem Anexo
					 */
					$sql = "SELECT arqid FROM tarefa.anexo
										where tarid = {$dados['tarid']} and arqid is not null";
					$boAnexo = $this->db->pegaUm($sql);

					/**
					 * Verifica se tem Restrição
					 */
					$sql = "SELECT resid FROM tarefa.restricao
										where tarid = {$dados['tarid']} and ressolucao = false";
					$boRestricao = $this->db->pegaUm($sql);

					/**
					 * Carrega Barra Situação e Data Prazo
					 */
					$barraExecucao = self::carregaBarraSituacao($dados);
					$dataPrazo     = self::carregaPrazoAtendimento($dados);

					$sqlTemp = "SELECT t.tarid, t._tarpai, t._tarordem FROM tarefa.tarefa t where $where order by t._tarordem";
					$arNivel = $this->db->carregar($sqlTemp);
					$arNivel = ($arNivel) ? $arNivel : array();
					$i = 0;
					$boCima = $boBaixo = false;
					$nivelCorrente = (strlen($dados['_tarordem']) / 4);

					/*
					 * BLOCO PARA FORMA CÓDIGO DA TAREFA
					 */
					$codTarefa = '';
					$arCodTarefa = array();
					self::formaCodTarefa( $dados['_tarordem'], $arCodTarefa );
					$arCodTarefa[0] = $dados['_tartarefa'];
					$codTarefa = implode(".", $arCodTarefa);

					/**
					 * BLOCO PARA VERIFICA SE PODE MUDAR ORDEM
					 */
					foreach($arNivel as $nivel){
						$tarnivel = (strlen($nivel['_tarordem']) / 4);
						if($dados['tarid'] != $nivel['tarid']){
							if($dados['_tarpai'] == $nivel['_tarpai']  &&  $nivelCorrente == $tarnivel){
								if($i < $count){
									$boCima = true;
								}
								if($i > $count){
									$boBaixo = true;
								}
							}
						}
						$i++;
					}

					$itens[] =
				  	  '{ \'tarpai\':\''     				. $dados['_tarpai'] . '\','
				  	  . ' \'tarid\':\''     				. $dados['tarid'] . '\','
				  	  . ' \'tartarefa\':\'' 				. $dados['_tartarefa'] . '\','
				  	  . ' \'boFilho\':\''   				. $boFilho . '\','
				  	  . ' \'boAnexo\':\''   				. $boAnexo . '\','
				  	  . ' \'boRestricao\':\''   			. $boRestricao . '\','
				  	  . ' \'img\':\''   					. $img . '\','
				  	  . ' \'taraberto\':\''     			. $dados['taraberto'] . '\','
				  	  . ' \'tardataprazoatendimento\':\''	. $dados['tardataprazoatendimento'] . '\','
				  	  . ' \'nome\':\''						. $dados['nome'] . '\','
				  	  . ' \'barraExecucao\':\''				. $barraExecucao . '\','
				  	  . ' \'dataPrazo\':\''					. $dataPrazo . '\','
				  	  . ' \'tarordem\':\''					. $dados['_tarordem'] . '\','
				  	  . ' \'boCima\':\''					. $boCima . '\','
				  	  . ' \'boBaixo\':\''					. $boBaixo . '\','
				  	  . ' \'setorrespon\':\''               . $dados['setorrespon'] . '\','
				  	  . ' \'codTarefa\':\''                 . $codTarefa . '\','
				  	  . ' \'tardepexterna\':\''			    . trim($dados['tardepexterna']) . '\','
				  	  . ' \'prioridade\':\''				. $dados['prioridade'] . '\','
				  	  . ' \'tartitulo\': "' 				. $dados['tartitulo']   . '"}';
				  	  $count++;
				}
				header('content-type: application/json;charset=iso-8859-1');
				echo 'var arDados=[' , implode(',', $itens) , '];';
			} else {
				echo '';
			}
			die;
	}

	/**
	 * FUNÇÃO PARA FORMAR CÓDIGO DA TAREFA
	 *
	 * @param string $_tarordem
	 * @param array $arCodTarefa
	 * @return $arCodTarefa
	 */
	private function formaCodTarefa( $tarordem, &$arCodTarefa ){
		if( strlen( $tarordem ) < 5 ){
			$blocoCod = substr( $tarordem, 0, 4 );
			return $arCodTarefa[] = intval($blocoCod);
		}

		$blocoCod = substr( $tarordem, 0, 4 );
		$arCodTarefa[] = intval($blocoCod);
		return self::formaCodTarefa( substr( $tarordem, 4 ), $arCodTarefa );
	}

	public function montaFilhos($post = array()){
		$tarid 		= $_POST['tarid'];
		$tarpai 	= $_POST['tarpai'];
		$tartarefa  = $_POST['tartarefa'];
		$trId 		= $_POST['trId'];
		$where = "";
		/*if($_POST['arFiltros']){
			$arFiltros = explode(",", $_POST['arFiltros']);
			if($arFiltros[0]){
			$where .= " and t.unaidsetororigem = '{$arFiltros[0]}' ";
			}
			if($arFiltros[1]){
			$where .= " and t.unaidsetorresponsavel = '{$arFiltros[1]}' ";
			}
			if($arFiltros[2]){
			$where .= " and t.usucpfresponsavel = '{$arFiltros[2]}' ";
			}
			if($arFiltros[3]){
			$situacao = str_replace('.',',',$arFiltros[3]);
			$situacao = substr($situacao,0,strlen($situacao)-1);
			$where .= " and t.sitid in ({$situacao}) ";
			}
			}*/
		$sql = "SELECT  t.tarid,
						t.tartitulo, 
						t._tarpai, 
						t._tartarefa, 
						to_char(t.tardataprazoatendimento, 'DD/MM/YYYY'), 
						t.taraberto,
						t.usucpfresponsavel,
						case when u.usunome is null then 'Usuário Indefinido'
						else u.usunome
						end as nome,
						tu.unasigla as setorrespon,
						t._tarordem,
						t.tardepexterna,
						t.tarprioridade as prioridade
				FROM tarefa.tarefa t
					left join seguranca.usuario u on t.usucpfresponsavel = u.usucpf
					inner join tarefa.unidade tu on t.unaidsetorresponsavel = tu.unaid
				where t._tartarefa = {$tartarefa} and t._tarpai = {$tarid} 
				$where
				order by t._tarordem ";
				$arDados = $this->db->carregar($sql);
				if(is_array($arDados) && $arDados[0] != "" ){
					$itens = array();
					foreach($arDados as $dados){
						//$img = $dados["taraberto"] == 't' ?  "menos.gif" : "mais.gif";
						$img = "mais.gif";
						/**
						 * Verifica se tem Filho
						 */
						$boFilho = $this->db->pegaUm("SELECT t.tarid
											FROM tarefa.tarefa t
										where t._tartarefa = {$tartarefa} 
										and t._tarpai = {$dados['tarid']} 
										order by t.tarid");

						/**
						 * Verifica se tem Anexo
						 */
						$sql = "SELECT arqid FROM tarefa.anexo
										where tarid = {$dados['tarid']} and arqid is not null";
						$boAnexo = $this->db->pegaUm($sql);

						/**
						 * Verifica se tem Restrição
						 */
						$sql = "SELECT resid FROM tarefa.restricao
										where tarid = {$dados['tarid']} and ressolucao = false";
						$boRestricao = $this->db->pegaUm($sql);

						/**
						 * Carrega Barra Situação e Data Prazo
						 */
						$barraExecucao = self::carregaBarraSituacao($dados);
						$dataPrazo     = self::carregaPrazoAtendimento($dados);

						/*
						 * BLOCO PARA FORMA CÓDIGO DA TAREFA
						 */
						$codTarefa = '';
						$arCodTarefa = array();
						self::formaCodTarefa( $dados['_tarordem'], $arCodTarefa );
						$arCodTarefa[0] = $dados['_tartarefa'];
						$codTarefa = implode(".", $arCodTarefa);

						$itens[] =
				  	  '{ \'tarpai\':\''     			  . $tarid . '\','
				  	  . ' \'tarid\':\''     			  . $dados['tarid'] . '\','
				  	  . ' \'tartarefa\':\''				  . $tartarefa . '\','
				  	  . ' \'boFilho\':\''   			  . $boFilho . '\','
				  	  . ' \'boAnexo\':\''   			  . $boAnexo . '\','
				  	  . ' \'boRestricao\':\''   		  . $boRestricao . '\','
				  	  . ' \'trId\':\''   				  . $trId . '\','
				  	  . ' \'img\':\''   				  . $img . '\','
				  	  . ' \'taraberto\':\''     		  . $dados['taraberto'] . '\','
				  	  . ' \'tardataprazoatendimento\':\'' . $dados['tardataprazoatendimento'] . '\','
				  	  . ' \'nome\':\''					  . $dados['nome'] . '\','
				  	  . ' \'barraExecucao\':\''			  . $barraExecucao . '\','
				  	  . ' \'dataPrazo\':\''				  . $dataPrazo . '\','
				  	  . ' \'setorrespon\':\''             . $dados['setorrespon'] . '\','
				  	  . ' \'codTarefa\':\''               . $codTarefa . '\','
				  	  . ' \'tardepexterna\':\''			  . trim($dados['tardepexterna']) . '\','
				  	  . ' \'prioridade\':\''			  . $dados['prioridade'] . '\','
				  	  . ' \'tartitulo\': "' 			  . $dados['tartitulo']   . '"}';
					}
					header('content-type: application/json;charset=iso-8859-1');
					echo 'var arDados=[' , implode(',', $itens) , '];';
				} else {
					echo '';
				}
				die;
	}


	private function carregaBarraSituacao($post = array()){
		$tarid = $_POST['tarid'];

		$item = "";
		$situacao = $this->db->carregar("SELECT d.tarid, sd.sitid, sd.sitdsc, d.tarporcentoexec FROM tarefa.tarefa d
									inner join tarefa.situacaotarefa sd on d.sitid = sd.sitid
									where d.tarid = {$tarid}");

		switch($situacao[0]["sitid"]){
			// Não iniciado
			case 1:
				$cor_texto = '#909090';
				$cor_barra = '#bbbbbb';
				$cor_sombra = '#efefef';
				break;
				// Em andamento
			case 2:
				$cor_texto = '#209020';
				$cor_barra = '#339933';
				$cor_sombra = '#dcffdc';
				break;
				// Suspenso
			case 3:
				$cor_texto = '#aa9020';
				$cor_barra = '#bba131';
				$cor_sombra = '#feffbf';
				break;
				// Cancelado
			case 4:
				$cor_texto = '#aa2020';
				$cor_barra = '#cc3333';
				$cor_sombra = '#ffe7e7';
				break;
				// Concluído
			case 5:
				$cor_texto = '#2020aa';
				$cor_barra = '#3333cc';
				$cor_sombra = '#d4e7ff';
				break;
		}

		$retorno = sprintf(
			'<span style="color:%s; font-size:10px;">%s</span>' .
			'<div style="text-align:left; margin-left:5px; padding:1px 0 1px 0; height:6px; max-height:6px; width:75px; border:1px solid #888888; background-color:%s;" title="%d%%">' .
				'<div style="font-size:4px; width:%d%%; height:6px; max-height:6px; background-color:%s;">' .
				'</div>'.
			'</div>',
		$cor_texto,
		$situacao[0]["sitdsc"],
		$cor_sombra,
		$situacao[0]["tarporcentoexec"],
		$situacao[0]["tarporcentoexec"],
		$cor_barra
		);

		$retorno .= '@@'.$situacao[0]["sitid"].'@@'.$situacao[0]["tarporcentoexec"];
		//echo $retorno;
		return $retorno;
	}

	public function atualiza_barra_status($post = array()){
		$tarid = $_POST['id'];
		# pega situação anterior
		$sitidAnterior = $this->db->pegaUm("SELECT sitid FROM tarefa.tarefa WHERE tarid = {$tarid}");
		# altera situação para situação escolhida
		$this->db->executar("UPDATE
						tarefa.tarefa
					   SET
					   	tarporcentoexec = ".trim($_POST['percentual']).",
					   	sitid = ".trim($_POST['codstatus'])."
					   WHERE
					   	tarid = {$tarid}");
		$obTarefa = new Tarefa($tarid);
		$_POST['acodsc'] 		= $_SESSION['tarefa']['mensagem'];
		$_POST['sitidAnterior'] = $sitidAnterior;
		$_POST['sitid'] 		= trim($_POST['codstatus']);
		# salvamos a mensagem
		$obTarefa->salvarAcompanhamentoPelaArvore($_POST, 'situacao');
		$retorno = $this->db->commit();
		unset($obTarefa);
		echo $retorno;
		die;
	}

	private function carregaPrazoAtendimento($post = array()){
		$tardataprazoatendimento = $this->db->pegaUm("SELECT
								tardataprazoatendimento
							  FROM 
							  	tarefa.tarefa m
							  WHERE
							  	tarid = {$_POST['tarid']}");

		if(($tardataprazoatendimento)){
			$tardataprazoatendimento = strftime("%d/%m/%Y",strtotime($tardataprazoatendimento));
		} else {
			$tardataprazoatendimento = "";
		}

		return $tardataprazoatendimento;
		die;
	}

	public function atualiza_data($post = array()){
		$tarid = $_POST['tarid'];
		$obData = new Data();

		# pega data prazo anterior
		$tardataprazoatendimentoAnterior = $this->db->pegaUm("SELECT ".trim($_POST['data_alterada'])." FROM tarefa.tarefa WHERE tarid = {$tarid}");
		$tardataprazoatendimentoAnterior = $obData->formataData($tardataprazoatendimentoAnterior,"dd/mm/YYYY");

		# altera data prazo para data prazo escolhida
		$tardataprazoatendimento = $obData->formataData($_POST['nova_data'],"YYYY-mm-dd");
		$this->db->executar("UPDATE
						tarefa.tarefa
					   SET
					   	".trim($_POST['data_alterada'])." = '".trim($tardataprazoatendimento)."'
					   WHERE
					   	tarid = {$tarid}");
			
		$obTarefa = new Tarefa($tarid);
		$tardataprazoatendimento = $obData->formataData($_POST['nova_data'],"dd/mm/YYYY");
		$_POST['acodsc'] 						  = $_SESSION['tarefa']['mensagem'];
		$_POST['tardataprazoatendimentoAnterior'] = $tardataprazoatendimentoAnterior;
		$_POST['tardataprazoatendimento'] 		  = $tardataprazoatendimento;
		# salvamos a mensagem
		$obTarefa->salvarAcompanhamentoPelaArvore($_POST, 'prazo');
			
		if($_SESSION['tarefa']['boEnviaEmailRespon'] && $obTarefa->usucpfresponsavel){
			$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);
			$email = $obTarefa->recuperaEmailPorCpf($obTarefa->usucpfresponsavel);
			enviarEmailTarefa($obTarefa->usucpfresponsavel, $email, $arAcompanhamento, $obTarefa->tartitulo, $obTarefa->tarid);
		}
			
		$retorno = $this->db->commit();
		echo $retorno;
		die;
	}

	public function excluir_solicitante($post = array()){
		unset($_SESSION['arSolicitante'][0][$_POST['solid']]);
		die;
	}

	public function novaTarefa($post = array()){
		unset($_SESSION['arSolicitante']);
		unset($_SESSION['tarid']);
		unset($_SESSION['_tartarefa']);
		unset($_SESSION['dados_tarefa']);
		unset($_SESSION['cadTarefa']);
		unset($obTarefa);
		unset($acodsc);
		die;
	}

	public function mudaPosicaoAjax($post = array()){
		extract($_POST);
		$ordem1 = $this->db->pegaUm("SELECT _tarordem FROM tarefa.tarefa WHERE tarid = {$tarid1}");
		$ordem2 = $this->db->pegaUm("SELECT _tarordem FROM tarefa.tarefa WHERE tarid = {$tarid2}");
		if($ordem1 && $ordem2){
			$this->db->executar("UPDATE
							tarefa.tarefa
						   SET
						   	_tarordem = '$ordem1'
						   WHERE
						   	tarid = {$tarid2}");

			$this->db->executar("UPDATE
							tarefa.tarefa
						   SET
						   	_tarordem = '$ordem2'
						   WHERE
						   	tarid = {$tarid1}");

		}
		/*
		 * Verifica se o primeiro registro alterado tem filhos, se tiver altera a ordem dos filhos
		 */
		$sql = "SELECT tarid,_tarordem FROM tarefa.tarefa WHERE _tarpai = $tarid1";
		$arFilhos = $this->db->carregar($sql);
		$arFilhos = ($arFilhos) ? $arFilhos : array();
		foreach($arFilhos as $filhos){
			$tarid = $filhos['tarid'];
			$ordemCorrente = $filhos['_tarordem'];

			$tamanhoPai   = strlen($ordem2);
			$tamanhoFilho = strlen($ordemCorrente);
			$ordemPt1     = substr($ordemCorrente,0,$tamanhoPai);
			$ordemPt2     = substr($ordemCorrente,$tamanhoPai,$tamanhoFilho);

			$ordemFilho = $ordem2.$ordemPt2;

			$this->db->executar("UPDATE
							tarefa.tarefa
						   SET
						   	_tarordem = '$ordemFilho'
						   WHERE
						   	tarid = $tarid ");
			self::mudaOrdemFilhos($tarid,$ordemFilho);
		}
		/*
		 * Verifica se o segundo registro alterado tem filhos, se tiver altera a ordem dos filhos
		 */
		$sql = "SELECT tarid,_tarordem FROM tarefa.tarefa WHERE _tarpai = $tarid2";
		$arFilhos = $this->db->carregar($sql);
		$arFilhos = ($arFilhos) ? $arFilhos : array();
		foreach($arFilhos as $filhos){
			$tarid = $filhos['tarid'];
			$ordemCorrente = $filhos['_tarordem'];

			$tamanhoPai   = strlen($ordem1);
			$tamanhoFilho = strlen($ordemCorrente);
			$ordemPt1     = substr($ordemCorrente,0,$tamanhoPai);
			$ordemPt2     = substr($ordemCorrente,$tamanhoPai,$tamanhoFilho);

			$ordemFilho = $ordem1.$ordemPt2;

			$this->db->executar("UPDATE
							tarefa.tarefa
						   SET
						   	_tarordem = '$ordemFilho'
						   WHERE
						   	tarid = $tarid ");
			self::mudaOrdemFilhos($tarid,$ordemFilho);
		}
		$this->db->commit();
		die;
	}

	/*
	 * Funçao chamada para mudar ordem dos filhos recursivamente
	 */
	private function mudaOrdemFilhos($tarid, $ordem){
		$sql = "SELECT tarid,_tarordem FROM tarefa.tarefa WHERE _tarpai = $tarid";
		$arFilhos = $this->db->carregar($sql);
		$arFilhos = ($arFilhos) ? $arFilhos : array();
		foreach($arFilhos as $filhos){
			$tarid = $filhos['tarid'];
			$ordemCorrente = $filhos['_tarordem'];

			$tamanhoPai   = strlen($ordem);
			$tamanhoFilho = strlen($filhos['_tarordem']);
			$ordemPt1     = substr($filhos['_tarordem'],0,$tamanhoPai);
			$ordemPt2     = substr($filhos['_tarordem'],$tamanhoPai,$tamanhoFilho);

			$ordemFilho = $ordem.$ordemPt2;

			$this->db->executar("UPDATE
						tarefa.tarefa
					   SET
					   	_tarordem = '$ordemFilho'
					   WHERE
					   	tarid = $tarid ");
			self::mudaOrdemFilhos($tarid,$ordemCorrente);
		}
	}

	public function mudatarAberto($post = array()){
		$this->db->executar("UPDATE
						tarefa.tarefa
					   SET
					   	taraberto = {$_POST['tarAberto']}
					   WHERE
					   	tarid = {$_POST['tarid']}");
		$this->db->commit();
		die;
	}

	public function addSolicitante($post = array()){
		header('content-type: text/html; charset=UTF-8');
		$_SESSION['cadTarefa'] = $_POST;
		die;
	}

	public function unaidSetorResp($post = array()){
		if($_SESSION['tarefa']['boPerfilSuperUsuario']){
			$habilitado = 'S';
			header('content-type: text/html; charset=ISO-8859-1');
			$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
							from tarefa.usuarioresponsabilidade ur
	   						inner join seguranca.usuario u on ur.usucpf = u.usucpf
	   						where ur.rpustatus = 'A' ";			
		} else {
			$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
			header('content-type: text/html; charset=ISO-8859-1');
			$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
							from tarefa.usuarioresponsabilidade ur
	   						inner join seguranca.usuario u on ur.usucpf = u.usucpf
	   						where ur.unaid = {$_POST['unaid']} and ur.rpustatus = 'A' ";
			if(!$_POST['unaid']){
				$sql = array();
			}
		}
		if(isset($_POST['boFiltro']) && $_POST['boFiltro']){
			die($this->db->monta_combo('filtrousucpfresponsavel', $sql, $habilitado, "Selecione um Setor Responsável", '', '', '', '405', 'N', 'filtrousucpfresponsavel',false,null,'Responsável pela Tarefa'));
		} else {
			die($this->db->monta_combo('usucpfresponsavel', $sql, $habilitado, "Selecione...", '', '', '', '400', 'N', 'usucpfresponsavel',false,null,'Responsável pela Tarefa'));
		}
	}

	public function recuperaResponsavelPorTarid($post = array()){
		if($_SESSION['tarefa']['boPerfilSuperUsuario']){
			$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
						from tarefa.usuarioresponsabilidade ur
   						inner join seguranca.usuario u on ur.usucpf = u.usucpf
   						where ur.rpustatus = 'A' ";
		} else {
			$sql = "select unaidsetororigem, unaidsetorresponsavel from tarefa.tarefa where tarid = {$_POST['tarid']} ";
			$arSetores = $this->db->carregar($sql);
			$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
							from tarefa.usuarioresponsabilidade ur
	   						inner join seguranca.usuario u on ur.usucpf = u.usucpf
	   						where ur.unaid = {$arSetores[0]['unaidsetorresponsavel']} and ur.rpustatus = 'A' ";
		}
		header('content-type: text/html; charset=ISO-8859-1');
		die($this->db->monta_combo('usucpfresponsavelArvore', $sql, $habilitado, "Selecione...", 'aposAlterarResponsavel', '', '', '200', 'N', 'usucpfresponsavelArvore',false,null,'Responsável pela Tarefa'));
	}

	public function atualizaResponsavel($dados = array()){
		# Feito por Alexandre Dourado
		if(!$dados['tarid']) {
			echo "Tarefa não encontrada. <a href='tarefa.php?modulo=principal/listaTarefas&acao=A'>Clique aqui e refaça o procedimento.</a>";
			exit;
		}
		# pega usuário responsável anterior
		$usucpfresponsavelAnterior = $this->db->pegaUm("SELECT usucpfresponsavel FROM tarefa.tarefa WHERE tarid = {$dados['tarid']}");
		# altera usuário para usuário escolhido
		$this->db->executar("UPDATE tarefa.tarefa SET usucpfresponsavel = '{$dados['usucpfresponsavel']}' WHERE tarid = {$dados['tarid']}");
		$obTarefa = new Tarefa($dados['tarid']);
		$dados['acodsc'] 					= $_SESSION['tarefa']['mensagem'];
		$dados['usucpfresponsavelAnterior'] = $usucpfresponsavelAnterior;
		$dados['usucpfresponsavel'] 		= $dados['usucpfresponsavel'];
		# salvamos a mensagem
		$obTarefa->salvarAcompanhamentoPelaArvore($dados, 'respon', false);
		$this->db->commit();
					
		//if($_SESSION['tarefa']['boEnviaEmailRespon']){
//		$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);
//		$email = $obTarefa->recuperaEmailPorCpf($usucpfresponsavel);
//		enviarEmailTarefa($usucpfresponsavel, $email, $arAcompanhamento, $obTarefa->tartitulo, $obTarefa->tarid);
		//}
//		unset($_SESSION['tarefa']['boEnviaEmailRespon']);
		
		if($dados['usucpfresponsavel']){
			$usucpfresponsavel = $dados['usucpfresponsavel'];
			$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);	
			$email = $obTarefa->recuperaEmailPorCpf($usucpfresponsavel);
			$nrtarefa = $obTarefa->pegaAtividade($obTarefa->tarid);
			enviarEmailTarefa($usucpfresponsavel,$email, $arAcompanhamento, $obTarefa->tartitulo, $nrtarefa );
		}

		header('content-type: application/json;charset=iso-8859-1');
		echo $this->db->pegaUm("SELECT
								case when u.usunome is null then tu.unasigla || ' - Usuário Indefinido'
								else tu.unasigla || ' - ' || u.usunome
								end as nome
						  FROM tarefa.tarefa t
  						   		inner join seguranca.usuario u on t.usucpfresponsavel = u.usucpf
								inner join tarefa.unidade tu on t.unaidsetorresponsavel = tu.unaid
		 				  WHERE t.tarid = {$dados['tarid']}");							   	
		die;
	}

	public function carregaCabecalho($post = array()){
		header('content-type: text/html; charset=ISO-8859-1');
		echo cabecalhoTarefa($_POST['tarid'], $this->db);
		die;
	}

	public function carregaBlocoAtendimento($post = array()){
		header('content-type: text/html; charset=ISO-8859-1');
		$obTarefa = new Tarefa($_POST['tarid']);
		echo blocoDadosAtendimento($obTarefa, 'S', true, false, '', array(), false, $this->db);
		die;
	}

	public function carregaListaAtendimento($post = array()){
		header('content-type: text/html; charset=ISO-8859-1');
		echo listaAtendimento($_POST['tarid'], $this->db);
		die;
	}

	public function montaArvore($post = array()){
		header('content-type: text/html; charset=ISO-8859-1');
		echo montarArvore(null,true);
		die;
	}

	public function recuperaSetorOrigemSetorResponCpfResponPorTarid($post = array()){
		$sql = "SELECT unaidsetororigem, unaidsetorresponsavel, usucpfresponsavel FROM tarefa.tarefa WHERE tarid = {$_POST['tarid']}";
		$arDados = $this->db->pegaLinha($sql);
		/*$arDados['espkit'] = iconv("iso-8859-1","utf-8",$arDados['espkit']);
		 $arDados['espnome'] = iconv("iso-8859-1","utf-8",$arDados['espnome']);
		 $arDados['espunidademedida'] = iconv("iso-8859-1","utf-8",$arDados['espunidademedida']);*/
		echo simec_json_encode( $arDados );
		die;
	}

	public function gravaMensagemSessao($post = array()){
		$_SESSION['tarefa']['mensagem']           = $_POST['mensagem'];
		if($_POST['boEnviarEmailRespon']){
			$_SESSION['tarefa']['boEnviaEmailRespon'] = $_POST['boEnviarEmailRespon'];
		}
		die;
	}

	/**
	 * Verifica se data prazo passada é maior que a data pai ese existe data filha com a data maior que a data pai (recursivamente)
	 *
	 * @param unknown_type $_POST
	 */
	public function verificaDataPaiEDataFilha($dados = array()){
		# PEGAMOS A TAREFA ATUAL

		/*
		 * CORREÇÃO DE ERROS
		 * FEITO POR ALEXANDRE DOURADO  16/11/2009
		 */
		if(!$dados['tarid']) {
			$arDados['boAtividade'] = "";
			$arDados['boDataFilhaMaior'] = "";
			$arDados['dataprazoAnterior'] = "";
			echo simec_json_encode( $arDados );
			exit;
		}

		$sql = "SELECT _tarpai,
						to_char(tardataprazoatendimento, 'DD/MM/YYYY') as tardataprazoatendimento
			    FROM tarefa.tarefa WHERE tarid = {$dados['tarid']}";
		$arDadosObjPassado = $this->db->pegaLinha($sql);

		$taridPai = $arDadosObjPassado['_tarpai'];
		$dataprazoAnterior = $arDadosObjPassado['tardataprazoatendimento'];

		if(!$taridPai){
			$taridPai = $dados['tarid'];
		}

		# RECUPERAMOS A DATA DA TAREFA PAI
		$sql = "SELECT tarid,
					   to_char(tardataprazoatendimento, 'DD/MM/YYYY') as tardataprazoatendimento
				FROM tarefa.tarefa WHERE tarid = {$taridPai}";
		$arDados = $this->db->pegaLinha($sql);

		# VERIFICAMOS SE É TAREFA OU ATIVIDADE
		$obTarefa = new Tarefa($dados['tarid']);
		$arDados['boAtividade'] = $obTarefa->boAtividade();

		$arDtMaior = array();
		self::temDataFilhaMaior($dados, $arDtMaior);

		$boDataFilhaMaior = false;
		if(is_array($arDtMaior) && count($arDtMaior)){
			$boDataFilhaMaior = true;
		}
		$arDados['boDataFilhaMaior'] = $boDataFilhaMaior;

		$arDados['dataprazoAnterior'] = $dataprazoAnterior;

		echo simec_json_encode( $arDados );
		die;
	}

	/*
	 * VERFICA SE TEM DATA FILHA MAIOR QUE A DATA PASSADA.
	 */
	private function temDataFilhaMaior($post, &$arDtMaior){
		$arDados = $this->db->carregar("select tarid,
											   to_char(tardataprazoatendimento, 'DD/MM/YYYY') as tardataprazoatendimento from tarefa.tarefa where _tarpai = {$_POST['tarid']} ");

		$obData = new Data();
		if($arDados){
			foreach($arDados as $dados){
				if(!$obData->diferencaEntreDatas($_POST['dataPrazoAtual'], $dados['tardataprazoatendimento'], 'maiorDataBolean', null, 'dd/mm/yyyy')){
					$arDtMaior[] = 1;
					break;
				} else {
					$_POST['tarid'] = $dados['tarid'];
					self::temDataFilhaMaior($_POST, $arDtMaior);
				}
			}
		}
		return false;
	}

	public function filtroArvore($post = array()){
		extract($_POST);
		unset($_SESSION['tarefa']['filtroArvore']);
		if($filtrotarid){
			$_SESSION['tarefa']['filtroArvore']['filtrotarid'] = $filtrotarid;
		}
		if($filtrotartitulo){
			$_SESSION['tarefa']['filtroArvore']['filtrotartitulo'] = $filtrotartitulo;
		}
		if($filtrosidoc){
			$_SESSION['tarefa']['filtroArvore']['filtrosidoc'] = $filtrosidoc;
		}
		if($filtrounaidsetororigem){
			$_SESSION['tarefa']['filtroArvore']['filtrounaidsetororigem'] = $filtrounaidsetororigem;
		}
		if($filtrounaidsetorresponsavel){
			$_SESSION['tarefa']['filtroArvore']['filtrounaidsetorresponsavel'] = $filtrounaidsetorresponsavel;
		}
		if($filtrousucpfresponsavel){
			$_SESSION['tarefa']['filtroArvore']['filtrousucpfresponsavel'] = $filtrousucpfresponsavel;
		}
		if(isset($filtrosituacao) && is_array($filtrosituacao)){
			$_SESSION['tarefa']['filtroArvore']['filtrosituacao'] = $filtrosituacao;
		}
		if(isset($filtroprazoini)){
			$_SESSION['tarefa']['filtroArvore']['filtroprazoini'] = $filtroprazoini;
		}
		if(isset($filtroprazofim)){
			$_SESSION['tarefa']['filtroArvore']['filtroprazofim'] = $filtroprazofim;
		}
		
		die;
	}

	public function montaListaAnexo($post = array()){
		header('content-type: text/html; charset=ISO-8859-1');
		$sql = "select  t.tarid,
						t._tartarefa, 
						t._tarpai, 
						t._tarordem,
						t.tartitulo
					from tarefa.tarefa t
					where t._tartarefa = {$_POST["_tartarefa"]}
					order by t._tarordem";
		$arDados = $this->db->carregar($sql);

		$table =  "<table class=\"listagem\" width=\"95%\" align=\"center\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\">
						<thead>
							<tr>
								<td width=\"27%\" class=\"title\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\" valign=\"top\">
									<strong>Tarefa / Atividade</strong>
								</td>
								<td class=\"title\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\" valign=\"top\">
									<strong>Documento</strong>
								</td>
							</tr>
						</thead>
				   ";
		$i = 0;
		foreach($arDados as $dados){
			if(($i % 2) == 1) {
				$fundo="#F7F7F7";
			} else {
				$fundo="#FFFFFF";
			}


			/*
			 * BLOCO PARA FORMA CÓDIGO DA TAREFA
			 */
			$codTarefa = '';
			$arCodTarefa = array();
			self::formaCodTarefa( $dados['_tarordem'], $arCodTarefa );
			$arCodTarefa[0] = $dados['_tartarefa'];
			$codTarefa = implode(".", $arCodTarefa);

			$tamanho = strlen($dados['_tarordem']);
			$nivel = $tamanho / 4;

			$espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$espacoTemp = "";

			for ($y = 1; $y < $nivel; $y++) {
				$espacoTemp = $espacoTemp . $espaco;
			}

			if($espacoTemp){
				$seta = "<img src=\"../imagens/seta_filho.gif\">";
			}

			$table .= "<tr onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$fundo';\" bgcolor=\"$fundo\">";
			$table .= "<td>".$espacoTemp.$seta.$codTarefa.' - '.$dados['tartitulo']./*' - '.$dados['_tarordem'] .*/"</td>";

			$sql = "select  a.anxid,
									a.arqid, 
									a.anxassunto,
									td.tpddescricao
							from tarefa.anexo a
							left join tarefa.tipodocumento td on a.tpdid = td.tpdid
						where tarid = {$dados['tarid']} ";
			$arDocumento = $this->db->carregar($sql);
			$arDocumento = ($arDocumento) ? $arDocumento : array();
			$table .= "<td>
								<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" >
									";
			foreach($arDocumento as $documento){
				$table .= "<tr>";
				$table .= "<td>";
				$table .= "<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href='tarefa.php?modulo=principal/cadDocumento&acao=A&anxid={$documento['anxid']}'\" style=\"border:0; cursor:pointer;\" title=\"Alterar Tarefa / Atividade\">
									 &nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir Documento\" onClick=\"excluirAnexo('{$documento['arqid']}');\" >
									 &nbsp;<img src=\"../imagens/anexo.gif\" style=\"border:0; cursor:pointer;\" title=\"Abrir Anexo\" onClick=\"abrirAnexo('{$documento['arqid']}');\" >&nbsp;&nbsp;";
				$table .= $documento['anxassunto'].' - '.$documento['tpddescricao'];
				$table .= "</td>";
				$table .= "<td>";
				$table .= "</td>";
				$table .= "</tr>";
			}
			$table .= " 	</table>
		    			   </td>";
			$table .= "</tr>";
			$i++;
		}
		$table .= "</table>";
		echo $table;
		die;
	}

	public function abaAtendimento(){
		header('content-type: text/html; charset=ISO-8859-1');
		$tarid = $_POST['tarid'];
		$obTarefa = new Tarefa($tarid);
		monta_titulo( "Atendimento", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
		?>
<div id="divBlocoAtendimento"><?php
echo blocoDadosAtendimento($obTarefa, 'S', true, false, '', array(), false, $this->db);
?></div>
<div id=buttonAcao>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center" style="border-top: none">
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align: center"><input
			type="button" value="Salvar" onclick="enviaForm();" /></td>
	</tr>
</table>
</div>
<div id="divListaAtendimento"><?php
echo listaAtendimento($tarid, $this->db);
?></div>
<?php
die;
	}

	public function abaRestricao(){
		header('content-type: text/html; charset=ISO-8859-1');
		$tarid = $_POST['tarid'];
		if(!isset($_POST['boNaoMostraTitulo']) && !$_POST['boNaoMostraTitulo']){
			monta_titulo( "Atendimento", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
		}
		echo dadosRetricao($this->db, $tarid);
		die;
	}

	public function salvarRestricao(){
		header('content-type: text/html; charset=ISO-8859-1');
		$obRestricao = new Restricao($_POST['resid']);
		if(isset($_POST['ressolucao']) && $_POST['ressolucao']){
			$obRestricao->ressolucao = $_POST['ressolucao'];
		}
		$obRestricao->tarid = $_POST['tarid'];
		$obRestricao->resdescricao = iconv( "UTF-8", "ISO-8859-1", $_POST['resdescricao']);
		$obRestricao->resmedida = iconv( "UTF-8", "ISO-8859-1", $_POST['resmedida']);
		$obRestricao->usucpf = $_SESSION['usucpf'];
		$obRestricao->salvar();
		$obRestricao->commit();
		unset($obRestricao);
		echo listaRetricao($this->db, $_POST['tarid']);
		die;
	}

	public function excluirRestricao(){
		header('content-type: text/html; charset=ISO-8859-1');
		$obRestricao = new Restricao();

		$obRestricao->excluir($_POST['resid']);
		$obRestricao->commit();
		unset($obRestricao);

		echo listaRetricao($this->db, $_POST['tarid']);
		die;
	}

	public function pesquisaSidoc(){
		//header('content-type: text/html; charset=ISO-8859-1');

		$msconnect = mssql_connect("MECSRV14", "sysdbsimec_consulta", "sysdbsimec_consulta") or die("Não foi possível a conexão com o servidor");
		//ver($msconnect,d);
		$msdb = mssql_select_db("DBPSIDOC", $msconnect) or die("Não foi possível selecionar o banco de dados");

		$codSidoc = $_POST['codSidoc'];
		/*if(is_numeric($_POST['codSidoc'])){
			$codSidoc = number_format($_POST['codSidoc']);
			$codSidoc = str_replace(array(',','.'),'',$codSidoc);
			}
			*/
		$sql = "select top 1
						NumeroAnexador
					from VW_SIDOC_SIMEC_DOCUMENTO 
					where NumeroSIDOC = '{$codSidoc}'
					";

		$rs = mssql_query($sql);

		$arDados = array();
		while( $row = mssql_fetch_assoc( $rs )){
			$arDados[] = $row;
		}
		mssql_close();
		if($arDados){
			$arDados = current($arDados);
		}
		echo simec_json_encode( $arDados );
		die;
	}

	public function excluirIescodigo($iescod){
		$iesCod =  $iescod['iescodDel'];
		/*
		 * Correção por Alexandre Dourado 17/11/09
		 */
		/*if(!$_SESSION['dados_tarefa']['tarid']) {
			echo "<p align=center>Tarefa não selecionada. <a href='tarefa.php?modulo=principal/listaTarefas&acao=A'>Clique aqui e refaça o procedimento.</a></p>";
			exit;
		}

		$obTarefa = new Tarefa();
		$sql = "DELETE FROM tarefa.instituicaorelacionada WHERE iesid = ".$_POST['iescodDel']." AND tarid = ".$_SESSION['dados_tarefa']['tarid'];
		$arDel = array();
		array_push( $arDel, $_POST['iescodDel']);
		$_SESSION['iescodSession'] = array_diff( $_SESSION['iescodSession'], $arDel );
		$this->db->executar( $sql );
		$this->listarIescodigo();*/
		
		
		/*************** código teste ********************************************/
		/*if(!$_SESSION['dados_tarefa']['tarid']) {
			echo "<p align=center>Tarefa não selecionada. <a href='tarefa.php?modulo=principal/listaTarefas&acao=A'>Clique aqui e refaça o procedimento.</a></p>";
			exit;
		}*/

		//$obTarefa = new Tarefa();
		//$sql = "DELETE FROM tarefa.instituicaorelacionada WHERE iesid = ".$iesCod." AND tarid = ".$_SESSION['dados_tarefa']['tarid'];
		$arDel = array();
		array_push( $arDel, $iesCod);
		$_SESSION['iescodSession'] = array_diff( $_SESSION['iescodSession'], $arDel );
		//$this->db->executar( $sql );
		$this->listarIescodigo();
		
		
	}
	public function listarIescodigo(){
		header('content-type: text/html; charset=ISO-8859-1');
		echo "<table class=\"tabela_listagem\" width=\"600px\" id=\"listaInstituicao\">
          		  	<tr>
          		  		<th>Ação</th>
          		  		<th>Sigla</th>
          		  		<th>Instituição</th>
          		  		<th>UF</th>
          		  	</tr>"; 
		if($_SESSION['iescodSession']) {
			foreach( $_SESSION['iescodSession'] as $cod ) {
				if( $cod != '' ){

					$sql = "select iesid,iescodigo,iessigla, iesnome, iesuf FROM ies.ies where iesid = ".$cod;
					$rs = $this->db->carregar( $sql );
					echo "<tr>
							<td> <img src=\"/imagens/excluir.gif\" onclick=\"excluiIescodigo(".$rs[0]['iesid'].");\" border=0 style=\"cursor: pointer;\"></img> </td>
							<td> ".$rs[0]['iessigla'] ."</td>
							<td> ".$rs[0]['iesnome'] ."</td>
							<td> ".$rs[0]['iesuf'] ."</td>
						  </tr>"; 
				}
			}
		} else {
			echo "<tr><td colspan='4'>Não existem registros.</td></tr>";
		}
		echo "</table>";
		die();
	}

}

if(isset($_REQUEST['tipo'])) {
	$obAjax = new Ajax();
	$obAjax->$_REQUEST['tipo']($_POST);
}
?>