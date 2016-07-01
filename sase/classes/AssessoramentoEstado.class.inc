<?php

/**
 * Classe Macro Categoria
 *
 *
 * @author Lindalberto Filho, Modelo desenvolvido por Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Macro Categorias
 * TODO classe não testada
 */
class AssessoramentoEstado extends Modelo
{

	/**
	 *
	 */
	public $tpdid = TPDID_SASE_ASSESSORAMENTO;

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.assessoramentoestado";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('aseid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'aseid' => null,
		'estuf' => null,
		'stacod' => null,
		'usucpf' => null,
		'docid' => null,
		'asestatus' => null,
        'aseleipne' => null
	);

	/**
	 * Id do último assessorado inserido
	 *
	 * @var integer
	 */
	public $inserido;

    public $arquivo;

    public function __construct(){
        $this->arquivo = new FilesSimec();
    }

	/**
	 * Associacao em array da situacao com seu respectivo esdid
	 */
	public $situacaoEsdid = array(
			0 => ESDID_SASE_SEM_INFORMACAO, // nao assessorado
			1 => ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA, // assessorado
			2 => ESDID_SASE_COM_COMISSAO_COORDENADORA_CONSTITUIDA, // com comissao
			3 => ESDID_SASE_COM_DIAGNOSTICO_CONCLUIDO, // tem diagnostico
			4 => ESDID_SASE_COM_DOCUMENTO_BASE_ELABORADO, // tem texto-base
			5 => ESDID_SASE_COM_CONSULTA_PUBLICA_REALIZADA, // realizou a conferencia
			6 => ESDID_SASE_COM_PROJETO_DE_LEI_ELABORADO, // tem projeto de lei - pl
			7 => ESDID_SASE_COM_PROJETO_DE_LEI_ENVIADO_PELO_LEGISLATIVO,
			8 => ESDID_SASE_COM_LEI_APROVADA,
			9 => ESDID_SASE_COM_LEI_SANCIONADA
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'estuf',
		'usucpf',
		'asestatus',
		'sa.stadsc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'aseid',
		'estuf',
		'stacod',
		'usucpf',
		'docid',
		'asestatus'	,
        'aseleipne'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'stacod',
		'asestatus'
	);

	/**
	 * Monta query para a função monta_lista da classe_simec.
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){

		$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

        if (in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis) == false) {
            if (in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) == true
                || in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) == true
            ) {

                $acao = "'<a style=\"cursor:pointer\" onclick=\"editarAssessoramentoEstado( ' || a.aseid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirAssessoramentoEstado( ' || a.aseid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>'";

                $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="editarAssessoramentoEstado( ' || a.aseid || ' )"><img src="/imagens/alterar.gif"/></a>
				<a style="cursor:pointer" onclick="excluirAssessoramentoEstado( ' || a.aseid || ' )"><img src="/imagens/excluir.gif"></a>
				<a style="cursor:pointer" onclick="downloadArquivo( ' || a.aseleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
            } else {
                $acao = "'<a style=\"cursor:pointer\" onclick=\"editarAssessoramentoEstado( ' || a.aseid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>'";
                $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="editarAssessoramentoEstado( ' || a.aseid || ' )"><img src="/imagens/alterar.gif"/></a>
				<a style="cursor:pointer" onclick="downloadArquivo( ' || a.aseleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
            }
        } else {
            $acao = $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="downloadArquivo( ' || a.aseleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
        }

        $indLeiVerde = <<<HTML
'<img title="Com lei" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indLeiVermelho = <<<HTML
'<img title="Sem lei" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;

		//filtro segurança
		$sql_temp = " SELECT
					p.pflcod AS pflcod,
					ur.estuf AS estuf,
					ur.muncod AS muncod
				 FROM seguranca.usuario u
				 INNER JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				 INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = 183
				 INNER JOIN sase.usuarioresponsabilidade ur on ur.pflcod = p.pflcod and ur.usucpf = u.usucpf and ur.rpustatus = 'A'
				where u.usucpf = '".$_SESSION['usucpf']."'; ";
		$responsabilidades = $this->carregar( $sql_temp );

		// resgata municipios
		/*
		$municipios = array();
		if( !empty($responsabilidades) )
		foreach ($responsabilidades as $key => $value) {

			if( !empty($value['muncod']) )
				array_push($municipios, $value['muncod']);
		}
		*/

		// resgata estados
		$estados = array();
		if( !empty($responsabilidades) )
		foreach ($responsabilidades as $key => $value) {

			if( !empty($value['estuf']) )
				array_push($estados, $value['estuf']);
		}

		if( array_search(PFLCOD_SASE_EXECUTIVO, $perfis) !== false
			&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
			$and = " AND ( a.estuf in ('".(implode('\',\'',$estados))."') ) ";
		}

		if( array_search(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) !== false
			&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
			$and = " AND ( a.estuf in ('".(implode('\',\'',$estados))."') ) ";
		}


		$sql = "
			SELECT
				CASE
				  when a.aseleipne is not null then {$acaoD}
				  else {$acao}
				END as acao,
				a.estuf,
				sa.stadsc,
				CASE
				  when a.aseleipne is not null then {$indLeiVerde}
				  else {$indLeiVermelho}
				END as indicadorLei,
				CASE WHEN a.asestatus = 'I' THEN 'Inativo' WHEN a.asestatus = 'A' THEN 'Ativo' END AS asestatus
			FROM " . $this->stNomeTabela . " a
			INNER JOIN sase.situacaoassessoramento sa on(sa.stacod = a.stacod)";
		$sql .= " WHERE 1=1 ";
		$sql .= $and;

        if ($filtros['a.aseleipne']){
            switch($filtros['a.aseleipne']){
                case '1':
                    $sql .= " AND a.aseleipne is not null";
                    break;
                case '2':
                    $sql .= " AND a.aseleipne is null";
                    break;
            }
            unset($filtros['a.aseleipne']);
        }

		if( $filtros != false ){

			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
        $sql .= " ORDER BY a.estuf";

		return $sql;
	}

	public function montaListaXls( $filtros = false ){
		global $db;

		$sql = "
			SELECT
				a.estuf,
				sa.stadsc,
				CASE WHEN a.asestatus = 'I' THEN 'Inativo' WHEN a.asestatus = 'A' THEN 'Ativo' END AS asestatus
			FROM " . $this->stNomeTabela . " a
			INNER JOIN sase.situacaoassessoramento sa on(sa.stacod = a.stacod)";

		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}

		//$res = $db->carregar($sql);

		$cabecalho = array("Ação","Estado","Situação","Status");
		$alinhamento = array('left','left','left','left');
		$larguras = array('5%','5%','85%', '5%');

		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=simec_sase_assistenciatecnicaestado_" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=simec_sase_assistenciatecnicaestado_" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");

		$db->monta_lista($sql,$cabecalho,100000,5,'N','','N','listaAssessoramento',$larguras,$alinhamento);
		//$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');
	}

	/**
	 * Atualiza Assessoramento Estado
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarAssessoramentoEstado(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Assessoramento Estado
 	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirAssessoramentoEstado( $aseid ){
		$this->carregarPorId( $aseid );
		$this->arAtributos['asestatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaAssessoramentoEstado( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Assessoramento Estado Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarAssessoramentoEstado(){
		if( $this->validaCamposObrigatorios() ){
			$this->aseid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}


	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaAssessoramento( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Carrega Objeto por ID
	 *
	 * @param Integer $aseid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function carregaAssessoramento( $aseid ){
		$this->carregarPorId( $_GET['aseid'] );

		return $this->verificaExistenciaDocumentoWF( $aseid );
	}

	/**
	 * Verifica existencia de documento workflow, caso não existe cria
	 *
	 * @param Integer $aseid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function verificaExistenciaDocumentoWF( $aseid ){

		if( $this->docid == null ){
			$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, "Documento Assistência Técnica {$aseid}", $this->situacaoEsdid[$this->stacod] );
			$this->alterar();
			return $this->commit();
		}

		return '1';

	}


	/**
	 * Insert
	 *
	 * @param array $dadosAssessoramento
	 * @author Sávio Resende
	 * @return $this
	 */
	public function insereAssessoramento( Array $dadosAssessoramento ){
		$validacao = $this->validaDadosPorInserir( $dadosAssessoramento );
		if( $validacao == 'duplicidade' ){

			$assessoramento = $this->buscaAssessoramentos( array('muncod'=>$dadosAssessoramento['muncod']) );
			$this->inserido = $assessoramento->lista[0];
			$this->inserido['info'] = 'duplicidade';

		}else if( is_string($validacao) ){

			$this->inserido = array( 'info' => 'camposvazios', 'dado' => $this->atributosObrigatoriosString[$validacao] );

		}else{

			$this->popularObjeto( $this->arCampos, $dadosAssessoramento );
			$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, '' );
			$this->arAtributos['aseid'] = $this->inserir();
			$this->inserido = $this->arAtributos['aseid'];
			$this->commit();

		}
		return $this;
	}

	/**
	 * Valida dados por serem inseridos (campos obrigatorios)
	 *
	 * @param array dadosAssessoramento
	 * @author Sávio Resende
	 */
	public function validaDadosPorInserir( Array $dadosAssessoramento ){

		if( $this->verificaDuplicidade( $dadosAssessoramento ) ) return 'duplicidade';

		foreach ($this->arAtributosObrigatorios as $chave => $valor)
		if( !isset($dadosAssessoramento[$valor]) || !$dadosAssessoramento[$valor] )
			return $valor;

		return true;
	}

	/**
	 * Verifica se já existe assessoramento para o estado ou municipio especificado no array
	 *
	 * @param array $dadosAssessoramento
	 * @author Sávio Resende
	 */
	public function verificaDuplicidade( Array $dadosAssessoramento ){
		if( !isset($dadosAssessoramento['estuf'])) return false;

		$sql = "
			SELECT COUNT(*) as total
			FROM " . $this->stNomeTabela . "
			WHERE
				estuf = '" . $dadosAssessoramento['estuf'] . "' ";
		$numeroResultados = $this->pegaUm( $sql );
		if( $numeroResultados > 0 ) return true;

		return false;
	}

	/**
	 * Resgata o estado do documento do Assessoramento montado no objeto
	 *
	 * @author Sávio Resende
	 */
	public function resgataEsdid(){
		$sql = " select esdid from workflow.documento where docid = " . $this->docid . " ";
		return $this->pegaUm( $sql );
	}

	/**
	 * Pega Dsc Situacao
	 *
	 * @param array $arDados
	 * @return $situacao
	 * @author Eduardo Dunice
	 */
	public function pegaDscSituacao( $estuf )
	{
	    $stacod = self::pegaStacod();

        require_once APPRAIZ.'sase/classes/SituacaoAssessoramento.class.inc';

        $situacao = new SituacaoAssessoramento($stacod);

		return $situacao->stadsc;
	}

	/**
	 * Pega Stacod
	 *
	 * @param array $arDados
	 * @return $situacao
	 * @author Eduardo Dunice
	 */
	public function pegaStacod($estuf)
	{
	    $sql = "SELECT stacod
                FROM sase.assessoramento
	            WHERE
                    estuf = '$estuf'
	                AND asestatus = 'A'";

	    return $this->pegaUm($sql);
	}
}
