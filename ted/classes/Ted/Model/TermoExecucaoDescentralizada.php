<?php

class Ted_Model_TermoExecucaoDescentralizada extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ted.termocompromisso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('tcpid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tcpid' => null,
        'docid' => null,
        'ungcodproponente' => null,
        'ungcodconcedente' => null,
        'rugid' => null,
        'pliid' => null,
        'unridproponente' => null,
        'unridconcedente' => null,
        'tcobjptxtrelacao' => null,
        'dircod' => null,
        'usucpfconcedente' => null,
        'usucpfproponente' => null,
        'cooid' => null,
        'entid' => null,
        'tcpobsrelatorio' => null,
        'ungcodpoliticafnde' => null,
        'dircodpoliticafnde' => null,
        'tcpnumtransfsiafi' => null,
        'tcpnumprocessofnde' => null,
        'tcpprogramafnde' => null,
        'tcpobsfnde' => null,
        'ungcodemitente' => null,
        'gescodemitente' => null,
        'tcpstatus' => null,
        'tcpobscomplemento' => null,
        'tcpbancofnde' => null,
        'tcpagenciafnde' => null,
    );

    private $filterDaysOf;

    protected $_temporaryTable;

    /**
     * @param null $id
     */
    public function __construct($id = null)
    {
        parent::__construct();
        $id = ($id) ? preg_replace('/[^0-9]/', '', $id) : Ted_Utils_Model::capturaTcpid();
        if ($id) {
            $this->carregarPorId($id);
        }
    }

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array();

    /**
     * Valida campos obrigatorios no objeto populado
     *
     * @author Sávio Resende - Copiador por Lindalberto Filho
     * @return bool
     */
    public function validaCamposObrigatorios()
    {
        foreach ($this->arAtributosObrigatorios as $chave => $valor) {
            if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor])) {
                return false;
            }
        }
        return true;
    }

    /**
     * Captura o TCPID setado no objeto
     * @return multitype:
     */
    public function capturaTcpid()
    {
        return $this->arAtributos['tcpid'];
    }

    /**
     * @return $this
     */
    public function pegaTermoParcial()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();

        $strSQL = "
            SELECT
        		tcpid,
				ungcodconcedente,
				ungcodproponente,
				ungcodpoliticafnde,
				dircodpoliticafnde,
				CASE WHEN ungcodpoliticafnde IS NOT NULL THEN ungcodpoliticafnde || '_ungcod'
				ELSE dircodpoliticafnde || '_dircod' END AS codpoliticafnde
			FROM
				{$this->stNomeTabela}
			WHERE
				tcpid = {$tcpid}
	    ";

        $dadosTermo = $this->pegaLinha($strSQL);
        $this->popularDadosObjeto($dadosTermo);
        return $this;
    }

    /**
     * @return array|bool|void
     */
    public function pegaTermoCompleto()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();
        if ($tcpid == null || trim($tcpid) == '') {
            return false;
        }

        $strSQL = "
            SELECT
                *
            FROM {$this->stNomeTabela}
            WHERE tcpid = {$tcpid}
        ";

        $dadosTermo = $this->pegaLinha($strSQL);
        if (!$dadosTermo)
            return array();

        return $dadosTermo;
    }

    /**
     * Caso a seção exista, o sistema pode capturar o proponente para a página de proponente através deste método.
     * @return Ambigous <boolean, string>
     */
    public function capturaProponente()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();

        $strSQL = "
    	  SELECT
    	      ungcodproponente
          FROM {$this->stNomeTabela}
          WHERE tcpid = {$tcpid}
        ";

        return $this->pegaUm($strSQL);
    }

    /**
     * Caso a seção exista, o sistema pode capturar o concedente para a página de proponente através deste método.
     * @return Ambigous <boolean, string>
     */
    public function capturaConcedente()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();

        $strSQL = "
    	    SELECT
    	        ungcodconcedente, ungcodpoliticafnde, dircodpoliticafnde, tcpnumprocessofnde
            FROM {$this->stNomeTabela}
            WHERE tcpid = {$tcpid}
        ";

        return $this->pegaLinha($strSQL);
    }

    /**
     * Inserção de um novo termo, inicializando pela gravação do proponente.
     * @param Array $dados $_POST contendo os campos do formulário referentes às colunas da tabela.
     * @return boolean|unknown
     */
    public function gravarTermoProponente($dados)
    {
        $this->arAtributos['tcpid'] = Ted_Utils_Model::capturaTcpid();
        $this->popularDadosObjeto($dados['termo']);

        if (!$this->verificaExistenciaRepresentanteLegal($this->arAtributos['ungcodproponente'])) {
            return false;
        }

        $unidadeGestora = new Ted_Model_UnidadeGestora();
        $unidadeGestora->atualizaDadosUnidadeGestora($dados['unidade']);

        if ($this->arAtributos['tcpid']) {
            return $this->atualizarTermoExecucaoDescentralizada();
        }

        require_once APPRAIZ . 'includes/workflow.php';
        $this->arAtributos['docid'] = wf_cadastrarDocumento(WF_TPDID_DESCENTRALIZACAO, 'Termo Cooperacao');
        return $this->cadastrarTermoExecucaoDescentralizada();
    }

    /**
     * 
     * @param Array $dados $_POST contendo os campos do formulário referentes às colunas da tabela.
     * @return boolean
     */
    public function gravarTermoConcedente(array $dados)
    {
        $this->arAtributos['tcpid'] = Ted_Utils_Model::capturaTcpid();
        if (!$this->arAtributos['tcpid']) {
            return false;
        }

        $this->popularDadosObjeto($dados['concedente']['termo']);
        if (!$this->verificaExistenciaRepresentanteLegal($this->arAtributos['ungcodconcedente'], 'usucpfconcedente'))
            return false;

        if ($this->arAtributos['ungcodpoliticafnde'] == '0') {
            $this->arAtributos['ungcodpoliticafnde'] = NULL;
        } else {
            $fnde = explode("_", $this->arAtributos['ungcodpoliticafnde']);
            $this->arAtributos['ungcodpoliticafnde'] = NULL;
            $arCamposNulo = array();

            if ($fnde[1] == 'ungcod') {
                $this->arAtributos['ungcodpoliticafnde'] = $fnde[0];
                $arCamposNulo[] = 'dircodpoliticafnde';
            } else if ($fnde[1] == 'dircod') {
                $this->arAtributos['dircodpoliticafnde'] = $fnde[0];
                $arCamposNulo[] = 'ungcodpoliticafnde';
            }
        }

        return $this->atualizarTermoExecucaoDescentralizada($arCamposNulo);
    }

    /**
     * Cadastrar TED
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    public function cadastrarTermoExecucaoDescentralizada()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->arAtributos['tcpid'] = $this->inserir();
            return ($this->commit()) ? $this->arAtributos['tcpid'] : false;
        }

        return false;
    }

    /**
     * Atualiza TED
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    public function atualizarTermoExecucaoDescentralizada($arCamposNulo = array())
    {
        if ($this->validaCamposObrigatorios()) {
            $this->alterar($arCamposNulo);
            return $this->commit();
        }

        return false;
    }

    /**
     * Função para verificar existência de um representanteLegal para determina Unidade Gestora
     * @param unknown $codigoUnidadeGestora
     * @return boolean
     */
    public function verificaExistenciaRepresentanteLegal($codigoUnidadeGestora, $usucpf = "usucpfproponente")
    {
        if (!$codigoUnidadeGestora) {
            return false;
        }

        $sql = "SELECT cpf FROM ted.representantelegal WHERE ug = '{$codigoUnidadeGestora}'";
        if (($this->arAtributos[$usucpf] = $this->pegaUm($sql))) {
            return true;
        }

        return false;
    }

    /**
     * Preenche o parecer do RCO pela coordenação
     * @param $descricao
     * @return bool
     */
    public function updateRelatorioRCO($descricao)
    {
        $strSQL = sprintf("
            update {$this->stNomeTabela} set tcpobsrelatorio = '%s' where tcpid = %d
        ", $descricao, Ted_Utils_Model::capturaTcpid());

        $this->executar($strSQL);
        return $this->commit();
    }

    /**
     * Retorna a query completa para listagem de termos por perfil e situações
     * @param null $where
     * @param null $joins
     * @return string
     */
    public function getQueryListaTermos($where = null, $joins = null)
    {
        $clausula = array();
        $filtro = new Ted_Model_Responsabilidade();
        $clausula = $filtro->getClausleWhere();

        if (is_null($where)) $this->temporaryTable(null);

        $strSQL = "
            {$this->_temporaryTable}

            SELECT
                vTable.tcpid,
                vTable.decricao,
                vTable.tcpnumtransfsiafi as siafi,
                vTable.unidadegestorap,
                vTable.unidadegestorac,
                coalesce(vTable.identificacao, ' - ') as titulo_obj_despesa,
                vTable.esddsc,
                vTable.coodsc,
                vTable.vigencia
            FROM (
                SELECT DISTINCT
                        tcp.tcpid,
                        prev.tcpnumtransfsiafi,
                        tcp.tcpid || case when  (select count(*) from tmp_ted_historico hst where hst.aedid = 1620 and hst.docid = tcp.docid)  > 0 then '.' ||  (select count(*) from tmp_ted_historico hst where hst.aedid = 1620 and hst.docid = tcp.docid) ::varchar else '' end as decricao,
                        unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
                        unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,
                        jv.identificacao,
                        esd.esddsc as esddsc,
                        coalesce(cdn.coodsc, '-') as coodsc,
                        (select
                                case when a.vigdata is not null then
                                    TO_CHAR(a.vigdata, 'DD/MM/YYYY')
                                when t.dtvigenciafinal is not null then
                                    TO_CHAR(t.dtvigenciafinal, 'DD/MM/YYYY')
                                else
                                    null
                                end as vigencia
                            from ted.termocompromisso t
                            left join ted.aditivovigencia a on (a.tcpid = t.tcpid)
                            where t.tcpid = tcp.tcpid
                            order by a.vigid desc limit 1) as vigencia

                FROM {$this->stNomeTabela} tcp

                " . (is_array($joins) ? " " . implode(" ", $joins) : '') . "
                LEFT JOIN ted.coordenacao cdn           ON cdn.cooid = tcp.cooid
                JOIN public.unidadegestora unp          ON unp.ungcod = tcp.ungcodproponente
                JOIN public.unidadegestora unc          ON unc.ungcod = tcp .ungcodconcedente
                JOIN ted.representantelegal rpp         ON rpp.ug = tcp.ungcodproponente
                LEFT JOIN ted.representantelegal rpc    ON rpc.ug = tcp.ungcodconcedente
                JOIN workflow.documento doc             ON doc.docid = tcp.docid
                JOIN workflow.estadodocumento esd       ON esd.esdid = doc.esdid
                JOIN ted.justificativa jv               ON (jv.tcpid = tcp.tcpid)
                LEFT JOIN tmp_ted_transfsiafi prev      ON (prev.tcpid = tcp.tcpid)
                WHERE tcp.tcpstatus = 'A'
                %s
                ORDER BY tcpid DESC
            ) vTable
        ";

        if (is_array($where) && count($where)) {
            if (!empty($where[0])) {
                $complemento = ' AND ' . $where[0];
            } else {
                $complemento = '';
            }
        } else $complemento = '';

        //se a $clausula é falsa, então necessita de perfil de gestor ou super usuário para visualizar os termos
        if ($clausula) {
            if (is_array($where) && count($where)) {
                $strSQL = sprintf($strSQL, 'AND (' . $clausula . ') ' . $complemento);
            } else {
                $strSQL = sprintf($strSQL, 'AND ' . $clausula);
            }
        } else {
            if (possui_perfil_gestor(array(PERFIL_UG_REPASSADORA, PERFIL_CGSO, PERFIL_SUPER_USUARIO))) {
                $strSQL = sprintf($strSQL, $complemento);
            } else {
                //Usuários que não possuem nenhuma UG atribuida ao seu perfil
                $strSQL = sprintf($strSQL, ' AND tcp.tcpid IS NULL ');
            }
        }

        if ($this->filterDaysOf)
            $strSQL = $strSQL . ' ' . $this->filterDaysOf;

        //ver($where);
        return $strSQL;
    }

    /**
     * @param array $request
     * @return string
     */
    public function buildWhere(array $request)
    {
        $where = array();

        if ($request['tcpid'])
            $where[] = "tcp.tcpid = {$request['tcpid']}";

        if ($request['unicod'])
            $where[] = "(unp.unicod = '{$request['unicod']}' or unc.unicod = '{$request['unicod']}')";

        if ($request['ungcodconcedente'])
            $where[] = "unc.ungcod = '{$request['ungcodconcedente']}'";

        if ($request['ungcodproponente'])
            $where[] = "unp.ungcod = '{$request['ungcodproponente']}'";

        if ($request['vencimento']) {

            switch ($request['vencimento']) {
                case -1:
                    $this->filterDaysOf = "
                        WHERE
                            CASE WHEN vTable.vigencia IS NOT NULL AND TO_CHAR(vTable.vigencia::DATE, 'YYYY-MM-DD')::DATE < NOW()::DATE THEN
                                NOW()::DATE - TO_CHAR(vTable.vigencia::DATE, 'YYYY-MM-DD')::DATE > 1
                            END
                    ";
                break;
                case -60:
                    $this->filterDaysOf = "
                        WHERE
                            CASE WHEN vTable.vigencia IS NOT NULL THEN
                                TO_CHAR(vTable.vigencia::DATE, 'YYYY-MM-DD')::DATE - NOW()::DATE <= -60
                            END
                    ";
                break;
                default:
                    $this->filterDaysOf = "
                        WHERE
                            CASE WHEN vTable.vigencia IS NOT NULL AND TO_CHAR(vTable.vigencia::DATE, 'YYYY-MM-DD')::DATE > NOW()::DATE THEN
                                TO_CHAR(vTable.vigencia::DATE, 'YYYY-MM-DD')::DATE - NOW()::DATE <= {$request['vencimento']}
                            END
                    ";
            }
        }

        if ($request['esdid'])
            $where[] = "doc.esdid = {$request['esdid']}";

        if ($request['tcpnumtransfsiafi']) {
            $where[] = " prev.tcpnumtransfsiafi = '{$request['tcpnumtransfsiafi']}' ";
        }

        $this->temporaryTable(($request['vencimento']) ? $request['vencimento'] : null);

        //ver($request, $where, d);
        return (is_array($where)) ? implode(" AND ", $where) : '';
    }

    /**
     * @param bool $vencimento
     * @return void(0)
     */
    private function temporaryTable($vencimento)
    {
        if ($vencimento) {
            //usado para gerar tabela temporária, quando na listagem de termos usar algum filtro de vencimento
            $strDml = "FROM workflow.historicodocumento WHERE aedid IN (1609, 1618, 2440)";
        } else {
            //usado sempre quando não houver filtro de vencimento do termo no post
            $strDml = "
                FROM workflow.historicodocumento
                WHERE aedid IN (1620, 1597, 1612, 2442, 1609, 1618, 2440)
                ORDER BY hstid DESC limit 1
            ";
        }

        $dml = "
            WITH tmp_ted_historico AS (
                SELECT htddata, docid, hstid, aedid, usucpf
                %s
            ),
            tmp_ted_transfsiafi AS (
                select distinct t.tcpid, p.tcpnumtransfsiafi from ted.termocompromisso t
                join ted.previsaoorcamentaria o on (o.tcpid = t.tcpid)
                join ted.previsaoparcela p on (p.proid = o.proid)
                where p.tcpnumtransfsiafi is not null and p.tcpnumtransfsiafi <> '0000'
            )
        ";

        $this->_temporaryTable = sprintf($dml, $strDml);
    }

    public function buildJoins($request)
    {
        $joins = array();
        if ($request['tcpnumtransfsiafi']) {
            $joins[] = "JOIN ted.previsaoorcamentaria po ON (po.tcpid = tcp.tcpid)";
        }

        return (count($joins)===0) ? false : $joins;
    }

    /**
     * Checa se concedente do termo de compromiso e FNDE
     * @return boolean
     */
    public static function concedenteIsFNDE()
    {
        if (!Ted_Utils_Model::isTcpId()) {
            return false;
        }

        $tcpid = Ted_Utils_Model::capturaTcpid();
        $sql = "SELECT TRUE FROM ted.termocompromisso WHERE tcpid = {$tcpid} AND ungcodconcedente = '" . UG_FNDE . "'";
        return (Ted_Utils_Model::dbGetInstance()->pegaUm($sql)) ? true : false;
    }

    /**
     * Pega o estado atual do termo no workflow
     * @return bool|null|string|void
     */
    public static function pegaEstadoAtual()
    {
        if (!Ted_Utils_Model::isTcpId()) {
            return false;
        }

        $tcpid = Ted_Utils_Model::capturaTcpid();
        $sql = "
            select
                d.esdid, ed.esddsc
            from
                ted.termocompromisso t
            inner join workflow.documento d on (d.docid = t.docid)
            inner join workflow.estadodocumento ed on (ed.esdid = d.esdid)
            where
                t.tcpid = {$tcpid}
        ";

        $dados = Ted_Utils_Model::dbGetInstance()->pegaUm($sql);
        return ($dados) ? $dados : null;
    }

    public function capturaReitor()
    {
        $sqlReitor = "
    		SELECT
    			u.usunome,
    			u.usucpf,
    			TO_CHAR(h.htddata, 'DD/MM/YYYY') AS htddata,
    			TO_CHAR(h.htddata, 'HH:II:SS') AS hora,
    			g.ungdsc
    		FROM {$this->stNomeTabela} t
    		INNER JOIN workflow.historicodocumento h ON h.docid = t.docid
    		INNER JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
    		INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf
    		LEFT JOIN unidadegestora g ON g.ungcod = t.ungcodconcedente
    		WHERE t.tcpid = {$_GET['ted']}
    			AND a.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . "
    		ORDER BY hstid asc";
    }

    public function pegarEstadoAtualTermo($retornarDescricao = false)
    {
        $docid = $this->pegarDocid();

        if ($docid) {
            $sql = "
				SELECT 
					ed.esdid,
       				ed.esddsc
  				FROM workflow.documento d
    			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
  				WHERE d.docid = {$docid}
			";
            $esddoc = $this->carregar($sql);
            if ($esddoc) {
                if (!$retornarDescricao) {
                    return (integer) $esddoc[0]['esdid'];
                }
                return array((int) $esddoc[0]['esdid'], $esddoc[0]['esddsc']);
            }
        }
        return false;
    }

    /**
     * Pega o id do documento do plano de trabalho
     *
     * @param integer $lbrid
     * @return integer
     */
    public function pegarDocid()
    {
        if (isset($_GET['ted'])) {
            $sql = "
    			SELECT 
    				docid
    			FROM {$this->stNomeTabela}
    			WHERE tcpid = {$this->arAtributos['tcpid']}
    		";
            return $this->pegaUm($sql);
        }
        return false;
    }

    public function capturaSecretarioTermo()
    {
        $sqlPresidente = "
    		SELECT 1  
    		FROM {$this->stNomeTabela} 
    		WHERE ungcodconcedente = '153173' and tcpid = {$this->arAtributos['tcpid']}";

        $rsPresidente = $this->pegaLinha($sqlPresidente);

        $where = '';
        if ($rsPresidente) {
            $where .= " AND a.esdiddestino IN ( " . EM_EMISSAO_NOTA_CREDITO . ", " . EM_ANALISE_PELA_SPO . " )";
        } else {
            $where .= " AND a.esdiddestino = " . EM_ANALISE_PELA_CGSO . " ";
        }

        $sqlSecretario = "
    		SELECT
		    	u.usunome,
		    	u.usucpf,
		    	to_char(h.htddata, 'DD/MM/YYYY') AS htddata,
		    	to_char(h.htddata, 'HH:II:SS') AS hora,
		    	g.ungdsc
		    FROM {$this->stNomeTabela} t
		    INNER JOIN workflow.historicodocumento h ON h.docid = t.docid
		    INNER JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
		   	INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf
		    LEFT JOIN unidadegestora g ON g.ungcod = t.ungcodconcedente
		    WHERE t.tcpid = {$this->arAtributos['tcpid']}
		    {$where}
		    ORDER BY hstid ASC
		";

        $rsSecretario = $this->pegaLinha($sqlSecretario);

        return array($rsPresidente, $rsSecretario);
    }

    public function capturaContagemTermo()
    {
        $sql = "
    		SELECT 
    			COUNT(*) 
    		FROM workflow.historicodocumento 
    		WHERE
    		    aedid = " . WF_ACAO_SOL_ALTERACAO . "
    			AND docid = (
    			    SELECT docid FROM {$this->stNomeTabela}
    			    WHERE tcpid = {$this->arAtributos['tcpid']}
    			)
    	";
        return $this->pegaUm($sql);
    }

    /**
     * Verifica se o termo de compromisso
     * esta "Em solicitação de alteraçao"
     * @return bool
     */
    public static function emSolicitacaoDeAlteracao()
    {
        if (!Ted_Utils_Model::isTcpId()) {
            return false;
        }

        $tcpid = Ted_Utils_Model::capturaTcpid();

        $sql = "
            select
                d.esdid --t.ungcodproponente, t.docid, t.tcpid,
            from
                ted.termocompromisso t
            inner join workflow.documento d on (d.docid = t.docid)
            where
                t.tcpid = {$tcpid}
        ";

        $esdid = Ted_Utils_Model::dbGetInstance()->pegaUm($sql);
        return ($esdid == ALTERAR_TERMO_COOPERACAO) ? true : false;
    }

    /**
     * Faz a divisão em array sobre os dados do proponente e do representante legal substituto
     * @param array $fromPost
     * @return array
     */
    public function extractArraySlice(array $fromPost)
    {
        $arrayKeysRlp = array('rlid', 'cpf', 'nome', 'funcao', 'email', 'ungcod');
        $arrayKeysCoordencao = array('corid', 'nomecoordenacao', 'dddcoordenacao', 'telefonecoordenacao');
        $arrRepresentante = $arrProponente = $arrCoordenacao = array();

        if (isset($fromPost['ungcodconcedente'])) {
            $fromPost['ungcod'] = $fromPost['ungcodconcedente'];
        }

        foreach ($fromPost as $k => $value) {
            foreach ($arrayKeysRlp as $i => $v) {
                if ($arrayKeysRlp[$i] == $k) {
                    $arrRepresentante[$k] = $value;
                    unset($fromPost[$k]);
                }

                if ($arrayKeysCoordencao[$i] == $k) {
                    $arrCoordenacao[$k] = $value;
                    unset($fromPost[$k]);
                }
            }

            if ($k == 'ungdsc') {
                $arrProponente['termo']['ungcodproponente'] = $value;
            }

            if ($k == 'ungcodconcedente') {
                $arrProponente['termo']['ungcodconcedente'] = $value;
            }

            if ($k == 'tcpid' && !empty($value)) {
                $arrProponente['termo']['tcpid'] = $value;
            }

            if ($k == 'ungcodpoliticafnde' && !empty($value)) {
                //$tmpValue = explode('_', $value);

                $arrProponente['termo']['ungcodpoliticafnde'] = $value;
            }
        }

        $arrRepresentante['substituto'] = 't';

        $ungcod = ($arrProponente['termo']['ungcodproponente']) ? $arrProponente['termo']['ungcodproponente'] : $arrProponente['termo']['ungcodconcedente'];

        $chave = ($arrProponente['termo']['ungcodproponente']) ? 'proponente' : 'concedente';

        $arrProponente['termo']["usucpf{$chave}"] = $_SESSION['usucpf'];

        $arrCoordenacao['ungcod'] = "'$ungcod'";

        if (strlen($fromPost['ungddd'])) {
            $fromPost['ungfone'] = "{$fromPost['ungddd']}-{$fromPost['ungfone']}";
        }

        $arrProponente['unidade'] = array(
            'ungendereco' => $fromPost['ungendereco'],
            'ungbairro' => $fromPost['ungbairro'],
            'muncod' => $fromPost['muncod'],
            'ungcep' => $fromPost['ungcep'],
            'ungfone' => $fromPost['ungfone'],
            'ungemail' => $fromPost['ungemail'],
            'ungcod' => "'$ungcod'"
        );

        return array(
            $chave => $arrProponente,
            'representante_legal' => $arrRepresentante,
            'coordenacao' => $arrCoordenacao
        );
    }

    /**
     * Verifica se é momento de gerar numero de processo para termos FNDE
     * @return bool
     */
    public function precisaGerarNumeroProcessoFNDE()
    {
        $dadosConcedente = $this->capturaConcedente();
        $situacao = Ted_Utils_Model::pegaSituacaoTed();

        if (empty($dadosConcedente['tcpnumprocessofnde'])
            && Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE()
            && $situacao['esdid'] == TERMO_EM_ANALISE_ORCAMENTARIA_FNDE)
        {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Recupera numero de processo FNDE
     * @return string
     */
    public function recuperaNumeroProcessoFNDE()
    {
        $dadosConcedente = $this->capturaConcedente();

        if (!empty($dadosConcedente['tcpnumprocessofnde']) && Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE()) {

            $vlTmp = $dadosConcedente['tcpnumprocessofnde'];
            return substr($vlTmp, 0, 5) . '.' . substr($vlTmp, 5, 6) . '/' . substr($vlTmp, 11, 4) . '-' . substr($vlTmp, 15, 2);
        }
    }

    /**
     * Verifica se o termo está no momento de solicitar nota de crédito
     * para as naturezas de despesas cadastradas
     * @return bool
     */
    public function momentoSolicitarNC()
    {
        $situacao = Ted_Utils_Model::pegaSituacaoTed();

        return (Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE() && $situacao['esdid'] == EM_DESCENTRALIZACAO);
    }

    /**
     * Verifica se o termo está no momento de enviar para Programação Financeira
     * @return bool
     */
    public function momentoEnviarPF()
    {
        $situacao = Ted_Utils_Model::pegaSituacaoTed();

        return (Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE() && $situacao['esdid'] == EM_EXECUCAO);
    }

}
