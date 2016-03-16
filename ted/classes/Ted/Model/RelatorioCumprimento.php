<?php

/**
 * Class Ted_Model_RelatorioCumprimento
 * @author Lucas Gomes
 * @author Lindalberto Rufino
 */
class Ted_Model_RelatorioCumprimento extends Modelo
{
	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.relatoriocumprimento';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('recid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		"recid"=> NULL,
		"tcpid"=> NULL,
		"recnomeresponsavel" => NULL,
		"recatividadesprevistas" => NULL,
		"recendereco" => NULL,
		"recmetaprevista"=> NULL,
		"recatividadesexecutadas"=> NULL,
		"recemailresposavel"=> NULL,
		"recvlrrecebido"=> NULL,
		"recdificuldades"=> NULL,
		"recnumportaria"=> NULL,
		"recnumncdevolucao"=> NULL,
		"reccomentarios"=> NULL,
		"reccep"=> NULL,
		"recstatus"=> NULL,		
		"uocod"=> NULL,
		"recvlrutilizado"=> NULL,
		"recexpedidorrgresposavel"=> NULL,
		"reccargo"=> NULL,		
		"estuf"=> NULL,
		"recmetasadotadas"=> NULL,
		"recdtemissaorgresposavel"=> NULL,
		"recsiaperesponsavel"=> NULL,
		"gestaocod"=> NULL,
		"rectelefone"=> NULL,
		"recexecucaoobjeto"=> NULL,
		"recrgresponsavel"=> NULL,
		"ugcod"=> NULL,
		"recdtpublicacao"=> NULL,
		"reccpfresponsavel"=> NULL,
		"recmetaexecutada"=> NULL,
		"recnumnotacredito"=> NULL,
		"recnome"=> NULL,
		"muncod"=> NULL,
		"recvlrdevolvido"=> NULL,
		"reccnpj"=> NULL
	);

    /**
     * @throws
     */
	public function __construct()
    {
		$this->arAtributos['tcpid'] = Ted_Utils_Model::capturaTcpid();
		if (is_null($this->arAtributos['tcpid'])) {
			throw new Exception("Nenhum Termo encontrado.");
		}
	}
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'tcpid'
	);
	
	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende - Copiador por Lindalberto Filho
	 * @return bool
	*/
	public function validaCamposObrigatorios()
    {
		foreach($this->arAtributosObrigatorios as $chave => $valor) {
            if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]))
                return false;
        }

		return true;
	}
	
	/**
	 * Cadastrar Relatório de Cumprimento para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	function cadastrarRelatorio()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->arAtributos['recid'] = $this->inserir();
			return ($this->commit()) ? $this->arAtributos['recid'] : false;
		}
			
		return false;
	}
	
	/**
	 * Atualizar Relatório de Cumprimento para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarRelatorio()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->alterar();
			return $this->commit();
		}
		return false;
	}
	
	
	public function capturaRelatorioCumprimento()
    {
		$sql = "
			SELECT
				recid, 
				tcpid, 
				reccnpj, 
				recnome, 
				recendereco, 
				muncod, 
				estuf, 
				reccep,				
				substr(rectelefone, 1, 2) as rectelefoneddd,
				substr(rectelefone, 3, 9) as rectelefone,
				uocod, 
				ugcod, 
				gestaocod, 
				recnomeresponsavel, 
				reccpfresponsavel,
				recsiaperesponsavel, 
				recrgresponsavel, 
				to_char(recdtemissaorgresposavel, 'DD/MM/YYYY') as recdtemissaorgresposavel,
				recexpedidorrgresposavel, 
				reccargo, 
				recemailresposavel, 
				recnumportaria,
				to_char(recdtpublicacao, 'DD/MM/YYYY') as recdtpublicacao,
				recnumnotacredito, 
				recexecucaoobjeto, 
				recatividadesprevistas,
				recmetaprevista, 
				recatividadesexecutadas, 
				recmetaexecutada, 
				recdificuldades,
				recmetasadotadas, 
				reccomentarios, 
				recvlrrecebido, 
				recvlrutilizado,
			    recvlrdevolvido, 
				recnumncdevolucao, 
				recstatus
			FROM {$this->stNomeTabela}
			WHERE recstatus = 'A' 
				AND
			tcpid = ".$this->arAtributos['tcpid'];
            //ver($sql, d);
        $consulta = $this->pegaLinha($sql);
        return ($consulta) ? $consulta : null;
	}

	/**
	 * Cria ou atualiza um RCO
	 */
    public function save(array $dados)
    {
        $dados = $this->cleanUpData($dados);
        $dados['rectelefone'] = $dados['rectelefoneddd'].$dados['rectelefone'];

        $this->popularDadosObjeto($dados);
        if ($this->validaCamposObrigatorios()) {
            if (!empty($this->arAtributos['recid'])) {
                $return = $this->atualizarRelatorio();
            } else {
                $return = $this->cadastrarRelatorio();
            }

            if (is_numeric($return)) {
                $dados['recid'] = $return;
            }
            $this->saveNC($dados);
            return $return;
        }

        return false;
    }

    /**
     * Traz os dados para preenchimento padrão(automatico)
     * do relatório de cumprimento do objeto
     * @param $tcpid
     * @return array|bool
     */
    public function preenchimentoPadraoDoObjeto($tcpid)
    {
        if (!is_numeric($tcpid) && $tcpid < 1) {
            return false;
        }

        $strSQL = "
            SELECT
                u.ungcod, u.ungdsc, u.ungstatus, u.unicod, u.unitpocod, u.ungabrev, u.ungcnpj,
                u.ungendereco, u.ungfone, e.estuf, u.muncod, u.ungemail, u.ungbairro, u.ungcep, u.gescod
            FROM public.unidadegestora u
                JOIN territorios.municipio m ON (m.muncod = u.muncod)
                JOIN territorios.estado e ON (e.estuf = m.estuf)
            WHERE u.ungcod = (
              SELECT ungcodproponente FROM ted.termocompromisso tc WHERE tc.tcpid = {$tcpid}
            )
        ";

        $result = $this->pegaLinha($strSQL);
        if ($result) {

            $foneDddTmp = explode('-', $result['ungfone']);

            if (count($foneDddTmp) == 3) {
                $ddd = $foneDddTmp[0];
                $phone = $foneDddTmp[1].'-'.$foneDddTmp[2];
            }
            else if (count($foneDddTmp) == 2) {
                $ddd = '';
                $phone = $foneDddTmp[0].'-'.$foneDddTmp[1];
            }
            else {
                $ddd = '';
                $phone = '';
            }

            $array_retorno = array(
                'reccnpj'         => $result['ungcnpj'],
                'recnome'         => $result['ungdsc'],
                'recendereco'     => $result['ungendereco'],
                'estuf'           => $result['estuf'],
                'muncod'          => $result['muncod'],
                'reccep'          => $result['ungcep'],
                'rectelefoneddd'  => $ddd,
                'rectelefone'     => $phone,
                'uocod'           => $result['unicod'],
                'ugcod'           => $result['ungcod'],
                'gestaocod'       => $result['gescod'],
            );

            $modelWorkflow = new Ted_Model_Workflow();
            $representante = $modelWorkflow->getRepresentateLegalTramite();
            if ($representante) {
                $array_retorno['recnomeresponsavel'] = $representante['usunome'];
                $array_retorno['reccpfresponsavel']  = $representante['usucpf'];
                $array_retorno['recemailresposavel'] = $representante['usuemail'];
            }
        }

        return ($array_retorno) ? $array_retorno : false;
    }

    /**
     * Opções para o combo "Execução do Objeto" do formulário de Relatorio de Cumprimento do Objeto
     * @return array
     */
    public static function getOptionsExecucaoObjeto()
    {
        return array(
            1 => 'Houve cumprimento TOTAL',
            2 => 'Houve cumprimento PARCIAL',
            3 => 'Houve devolução integral'
        );
    }

    /**
     * Pega todas as NC de devolução cadastradas no relatorio de cumprimento do objeto
     */
    public function mostraNcDevolucao()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();
        $strSQL = "select * from ted.ncrelatoriocumprimento where tcpid = {$tcpid} and rcndevolucao = 't'";
        $results = $this->carregar($strSQL);
        //ver($results, $strSQL);
        echo '<table class="table"><tbody>';
        if ($results) {
            foreach ($results as $nc) {
                echo '<tr>
                        <td class="info">
                            <div class="col-md-1">
                                <span id="'.$nc['rcnid'].'" class="glyphicon glyphicon-trash remove-nc"></span>
                            </div>
                            <div class="col-md-8">
                                <input type="text" class="form-control" name="tmp_recnumnotacredito_dev[]" value="'.$nc['rcnnumnc'].'" />
                            </div>
                        </td>
                     </tr>';
            }
        }
        echo '</tbody></table>';
    }

    /**
     * Pega todas as notas de crédito cadastradas no relatorio de cumprimentio do objeto
     */
    public function mostraNc()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();
        $strSQL = "select * from ted.ncrelatoriocumprimento where tcpid = {$tcpid} and rcndevolucao = 'f'";
        $results = $this->carregar($strSQL);
        //ver($results, $strSQL);
        echo '<table class="table"><tbody>';
        if ($results) {
            foreach ($results as $nc) {
                echo '<tr>
                        <td class="info">
                            <div class="col-md-1">
                                <span id="'.$nc['rcnid'].'" class="glyphicon glyphicon-trash remove-nc"></span>
                            </div>
                            <div class="col-md-8">
                                <input type="text" class="form-control" name="tmp_recnumnotacredito[]" value="'.$nc['rcnnumnc'].'" />
                            </div>
                        </td>
                     </tr>';
            }
        }
        echo '</tbody></table>';
    }

    /**
     * @param array $dados
     * @return bool
     */
    protected function _insertNotaCrecito(array $dados)
    {
        $strSQL = "insert into ted.ncrelatoriocumprimento(tcpid, recid, rcnnumnc, rcndevolucao, rpustatus)
                   values(%d, %d, '%s', '%s', '%s')";
        $stmt = sprintf($strSQL, $this->arAtributos['tcpid'], $dados['recid'], $dados['rcnnumnc'], $dados['rcndevolucao'], 'A');
        //return $stmt;
        $this->executar($stmt);
        return ($this->commit()) ? true : false;
    }

    /**
     * @param $rcnid
     * @return bool
     */
    public function apagarNotaCredito($rcnid)
    {
        $strSQL = "DELETE FROM ted.ncrelatoriocumprimento WHERE rcnid = %d";
        $stmt = sprintf($strSQL, (int) $rcnid);
        $this->executar($stmt);
        return ($this->commit()) ? true : false;
    }

    /**
     * @param array $data
     * @return bool
     */
    public function saveNC(array $data)
    {
        $strSQL = "delete from ted.ncrelatoriocumprimento where tcpid = {$this->arAtributos['tcpid']}";
        $this->executar($strSQL);
        $this->commit();

        if (count($data['tmp_recnumnotacredito'])) {
            foreach ($data['tmp_recnumnotacredito'] as $nc) {
                $_data = array(
                    'recid' => $data['recid'],
                    'rcnnumnc' => $nc,
                    'rcndevolucao' => 'f',
                    'rpustatus' => 'A'
                );
                $this->_insertNotaCrecito($_data);
            }
        }

        if (count($data['tmp_recnumnotacredito_dev'])) {
            foreach ($data['tmp_recnumnotacredito_dev'] as $ncd) {
                $_data = array(
                    'tcpid' => $this->arAtributos['tcpid'],
                    'recid' => $data['recid'],
                    'rcnnumnc' => $ncd,
                    'rcndevolucao' => 't',
                    'rpustatus' => 'A'
                );
                $this->_insertNotaCrecito($_data);
            }
        }

        return true;
    }

    /**
     * Formata os valores para persistir em banco
     * @param $post
     * @return mixed
     */
    private function cleanUpData($post)
    {
        $arrayCleanFormat = array(
            'reccep',
            'rectelefone',
            'reccpfresponsavel',
            'reccnpj'
        );

        foreach ($arrayCleanFormat as $key) {
            if (isset($post[$key])) {
                $post[$key] = str_replace(array('.', '-', '/'), '', $post[$key]);
            }
        }

        $arrayValorMonetario = array(
            'recvlrrecebido',
            'recvlrutilizado',
            'recvlrdevolvido'
        );

        foreach ($arrayValorMonetario as $key) {
            if (isset($post[$key])) {
                $post[$key] = str_replace('.', '', $post[$key]);
                $post[$key] = str_replace(',', '.', $post[$key]);
                $post[$key] = ($post[$key] == '') ? 0 : trim($post[$key]);
            }
        }

        $datasPadrao = array(
            'recdtemissaorgresposavel',
            'recdtpublicacao'
        );

        foreach ($datasPadrao as $key) {
            if (isset($post[$key])) {
                $post[$key] = Ted_Utils_Model::formatDateUs($post[$key]);
            }
        }

        return $post;
    }

    /**
     * @param $tipo
     */
    public function listarAnexos($recid)
    {
        $strSQL = "
            SELECT
                arq.arqid,
                arq.arqnome AS descricao,
                arq.arqnome||'.'||arq.arqextensao,
                su.usunome,
                to_char(arq.arqdata, 'DD/MM/YYYY')
            FROM ted.relatoriocumprimentoanexo anx
                JOIN public.arquivo arq on (arq.arqid = anx.arqid)
                JOIN seguranca.usuario su ON (su.usucpf = anx.usucpf)
            WHERE
                arq.arqstatus = 'A' AND recid = {$recid}
            ORDER BY 1
        ";
        //ver($strSQL);

        require_once APPRAIZ . 'includes/library/simec/Listagem.php';
        $list = new Simec_Listagem();
        $list->setCabecalho(array(
                'Anexos - Parecer Técnico' => array(
                    'Arquivo',
                    'Descrição',
                    'Usuário',
                    'Data de Inserção'
                )
            ))
            ->addAcao('delete', 'desativarAnexo')
            ->addAcao('view', 'downloadAnexo')
            ->setQuery($strSQL);

        $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * @return bool
     */
    public function momentoRCOemAnaliseCoordenacao()
    {
        $situacao = Ted_Utils_Model::pegaSituacaoTed();

        if ($situacao['esdid'] == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD
            && (Ted_Utils_Model::possuiPerfil(array(COORDENADOR_SECRETARIA_AUTARQUIA, PERFIL_SUPER_USUARIO))
            || Ted_Utils_Model::uoEquipeTecnicaConcedente())) {
            return true;
        }

        return false;
    }

    /**
     * @return bool
     */
    public function permitePreenchimentoRCO()
    {
        $situacao = Ted_Utils_Model::pegaSituacaoTed();

        if (in_array($situacao['esdid'], array(EM_EXECUCAO, TERMO_EM_DILIGENCIA_RELATORIO))) {
            if ((Ted_Utils_Model::possuiPerfil(array(UO_EQUIPE_TECNICA, PERFIL_SUPER_USUARIO)) && Ted_Utils_Model::uoEquipeTecnicaProponente())
                || Ted_Utils_Model::possuiPerfil(array(PERFIL_COORDENADOR_SEC, PERFIL_SUPER_USUARIO))) {
                return true;
            }
        }

        return false;
    }

    /**
     * Validação usando zend form
     * @param Zend_Form $form
     * @param array $fromPost
     * @return bool
     */
    public function isValid(Zend_Form &$form, array $fromPost = array())
    {
        if ($form->isValid($fromPost)) {
            $isValid = true;

            /**
             * validação de datas do formulário
             */
            $recdtpublicacao = formata_data_sql($fromPost['recdtpublicacao']);
            $recdtemissaorgresposavel = formata_data_sql($fromPost['recdtemissaorgresposavel']);

            $recdtpublicacao = Ted_Utils_Model::getDateFragment($recdtpublicacao);
            $recdtemissaorgresposavel = Ted_Utils_Model::getDateFragment($recdtemissaorgresposavel);

            if (is_array($recdtpublicacao) && !checkdate($recdtpublicacao['mm'], $recdtpublicacao['dd'], $recdtpublicacao['yy'])) {
                $form->getElement('recdtpublicacao')->addError('Erro no formato da data. exemplo: dd/mm/yyyy');
                $form->markAsError();
                $_POST['recdtpublicacao'] = '';
                $isValid = false;
            }

            if (is_array($recdtemissaorgresposavel)
                && !checkdate($recdtemissaorgresposavel['mm'], $recdtemissaorgresposavel['dd'], $recdtemissaorgresposavel['yy']))
            {
                $form->getElement('recdtemissaorgresposavel')->addError('Erro no formato da data. exemplo: dd/mm/yyyy');
                $form->markAsError();
                $_POST['recdtemissaorgresposavel'] = '';
                $isValid = false;
            }

            /**
             * Valida se está sendo inserido nota de crédito
             */
            if (!is_array($fromPost['tmp_recnumnotacredito'])) {
                $form->getElement('recnumnotacredito')->addError('É obrigatório o cadastro da Nota de Crédito');
                $form->markAsError();
                $_POST['tmp_recnumnotacredito'] = array();
                $isValid = false;
            } else {
                foreach ($fromPost['tmp_recnumnotacredito'] as $nc) {
                    if (empty($nc)) {
                        $form->getElement('recnumnotacredito')->addError('É obrigatório o cadastro da Nota de Crédito');
                        $form->markAsError();
                        $_POST['tmp_recnumnotacredito'] = array();
                        $isValid = false;
                        break;
                    }
                }
            }
        }

        return $isValid;
    }

}