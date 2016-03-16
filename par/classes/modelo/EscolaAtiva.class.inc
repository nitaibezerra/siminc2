<?php
	
class Escolaativa extends Modelo{

	public function __construct( $esaid = null, $inuid = null ){
		
		parent::__construct();

		if( !$esaid && $inuid ){
			$esaid = $this->pegaUm( "select esaid from par.escolaativa where inuid = $inuid and esaano = 2012" );
		}
		
		if( $esaid ){
			$this->carregarPorId( $esaid );
		}
	}
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.escolaativa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "esaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'esaid' => null, 
									  	'inuid' => null, 
									  	'esaano' => null, 
									  	'entid' => null, 
									  	'esaqtdtecnicos' => null, 
									  	'esaqtdprofessores' => null, 
									  	'docid' => null,
    									'usucpfescolaativa' => null,
    									'esasituacaoadesao' => null, 
									  );
									  
	public function recuperarCoEscolas(){
		
		$sql = "select eaeid, eae.entid, e.entnome, e.entcodent, eaeqtdpredios, eaeqtdturmas, eaeqtdalunos1, eaeqtdalunos2, eaeqtdalunos3, eaeqtdalunos4, eaeqtdalunos5
				from par.escolaativaescolas eae
					inner join entidade.entidade e on e.entid = eae.entid
				where esaid = {$this->esaid} 
				order by e.entnome";
		
		$coEscolaAtivaEscolas = $this->carregar( $sql );
		$coEscolaAtivaEscolas = $coEscolaAtivaEscolas ? $coEscolaAtivaEscolas : array();
		
		return $coEscolaAtivaEscolas;											  
	}
	
	public function podeEditarEscolaAtiva(){
		
		if( $this->testa_superuser() ){ return true; }
		/*
		$documento = wf_pegarEstadoAtual( $this->docid );
		
		$esdid = (integer) $documento['esdid'];
		
		switch ( $esdid ) {
			case CTE_ESTADO_ESCOLA_ATIVA_EM_CORRECAO:
			case CTE_ESTADO_ESCOLA_ATIVA_EM_PREENCHIMENTO:
				$perfis = array( CTE_PERFIL_EQUIPE_MUNICIPAL, 
								 CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, 
								 CTE_PERFIL_EQUIPE_ESTADUAL_CADASTRO, 
								 CTE_PERFIL_EQUIPE_LOCAL_APROVACAO );
				break;
			case CTE_ESTADO_ESCOLA_ATIVA_EM_ANALISE:
			case CTE_ESTADO_ESCOLA_ATIVA_EM_REVISAO:
				$perfis = array( CTE_PERFIL_EQUIPE_ESCOLA_ATIVA );
				break;
			default:
				$perfis = array();
				break;
		}
		return cte_possuiPerfil( $perfis ) && date( 'Ymd' ) <= date(Ymd);
		*/
		
		return true;
	}
	
	public function verificarDocumentoEscolaAtiva(){
	
		if ( !$this->docid ){

			// verifica se é municipal ou estadual para gerar o nome do documento
			$estuf = cte_pegarEstuf( $this->inuid );
			$muncod = cte_pegarMuncod( $this->inuid );
			
			if( $estuf ){
				$sqlDescricao = "select estdescricao
								 from territorios.estado
								 where estuf = '" . $estuf . "'";
			}
			else{
				$sqlDescricao = "select mundescricao
								 from territorios.municipio
								 where muncod = '" . $muncod . "'";
			}
			$descricao = $this->pegaUm( $sqlDescricao );
			$docdsc = "Escola Ativa - " . $descricao;
			
			// cria documento
			$docid = wf_cadastrarDocumento( CTE_TIPO_DOCUMENTO_ESCOLA_ATIVA, $docdsc );
			
			if( $docid ){
				$this->docid = $docid;
				$this->salvar(); 
				$this->commit(); 
			}
		}
	}
	
	public function verificarPreenchimento(){

		if( !$this->esaqtdtecnicos || !$this->esaqtdprofessores ){
			return false;
		}
		else{
			$obEscolaAtivaEscolas = new EscolaAtivaEscolas();
			$coEscolaAtivaEscolas = $obEscolaAtivaEscolas->recuperarTodos( "*", array( "esaid = {$this->esaid}" ) );

			if( count( $coEscolaAtivaEscolas ) ){
				foreach( $coEscolaAtivaEscolas as $arEscolaAtivaEscolas ){
					
					$nrTotalAlunos = $arEscolaAtivaEscolas["eaeqtdalunos1"] + $arEscolaAtivaEscolas["eaeqtdalunos2"] + $arEscolaAtivaEscolas["eaeqtdalunos3"] + $arEscolaAtivaEscolas["eaeqtdalunos4"] + $arEscolaAtivaEscolas["eaeqtdalunos5"];
					if( !$nrTotalAlunos || !$arEscolaAtivaEscolas["eaeqtdpredios"] || !$arEscolaAtivaEscolas["eaeqtdturmas"] || ( $arEscolaAtivaEscolas["eaeqtdpredios"] > $arEscolaAtivaEscolas["eaeqtdturmas"] ) ){
						return false;
					}
				}
			}
			else{
				return false;				
			}
		}
		return true;
	}
	
	public function gerarSubacoesEscolaAtivaAnalisadas(){

		// Subação Professores
		$this->gerarSubacoesGlobais( CTE_TIPO_SUBACAO_PROFESSOR );
		
		// Subação Técnicos
		$this->gerarSubacoesGlobais( CTE_TIPO_SUBACAO_TECNICO );
		
		// Subação Prédios
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_PREDIO );
		
		// Subação Turmas
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_TURMA );
		
		// Subação Alunos 1° Ano
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_ANO_1 );
		
		// Subação Alunos 2° Ano
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_ANO_2 );
		
		// Subação Alunos 3° Ano
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_ANO_3 );
		
		// Subação Alunos 4° Ano
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_ANO_4 );
		
		// Subação Alunos 5° Ano
		$this->gerarSubacoesPorEscola( CTE_TIPO_SUBACAO_ANO_5 );
		
	}
	
	public function gerarSubacoesGlobais( $csTipo ){
		
		if( $csTipo == CTE_TIPO_SUBACAO_PROFESSOR ){
			$stPpsid = "1042, 1038, 88, 1040, 89, 1065";
			$sptunt = $this->esaqtdprofessores;
		}
		elseif( $csTipo == CTE_TIPO_SUBACAO_TECNICO ){
			$stPpsid = "1041, 818, 1039, 1037, 467";
			$sptunt = $this->esaqtdtecnicos;
		}
		
		$sql = "select sbaid 
				from cte.instrumentounidade iu
					inner join cte.pontuacao p on p.inuid = iu.inuid 
					inner join cte.acaoindicador a on a.ptoid = p.ptoid 
					inner join cte.subacaoindicador s on s.aciid = a.aciid
				where p.ptostatus = 'A'
				and iu.inuid = {$this->inuid}
				and s.ppsid in ( $stPpsid ) ";
		
		$sbaid = $this->pegaUm( $sql );				
		if( $sbaid ){

			$sql = "update cte.subacaoindicador set sbasituacaoarvore = 3 where sbaid = $sbaid";
			$this->executar( $sql );	
			$this->commit();	
			
			$sql = "select spt.sptid
					from cte.subacaoindicador s
						inner join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid
					where s.sbaid = $sbaid
					and sptano = 2011";
			
			$sptid = $this->pegaUm( $sql );

			if( $sptid ){
				$sql = "update cte.subacaoparecertecnico 
						set sptinicio = 1, 
						    sptfim = 12, 
						    ssuid = 3,
						    sptparecer = 'Aprovado.',
						    sptunt = $sptunt
						where sptid = $sptid";
			}
			else{
				$sql = "insert into cte.subacaoparecertecnico ( sbaid, sptunt, sptano, sptinicio, sptfim, tppid, ssuid, sptparecer )
														values( $sbaid, $sptunt, 2011, 1, 12, 4, 3, 'Aprovado.' ) returning sptid";
			}
			
			$this->executar( $sql );	
			$this->commit();	
		}	
	}
	
	public function gerarSubacoesPorEscola( $csTipo ){
		
		if( $csTipo == CTE_TIPO_SUBACAO_PREDIO ){
			$stPpsid = "1024, 278, 1016, 1023, 448, 449";
			$qfaqtd = "eaeqtdpredios";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_TURMA ){
			$stPpsid = "1045, 1046, 1047, 1048, 1044";
			$qfaqtd = "eaeqtdturmas";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_ANO_1 ){
			$stPpsid = "273, 1028, 411, 410, 1018, 1027";
			$qfaqtd = "eaeqtdalunos1";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_ANO_2 ){
			$stPpsid = "1019, 1029, 274, 1030, 412, 413";
			$qfaqtd = "eaeqtdalunos2";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_ANO_3 ){
			$stPpsid = "275, 414, 1031, 1032, 415, 1020";
			$qfaqtd = "eaeqtdalunos3";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_ANO_4 ){
			$stPpsid = "276, 1033, 1034, 417, 416, 1021";
			$qfaqtd = "eaeqtdalunos4";
		}		
		elseif( $csTipo == CTE_TIPO_SUBACAO_ANO_5 ){
			$stPpsid = "418, 1035, 277, 1036, 419, 1022";
			$qfaqtd = "eaeqtdalunos5";
		}		
		
		$sql = "select sbaid 
				from cte.instrumentounidade iu
					inner join cte.pontuacao p on p.inuid = iu.inuid 
					inner join cte.acaoindicador a on a.ptoid = p.ptoid 
					inner join cte.subacaoindicador s on s.aciid = a.aciid
				where p.ptostatus = 'A'
				and iu.inuid = {$this->inuid}
				and s.ppsid in ( $stPpsid ) ";
				
		$sbaid = $this->pegaUm( $sql );				
		if( $sbaid ){

			$sql = "update cte.subacaoindicador set sbasituacaoarvore = 3 where sbaid = $sbaid";
			$this->executar( $sql );	
			$this->commit();
			
			$sql = "delete from cte.qtdfisicoano 
					where sbaid = $sbaid
					and qfaano = 2011";		
					
			$this->executar( $sql );
			
			$coEscolaAtivaEscolas = $this->recuperarCoEscolas();
			
			foreach( $coEscolaAtivaEscolas as $arEscolaAtivaEscolas ){
				if( $arEscolaAtivaEscolas[$qfaqtd] ){
					
					$sql = "insert into cte.qtdfisicoano ( sbaid, qfaano, qfaqtd, entid )
			       								   values( $sbaid, 2011, {$arEscolaAtivaEscolas[$qfaqtd]}, {$arEscolaAtivaEscolas["entid"]} )";
			       								   
					$this->executar( $sql );	
				}
			}
			
			$this->commit();	
		}
	}
	
	public function recuperarNomeEntidade(){
		
		if( !$this->entid ) return "";
		
		$sql = "select entnome from entidade.entidade where entid = $this->entid ";
		return $this->pegaUm( $sql );
	}
	
	public function excluirAdesao(){

		$sql = "delete from cte.subacaoparecertecnico
				where sbaid in( 
					select s.sbaid
					from cte.instrumentounidade iu
						inner join cte.pontuacao p on p.inuid = iu.inuid and p.ptostatus = 'A'
						inner join cte.acaoindicador a on a.ptoid = p.ptoid
						inner join cte.subacaoindicador s on s.aciid = a.aciid and ( s.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					where iu.inuid = {$this->inuid}
				)
				and sptano = 2011;
				
				delete 
				from cte.qtdfisicoano
				where sbaid in( 
					select s.sbaid
					from cte.instrumentounidade iu
						inner join cte.pontuacao p on p.inuid = iu.inuid and p.ptostatus = 'A'
						inner join cte.acaoindicador a on a.ptoid = p.ptoid
						inner join cte.subacaoindicador s on s.aciid = a.aciid and ( s.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					where iu.inuid = {$this->inuid}
				)
				and qfaano = 2011;
				
				delete from par.escolaativaescolas
				where esaid = {$this->esaid};
				
				delete from par.escolaativa
				where esaid = {$this->esaid};
				
				update cte.instrumentounidade 
					set inusituacaoadesao = {$_REQUEST["inusituacaoadesao"]},
					usucpfescolaativa = '{$_SESSION["usucpf"]}'
				where inuid = {$this->inuid};
		";
		
		$this->carregar( $sql );
		$this->commit();
	}
	
	public function aderirEscolaAtiva(){
		
		$sql = "select inuid from par.escolaativa where inuid = {$_REQUEST["inuid"]};";
		$inuid = $this->pegaUm( $sql );
		
		$sql = "update cte.instrumentounidade 
					set inusituacaoadesao = 1,
					usucpfescolaativa = '{$_SESSION["usucpf"]}'
				where inuid = {$_REQUEST["inuid"]};";

		$this->carregar( $sql );
		$this->commit();
		
		if( !$inuid ){
			$sql = "insert into par.escolaativa ( inuid, esaano ) values ( {$_REQUEST["inuid"]}, 2011 ) ";
		}
		
		$this->carregar( $sql );
		$this->commit();
	}
	
	public function excluirDadosSubacoesEscolaAtivaAnalisadas(){
		$sql = "delete from cte.subacaoparecertecnico
				where sbaid in( 
					select s.sbaid
					from cte.instrumentounidade iu
						inner join cte.pontuacao p on p.inuid = iu.inuid and p.ptostatus = 'A'
						inner join cte.acaoindicador a on a.ptoid = p.ptoid
						inner join cte.subacaoindicador s on s.aciid = a.aciid and ( s.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					where iu.inuid = {$this->inuid}
				)
				and sptano = 2011;
				
				delete 
				from cte.qtdfisicoano
				where sbaid in( 
					select s.sbaid
					from cte.instrumentounidade iu
						inner join cte.pontuacao p on p.inuid = iu.inuid and p.ptostatus = 'A'
						inner join cte.acaoindicador a on a.ptoid = p.ptoid
						inner join cte.subacaoindicador s on s.aciid = a.aciid and ( s.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					where iu.inuid = {$this->inuid}
				)
				and qfaano = 2011;
		";
				
		$this->executar( $sql );
		$this->commit();
	}
	
}