<?php

class Ted_Model_TramitaLote extends Modelo
{
    /**
     * Identificador do documento
     */
    const TPDID = 97;

    /**
     * Unidade Gestora FNDE
     */
    const UNGCOD_FNDE = '153173';

    /**
     * @var
     */
    protected $_usucpf;

    /**
     * @var
     */
    protected $_where;

    /**
     * @throws Exception
     */
    public function __construct()
    {
        $this->_usucpf = $_SESSION['usucpf'];
        if (!$this->_usucpf) {
            throw new Exception('usucpf is null');
        }
    }

    /**
     * @return array
     */
    public function getSituacoesTermos()
    {
        $strSQL = "
            SELECT DISTINCT
                esdid as codigo,
                esddsc as descricao
            FROM
                workflow.estadodocumento esd
            INNER JOIN workflow.acaoestadodoc 	       aed ON (aed.esdidorigem = esd.esdid)
            INNER JOIN workflow.estadodocumentoperfil  esp ON (esp.aedid = aed.aedid)
            INNER JOIN seguranca.perfilusuario 	       pus ON (pus.pflcod = esp.pflcod)
            WHERE
                esd.tpdid = %d
                AND aed.aedstatus = 'A'
                AND esd.esdstatus = 'A'
                AND pus.usucpf = '%s'
                AND esd.esdid in (642, 635, 636, 637, 638)
            ORDER BY esddsc
        ";

        $stmt = sprintf($strSQL, self::TPDID, $this->_usucpf);
        //ver($stmt, d);
        $collection = $this->carregar($stmt);
        $options = array();
        if (is_array($collection)) {
            foreach ($collection as $row) {
                $options[$row['codigo']] = $row['descricao'];
            }
        }

        return $options;
    }

    /**
     * Verifica se o usuário logado é vinculado ao FNDE
     * @return bool
     */
    protected function _userIsFNDE()
    {
        $strSQL = "
            SELECT
                true
            FROM ted.usuarioresponsabilidade
            WHERE
                usucpf = '%s'
                AND ungcod = '%s'
                AND rpustatus = 'A'
        ";

        $userIsFNDE = $this->pegaUm(sprintf($strSQL, $this->_usucpf, self::UNGCOD_FNDE));
        return ($userIsFNDE) ? true : false;
    }

    /**
     * @param array $dados
     * @return array
     */
    public function getAcoesTermos($esdid)
    {
        $this->_verificaCondicao($esdid);

        $strSQL = "
            SELECT DISTINCT
				aed.aedid as codigo,
				aed.aeddscrealizar as descricao
			FROM
				workflow.acaoestadodoc aed
			INNER JOIN workflow.estadodocumentoperfil esp ON (esp.aedid = aed.aedid)
			INNER JOIN seguranca.perfilusuario pus ON (pus.pflcod = esp.pflcod)
			WHERE
				aedstatus = 'A'
				AND aed.esdidorigem = {$esdid}
				AND aed.aedvisivel = 't'
				{$this->_where}
			ORDER BY 2
        ";

        $collection = $this->carregar($strSQL);
        $options = array();
        if (is_array($collection)) {
            foreach ($collection as $row) {
                $options[$row['codigo']] = $row['descricao'];
            }
        }

        return $options;
    }

    /**
     * @param $esdid
     * @return $this
     */
    protected function _verificaCondicao($esdid)
    {
        $situacoes = array(642,637,635);
        if (in_array($esdid, $situacoes) && !$this->testa_superuser()) {
            if ($this->_userIsFNDE()) {
                $this->_where = " and aedcondicao ilike '%verificaConcedenteFnde(%' ";
            } else {
                $this->_where = " and aedcondicao not ilike '%verificaConcedenteFnde(%' ";
            }
        } else {
            $this->_where = '';
        }

        return $this;
    }

