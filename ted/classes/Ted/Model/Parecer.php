<?php

/**
 * Class Ted_Model_Parecer
 */
class Ted_Model_Parecer extends Modelo
{
	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.parecertecnico';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ptecid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'ptecid' 			  => NULL,			
		'considentproponente' => NULL,
		'considproposta' 	  => NULL,
		'considobjeto' 		  => NULL,
		'considobjetivo' 	  => NULL,
		'considjustificativa' => NULL,
		'considvalores' 	  => NULL,
		'considcabiveis' 	  => NULL,
		'usucpfparecer' 	  => NULL,
		'tcpid' 			  => NULL,
	);
	
	public function __construct($tcpid = null)
    {
		$this->arAtributos['tcpid'] = ($tcpid) ? $tcpid : Ted_Utils_Model::capturaTcpid();
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
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
		    if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]))
			    return false;
			
		return true;
	}
	
	/**
	 * Cadastrar Parecer Técnico para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	function cadastrarParecerTecnico()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->arAtributos['ptecid'] = $this->inserir();
			return $this->commit();
		}
			
		return false;
	}
	
	/**
	 * Atualizar Parecer Técnico para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarParecerTecnico()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->alterar();
			return $this->commit();
		}
		return false;
	}
	
	/**
	 * Captura dados para utilização na Aba de Parecer Técnico.
	 * @return Ambigous <boolean, multitype:>
	 */
	public function capturaDadosParecerTecnico()
    {
		$strSQL ="
            SELECT
                ptecid,
                considentproponente,
                considproposta,
                considobjeto,
                considobjetivo,
                considjustificativa,
                considvalores,
                considcabiveis,
                usucpfparecer,
                usucpfparecer || ' - ' ||seg.usunome as usunome
            FROM
                {$this->stNomeTabela}
            LEFT JOIN seguranca.usuario seg on(seg.usucpf = {$this->stNomeTabela}.usucpfparecer)
            WHERE
                tcpid = {$this->arAtributos['tcpid']}
	    ";
			
		$consulta = $this->pegaLinha($strSQL);
		return $consulta != null ? $consulta : $this->criarParecerTecnico();
	}
	
	/**
	 * Cria Parecer Técnico caso não exista
	 */
	public function criarParecerTecnico()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->cadastrarParecerTecnico();
			return $this->capturaDadosParecerTecnico();
		}
		return false;
	}
	
	/**
	 * Aba de Parecer Técnico
	 * @param $_POST $dados contendo os campos do formulário referentes às colunas da tabela
	 * @return boolean
	 */
	function gravarTermoParecerTecnico($dados)
	{
		$this->popularDadosObjeto($dados);
		$this->arAtributos['usucpfparecer'] = $_SESSION['usucpf'];
		if (!is_null($this->arAtributos['ptecid'])) {
			if ($this->validaCamposObrigatorios()) {
				return $this->atualizarParecerTecnico();
			}	
		}		
		return false;
	}

    private function getTemplatePrint()
    {
        return "
            <h1>Parecer Técnico (Entidade Concedente)</h1>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre a entidade proponente:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre a proposta:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre o objeto:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre o objetivo:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre a justificativa:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Considerações sobre os valores:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Outras considerações cabíveis:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
            <p>
                <strong style='text-align:left;font-size:20px;'>Parecer Técnico elaborado por:</strong><br />
                <span style='text-align:justify;'>%s</span>
            </p>
        ";
    }

    public function getPrint()
    {
        $resulset = $this->capturaDadosParecerTecnico();
        if ($resulset) {
            unset($resulset['ptecid'], $resulset['usucpfparecer']);
            $template = $this->getTemplatePrint();
            echo vsprintf($template, $resulset);
        }

        return false;
    }
}