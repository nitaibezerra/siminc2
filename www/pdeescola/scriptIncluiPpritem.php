<?php
$tInicio = getmicrotime();
// obtém o tempo inicial da execução
include_once "config.inc";
include_once '_funcoes.php';
include_once APPRAIZ . "includes/classes_simec.inc";
set_time_limit( 0 );
ob_start();
/*
 * Classe pdePreenchimento
 * Corrige "verificação de pendências"
 */
class pdePreenchimento {
	
	private $pdeid;
	private $boTransacao = false;
	private $tmeids = "";
	private $db;
	private $tbAtualizada = "";
	public $totalAlterado = 0;
	public $totalNaoAlterado = 0;
	public $boExibeTotal = null;

	public function __construct() {
//		pde_verificaSessao();
//		$this->pdeid = $_SESSION["pdeid"];
		$this->db = new cls_banco();
	}
	
	public function arrayParaInsercaoSceid(){
		$arSceid = array();
		
		# POSSIBILIDADES MATRICULA INICIAL
		# ARRAY DE SCEID
		$arSceid['p9_1'][] = 28;
		$arSceid['p9_1'][] = 23;
		
		$arSceid['p10_1'][] = 1;
		$arSceid['p10_1'][] = 2;
		$arSceid['p10_1'][] = 3;
		$arSceid['p10_1'][] = 4;
		$arSceid['p10_1'][] = 5;
		$arSceid['p10_1'][] = 6;
		$arSceid['p10_1'][] = 7;
		$arSceid['p10_1'][] = 8;
		$arSceid['p10_1'][] = 9;
		$arSceid['p10_1'][] = 10;
		
		$arSceid['p11_1'][] = 20;
		$arSceid['p11_1'][] = 21;
		$arSceid['p11_1'][] = 22;
		
		$arSceid['p12_1'][] = 24;
		$arSceid['p12_1'][] = 25;
		$arSceid['p12_1'][] = 26;
		$arSceid['p12_1'][] = 27;
		$arSceid['p12_1'][] = 29;
		$arSceid['p12_1'][] = 30;
		$arSceid['p12_1'][] = 31;
		$arSceid['p12_1'][] = 32;
		
		#FIM ARRAY POSSIBILIDADES MATRICULA INICIAL
		
		# POSSIBILIDADES APRIVEITAMENTO ALUNO
		$arSceid['p10_2'][] = 1;
		$arSceid['p10_2'][] = 2;
		$arSceid['p10_2'][] = 3;
		$arSceid['p10_2'][] = 4;
		$arSceid['p10_2'][] = 5;
		$arSceid['p10_2'][] = 10;
		
		$arSceid['p10_3'][] = 6;
		$arSceid['p10_3'][] = 7;
		$arSceid['p10_3'][] = 8;
		$arSceid['p10_3'][] = 9;
		
		$arSceid['p11_2'][] = 20;
		$arSceid['p11_2'][] = 21;
		$arSceid['p11_2'][] = 22;
		
		$arSceid['p12_2'][] = 24;
		$arSceid['p12_2'][] = 25;
		$arSceid['p12_2'][] = 26;
		$arSceid['p12_2'][] = 27;
		$arSceid['p12_2'][] = 29;
		$arSceid['p12_2'][] = 30;
		$arSceid['p12_2'][] = 31;
		$arSceid['p12_2'][] = 32;
		# FIM POSSIBILIDADES APRIVEITAMENTO ALUNO
		
		# POSSIBILIDADES DISTORCAO IDADE SERIE
		$arSceid['p10_5'][] = 1;
		$arSceid['p10_5'][] = 2;
		$arSceid['p10_5'][] = 3;
		$arSceid['p10_5'][] = 4;
		$arSceid['p10_5'][] = 5;
		
		$arSceid['p10_6'][] = 6;
		$arSceid['p10_6'][] = 7;
		$arSceid['p10_6'][] = 8;
		$arSceid['p10_6'][] = 9;
		# FIM POSSIBILIDADES DISTORCAO IDADE SERIE
		
		# POSSIBILIDADES APRIVEITAMENTO ALUNO CICLO ETAPA
		$arSceid['p10_4'][] = 11;
		$arSceid['p10_4'][] = 12;
		$arSceid['p10_4'][] = 13;
		$arSceid['p10_4'][] = 14;
		$arSceid['p10_4'][] = 15;
		$arSceid['p10_4'][] = 16;
		$arSceid['p10_4'][] = 17;
		$arSceid['p10_4'][] = 18;
		$arSceid['p10_4'][] = 19;
		# FIM POSSIBILIDADES APRIVEITAMENTO ALUNO CICLO ETAPA
		return $arSceid;
	}
	
