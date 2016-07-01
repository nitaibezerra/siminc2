<?php

include_once APPRAIZ . 'maismedicos/classes/Ws_Unasus_Soap.class.inc';

class Ws_Tutor extends Ws_Unasus_Soap{
	
	protected $arrTutores;
	
	protected $arrSupervisores;
	
	protected $arrAutorizacaoPagamento;
	
	protected $arrPlanosTrabalho;
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "maismedicos.ws_tutores";
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "wstid" );
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos     = array(
								  'wstid'=> null,
								  'wstdata'=> null,
								  'noTutor'=> null,
								  'nuCpf'=> null,
								  'deEmail'=> null,
								  'nuFone'=> null,
								  'deLogradouro'=> null,
								  'deBairro'=> null,
								  'deComplemento'=> null,
								  'nuCep'=> null,
								  'ibgeTutor'=> null,
								  'ufTutor'=> null,
								  'municTutor'=> null,
								  'noFormacaoProfissional'=> null,
								  'dsTitulacao'=> null,
								  'dsComplementoFormacao'=> null,
								  'idInstituicao'=> null,
								  'nuCnpjInstituicao'=> null,
								  'noInstituicao'=> null,
								  'municInstituicao'=> null,
								  'ufInstituicao'=> null,
								  'noMae'=> null,
								  'dtNascimento'=> null,
								  'bancoComum_agenciaConta'=> null,
								  'bancoComum_dvAgenciaConta'=> null,
								  'bancoComum_contaCorrente'=> null,
								  'bancoComum_dvContaCorrente'=> null,
								  'bancoComum_bancoConta'=> null,
								  'bancoComum_municipioConta'=> null,
								  'bancoComum_ufConta'=> null,
								  'bancoBolsa_agenciaContaBolsa'=> null,
								  'bancoBolsa_dvAgenciaContaBolsa'=> null,
								  'bancoBolsa_bancoContaBolsa'=> null,
								  'bancoBolsa_municipioContaBolsa'=> null,
								  'bancoBolsa_ufContaBolsa'=> null,
								  'situacao'=> null,
								  'validado'=> null,
								  'wststatus'=> null,
								  
	);
	
    public function getSupervisoresWS()
    {
 		$conexao_ws = $this->conexaoWS();
		$this->arrSupervisores = $conexao_ws->getSupervisores(self::TOKEN_WS,4);
		return $this->arrSupervisores;
    }
    
    public function antigoGetSupervisoresWS()
    {
 		$conexao_ws = $this->conexaoAntigaWS();
		return $conexao_ws->getSupervisores(self::TOKEN_WS,4);
    }
    
    public function getTutoresWS()
    {
 		$conexao_ws = $this->conexaoWS();
		$this->arrTutores = $conexao_ws->getTutores(self::TOKEN_WS,4);
		return $this->arrTutores;
    }
    
    public function antigoGetTutoresWS()
    {
 		$conexao_ws = $this->conexaoAntigaWS();
		return $conexao_ws->getTutores(self::TOKEN_WS,4);
    }
    
    public function getPlanoTrabalhoWS()
    {
    	$mes = $mes ? (int)$mes : (int)date("n");
    	$ano = $ano ? (int)$ano : (int)date("Y");
    	
    	$conexao_ws = $this->conexaoWS();
    	$this->arrPlanosTrabalho = $conexao_ws->getPlanoTrabalho(self::TOKEN_WS,4,$mes,$ano);
    	return $this->arrPlanosTrabalho;
    }
    
    public function getValidacaoPagamentoWS($mes = null, $ano = null,$tipo = null)
    {
    	$mes = $mes ? (int)$mes : (int)date("n");
    	$ano = $ano ? (int)$ano : (int)date("Y");
    	
 		$this->arrAutorizacaoPagamento = $this->conexaoWS()->getValidacaoPagamento(self::TOKEN_WS,$tipo,$mes,$ano);
    }
    
    public function antigoGetAutorizacaoPagamentoWS($mes = null, $ano = null)
    {
    	$mes = $mes ? (int)$mes : (int)date("n");
    	$ano = $ano ? (int)$ano : (int)date("Y");
 		$this->arrAutorizacaoPagamento = $this->conexaoAntigaWS()->getAutorizacaoPagamento(self::TOKEN_WS,$mes,$ano);
    }
    
    public function atualizaTutoresMaisMedicos()
    {
    	$this->importaTutoresMaisMedicos();
    	
    	$sql = "select un.uniid, 'T', nucpf, notutor, dtnascimento::timestamp, deemail, '('||substr(nufone,1,2)||')'||substr(nufone,3,8) as fone, 
				nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa) as agencia,'', validado 
				from maismedicos.ws_tutores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgetutor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and nucpf in ( select tutcpf from maismedicos.tutor where tuttipo = 'T') 
				and to_char(wstdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wststatus = 'A';";
    	$arrTutoresAtualizados = $this->carregarColuna($sql);
    	$msg = "<p>".count($arrTutoresAtualizados)." tutor(es) atualizado(s)</p>";
    	

    	
    	$sql = "select    un.uniid, 'T', nucpf, notutor, dtnascimento::timestamp, deemail, '('||substr(nufone,1,2)||')'||substr(nufone,3,8) as fone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
						1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa), '', true 
				from maismedicos.ws_tutores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgetutor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A'  and validado is true and situacao = 1 and nucpf not in ( select tutcpf from maismedicos.tutor where tuttipo = 'T' )
				and to_char(wstdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD')
				and wststatus = 'A';";
    	$arrTutoresInseridos = $this->carregarColuna($sql);
    	$msg.= "<p>".count($arrTutoresInseridos)." tutor(es) inserido(s)</p>";
    	
    	$sql = "
				insert into maismedicos.tutor ( uniid, tuttipo, tutcpf, tutnome, tutdatanascimento, tutemail, tuttelefone, tutnomemae, tutcep, tutlogradouro, tutnumero, tutcomplemento, tutbairro, estuf, muncod, bncid, tutagencia, tutconta, tutvalidade )
				select    un.uniid, 'T', nucpf, notutor, dtnascimento::timestamp, deemail, '('||substr(nufone,1,2)||')'||substr(nufone,3,8) as fone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
						1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa), '', true 
				from maismedicos.ws_tutores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgetutor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A'  and validado is true  and situacao = 1 and nucpf not in ( select tutcpf from maismedicos.tutor where tuttipo = 'T' )
				and to_char(wstdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD')
				and wststatus = 'A';";
    	$this->executar($sql);
    	$this->commit();

    	$sql = "
				update maismedicos.tutor set 
					   uniid = a.uniid,
					   tutnome = a.notutor,
					   tutdatanascimento = a.dtnascimento,
					   tutemail = a.deemail,
					   tuttelefone = a.fone,
					   tutnomemae = a.nomae,
					   tutcep = a.nucep,
					   tutlogradouro = a.delogradouro,
					   tutnumero = a.decomplemento,
					   tutbairro = a.debairro,
					   estuf = a.estuf,
					   muncod = a.muncod,
					   bncid = 1,
					   tutagencia = a.agencia,
					   tutconta = '', 
					   tutvalidade = a.validado,
					   tutstatus = situacao
				from
				(select un.uniid, 'T', nucpf, notutor, dtnascimento::timestamp, deemail, '('||substr(nufone,1,2)||')'||substr(nufone,3,8) as fone, 
				nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa) as agencia,'', coalesce(validado, false) as validado,case when situacao = 1 then 'A' else 'I' end as situacao
				from maismedicos.ws_tutores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgetutor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and nucpf in ( select tutcpf from maismedicos.tutor where tuttipo = 'T') 
				and to_char(wstdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wststatus = 'A') a
				where tutcpf = a.nucpf and tuttipo = 'T';";
    	$this->executar($sql);
    	$this->commit();
    	
    echo $msg;
    
    $this->enviarEmailAuditoriaRotinas('Atualização do Cadastro de Tutores/Supervisores do Mais Médicos', $msg);
	
	return $msg;
    	
    }
    
    public function atualizaSupervisoresMaisMedicos()
    {
    	$this->importaSupervisoresMaisMedicos();
    	
    	$sql = "select un.uniid, 'S', nucpf, nosupervisor, dtnascimento::timestamp, deemail, defone as fone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa) as agencia, '', validado 
				from maismedicos.ws_supervisores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgesupervisor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and nucpf in ( select tutcpf from maismedicos.tutor where tuttipo = 'S' )
    			and to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A'";
    	$arrSupervisoresAtualizados = $this->carregarColuna($sql);
    	$msg = "<p>".count($arrSupervisoresAtualizados)." supervisor(es) atualizado(s)</p>";
    	
    	
    	$sql = "select    un.uniid, 'S', nucpf, nosupervisor, dtnascimento::timestamp, deemail, defone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa), '', validado 
				from maismedicos.ws_supervisores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgesupervisor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and validado is true and situacao = 1 and nucpf not in ( select tutcpf from maismedicos.tutor where tuttipo = 'S' )
				and to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD')
    			and wssstatus = 'A';";
    	$arrSupervisoresInseridos = $this->carregarColuna($sql);
    	$msg.= "<p>".count($arrSupervisoresInseridos)." supervisor(es) inserido(s)";
    	
    	$sql = "
				insert into maismedicos.tutor ( uniid, tuttipo, tutcpf, tutnome, tutdatanascimento, tutemail, tuttelefone, tutnomemae, tutcep, tutlogradouro, tutnumero, tutcomplemento, tutbairro, estuf, muncod, bncid, tutagencia, tutconta, tutvalidade )
				select    un.uniid, 'S', nucpf, nosupervisor, dtnascimento::timestamp, deemail, defone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				                1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa), '', validado 
				from maismedicos.ws_supervisores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgesupervisor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and validado is true and  situacao = 1 and nucpf not in ( select tutcpf from maismedicos.tutor where tuttipo = 'S' )
				and to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD')
    			and wssstatus = 'A';";
    	$this->executar($sql);
    	$this->commit();

    	$sql = "
				update maismedicos.tutor set 
					uniid = a.uniid,
					tutnome = a.nosupervisor,
					tutdatanascimento = a.dtnascimento,
					tutemail = a.deemail,
					tuttelefone = a.fone,
					tutnomemae = a.nomae,
					tutcep = a.nucep,
					tutlogradouro = a.delogradouro,
					tutnumero = a.decomplemento,
					tutbairro = a.debairro,
					estuf = a.estuf,
					muncod = a.muncod,
					bncid = 1,
					tutagencia = a.agencia,
					tutconta = '', 
					tutvalidade = a.validado,
					tutstatus = situacao
				from
				(select un.uniid, 'S', nucpf, nosupervisor, dtnascimento::timestamp, deemail, defone as fone, nomae, nucep, delogradouro, decomplemento, '', debairro, m.estuf, m.muncod,
				1, trim(bancobolsa_agenciacontabolsa)||'-'||trim(bancobolsa_dvagenciacontabolsa) as agencia, '', coalesce(validado, false) as validado ,case when situacao = 1 then 'A' else 'I' end as situacao
				from maismedicos.ws_supervisores u
				inner join territorios.municipio m ON substr(m.muncod ,1,6) = u.ibgesupervisor::text
				inner join maismedicos.universidade un ON un.idunasus = u.idinstituicao::integer
				where un.unistatus = 'A' and nucpf in ( select tutcpf from maismedicos.tutor where tuttipo = 'S' )
    			and to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A') a
				where tutcpf = a.nucpf and tuttipo = 'S';";
    	
    	$this->executar($sql);
    	$this->commit();

    	echo $msg;
    	
    	$this->enviarEmailAuditoriaRotinas('Atualização do Cadastro de Tutores/Supervisores do Mais Médicos', $msg);
	
		return $msg;
    }
    
    public function atualizaPlanoTrabalhoMaisMedicos()
    {
    	return $this->importaPlanoTrabalhoMedicos();
    }
    
    public function importaTutoresMaisMedicos()
    {
    	$this->getTutoresWS();
    	if($this->arrTutores)
    	{
    		//Inativa os dados coletados no mesmo dia
    		$this->inativaTutoresCadastradosHoje();
    		
    		$arrTutCadastrados = $this->recuperaTutoresCadastradosHoje();
    		
    		foreach($this->arrTutores->item as $arrTut)
    		{
    			//Transforma o objeto em array
    			$arrTut = (array)$arrTut;
    			
    			if($arrTut['bancoComum'])
    			{
    				$arrTut['bancoComum'] = (array) $arrTut['bancoComum'];
    				$arrTut['bancoComum']['item'] = (array) $arrTut['bancoComum']['item'];
    				foreach($arrTut['bancoComum']['item'] as $chave => $valor)
    				{
    					$arrTut['bancoComum_'.$chave] = $valor;
    				}	
    			}
    			
    			if($arrTut['bancoBolsa'])
    			{
    				$arrTut['bancoBolsa'] = (array) $arrTut['bancoBolsa'];
    				$arrTut['bancoBolsa']['item'] = (array) $arrTut['bancoBolsa']['item'];
    				foreach($arrTut['bancoBolsa']['item'] as $chave => $valor)
    				{
    					$arrTut['bancoBolsa_'.$chave] = $valor;
    				}	
    			}
    			
    			if(!$arrTut['validado'])
    			{
    				unset($arrTut['validado']);
    			}
    			    			
    			//Evita que os tutores sejam cadastrados mais de uma vez no mesmo dia
    			if($arrTutCadastrados[$arrTut['idinstituicao']])
    			{
    				if( !in_array( $arrTut['nuCpf'] , $arrTutCadastrados[$arrTut['idInstituicao']] ) )
    				{
    					$n = new Ws_Tutor();
    					$n->popularDadosObjeto($arrTut);
    					if($n->salvar())
    						$n->commit();
    				}
    			}else{
	    			$n = new Ws_Tutor();
	    			$n->popularDadosObjeto($arrTut);
	    			if($n->salvar())
	    				$n->commit();
    			}
    		}
    		//Atualiza os não validados para false
    		//$this->invalidaTutores();
    	}
    }
    
    public function importaSupervisoresMaisMedicos()
    {
    	$this->getSupervisoresWS();
    	if($this->arrSupervisores)
    	{
    		//Inativa os dados coletados no mesmo dia
    		$this->inativaSupervisoresCadastradosHoje();
    		
    		$arrSupCadastrados = $this->recuperaSupervisoresCadastradosHoje();
    		
    		foreach($this->arrSupervisores->item as $arrSup)
    		{
    			
    			//Transforma o objeto em array
    			$arrSup = (array)$arrSup;
    			 
    			if($arrSup['bancoComum'])
    			{
    				$arrSup['bancoComum'] = (array) $arrSup['bancoComum'];
    				$arrSup['bancoComum']['item'] = (array) $arrSup['bancoComum']['item'];
    				foreach($arrSup['bancoComum']['item'] as $chave => $valor)
    				{
    					$arrSup['bancoComum_'.$chave] = $valor;
    				}
    			}
    			 
    			if($arrSup['bancoBolsa'])
    			{
    				$arrSup['bancoBolsa'] = (array) $arrSup['bancoBolsa'];
    				$arrSup['bancoBolsa']['item'] = (array) $arrSup['bancoBolsa']['item'];
    				foreach($arrSup['bancoBolsa']['item'] as $chave => $valor)
    				{
    					$arrSup['bancoBolsa_'.$chave] = $valor;
    				}
    			}
    			
    			if(!$arrSup['validado'])
    			{
    				unset($arrSup['validado']);
    			}
    			
    			//remove o tutorResponsavel
    			unset($arrSup['tutorResponsavel']);
    			
    			//Evita que os supervisores sejam cadastrados mais de uma vez no mesmo dia
    			if($arrSupCadastrados[$arrSup['idinstituicao']])
    			{
    				if( !in_array( $arrSup['nuCpf'] , $arrSupCadastrados[$arrSup['idInstituicao']] ) )
    				{
    					$n = new Ws_Supervisor();
    					$n->popularDadosObjeto($arrSup);
    					if($n->salvar())
    						$n->commit();
    				}
    			}else{
	    			$n = new Ws_Supervisor();
	    			$n->popularDadosObjeto($arrSup);
	    			if($n->salvar())
	    				$n->commit();
    			}
    		}
    		
    		//Atualiza os não validados para false
    		//$this->invalidaSupervisores();
    	}
    }
    
    private function importaPlanoTrabalhoMedicos()
    {
    	
    	include_once APPRAIZ.'maismedicos/classes/Ws_Planotrabalho.class.inc';
    	include_once APPRAIZ.'maismedicos/classes/Ws_Planotrabalho_Itens.class.inc';
    	 
    	$this->getPlanoTrabalhoWS();
    	
    	if($this->arrPlanosTrabalho){
    		
    		$n = new Ws_Planotrabalho();
    		$r = new Ws_Planotrabalho_Itens();
    	
    		// INATIVA INSTITUICOES ANTES DE INSERIR CARGA NOVA DO WS
    		$this->inativaPlanosTrabalhoCadastrados();
    		
    		$countItens=0;
    		$countPlanos=0;
    		$countNaoLocalizados=0;
    		foreach($this->arrPlanosTrabalho->item as $obPtr){
    			
    			$arPtr['ptrmes'] = $obPtr->mes;
    			$arPtr['ptrano'] = $obPtr->ano;
    			$arPtr['ptrinstituicao'] = $obPtr->instituicao;
    			$arPtr['observacao'] = $obPtr->observacao;
    			
    			$n->popularDadosObjeto($arPtr);
    			$idPtr = $n->salvar();
    			$n->clearDados();
    			++$countPlanos;
    			 
    			if($idPtr){
    				
    				if($obPtr->profissionalNaoLocalizado->item){
    					
    					foreach($obPtr->profissionalNaoLocalizado->item as $obNaoLocalizado){
    							
    						$arNaoLocalizado['cpfProfissional'] = $obNaoLocalizado->cpfProfissional;
    						$arNaoLocalizado['nomeProfissional'] = $obNaoLocalizado->nomeProfissional;
    						$arNaoLocalizado['ibge'] = $obNaoLocalizado->ibge;
    						$arNaoLocalizado['naolocalizado'] = true;
    						$arNaoLocalizado['ptrid'] = $idPtr;
    							
    						$r->popularDadosObjeto($arNaoLocalizado);
    						$r->salvar();
    						$r->clearDados();
    						++$countNaoLocalizados;
    							
    					}
    				}
    	
    				if($obPtr->itemPlanoTrabalho->item){
    	
    					foreach($obPtr->itemPlanoTrabalho->item as $obItem){
    	
    							$arItem['cpfProfissional'] = $obItem->cpfProfissional;
    							$arItem['nomeProfissional'] = $obItem->nomeProfissional;
    							$arItem['situacao'] = $obItem->situacao;
    							$arItem['ibge'] = $obItem->ibge;    							
    							$arItem['municipio'] = $obItem->municipio;
    							$arItem['uf'] = $obItem->uf;
    							$arItem['cpfTutor'] = $obItem->cpfTutor;
    							$arItem['nomeTutor'] = $obItem->nomeTutor;
    							$arItem['cpfSupervisor'] = $obItem->cpfSupervisor;
    							$arItem['nomeSupervisor'] = $obItem->nomeSupervisor;
    							$arItem['justificativaProfissional'] = $obItem->justificativaProfissional;
    							$arItem['ptrid'] = $idPtr;
    							     							
    							$r->popularDadosObjeto($arItem);
    							$r->salvar();
    							$r->clearDados();
    							++$countItens;
    								
    					}
    				}
    			}
    		}
    		
    		$r->commit();
    		$n->commit();    		
    	}
    	
    	$txt = $countPlanos.' planos de trabalho carregados.<br/>';
    	$txt .= $countItens.' itens dos planos de trabalho.<br/>';
    	$txt .= $countNaoLocalizados.' profissionais não localizado.<br/>';
    	return $txt;
    }
    
    
    private function recuperaTutoresCadastradosHoje()
    {
    	$sql = "select nuCpf, idinstituicao from maismedicos.ws_tutores 
    			where wststatus = 'A' ";
    	$arrDados = $this->carregar($sql);
    	if($arrDados){
    		foreach($arrDados as $d)
    		{
    			$arrTut[$d['idinstituicao']][] = $d['nucpf'];
    		}
    	}
    	return $arrTut ? $arrTut : array();
    }
    
    
    private function inativaTutoresCadastradosHoje()
    {
    	$sql = "update maismedicos.ws_tutores set wststatus = 'I'
    			where to_char(wstdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wststatus = 'A' ";
    	$this->executar($sql);
    	$this->commit();
    }
    
    private function inativaSupervisoresCadastradosHoje()
    {
    	$sql = "update maismedicos.ws_supervisores set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A' ";
    	$this->executar($sql);
    	$this->commit();
    }
    
    private function inativaPlanosTrabalhoCadastrados()
    {
    	$sql = "update maismedicos.ws_planotrabalho set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A';
    			update maismedicos.ws_planotrabalho_itens set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A';";
    	$this->executar($sql);
    	$this->commit();
    }
    
    
    private function inativaAutorizacaoPagamentoCadastradaHoje($tipo)
    {
    	$sql = "update maismedicos.ws_pagamento set wspstatus = 'I'
    			where to_char(wspdata,'YYYY-MM') = to_char(now(),'YYYY-MM') and wspstatus = 'A' and tipoBolsista = '$tipo'";
    	
    	$this->executar($sql);
    	
    	$this->commit();
    }
    
    private function recuperaSupervisoresCadastradosHoje()
    {
    	$sql = "select nucpf, idinstituicao from maismedicos.ws_supervisores 
    			where wssstatus = 'A' ";
    	$arrDados = $this->carregar($sql);
    	if($arrDados){
    		foreach($arrDados as $d)
    		{
    			$arrSup[$d['idinstituicao']][] = $d['nucpf'];
    		}
    	}
    	return $arrSup ? $arrSup : array();
    }
    
    private function recuperaAutorizacaoPagamentoCadastradaHoje($tipo)
    {
    	$sql = "select mesReferencia, anoReferencia, cpfBolsista from maismedicos.ws_pagamento 
    			where to_char(wspdata,'YYYY-MM') = to_char(now(),'YYYY-MM') and wspstatus = 'A' and tipoBolsista = '$tipo'";
    	
    	$arrDados = $this->carregar($sql);
    	
    	if($arrDados){
    		foreach($arrDados as $d)
    		{
    			$arrAut[$d['anoreferencia'].$d['mesreferencia']][] = $d['cpfolsista'];
    		}
    	}
    	
    	return $arrAut ? $arrAut : array();
    }
    
    public function autorizaPagamento($mes = null,$ano = null,$tipo = null)
    {
    	$mes = $mes ? (int)$mes : (int)$_POST['mes'];
    	$ano = $ano ? (int)$ano : (int)$_POST['ano'];
    	$tipo = $tipo ? $tipo : $_POST['tipo'];
    	$this->getValidacaoPagamentoWS($mes,$ano,$tipo);
    	    	
    	//Trata o mês < 10    	
    	$mes_tbl_pag = strlen($mes) < 2 ? "0".$mes : $mes;
		
	    if($this->arrAutorizacaoPagamento->item)
	    {
	    		    	
	    	// Instancia classe do ws de pagamentos
	    	$n = new Ws_Pagamento();
	    	
    		//Inativa os dados coletados no mesmo dia
    		$this->inativaAutorizacaoPagamentoCadastradaHoje($tipo);
    		
    		// Recupera autorizacoes ja cadastradas
    		$arrAutCadastrados = $this->recuperaAutorizacaoPagamentoCadastradaHoje($tipo);
    		
    		foreach($this->arrAutorizacaoPagamento->item as $arrAut)
    		{
    			$arrAut = (array) $arrAut;
    			if($arrAut['nomeSupervisor'] && $arrAut['cpfSupervisor']){
    				
	    			//Evita que mais de uma autorização de pagamento seja cadastrada no mesmo dia
	    			if($arrAutCadastrados[$arrAut['anoreferencia'].$arrAut['mesreferencia']])
	    			{
	    				// Se o cpf do bolsista ja existir nao insere
	    				if( !in_array( $arrAut['cpfbolsista'] , $arrAutCadastrados[$arrAut['anoreferencia'].$arrAut['mesreferencia']] ) )
	    				{
	    					
	    					$arrAut['cpfBolsista'] = $arrAut['cpfSupervisor'];
	    					$arrAut['nomeBolsista'] = $arrAut['nomeSupervisor'];
	    					$arrAut['autorizado'] = !$arrAut['autorizado'] ? "0" : $arrAut['autorizado'];
	    					$arrAut['tipoBolsista'] = $tipo;
	    					$n->popularDadosObjeto($arrAut);
	    					$n->salvar();
	    					$n->clearDados();
	    						
	    				}
	    				
	    			}else{
		    			
		    			$arrAut['cpfBolsista'] = $arrAut['cpfSupervisor'];
		    			$arrAut['nomeBolsista'] = $arrAut['nomeSupervisor'];
		    			$arrAut['autorizado'] = !$arrAut['autorizado'] ? "0" : $arrAut['autorizado'];
		    			$arrAut['tipoBolsista'] = $tipo;
		    			$n->popularDadosObjeto($arrAut);
		    			$n->salvar();
		    			$n->clearDados();
	    				
	    			}
    			}
    		}
    		
    		$n->commit();
	    		
    		//Alimenta a tabela de autorização de pagamento
    		$sql = "/* Atentar para o Mês e Ano de Referência */
					update maismedicos.autorizacaopagamento set apgstatus = 'I' where apgmes = '$mes_tbl_pag' and apgano = '$ano' and rmdid is null and apgstatus = 'A' and apgtipo = '$tipo' and apgenviado = false
					and apgcpf not in (
						select d.nu_cpf from maismedicos.folhapagamento f
						join maismedicos.remessacabecalho c on f.fpgid = c.fpgid
						join maismedicos.remessadetalhe d on d.rmcid = c.rmcid 
						join maismedicos.tutor t on t.tutcpf = d.nu_cpf
						where substr(fpgmes, 1, 2) = '$mes_tbl_pag' and substr(fpgmes, 3, 4) = '$ano'
						and fpgstatus = 'A'
						and tuttipo = '$tipo'
					);
					
					insert into maismedicos.autorizacaopagamento ( apgcpf, apgnome, apgtipo, apgmes, apgano, apgcpfresponsavel, apgnomeresponsavel, apgstatus, apgjustificativa )
					select cpfbolsista, nomebolsista, tipobolsista, '$mes_tbl_pag', anoReferencia, cpfAutorizador, nomeAutorizador, 
					                Case when autorizado is true then 'A' else 'I' end as apgstatus, descricao
					from maismedicos.ws_pagamento where mesReferencia = '$mes' and anoReferencia = '$ano' and wspstatus = 'A' and tipobolsista = '$tipo' 
    				and cpfbolsista not in (
    					select apgcpf from maismedicos.autorizacaopagamento 
    					where apgmes = '$mes_tbl_pag' and apgano = '$ano' 
    					and apgstatus = 'A' and apgtipo = '$tipo' 
    					and rmdid is null and apgenviado = false)    				
    				and cpfbolsista not in (
						select d.nu_cpf from maismedicos.folhapagamento f
						join maismedicos.remessacabecalho c on f.fpgid = c.fpgid
						join maismedicos.remessadetalhe d on d.rmcid = c.rmcid 
						join maismedicos.tutor t on t.tutcpf = d.nu_cpf
						where substr(dt_ini_periodo,1,4) = '$ano' and substr(dt_ini_periodo,5,2) = '$mes_tbl_pag'
						and fpgstatus = 'A'
						and tuttipo = '$tipo'
					);";
    		
// 	    		ver($sql, d);

			$this->executar($sql);
			$this->commit();
			
			$_SESSION['maismedicos']['ws_tutor']['alert'] = "Operação realizada com sucesso.";
			
    	}else{
    		
    		$_SESSION['maismedicos']['ws_tutor']['alert'] = "Não foi possível realizar a operação.";
    		
    	}
    	
    	header("Location: maismedicos.php?modulo=principal/importarAutorizacaoPagamento&acao=A");
    	exit;
    } 
     
    
    public function invalidaTutores()
    {
    	$sql = "update maismedicos.ws_tutores set validado = false where validado is null";
    	$this->executar($sql);
    	$this->commit();
    }
    
    public function invalidaSupervisores()
    {
    	$sql = "update maismedicos.ws_supervisores set validado = false where validado is null";
    	$this->executar($sql);
    	$this->commit();
    }
    
   
    
}