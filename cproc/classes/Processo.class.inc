<?php

/**
 * Classe Modelo DisponibilidadeVistas CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo DisponibilidadeVistas CPROC
 */
class Processo extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.processo";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("prcid");

	/**
	 * Status
	 * @name $status
	 * @var array
	 * @access protected
	 */
	protected $status = false;
	
	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
			"prcid" => null,
			"docid" => null,
			// "co_mantenedora" => null, // substituido por mntid
			"mntid" => null, // aponta para gestaodocumentos.mantenedoras
			// "co_ies" => null, // substituido por isesid
			"iesid" => null, // aponta para gestaodocumentos.instituicaoensino
			"muncod" => null,
			"estuf" => null,
			"tprid" => null,
			// "modid" => null,
			"prccurso" => null,
			"prclocaloferta" => null,
			// "prcpoloferta" => null,
			"prcnumsidoc" => null,
			"prcdtentradaproc" => null,
			"prcstatusead" => null,
			"prcassunto" => null,
			"prcdtinclusao" => null,
			"prcstatus" => null,
			"entid" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"prcid",
			"docid" => null,
			"mntid",
			"iesid",
			"muncod",
			"estuf",
			"tprid",
			// "modid",
			"prccurso",
			"prclocaloferta",
			// "prcpoloferta",
			"prcnumsidoc",
			"prcdtentradaproc",
			"prcstatusead",
			"prcassunto",
			"prcdtinclusao",
			"prcstatus",
			"entid"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"muncod",
			"estuf",
			"tprid",
			// "modid",
			"prcnumsidoc",
			"prcdtentradaproc",
			"prcstatusead",
			"prcdtinclusao",
			"prcstatus"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid"
		);

	/**
	 * Responsáveis pelo Processo
	 */
	protected $responsaveis = array(
			'coordenador' => '',
			'tecnico' => ''
		);

	/**
	 * Responsáveis pelo Processo
	 */
	protected $situacao = array(
			'fase' => '',
			'status' => ''
		);


	public function __construct( $id ){
		parent::__construct( $id );

		// $this->getResponsaveis();
		// $this->getSitacao();
	}

	/**
	 * Recupera Coordenador
	 */
	public static function resgataCoordenador( $prcid = false ){
		global $db;

		if( !$prcid ) 
			return;

		$sql = " SELECT 
					uc.usunome AS usunome
				 FROM cproc.coordenadoresponsavel cr
				 LEFT JOIN seguranca.usuario uc ON uc.usucpf = cr.usucpf
				 WHERE prcid = " . $prcid . "
				 ORDER BY cr.crpdthrdistribuicao DESC LIMIT 1 ";
		return $db->pegaUm( $sql );
	}
	
	/**
	 * Recupera o CPF do coordenador do processo
	 * @param string $prcid c�digo identificador do processo
	 */
	public static function resgataCpfCoordenador( $prcid = false ){
		global $db;
		
		if ( !$prcid )
			return;
		
		$sql = " SELECT
					uc.usucpf AS usucpf
				 FROM cproc.coordenadoresponsavel cr
				 LEFT JOIN seguranca.usuario uc ON uc.usucpf = cr.usucpf
				 WHERE prcid = " . $prcid . "
				 ORDER BY cr.crpdthrdistribuicao DESC LIMIT 1 ";
		return $db->pegaUm( $sql );
	}
	
	/**
	 * Recupera Tecnico
	 */
	public static function resgataTecnico( $prcid = false ){
		global $db;

		if( !$prcid ) 
			return;

		$sql = " SELECT 
					ut.usunome AS usunome
				 FROM cproc.tecnicoresponsavel tr
				 LEFT JOIN seguranca.usuario ut ON ut.usucpf = tr.usucpf
				 WHERE prcid = " . $prcid . " 
				 ORDER BY tr.trpdthrdesignacao DESC LIMIT 1 ";
		return $db->pegaUm( $sql );
	}
	
	/**
	 * Recupera o CPF do t�cnico do processo
	 * @param string $prcid c�digo identificador do processo
	 */
	public static function resgataCpfTecnico( $prcid = false ){
		global $db;
		
		if ( !$prcid )
			return;
		
		$sql = " SELECT 
					ut.usucpf AS usucpf
				 FROM cproc.tecnicoresponsavel tr
				 LEFT JOIN seguranca.usuario ut ON ut.usucpf = tr.usucpf
				 WHERE prcid = " . $prcid . " 
				 ORDER BY tr.trpdthrdesignacao DESC LIMIT 1 ";
				return $db->pegaUm( $sql );
			}
	
	/**
	 * Recupera Fase
	 */
	public static function resgataFase( $prcid = false ){
		global $db;

		if( !$prcid ) 
			return;

		$sql = " SELECT 
					f.fasdsc AS fasdsc
				 FROM cproc.faseprocesso fp
				 LEFT JOIN cproc.fase f ON f.fasid = fp.fasid
				 WHERE fp.prcid = " . $prcid . " 
				 ORDER BY fp.fprdthratribuicao DESC LIMIT 1 ";
		return $db->pegaUm( $sql );
	}

	/**
	 * Recupera Status
	 */
	public static function resgataStatus( $prcid = false ){
		global $db;

		if( !$prcid ) 
			return;

		$sql = " SELECT 
					s.stsdsc AS stsdsc
				 FROM cproc.statusprocesso sp
				 LEFT JOIN cproc.status s ON s.stsid = sp.stsid
				 WHERE sp.prcid = " . $prcid . " 
				 ORDER BY sp.stpdthratribuicao DESC LIMIT 1 ";
		return $db->pegaUm( $sql );
	}

	/**
	 * Recupera os Responsáveis
	 *
	 * @todo por ajustar
	 */
	public function getResponsaveis(){
		global $db;

		if( !$this->prcid )
			return $this;

		$sql = "SELECT
					(SELECT uc.usunome 
					FROM cproc.coordenadoresponsavel cr
					LEFT JOIN seguranca.usuario uc ON uc.usucpf = cr.usucpf
					WHERE prcid = prc.prcid) as coordenador,

					(SELECT ut.usunome
					FROM cproc.tecnicoresponsavel tr
					LEFT JOIN seguranca.usuario ut ON ut.usucpf = tr.usucpf
					WHERE prcid = prc.prcid) as tecnico
					
				FROM cproc.processo prc
				WHERE prc.prcid = " . $this->prcid . " ";
		$resultado = $db->carregar( $sql );
		if( $resultado[0]['coordenador'] || $resultado[0]['tecnico'] ){
			$this->responsaveis['coordenador'] = ( $resultado[0]['coordenador'] )? $resultado[0]['coordenador']:'' ;
			$this->responsaveis['tecnico'] = ( $resultado[0]['tecnico'] )? $resultado[0]['tecnico']:'' ;
		}

		return $this;
	}

	/**
	 * Recupera os Responsáveis
	 *
	 * @todo por ajustar
	 */
	public function getSitacao(){
		global $db;

		if( !$this->prcid )
			return $this;

		$sql = " SELECT
					f.fasdsc as fase,
					s.stsdsc as status
				 FROM cproc.processo prc
				 LEFT JOIN (SELECT MAX(fprid), prcid, fasid FROM cproc.faseprocesso WHERE prcid = prc.prcid) fp
				 LEFT JOIN cproc.fase f ON f.fasid = fp.fasid
				 LEFT JOIN (SELECT MAX(stpid), prcid, stsid FROM cproc.statusprocesso WHERE prcid = prc.prcid) sp
				 LEFT JOIN cproc.status s ON s.stsid = sp.stsid ";
		$resultado = $db->carregar( $sql );
		if( $resultado[0]['fase'] || $resultado[0]['status'] ){
			$this->situacao['fase'] = ( $resultado[0]['fase'] )? $resultado[0]['fase']:'' ;
			$this->situacao['status'] = ( $resultado[0]['status'] )? $resultado[0]['status']:'' ;
		}

		return $this;
	}

	/**
	 * Permissões para o ofrmulario cadastro de processo
	 */
	public static function permissoesFormularioDadosDoProcesso( $usucpf ){
		global $db;
		$habil = 'N';

		if( !$usucpf )
			return $habil;
		
		$sql = " SELECT 
					p.pflcod AS pflcod, 
					p.pfldsc AS perfil, 
					s.sisid AS sisid, 
					s.sisdsc AS sistema
				 FROM seguranca.perfilusuario pu
				 LEFT JOIN seguranca.perfil p ON p.pflstatus = 'A' AND p.pflcod = pu.pflcod
				 LEFT JOIN seguranca.sistema s ON s.sisid = p.sisid
				 WHERE pu.usucpf = '" . $usucpf . "' AND s.sisid = " . CPROC_SISID;
		$resposta = $db->carregar( $sql );
		
		foreach ($resposta as $key => $value) {

			if( 
				$value['pflcod'] == PERFIL_APOIO_CPROC 
				|| $value['pflcod'] == PERFIL_SUPER_USUARIO  ){
				$habil = 'S';
			}
		}

		return $habil;
	}
	
	/**
	 * Busca dados do Coordenador Responsável pelo processo
	 */
	public function buscaDadosCoordenadorResponsavel(){
		global $db;

		$sql = "
			SELECT
				cr.crpid,
				cr.usucpf,
				TO_CHAR(cr.crpdthrdistribuicao,'DD/MM/YYYY') as crpdthrdistribuicao,
				cr.crpprazodefinido,
				TO_CHAR(tr.trpdtfinalprazo,'DD/MM/YYYY') as trpdtfinalprazo
			FROM cproc.coordenadoresponsavel cr
			LEFT JOIN cproc.tecnicoresponsavel tr ON tr.prcid = cr.prcid
			WHERE cr.prcid = {$this->prcid}
		";

		return $db->carregar( $sql );
	}

	/*
	 *
	 */
	public function pegaDataExecucao(){
		global $db;

		$sql = "
			SELECT TO_CHAR(pzedtfinalprazo,'DD/MM/YYYY')
			FROM cproc.prazoexecucao
			WHERE prcid = {$this->prcid}
		";
		// ver($sql,d);
		return $db->pegaUm( $sql );
	}

	/*
	 *
	 */
	public function pegaDataManifestacao(){
		global $db;

		$sql = "
			SELECT TO_CHAR(pzmdtfinalprazo,'DD/MM/YYYY')
			FROM cproc.prazomanifestacao
			WHERE prcid = {$this->prcid}
		";
		return $db->pegaUm( $sql );
	}
}