	private function arrayParaInsercaoTmeid(){
		$arTmeId = array();
		
		# POSSIBILIDADES MATRICULA INICIAL
		$arTmeId['p9_1'][]  = 9;
		$arTmeId['p10_1'][] = 2;
		$arTmeId['p10_1'][] = 3;
		$arTmeId['p11_1'][] = 6;
		$arTmeId['p12_1'][] = 10;
		#FIM ARRAY POSSIBILIDADES MATRICULA INICIAL
		
		# POSSIBILIDADES APRIVEITAMENTO ALUNO
		
		$arTmeId['p10_2'][] = 2;
		
		$arTmeId['p10_3'][] = 3;
		
		$arTmeId['p11_2'][] = 6;
		
		$arTmeId['p12_2'][] = 10;
		# FIM POSSIBILIDADES APRIVEITAMENTO ALUNO
		return $arTmeId;
	}
	
	public function preenchePpritemTelasCadastro(){
		# Chamamos os arrays de configuração para inserção
		$arSceid = self::arrayParaInsercaoSceid();
		$arTmeId = self::arrayParaInsercaoTmeid();
		
		$this->boExibeTotal = true;
		
		$arPdeEscola = $this->db->carregar("SELECT pdeid FROM pdeescola.pdeescola order by pdeid ");
		$totalAlterado;
		$totalNaoAlterado;
		foreach($arPdeEscola as $pdeid){
			$this->pdeid = $pdeid['pdeid'];
		
			# Query matricula inicial e verificação matricula inicial
			$arMatriculaInicial = $this->db->carregar("SELECT m.maiid,
														m.pdeid,
														m.sceid,
														s.tmeid
														FROM pdeescola.matriculainicial m
														LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
														WHERE m.pdeid = $this->pdeid
														AND m.ppritem is null"
													  );
			
			if(is_array($arMatriculaInicial)){
				$countTb = 1;
				foreach ($arMatriculaInicial as $matriculaInical){
					$nmPpritem = null;
					if(in_array($matriculaInical['sceid'], $arSceid['p9_1']) && in_array($matriculaInical['tmeid'], $arTmeId['p9_1']) ){
						$nmPpritem = 'p9_1';
					}
					if(in_array($matriculaInical['sceid'], $arSceid['p10_1']) && in_array($matriculaInical['tmeid'], $arTmeId['p10_1']) ){
						$nmPpritem = 'p10_1';
					}
					if(in_array($matriculaInical['sceid'], $arSceid['p11_1']) && in_array($matriculaInical['tmeid'], $arTmeId['p11_1']) ){
						$nmPpritem = 'p11_1';
					}
					if(in_array($matriculaInical['sceid'], $arSceid['p12_1']) && in_array($matriculaInical['tmeid'], $arTmeId['p12_1']) ){
						$nmPpritem = 'p12_1';
					}
					$sql = "UPDATE pdeescola.matriculainicial SET ppritem = '$nmPpritem' WHERE maiid = ".$matriculaInical['maiid']." AND pdeid = $this->pdeid";
					$this->db->executar($sql);
					$this->boTransacao = true;
					if($countTb == 1){
						$this->tbAtualizada .= " ->Matricula Inicial<- ";
					}
					$countTb++;
				}
			}
			unset($arMatriculaInicial);
			unset($matriculaInical);
			
			# Query aproveitamento aluno e verificação aproveitamento aluno
			$arAproveitamentoAluno = $this->db->carregar("SELECT aa.apaid,aa.pdeid,aa.sceid,s.tmeid FROM pdeescola.aproveitamentoaluno aa
			left JOIN pdeescola.seriecicloescolar s ON s.sceid = aa.sceid
			WHERE aa.pdeid = $this->pdeid AND aa.ppritem is null");
			
			if(is_array($arAproveitamentoAluno)){
				$countTb = 1;
				foreach ($arAproveitamentoAluno as $aproveitamentoAluno){
					$nmPpritem = null;
					if(in_array($aproveitamentoAluno['sceid'], $arSceid['p10_2']) && in_array($aproveitamentoAluno['tmeid'], $arTmeId['p10_2']) ){
						$nmPpritem = 'p10_2';
					}
					if(in_array($aproveitamentoAluno['sceid'], $arSceid['p10_3']) && in_array($aproveitamentoAluno['tmeid'], $arTmeId['p10_3']) ){
						$nmPpritem = 'p10_3';
					}
					if(in_array($aproveitamentoAluno['sceid'], $arSceid['p11_2']) && in_array($aproveitamentoAluno['tmeid'], $arTmeId['p11_2']) ){
						$nmPpritem = 'p11_2';
					}
					if(in_array($aproveitamentoAluno['sceid'], $arSceid['p12_2']) && in_array($aproveitamentoAluno['tmeid'], $arTmeId['p12_2']) ){
						$nmPpritem = 'p12_2';
					}
					$sql = "UPDATE pdeescola.aproveitamentoaluno SET ppritem = '$nmPpritem' WHERE apaid = ".$aproveitamentoAluno['apaid']." AND pdeid = $this->pdeid";
					$this->db->executar($sql);
					$this->boTransacao = true;
					if($countTb == 1){
						$this->tbAtualizada .= " ->Aproveitamento Aluno<- ";
					}
					$countTb++;
				}
			}
			unset($arAproveitamentoAluno);
			unset($aproveitamentoAluno);
			# fim verificação matricula inicial
			
			# Query distorção idade serie e verificação distorção idade serie
			$arDistorcaoIdadeSerie = $this->db->carregar("SELECT die.disid,die.pdeid,die.sceid,s.tmeid FROM pdeescola.distorcaoidadeserie die 
			left JOIN pdeescola.seriecicloescolar s ON s.sceid = die.sceid
			where die.pdeid = $this->pdeid AND die.ppritem is null");
			
			if(is_array($arDistorcaoIdadeSerie)){
				$countTb = 1;
				foreach ($arDistorcaoIdadeSerie as $distorcaoIdadeSerie){
					$nmPpritem = null;
					if(in_array($distorcaoIdadeSerie['sceid'], $arSceid['p10_5'])){
						$nmPpritem = 'p10_5';
					}
					if(in_array($distorcaoIdadeSerie['sceid'], $arSceid['p10_6'])){
						$nmPpritem = 'p10_6';
					}
					$sql = "UPDATE pdeescola.distorcaoidadeserie SET ppritem = '$nmPpritem' WHERE disid = ".$distorcaoIdadeSerie['disid']." AND pdeid = $this->pdeid";
					$this->db->executar($sql);
					$this->boTransacao = true;
					if($countTb == 1){
						$this->tbAtualizada .= " ->Distorção Idade Série<- ";
					}
					$countTb++;
				}
			}
			unset($arDistorcaoIdadeSerie);
			unset($distorcaoIdadeSerie);
			# fim verificação distorção idade serie
			
			# Query aproveitamento aluno Ciclo e verificação aproveitamento aluno Ciclo
			$arAproveitamentoAlunoCiclo = $this->db->carregar("SELECT * FROM pdeescola.aproveitamentoalunocicloetapa WHERE pdeid = $this->pdeid AND ppritem is null");
			
			if(is_array($arAproveitamentoAlunoCiclo)){
				$countTb = 1;
				foreach ($arAproveitamentoAlunoCiclo as $aproveitamentoAlunoCiclo){
					$nmPpritem = null;
					if(in_array($aproveitamentoAlunoCiclo['sceid'], $arSceid['p10_4'])){
						$nmPpritem = 'p10_4';
					}
					$sql = "UPDATE pdeescola.aproveitamentoalunocicloetapa SET ppritem = '$nmPpritem' WHERE apaid = ".$aproveitamentoAlunoCiclo['apaid']." AND pdeid = $this->pdeid";
					$this->db->executar($sql);
					$this->boTransacao = true;
					if($countTb == 1){
						$this->tbAtualizada .= " ->Aproveitamento Aluno Ciclo<- ";
					}
					$countTb++;
				}
			}
			unset($arAproveitamentoAlunoCiclo);
			unset($aproveitamentoAlunoCiclo);
			# fim verificação aproveitamento aluno Ciclo
			
			//commit
			if($this->boTransacao){
				$this->db->commit();
				echo 'Id: ' . $this->pdeid . ' - Atualizado' . $this->tbAtualizada .'<br>';
				$this->totalAlterado++;
			} else {
				echo 'Id: ' . $this->pdeid . ' - Não efetuado (OK)' . $this->tbAtualizada .'<br>';
				$this->totalNaoAlterado++;
			}
			$this->boTransacao = false;
			$this->tbAtualizada = "";
			ob_get_contents();
			flush();
			ob_flush();
			flush();
			ob_flush();
			flush();
			ob_flush();
			ob_end_clean();	
		
		}
	}

    private function configArrayPdePreenchimento($arPossiveisMatriculaInicial
										    	,$arPossiveisAproveitamentoAluno
												,$arPossiveisDistorcaoIdadeSerie
												,$arPossiveisAproveitamentoAlunoCiclo
												,$arPossiveisDependenciaCondUso
												,$arPossiveisDiciplinaCritica
												,$arPossiveisPessoalTecnicoFormacao
												,$arPossiveisRelacaoAlunoDocente
												,$arPossiveisTurmaSemProfessor
												,$arPossiveisFonteDestinacaoRecurso
												,$arPossiveisPrevisaoRecurso
												,$arPossiveisEscolaProveParaAluno
												,$arPossiveisMedidaProjetoAtual
												,$arPossiveisMedidaProjetoImplantado
												,$arPossiveisMudancaMedidaProjeto
												,$arPossiveisTrabalhoSecretariaEducacao
												,$arPossiveisParticipacaoProfessorFunc
												,$arPossiveisParticipacaoColegiadoConselho
												,$arPossiveisMedidaProjetoParceria
												,$arPossiveisAvaliacaoRelacaoSecretaria
												,$arPossiveisAvaliacaoRelacaoComunidade
												,$arPossiveisFormaSelecaoDiretor
												,$arPossiveisRotatividade
												,$arPossiveisPercentualJornadaIntegral
												){
    	# POSSIBILIDADES MATRICULA INICIAL
		$arPossiveisMatriculaInicial['p9_1']  = 'p9_1';
		$arPossiveisMatriculaInicial['p10_1'] = 'p10_1';
		$arPossiveisMatriculaInicial['p11_1'] = 'p11_1';
		$arPossiveisMatriculaInicial['p12_1'] = 'p12_1';
		
		# POSSIBILIDADES APRIVEITAMENTO ALUNO
		$arPossiveisAproveitamentoAluno['p10_2']  = 'p10_2';
		$arPossiveisAproveitamentoAluno['p10_3']  = 'p10_3';
		$arPossiveisAproveitamentoAluno['p11_2']  = 'p11_2';
		$arPossiveisAproveitamentoAluno['p12_2']  = 'p12_2';
		
		# POSSIBILIDADES DISTORCAO IDADE SERIE
		$arPossiveisDistorcaoIdadeSerie['p10_5']  = 'p10_5';
		$arPossiveisDistorcaoIdadeSerie['p10_6']  = 'p10_6';
		
		# POSSIBILIDADES APRIVEITAMENTO ALUNO CICLO ETAPA
		$arPossiveisAproveitamentoAlunoCiclo['p10_4']  = 'p10_4';
		
		# POSSIBILIDADES DEPENDENCIAS CONDIÇÃO USO
		$arPossiveisDependenciaCondUso['p8']  = 'p8';
		
		# POSSIBILIDADES DISCPLINA CRITICA
		$arPossiveisDiciplinaCritica['p13']  = 'p13';
		
		# POSSIBILIDADES PESSOAL TECNICO FORMÇÃO
		$arPossiveisPessoalTecnicoFormacao['p14_1']  = 'p14_1';
		
		# POSSIBILIDADES RELAÇÃO ALUNO DOCENTE
		$arPossiveisRelacaoAlunoDocente['p14_2']  = 'p14_2';
		
		# POSSIBILIDADES TURMA SEM PROFESSOR
		$arPossiveisTurmaSemProfessor['p14_3']  = 'p14_3';
		
		# POSSIBILIDADES FONTE DESTINAÇÃO RECURSO
		$arPossiveisFonteDestinacaoRecurso['p15']  = 'p15';
		
		# POSSIBILIDADES PREVISAO RESURSO
		$arPossiveisPrevisaoRecurso['p16']  = 'p16';
		
		# POSSIBILIDADES PREVISAO RESURSO
		$arPossiveisEscolaProveParaAluno['p17']  = 'p17';
		
		# POSSIBILIDADES MEDIDA PROJETO ATUAL
		$arPossiveisMedidaProjetoAtual['p18']  = 'p18';
		
		# POSSIBILIDADES MEDIDA PROJETO IMPLANTADO
		$arPossiveisMedidaProjetoAtual['p19']  = 'p19';
		
		# POSSIBILIDADES MUDANÇA MEDIDA PROJETO
		$arPossiveisMudancaMedidaProjeto['p20']  = 'p20';
		
		# POSSIBILIDADES TRABALHO SECRETARIA EDUCACAO
		$arPossiveisTrabalhoSecretariaEducacao['p21']  = 'p21';
		
		# POSSIBILIDADES PARTICIPACAO PROEF FUNC
		$arPossiveisParticipacaoProfessorFunc['p22']  = 'p22';
		
		# POSSIBILIDADES PARTICIPACAO COLEGIADO CONSELHO
		$arPossiveisParticipacaoColegiadoConselho['p23']  = 'p23';
		
		# POSSIBILIDADES MEDIDA PROJETO PARCERIA
		$arPossiveisMedidaProjetoParceria['p24']  = 'p24';
		
		# POSSIBILIDADES AVALIACAO RELACAO SECRETARIA
		$arPossiveisAvaliacaoRelacaoSecretaria['p25']  = 'p25';
		
		# POSSIBILIDADES AVALIACAO RELACAO COMUNIDADE
		$arPossiveisAvaliacaoRelacaoComunidade['p26']  = 'p26';
		
		# POSSIBILIDADES FORMA SELECAO DIRETOR
		$arPossiveisFormaSelecaoDiretor['p27']  = 'p27';
		
		# POSSIBILIDADES ROTATIVIDADE
		$arPossiveisRotatividade['p28']  = 'p28';
    }
    
	/*
	 * Método que corrige os dados da tabela pdepreenchimento que foram apagados errados. 
	 */
    public function corrigePdePreenchimentoApagados(){
    	$this->boExibeTotal = true;
		$arPdeEscola = $this->db->carregar("SELECT pdeid FROM pdeescola.pdeescola order by pdeid");
		foreach($arPdeEscola as $pdeid){
			$this->pdeid = $pdeid['pdeid'];
			# Query aproveitamento aluno 
			$sql = "select (sum(coalesce(aa.apaqtdmatriculainicial,0)) + 
					    sum(coalesce(aa.apaqtdadmitidosaposmarco,0)) +
					    sum(coalesce(aa.apaqtdafastadosabandono,0)) +     
					    sum(coalesce(aa.apaqtdafastadostransferencia,0)) +     
					    sum(coalesce(aa.apaqtdmatriculafinal,0) ) +
					    sum(coalesce(aa.apaqtdaprovados,0)) +     
					    sum(coalesce(aa.apaqtdreprovados,0)) ) as preenchido,
					    aa.ppritem
					 from pdeescola.aproveitamentoaluno aa
					left join pdeescola.seriecicloescolar s on s.sceid = aa.sceid
					where aa.pdeid = $this->pdeid 
					group by aa.ppritem";			
			$arDados = $this->db->carregar($sql);
			if(count($arDados) && is_array($arDados)){
				foreach($arDados as $dados){
					if(!empty($dados['ppritem']) && $dados['ppritem']){
						# Verifica na tabela pdepreenchimento se já existe registro
						if ($dados['preenchido'] > 0){
							$sql = "SELECT * FROM pdeescola.pdepreenchimento WHERE pdeid = ".$this->pdeid." and ppritem = '".$dados['ppritem']."'";
							$arPdePreenchimento = $this->db->carregar($sql);
							if(!is_array($arPdePreenchimento)){
								$sql = "INSERT INTO pdeescola.pdepreenchimento(pdeid,ppritem,pprinstrumento,pdeanoreferencia) VALUES(".$this->pdeid.",'".$dados['ppritem']."','i1',2008)";
								$this->db->executar($sql);
								$this->boTransacao = true;
							}
						}
					}
				}
			}
			
			# Query distorção idade serie 
			$sql = "SELECT die.ppritem FROM pdeescola.distorcaoidadeserie die 
					left JOIN pdeescola.seriecicloescolar s ON s.sceid = die.sceid
			   WHERE die.pdeid = $this->pdeid and disqtdmatriculafinal is not null ";
			$arDados = $this->db->carregar($sql);
			if(count($arDados) && is_array($arDados)){
				$ppritemAnterior = "";
				foreach($arDados as $dados){
					if(!empty($dados['ppritem']) && $dados['ppritem']){
						# Se o ppritem for diferente do anterior ele verifica na tabela pdepreenchimento se já existe registro
						if ($dados['ppritem'] != $ppritemAnterior){
							$sql = "SELECT * FROM pdeescola.pdepreenchimento WHERE pdeid = ".$this->pdeid." and ppritem = '".$dados['ppritem']."'";
							$arPdePreenchimento = $this->db->carregar($sql);
							# senão existi registro então insere 
							if(!is_array($arPdePreenchimento)){
								$sql = "INSERT INTO pdeescola.pdepreenchimento(pdeid,ppritem,pprinstrumento,pdeanoreferencia) VALUES(".$this->pdeid.",'".$dados['ppritem']."','i1',2008)";
								$this->db->executar($sql);
								$this->boTransacao = true;
							}
							$ppritemAnterior = $dados['ppritem'];
						}
					}
				}
			}
			
			//commit
			if($this->boTransacao){
				$this->db->commit();
				echo 'Id: ' . $this->pdeid . ' - Atualizado<br>';
				$this->totalAlterado++;
			} else {
				echo 'Id: ' . $this->pdeid . ' - Não efetuado (OK)<br>';
				$this->totalNaoAlterado++;
			}
			ob_get_contents();
			flush();
			ob_flush();
			flush();
			ob_flush();
			flush();
			ob_flush();
			ob_end_clean();
			$this->boTransacao = false;
		}
	}
    
    public function corrigePdePreenchimento(){
		# Chamamos a configuração de arrays
		self::configArrayPdePreenchimento(&$arPossiveisMatriculaInicial
										,&$arPossiveisAproveitamentoAluno
										,&$arPossiveisDistorcaoIdadeSerie
										,&$arPossiveisAproveitamentoAlunoCiclo
										,&$arPossiveisDependenciaCondUso
										,&$arPossiveisDiciplinaCritica
										,&$arPossiveisPessoalTecnicoFormacao
										,&$arPossiveisRelacaoAlunoDocente
										,&$arPossiveisTurmaSemProfessor
										,&$arPossiveisFonteDestinacaoRecurso
										,&$arPossiveisPrevisaoRecurso
										,&$arPossiveisEscolaProveParaAluno
										,&$arPossiveisMedidaProjetoAtual
										,&$arPossiveisMedidaProjetoImplantado
										,&$arPossiveisMudancaMedidaProjeto
										,&$arPossiveisTrabalhoSecretariaEducacao
										,&$arPossiveisParticipacaoProfessorFunc
										,&$arPossiveisParticipacaoColegiadoConselho
										,&$arPossiveisMedidaProjetoParceria
										,&$arPossiveisAvaliacaoRelacaoSecretaria
										,&$arPossiveisAvaliacaoRelacaoComunidade
										,&$arPossiveisFormaSelecaoDiretor
										,&$arPossiveisRotatividadeglobal
										,&$arPossiveisPercentualJornadaIntegral
										);
    	$this->boExibeTotal = true;
    	
		$arPdeEscola = $this->db->carregar("SELECT pdeid FROM pdeescola.pdeescola order by pdeid");
		foreach($arPdeEscola as $pdeid){
			$this->pdeid = $pdeid['pdeid'];
			
			# Query matricula inicial
			$sqlMatricula = "SELECT m.maiid,
							m.pdeid,
							m.sceid,
							m.ppritem,
							s.tmeid
							FROM pdeescola.matriculainicial m
							LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
							WHERE pdeid = ".$this->pdeid." order by m.sceid";
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sqlMatricula,$arPossiveisMatriculaInicial);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query aproveitamento aluno 
			$sql = "SELECT aa.apaid,aa.pdeid,aa.sceid,s.tmeid,aa.ppritem FROM pdeescola.aproveitamentoaluno aa
											left JOIN pdeescola.seriecicloescolar s ON s.sceid = aa.sceid
								  	   WHERE aa.pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisAproveitamentoAluno);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query distorção idade serie
			$sql = "SELECT die.disid,die.pdeid,die.sceid,s.tmeid, die.ppritem FROM pdeescola.distorcaoidadeserie die 
											left JOIN pdeescola.seriecicloescolar s ON s.sceid = die.sceid
									   WHERE die.pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisDistorcaoIdadeSerie);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query aproveitamento aluno ciclo
			//$sql = "SELECT * FROM pdeescola.aproveitamentoalunocicloetapa WHERE pdeid = ".$this->pdeid;
			$sql = "SELECT 
					    (sum(coalesce(apaqtdmatriculainicial,0)) + 
					    sum(coalesce(apaqtdadmitidoaposmarco,0)) +
					    sum(coalesce(apaqtdafastadoabandono,0)) +     
					    sum(coalesce(apaqtdafastadotransferencia,0)) +     
					    sum(coalesce(apaqtdavalps,0)) +
					    sum(coalesce(apaqtdavalppda,0)) +     
					    sum(coalesce(apaqtdavalrfc,0))+     
					    sum(coalesce(apaqtdavalpmae,0)) +     
					    sum(coalesce(apaqtdmatriculaatual,0))+     
					    sum(coalesce(apaqtdtxpps,0))+
					    sum(coalesce(apaqtdtxpppda,0))+     
					    sum(coalesce(apaqtdtxppmae,0))+     
					    sum(coalesce(apaqtdtxpretencao,0)) ) as preenchido,
					    ppritem
					FROM 
					    pdeescola.aproveitamentoalunocicloetapa WHERE pdeid = $this->pdeid group by ppritem";
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisAproveitamentoAlunoCiclo);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query dependencias condicao uso
			$sql = "SELECT * FROM pdeescola.dependenciascondicaouso WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisDependenciaCondUso,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query disciplina critica
			$sql = "SELECT * FROM pdeescola.periododisciplinacritica pd
						inner join pdeescola.disciplinacritica d on pd.pdcid = d.pdcid
					WHERE pd.pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisDiciplinaCritica,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query técnico formação
			$sql = "SELECT * FROM pdeescola.pessoaltecnicoformacao WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisPessoalTecnicoFormacao,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query relação aluno docente
			$sql = "SELECT * FROM pdeescola.relacaoalunodocente WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisRelacaoAlunoDocente,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query turma sem professor
			$sql = "SELECT * FROM pdeescola.turmasemprofessor WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisTurmaSemProfessor,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query fonte destinacao recurso
			$sql = "SELECT * FROM pdeescola.fontedestinacaorecurso WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisFonteDestinacaoRecurso,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query Previsao Recurso
			$sql = "SELECT * FROM pdeescola.previsaorecursosescola WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisPrevisaoRecurso,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query escola prove para aluno
			$sql = "SELECT * FROM pdeescola.escolaproveparaaluno WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisEscolaProveParaAluno,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query medida projeto atual
			$sql = "SELECT * FROM pdeescola.medidaprojetoatual WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisMedidaProjetoAtual,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query medida projeto implantado
			$sql = "SELECT * FROM pdeescola.medidaprojetoimplantado WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisMedidaProjetoImplantado,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query mudança medida projeto
			$sql = "SELECT * FROM pdeescola.mudancamedidaprojeto WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisMudancaMedidaProjeto,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query trabalho secretária educação
			$sql = "SELECT * FROM pdeescola.trabalhosecretariaeducacao WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisTrabalhoSecretariaEducacao,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query participação professor funcionario
			$sql = "SELECT * FROM pdeescola.participacaoprofessorfuncionari WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisParticipacaoProfessorFunc,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query participação colegiado conselho
			$sql = "SELECT * FROM pdeescola.participacaocolegiadoconselho WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisParticipacaoColegiadoConselho,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query medida projeto parceria
			$sql = "SELECT * FROM pdeescola.medidaprojetoparceria WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisMedidaProjetoParceria,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query avaliação relação secretaria
			$sql = "SELECT * FROM pdeescola.avaliarelacaosecretaria WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisAvaliacaoRelacaoSecretaria,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query avaliação relação comunidade
			$sql = "SELECT * FROM pdeescola.avaliarelacaocomunidade WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisAvaliacaoRelacaoComunidade,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query forma seleção diretor
			$sql = "SELECT * FROM pdeescola.formaselecaodiretor WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisFormaSelecaoDiretor,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query rotatividade
			$sql = "SELECT * FROM pdeescola.rotatividade WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisRotatividade,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			
			# Query percentual jornada integral
			$sql = "SELECT * FROM pdeescola.percentualjornadaintegral WHERE pdeid = ".$this->pdeid;
			$boTransacaoTemp = self::limpaRegistroPdePreenchimento($this->tmeids,$this->pdeid,$sql,$arPossiveisPercentualJornadaIntegral,false);
			if($boTransacaoTemp) $this->boTransacao = true;
			

			//commit
			if($this->boTransacao){
				$this->db->commit();
				echo 'Id: ' . $this->pdeid . ' - Atualizado<br>';
				$this->totalAlterado++;
			} else {
				echo 'Id: ' . $this->pdeid . ' - Não efetuado (OK)<br>';
				$this->totalNaoAlterado++;
			}
			ob_get_contents();
			flush();
			ob_flush();
			flush();
			ob_flush();
			flush();
			ob_flush();
			 ob_end_clean();
			$this->boTransacao = false;
			
		} # Fim foreach pdeEscola
    }
    
    /*
     * Metodo responsável para deletar registros da pdePreechimento de questoes que não estão preenchidas
     */
	private function limpaRegistroPdePreenchimento(&$tmeids, &$pdeid, $sql = "", &$arPossiveis, $boExistePpritemTabela = true){
		$boTransacaoTemp = false;
		$arDados = 0;
		if($sql)
			$arDados = $this->db->carregar($sql);
			
		# Verifica se a query passada existe o campo ppritem.
		if($boExistePpritemTabela){
			if(count($arDados) && is_array($arDados)){
				foreach($arDados as $dados){
					$ppritem = $dados['ppritem'];
					if(isset($dados['preenchido'])){
						if($dados['preenchido'] > 0){
							unset($arPossiveis[$ppritem]);	
						}
					} elseif (in_array($ppritem, $arPossiveis)){
						unset($arPossiveis[$ppritem]);
					}
				}
				
				unset($arDados);
				unset($dados);
				
				if(is_array($arPossiveis) && count($arPossiveis)){
					$sql = "";
					foreach($arPossiveis as $ppritemDeletar){
						$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = '".$ppritemDeletar."'" ;
						$this->db->executar( $sql );
						$boTransacaoTemp = true;
					}
				}
			} else {
				if(is_array($arPossiveis) && count($arPossiveis)){
					$sql = "";
					foreach($arPossiveis as $ppritemDeletar){
						$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = '".$ppritemDeletar."'" ;
						$this->db->executar( $sql );
						$boTransacaoTemp = true;
					}
				}
			}
		} else { # $boExistePpritemTabela
			if(!is_array($arDados)){
				if(is_array($arPossiveis) && count($arPossiveis)){ # Verifica se foi passado um array de configurações
					$sql = "";
					foreach($arPossiveis as $ppritemDeletar){
						$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = '".$ppritemDeletar."'" ;
						$this->db->executar( $sql );
						$boTransacaoTemp = true;
					}
				}
			}
		}
		return $boTransacaoTemp;
	}


	public function verificaPlanoSuporteEstrategico(){
		$this->boExibeTotal = false;
		$arPdeEscola = $this->db->carregar("SELECT pdeid FROM pdeescola.pdeescola order by pdeid ");
		if(is_array($arPdeEscola) && count($arPdeEscola)){
			foreach($arPdeEscola as $pdeid){
				$this->pdeid = $pdeid['pdeid'];
				# Verifica se existe objetivos
				$arObjetivo = $this->db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = $this->pdeid AND pseidpai is null");
				if(is_array($arObjetivo) && count($arObjetivo)){
					# Navega nos objetivos encontrados
					$boTemMeta = false;
					foreach($arObjetivo as $objetivo){
						# Verifica se existe Estrategias
						$arEstrategia = $this->db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = $this->pdeid AND pseidpai = ". $objetivo['pseid']);
						if(is_array($arEstrategia) && count($arEstrategia)){
							# Navega nas Estrategias
							foreach($arEstrategia as $estrategia){
								# Verifica se existe Metas
								$arMeta = $this->db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = $this->pdeid AND pseidpai = ". $estrategia['pseid']);
								if(is_array($arMeta) && count($arMeta)){
									$boTemMeta = true;
								}
							}
						}		
					}
				}
				# Se existe não exite metas, limpa tabela preenchimento 
				if(!$boTemMeta){
					$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$this->pdeid." AND ppritem = 'popupObjetivosMetas'" ;
					$this->db->executar( $sql );
					$this->db->commit();
					echo 'Id: ' . $this->pdeid . ' - Atualizado<br>';
				} else {
					echo 'Id: ' . $this->pdeid . ' - Não efetuado (OK)<br>';
				}
			ob_get_contents();
			flush();
			ob_flush();
			flush();
			ob_flush();
			flush();
			ob_flush();
  		 	ob_end_clean();

			}
		}
	}
	
	public function verificaProjetoObrigatorio(){
		$boExclui = false;
		$arPdeEscola = $this->db->carregar("SELECT pdeid FROM pdeescola.pdeescola order by pdeid ");
		foreach($arPdeEscola as $pdeid){
			$this->pdeid = $pdeid['pdeid'];
			$possuiProjetos = verificaProjetos($this->pdeid,$this->db);
			if( !$possuiProjetos ){
				//se não existirem projetos cadastrados é deletado os projetos das telas: p19 à p24 e da tabela de preenchimento.
				$sql = "DELETE FROM pdeescola.medidaprojetoimplantado WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				$sql = "DELETE FROM pdeescola.mudancamedidaprojeto WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				$sql = "DELETE FROM pdeescola.trabalhosecretariaeducacao WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				$sql = "DELETE FROM pdeescola.participacaoprofessorfuncionari WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				$sql = "DELETE FROM pdeescola.participacaocolegiadoconselho WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				$sql = "DELETE FROM pdeescola.medidaprojetoparceria WHERE pdeid = $this->pdeid";
				$this->db->executar($sql); 
				//preenchimento;
				$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$this->pdeid." AND ppritem in('p18', 'p19', 'p20', 'p21', 'p22', 'p23', 'p24')" ;
				$this->db->executar( $sql );
				
				$boExclui = true;
			}
			if($boExclui){
				$this->db->commit();
				echo 'Id: ' . $this->pdeid . ' - Atualizado<br>';
			} else {
				echo 'Id: ' . $this->pdeid . ' - Não efetuado (OK)<br>';
			}
			ob_get_contents();
			flush();
			ob_flush();
			flush();
			ob_flush();
			flush();
			ob_flush();
  		 	ob_end_clean();
			
		$boExclui = false;
		}
	}
	
}

if($_GET['metodo']){
	$pdePreenchimento = new pdePreenchimento();
	$pdePreenchimento->$_GET['metodo']();
	if($pdePreenchimento->boExibeTotal){
		echo "<br />Total Alterado: ".$pdePreenchimento->totalAlterado."<br />";
		echo "Total Não Alterado: ".$pdePreenchimento->totalNaoAlterado."<br />";
		$totalEscolas = $pdePreenchimento->totalAlterado+$pdePreenchimento->totalNaoAlterado;
		echo "TOTAL DE ESCOLAS: ".$totalEscolas."<br />";
	}
}
	function getmicrotime(){
		list( $usec, $sec ) = explode( ' ', microtime() );
		return (float) $usec + (float) $sec; 
	}
?>
Tx.: <?= number_format( ( getmicrotime() - $tInicio ), 4, ',', '.' ); ?>s / <?=number_format(memory_get_usage()/(1024*1024),2,',','.');?>