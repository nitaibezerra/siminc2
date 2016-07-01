<?php

/**
 * Class Ted_Model_Workflow
 */
class Ted_Model_Workflow extends Modelo
{
	/**
	 * Id do termo de compromisso
	 * @var int tcpid
	 */
	private $tcpid;
	
	/**
	 * Id do documento
	 * @var int docid 
	 */
	private $docid;
	
	/**
	 * Pendencias encontradas para o termo
	 * @var array
	 */
	protected $_pendencias = array();

    /**
     * @var
     */
    protected $_htmlOutput = '';

    /**
     * Initialize()
     * @param null $tcpid
     */
    function __construct()
    {
        $this->tcpid = Ted_Utils_Model::capturaTcpid();
		if (!$this->tcpid) {
			throw new Exception ("Parâmetro TED nulo.");
		}

		$this->docid = Ted_Utils_Model::getDocid($this->tcpid);
        $this->buildData();
	}

    /**
     * Retorna conjunto de dados informando sobre as pendencias de um termo
     * @return array|bool|void
     */
    protected function _getQueryPendencias()
    {
        $strSQL = "
            SELECT DISTINCT

				CASE WHEN ungcodproponente IS NOT NULL
					THEN true
					ELSE false
				END as abaproponente,

				CASE WHEN ungcodconcedente IS NOT NULL
					THEN true
					ELSE false
				END as abaconcedente,

				CASE WHEN
				    (SELECT true FROM ted.justificativa j WHERE j.tcpid = tcp.tcpid AND j.identificacao IS NOT NULL AND j.objetivo IS NOT NULL)
					THEN true
					ELSE false
				END as abadescentralizacao,

				CASE WHEN
					( select count(*) from ted.previsaoorcamentaria po06 where po06.tcpid = tcp.tcpid AND po06.prostatus = 'A'
									and po06.ndpid is not null
									and po06.provalor is not null)
					=
					( select count(*) from ted.previsaoorcamentaria po05 where po05.tcpid = tcp.tcpid AND po05.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisao,

				CASE WHEN
				    (select true from ted.parecertecnico where tcpid = tcp.tcpid
				      AND
						considentproponente   IS NOT NULL AND
						considproposta  	  IS NOT NULL AND
						considobjeto  		  IS NOT NULL AND
						considobjetivo  	  IS NOT NULL AND
						considjustificativa   IS NOT NULL AND
						considvalores  		  IS NOT NULL AND
						considcabiveis  	  IS NOT NULL
				    )
					THEN true
					ELSE false
				END as abaparecertecnico,

				CASE WHEN
					( select count(*) from ted.previsaoorcamentaria po03 where po03.tcpid = tcp.tcpid
									AND po03.prostatus = 'A'
									and po03.ptrid is not null
									and po03.pliid is not null
									and po03.crdmesliberacao is not null )
					=
					( select count(*) from ted.previsaoorcamentaria po02 where po02.tcpid = tcp.tcpid AND po02.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisaoanalise,

				( select count(recid) from ted.relatoriocumprimento rec where rec.tcpid = tcp.tcpid ) as relcumprimento,

				CASE WHEN apo.arqid IS NOT NULL
					THEN true
					ELSE false
				END as abaanexo
			FROM
				ted.termocompromisso tcp
			LEFT JOIN ted.arquivoprevorcamentaria apo ON apo.tcpid = tcp.tcpid AND apo.arptipo = 'A'
			WHERE
				tcp.tcpid = {$this->tcpid}
        ";

        //ver($strSQL, d);
        $this->_pendencias = $this->pegaLinha($strSQL);
        return $this;
    }

    /**
     * Faz o tratamento para os dados das pendencias
     * @return object
     */
    protected function _setRegraPendencias()
    {
        if (!is_array($this->_pendencias) || !count($this->_pendencias)) {
            return false;
        }

        $this->_pendencias['abaproponente']		  = $this->_pendencias['abaproponente'] == 't' 		  ? true : false;
        $this->_pendencias['abaconcedente'] 	  = $this->_pendencias['abaconcedente'] == 't' 		  ? true : false;
        $this->_pendencias['abadescentralizacao'] = $this->_pendencias['abadescentralizacao'] == 't'  ? true : false;
        $this->_pendencias['abaprevisao']		  = $this->_pendencias['abaprevisao'] == 't' 		  ? true : false;
        $this->_pendencias['abaparecertecnico']	  = $this->_pendencias['abaparecertecnico'] == 't' 	  ? true : false;
        $this->_pendencias['abaprevisaoanalise']  = $this->_pendencias['abaprevisaoanalise'] == 't'   ? true : false;
        $this->_pendencias['relcumprimento']	  = $this->_pendencias['relcumprimento']>0	 		  ? true : false;
        $this->_pendencias['abaanexo']			  = $this->_pendencias['abaanexo'] == 't'			  ? true : false;

        return $this;
    }

    /**
     * Template para mostragem das pendencias encontradas
     * @param $mensagem
     * @param null $url
     */
    protected function _getTemplate($mensagem, $url = null)
    {
        $template = "
            <tr>
                <td>
                    <div class='text-center'>
                        <a class='' href='{$url}'>
                            <strong>- {$mensagem}.</strong>
                        </a>
                    </div>
                </td>
            </tr>
        ";

        return sprintf($template, $url, $mensagem);
    }

    /**
     * Monta toda a regra de pendencias do termo
     * @return object
     */
    public function buildData()
    {
        $this->_getQueryPendencias();
        $this->_setRegraPendencias();
        $urlBase = 'ted.php?modulo=principal/termoexecucaodescentralizada';

        if(!$this->_pendencias['relcumprimento'] && $estadoAtual == EM_EXECUCAO) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Relatório de cumprimento.',
                $urlBase.'/relatoriocuprimentoobjeto&acao=A&ted='.$this->tcpid
            );
        }

        if(!$this->_pendencias['abaparecertecnico'] && $estadoAtual == EM_ANALISE_OU_PENDENTE
            && (in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis))) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Parecer Técnico.',
                $urlBase.'/parecer&acao=A&ted='.$this->tcpid
            );
        }

        if(!$this->_pendencias['abaprevisaoanalise'] && $estadoAtual == EM_ANALISE_OU_PENDENTE
            && (in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis))) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Previsão Orçamentária.',
                $urlBase.'/previsao&acao=A&ted='.$this->tcpid
            );
        }

        if (!$this->_pendencias['abaproponente']) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Proponente.',
                $urlBase.'/proponente&acao=A&ted='.$this->tcpid
            );
        }

        if (!$this->_pendencias['abaconcedente']) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Concedente.',
                $urlBase.'/concedente&acao=A&ted='.$this->tcpid
            );
        }

        if (!$this->_pendencias['abadescentralizacao']) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Justificativa da Descentralização do Crédito.',
                $urlBase.'/justificativa&acao=A&ted='.$this->tcpid
            );
        }

        if (!$this->_pendencias['abaprevisao'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA)) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Previsão Orçamentária.',
                $urlBase.'/previsao&acao=A&ted='.$this->tcpid
            );
        }

        if (!$this->_pendencias['abaanexo'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA)) {
            $this->_htmlOutput.= $this->_getTemplate(
                'Anexo.',
                $urlBase.'/previsao&acao=A&ted='.$this->tcpid
            );
        }

        return $this;
    }

    /**
     * Retorna todos as pendencias encontradas
     * @return bool|string
     */
    public function getPendencias()
    {
        if (!strlen($this->_htmlOutput)) {
            return false;
        }

        return $this->_htmlOutput;
    }

    /**
     * Lista Reponsável pela UG Proponente
     * @return array|bool|void
     */
    public function getRepresentateLegalTramite()
    {
        $strSQL = "
            select
                hd.hstid,
                us.usucpf,
                us.usunome,
                us.usuemail
            from workflow.historicodocumento hd
                inner join workflow.acaoestadodoc ac on
                    ac.aedid = hd.aedid
                inner join workflow.estadodocumento ed on
                    ed.esdid = ac.esdidorigem
                inner join seguranca.usuario us on
                    us.usucpf = hd.usucpf
                left join workflow.comentariodocumento cd on
                    cd.hstid = hd.hstid
            where
                hd.docid = (select docid from ted.termocompromisso where tcpid = {$this->tcpid})
            and ac.esdidorigem = " . EM_APROVACAO_DA_REITORIA . " -- Representante legal do proponente / Aguardando aprovacao do proponente
                            and ac.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . " -- Em analise do Gabinete da secretaria/autarquia
        ";
        $rsRepProp = $this->pegaLinha($strSQL);

        $strSQL = "
            select
                hd.hstid,
                us.usucpf,
                us.usunome,
                us.usuemail
            from workflow.historicodocumento hd
                inner join workflow.acaoestadodoc ac on
                    ac.aedid = hd.aedid
                inner join workflow.estadodocumento ed on
                    ed.esdid = ac.esdidorigem
                inner join seguranca.usuario us on
                    us.usucpf = hd.usucpf
                left join workflow.comentariodocumento cd on
                    cd.hstid = hd.hstid
            where
                hd.docid = (select docid from ted.termocompromisso where tcpid = {$this->tcpid})
            and ac.esdidorigem = " . EM_EXECUCAO . " -- Em execução
            and ac.esdiddestino = " . ALTERAR_TERMO_COOPERACAO . " -- Solicitação de alteração
        ";
        $rsRepAlt = $this->pegaLinha($strSQL);

        if ($rsRepProp['hstid'] > $rsRepAlt['hstid'] || empty($rsRepProp['hstid'])) {
            return ($rsRepProp) ? $rsRepProp : false;
        } else {
            return false;
        }
    }
}