    /**
     * Renderiza listagem
     * @return void(0)
     */
    public function showTedList(array $post)
    {
        if ($post['esdid']) {
            $where[] = "doc.esdid = {$post['esdid']}";
        }

        $where[] = "
            tcp.tcpid in (select distinct tc.tcpid from ted.termocompromisso tc
            left join ted.previsaoorcamentaria po on tc.tcpid = po.tcpid
            where (po.proanoreferencia >= {$_SESSION['exercicio']} or po.proanoreferencia is null) and tcpstatus = 'A')
        ";

        $inner = NULL;

        $query = "
	        SELECT
				tcp.docid as id,
				tcpid,
				unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
				unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,
				esd.esddsc as esddsc,
				crd.coodsc
			FROM ted.termocompromisso tcp
                ".(is_array($inner) ? " ".implode(" ", $inner) : '')."
                LEFT JOIN ted.coordenacao crd ON crd.cooid = tcp.cooid
                LEFT JOIN public.unidadegestora unp ON unp.ungcod = tcp.ungcodproponente
                LEFT JOIN public.unidadegestora unc ON unc.ungcod = tcp.ungcodconcedente
                LEFT JOIN workflow.documento doc ON doc.docid = tcp.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                ".(is_array($where) ? "WHERE ".implode(" AND ", $where) : '')."
			ORDER BY tcp.tcpid
        ";

        $dados = $this->carregar($query);
        $dados = is_array($dados) ? $dados : array();
        $this->_filtroTermosPossiveis($dados, $post);

        $colunms = array(
            '<input type="checkbox" name="ckboxPai" id="ckboxPai" />',
            'Termos',
            'Unidade Gestora <br/>Proponente',
            'Unidade Gestora <br/>Concedente',
            'Situação Documento',
            'Coordenação'
        );

        /**
         * Componente para listagens.
         * @see Simec_Listagem
         */
        require APPRAIZ . 'includes/library/simec/Listagem.php';

        $list = new Simec_Listagem();
        $list->setCabecalho($colunms)
             ->addCallbackDeCampo('id', 'addCheckbox')
             ->setDados($dados)
             ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
             ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * @param array $collection
     * @param array $post
     * @return array
     */
    protected function _filtroTermosPossiveis(array $collection, array $post)
    {
        if (empty($post['aedid']) && count($collection) === 0) {
            return array();
        }

        require_once APPRAIZ . 'includes/workflow.php';

        $linhas = array();
        foreach ($collection as $dado) {

            //$_SESSION['elabrev']['tcpid'] = (int) $dado['tcpid'];

            $paramsWf = array('docid' => $dado['docid'], 'tcpid' => $dado['tcpid']);
            if (wf_acaoPossivel2($dado['docid'], $post['aedid'], $paramsWf)) {
                //unset($dado['docid'], $dado['tcpid']);
                $linhas[] = $dado;
            }
        }

        return $linhas;
    }

    /**
     * Verifica se deve ser adicionado o campo de comentário
     * na tramitação em lote
     * @return bool
     */
    public function verificaComentario($aedid)
    {
        if (!$aedid) return false;

        $strSQL = "
            SELECT esdsncomentario
            FROM workflow.acaoestadodoc
            WHERE aedid= {$aedid}
        ";

        $esdsncomentario = $this->pegaUm($strSQL);
        return ($esdsncomentario == 't') ? 'success' : 'fail';
    }

    /**
     * Recebe um post com os dados para executar a tramitação de termos em lote
     * @param array $post
     * @return boolean
     */
    public function executaTramitacao(array $post)
    {
        require_once APPRAIZ . 'includes/workflow.php';

        $cpf = str_pad($this->_usucpf, 11, 0, STR_PAD_LEFT);

        $strSQL = "
            SELECT grpid
            FROM ted.tramitalote_grupo
            WHERE tpgid = %d AND usucpf = '%s'
        ";

        $stmt = sprintf($strSQL, LOTE_TIPO_DESCENTRALIZACAO, $cpf);
        $grpid = $this->pegaUm($stmt);

        if (!$grpid) {
            $strSQL = "
                INSERT INTO ted.tramitalote_grupo(tpgid, usucpf)
                VALUES(%d, '%s') RETURNING grpid
            ";

            $stmt = sprintf($strSQL, LOTE_TIPO_DESCENTRALIZACAO, $cpf);
            $grpid = $this->pegaUm($stmt);
        }

        $post['docid'] = explode(',', $post['docid']);
        $sqlItens = '';
        $_SESSION['ted']['extratodou'] = array();
        foreach ($post['docid'] as $prcid => $docid) {

            $strSQL = "SELECT tcpid FROM ted.termocompromisso WHERE docid = {$docid}";
            $tcpid = $this->pegaUm($strSQL);
            array_push($_SESSION['ted']['extratodou'], $tcpid);

            $paramsWf = array('docid' => $docid, 'prcid' => $prcid, 'emlote' => '1', 'cooid' => $post['cooid'], 'advid' => $post['advid']);
            $test = wf_alterarEstado($docid, $post['aedid'], $post['cmddsc'], $paramsWf);

            if ($test) {
                $strSQL = "
                    SELECT
						max(hstid) as hstid
					FROM workflow.historicodocumento
					WHERE docid = {$docid}
                ";

                $hstid = $this->pegaUm($strSQL);
                $sqlItens .= "INSERT INTO ted.tramitalote_itensgrupo(grpid, hstid) VALUES({$grpid}, {$hstid});";
            }
        }

        if (!empty($sqlItens)) {

            $this->executar($sqlItens);
            $this->commit();
            return true;

        } else {

            $strSQL = "
                UPDATE ted.tramitalote_grupo SET
			    grpstatus = 'I' WHERE grpid = $grpid
			";

            $this->executar($strSQL);
            $this->commit();
            return false;
        }
    }
}