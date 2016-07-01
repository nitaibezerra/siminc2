<?php

/**
 * Classe Assessoramento
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Assessoramento
 */

class Assessoramento extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.assessoramento";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('assid');//,'muncod','stacod','docid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'assid' => null,
		'estuf' => null,
		'muncod' => null,
		'stacod' => null,
		'usucpf' => null,
		'docid' => null,
		'assstatus' => null,
        'assleipne' => null
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'assid',
		'estuf',
		'muncod',
		'stacod',
		'usucpf',
		'docid',
		'assstatus',
        'assleipne'
	);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'estuf',
		'muncod'
	);

	protected $atributosObrigatoriosString = array(
		'estuf' => 'Estado',
		'muncod' => 'Município'
	);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
		'assid' => null,
		'stacod' => null,
		'docid' => null
	);

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
	 * Array de lista de assessoramentos
	 *
	 * @var array
	 */
	public $lista;

	/**
	 * Id do último assessorado inserido
	 *
	 * @var integer
	 */
	public $inserido;

	/**
	 *
	 */
	public $tpdid = TPDID_SASE_ASSESSORAMENTO;

    public $arquivo;

    public function __construct(){
        $this->arquivo = new FilesSimec();
    }

	/**
	 * Busca lista de dados baseado em filtro ou não
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function buscaAssessoramentos( $filtros = false ){
		$sql = $this->listaQuery( $filtros );

		$this->lista = $this->carregar( $sql );

		return $this;

	}

	/**
	 * Monta query para uma lista com todos os dados
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function listaQuery( $filtros = false ){
		$sql = "
			SELECT * FROM " . $this->stNomeTabela;
		if( $filtros != false ){
			$sql .= " WHERE ";
			foreach ($filtros as $chave => $valor)
				$sql .= " " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}

		return $sql;
	}

	/**
	 * Monta query para a função monta_lista da classe_simec.
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function montaListaQuery( $filtros = false ){

		$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

        if (in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis) == false) {
            if (in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) == true
                || in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) == true
            ) {

                $acao = "'<a style=\"cursor:pointer\" onclick=\"editarAssessoramento( ' || a.assid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirAssessoramento( ' || a.assid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>'";
                $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="editarAssessoramento( ' || a.assid || ' )"><img src="/imagens/alterar.gif"/></a>
				<a style="cursor:pointer" onclick="excluirAssessoramento( ' || a.assid || ' )"><img src="/imagens/excluir.gif"></a>
				<a style="cursor:pointer" onclick="downloadArquivo( ' || a.assleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
            } else {
                $acao = "'<a style=\"cursor:pointer\" onclick=\"editarAssessoramento( ' || a.assid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>'";
                $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="editarAssessoramento( ' || a.assid || ' )"><img src="/imagens/alterar.gif"/></a>
				<a style="cursor:pointer" onclick="downloadArquivo( ' || a.assleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
            }
        } else {
            $acao = $acaoD = <<<HTML
                '<a style="cursor:pointer" onclick="downloadArquivo( ' || a.assleipne || ' )"><img src="/imagens/consultar.gif"></a>'
HTML;
        }


        $indLeiVerde = <<<HTML
'<img title="Com lei" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indLeiVermelho = <<<HTML
'<img title="Sem lei" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;


		$sql = "
			SELECT
				CASE
				  when a.assleipne is not null then {$acaoD}
				  else {$acao}
				END as acao,
				e.estdescricao as estado,
				m.mundescricao as municipio,
				s.stadsc as situacao,
				CASE
				  when a.assleipne is not null then {$indLeiVerde}
				  else {$indLeiVermelho}
				END as indicadorLei,
				CASE WHEN a.assstatus = 'I' THEN 'Inativo' WHEN a.assstatus = 'A' THEN 'Ativo' END AS status
			FROM " . $this->stNomeTabela . " a
			JOIN territorios.estado e ON e.estuf = a.estuf
			JOIN territorios.municipio m ON m.muncod = a.muncod
			JOIN workflow.documento d ON d.docid = a.docid
			JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			JOIN sase.situacaoassessoramento s ON s.esdid = ed.esdid ";

			if( $filtros['a.stacod'] ){
				$sql .= " AND s.stacod = ".$filtros['a.stacod']." ";
				unset($filtros['a.stacod']);
			}
        $sql .= " WHERE 1=1 ";

        if ($filtros['a.assleipne']){
            switch($filtros['a.assleipne']){
                case '1':
                    $sql .= " AND a.assleipne is not null";
                    break;
                case '2':
                    $sql .= " AND a.assleipne is null";
                    break;
            }
            unset($filtros['a.assleipne']);
        }

		if( $filtros != false ){
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}

		return $sql;
	}

	public function montaListaXls( $filtros = false ){
		global $db;

        $indLeiVerde = <<<HTML
'<img title="Com lei" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indLeiVermelho = <<<HTML
'<img title="Sem lei" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;

		$sql = "
			SELECT
				e.estdescricao as estado,
				m.mundescricao as municipio,
				s.stadsc as situacao,
				CASE
				  when a.assleipne is not null then 'Com lei PNE'
				  else 'Sem lei PNE'
				END as indicadorLei,
				CASE WHEN a.assstatus = 'I' THEN 'Inativo' WHEN a.assstatus = 'A' THEN 'Ativo' END AS status
			FROM " . $this->stNomeTabela . " a
			JOIN territorios.estado e ON e.estuf = a.estuf
			JOIN territorios.municipio m ON m.muncod = a.muncod
			JOIN workflow.documento d ON d.docid = a.docid
			JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			JOIN sase.situacaoassessoramento s ON s.esdid = ed.esdid ";

		if( $filtros['a.stacod'] ){
			$sql .= " AND s.stacod = ".$filtros['a.stacod']." ";
			unset($filtros['a.stacod']);
		}

        $sql .= " WHERE 1=1 ";

        if ($filtros['a.assleipne']){
            switch($filtros['a.assleipne']){
                case '1':
                    $sql .= " AND a.assleipne is not null";
                    break;
                case '2':
                    $sql .= " AND a.assleipne is null";
                    break;
            }
            unset($filtros['a.assleipne']);
        }

		if( $filtros != false ){
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}

		//$res = $db->carregar($sql);
//	ver($sql, d);
		$cabecalho = array("Ação","Estado","Situação", "Lei PNE","Status");
		$alinhamento = array('left','left','left', "center",'left');
		$larguras = array('5%','5%','75%', '10%', '5%');

		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=simec_sase_assistenciatecnica_" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=simec_sase_assistenciatecnica_" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");

		$db->monta_lista($sql,$cabecalho,1000000,5,'N','','N','listaAssessoramento',$larguras,$alinhamento);
	}


	/**
	 * Tratamento de regras de perfis para geração de filtros para 'montaListaQuery'
	 *
	 */
	public function trataRegrasPerfis( $usucpf, $sql ){

		$perfis = pegaPerfilGeral( $usucpf );

		$sql_temp = " SELECT
					p.pflcod AS pflcod,
					ur.estuf AS estuf,
					ur.muncod AS muncod
				 FROM seguranca.usuario u
				 INNER JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				 INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = 183
				 INNER JOIN sase.usuarioresponsabilidade ur on ur.pflcod = p.pflcod and ur.usucpf = u.usucpf and ur.rpustatus = 'A'
				where u.usucpf = '".$usucpf."'; ";
		$responsabilidades = $this->carregar( $sql_temp );

		// resgata municipios
		$municipios = array();
		if( !empty($responsabilidades) )
		foreach ($responsabilidades as $key => $value) {

			if( !empty($value['muncod']) )
				array_push($municipios, $value['muncod']);
		}

		// resgata estados
		$estados = array();
		if( !empty($responsabilidades) )
		foreach ($responsabilidades as $key => $value) {

			if( !empty($value['estuf']) )
				array_push($estados, $value['estuf']);
		}

		$OR = false;

		if( array_search(PFLCOD_SASE_EXECUTIVO, $perfis) !== false
			&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
			$sql .= " AND ( e.estuf in ('".(implode('\',\'',$estados))."') )";
			$OR = true;
		}

		if( array_search(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) !== false
			&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
			$sql .= " AND ( e.estuf in ('".(implode('\',\'',$estados))."') )";
			$OR = true;
		}

		if( ( array_search(PFLCOD_SASE_SUPERVISOR, $perfis)!==false
			|| array_search(PFLCOD_SASE_TECNICO, $perfis)!==false)
			&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){

			if( $OR ){
				$sql = substr($sql, 0, -1);
				$sql .= " OR m.muncod in ('".(implode("','",$municipios))."') )";
			}else{
				$sql .= " AND ( m.muncod in ('".(implode("','",$municipios))."') )";
			}
		}

		return $sql;
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

		if( $validacao === 'duplicidade' ){

			$assessoramento = $this->buscaAssessoramentos( array('muncod'=>$dadosAssessoramento['muncod']) );
			$this->inserido = $assessoramento->lista[0];
			$this->inserido['info'] = 'duplicidade';

		}else if( is_string($validacao) ){

			$this->inserido = array( 'info' => 'camposvazios', 'dado' => $this->atributosObrigatoriosString[$validacao] );

		}else{

			if( empty($dadosAssessoramento['stacod']) ) $dadosAssessoramento['stacod'] = 0;

			$this->popularObjeto( $this->arCampos, $dadosAssessoramento );
			$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, '' );
			$this->arAtributos['assid'] = $this->inserir();
			$this->inserido = $this->arAtributos['assid'];
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
		if( !isset($dadosAssessoramento['estuf']) && !isset($dadosAssessoramento['muncod']) ) return false;

		$sql = "
			SELECT COUNT(*) as total
			FROM " . $this->stNomeTabela . "
			WHERE
				muncod = '" . $dadosAssessoramento['muncod'] . "' ";
		$numeroResultados = $this->pegaUm( $sql );

		if( $numeroResultados ) return true;

		return false;
	}

	/**
	 * Exclui Assessoramento
	 *
	 * @param integer $assid
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirAssessoramento( $assid ){
		$this->excluir( $assid );
		return $this->commit();
	}

	/**
	 * Exclui Logicamente Assessoramento
	 *
	 * @param integer $assid
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirLogicamenteAssessoramento( $assid ){
		$this->carregarPorId( $assid );
		$this->arAtributos['assstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Carrega Objeto por ID
	 *
	 * @param Integer $assid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function carregaAssessoramento( $assid ){
		$this->carregarPorId( $_GET['assid'] );

		return $this->verificaExistenciaDocumentoWF( $assid );
	}

	/**
	 * Verifica existencia de documento workflow, caso não existe cria
	 *
	 * @param Integer $assid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function verificaExistenciaDocumentoWF( $assid ){

		if( $this->docid == null ){
			$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, '', $this->situacaoEsdid[$this->stacod] );
			$this->alterar();
			return $this->commit();
		}

		return '1';
	}

	/**
	 * Monta assessoramento pelo docid
	 *
	 * @param $docid
	 * @author Sávio Resende
	 */
	public function carregaAssessoramentoPeloDocid( $docid ){
		$sql = " select assid from " . $this->stNomeTabela . " where docid = " . $docid . " ";
		$this->carregarPorId( $this->pegaUm( $sql ) );
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
	 * Atualiza Assessoramento
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarAssessoramento(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
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
	 * Pega Dsc Situacao
	 *
	 * @param array $arDados
	 * @return $situacao
	 * @author Eduardo Dunice
	 */
	public function pegaStacod($muncod)
	{
	    $sql = "SELECT stacod
                FROM sase.assessoramento
	            WHERE
                    muncod = '$muncod'
                    AND assstatus = 'A'";

	    $stacod = $this->pegaUm($sql);

		return $stacod;
	}

	/**
	 * Pega Dsc Situacao
	 *
	 * @param array $arDados
	 * @return $situacao
	 * @author Eduardo Dunice
	 */
	public function pegaDscSituacao($muncod)
	{
	    $stacod = self::pegaStacod($muncod);

        require_once APPRAIZ.'sase/classes/SituacaoAssessoramento.class.inc';

        $situacao = new SituacaoAssessoramento($stacod);

		return $situacao->stadsc;
	}
}