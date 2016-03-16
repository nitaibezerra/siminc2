<?php
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Ted_Model_Arquivo extends Modelo
{
	const ABA_PARECER_TECNICO = 'P';
	const ABA_ANEXO = 'A';
	const ABA_TRAMITE = 'T';
	const ABA_JURIDICO_PROPONENTE = 'JP';
	const ABA_JURIDICO_CONCEDENTE = 'JC';

	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.arquivoprevorcamentaria';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('arqid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'arqid' => NULL,
		'tcpid' => NULL,
		'arpdtinclusao' => NULL,
		'arpstatus' => NULL,
		'arpdsc' => NULL,
		'arptipo' => NULL
	);

    /**
     * @var
     */
    private $messageErro;

    /**
     *
     */
    public function __construct()
	{
		$this->arAtributos['tcpid'] = Ted_Utils_Model::capturaTcpid();
		if(is_null($this->arAtributos['tcpid']))
		{
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
     * Função para a inserção de anexo na Abas: Parecer Técnico (P), Anexos (A) e Trâmite (T).
     * @param $dados
     * @param $tipoAnexo
     * @return bool
     */
    public function inserirAnexo($dados, $tipoAnexo)
	{
		$descricao = $dados['descricaoanexo'];
		$anexoCod  = $dados['anexoCod'];

        if (!is_array($anexoCod) || count($anexoCod) === 0)
            return false;

		foreach ($anexoCod as $key => $value) {

			$descricao[$key] = substr($descricao[$key], 0, 255);
			$campos = array(
                "tcpid"=> $_GET['ted'],
                "arpdsc" => "'{$descricao[$key]}'",
                "arptipo" => "'{$tipoAnexo}'"
            );
			$file = new FilesSimec('arquivoprevorcamentaria', $campos, 'ted');
			if (!empty($dados["anexo_{$key}"]['name']) && $anexoCod[$key] != '') {
				$arquivoSalvo = $file->setUpload($descricao[$key], "anexo_{$value}");
			}
		}

        return $arquivoSalvo;
	}
	
	/**
	 * Desativo um anexo através do seu ID
	 * @param INT $arqid
	 * @return boolean
	 */
	public function desativarAnexo($arqid)
	{
        if ($this->getSchema($arqid)) {
            $strSQL = "UPDATE {$this->stNomeTabela} SET arpstatus = 'I' WHERE arqid = {$arqid}";
            $this->executar($strSQL);
            return $this->commit();
        }
	}
	
	public function listarAnexos($tipo)
	{
		$sql = "
			SELECT DISTINCT
                m.arqid,
                a.arqnome AS arqnome,
                --m.arpdsc,
                a.arqdescricao AS arqdescricao,
                su.usunome,
                TO_CHAR(a.arqdata, 'DD/MM/YYYY') AS criado
            FROM {$this->stNomeTabela} m
            LEFT JOIN public.arquivo a ON a.arqid = m.arqid
            JOIN seguranca.usuario su ON (su.usucpf = a.usucpf)
            WHERE
                arpstatus = 'A'
            AND
                tcpid = {$this->arAtributos['tcpid']} and m.arptipo = '{$tipo}'
	        ORDER BY 1
		";
        //ver($sql);
	
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
		->setQuery($sql);
		
		$list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
             ->turnOnPesquisator()
             ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
	}

    /**
     * Procura o caminho fisico do arquivo, baseado nos schemas [ted, monitora, elabrev]
     * @param $arqid
     * @return bool|string
     */
    private function getSchema($arqid)
    {
        $strSQL ="SELECT * FROM public.arquivo WHERE arqid = {$arqid}";
        if (!$this->pegaLinha($strSQL)) {
            return false;
        }

        $optionsSchema = array(
            'elabrev', 'monitora', 'ted', 'public'
        );

        require_once APPRAIZ . 'includes/classes/file.class.inc';
        $fileSys = new Files();
        foreach ($optionsSchema as $schema) {
            $caminho = APPRAIZ.'arquivos/'.$schema.'/'.floor($arqid/1000).'/'.$arqid;
            if ($fileSys->Download($caminho)) {
                return $caminho;
            }
        }

        return false;
    }

    /**
     * Faz o download do arquivo, se houver arquivo
     * @param $arqid
     * @return bool
     */
    public function getDownload($arqid)
    {
        $strSQL ="SELECT * FROM public.arquivo WHERE arqid = {$arqid}";
        $arquivo = $this->pegaLinha($strSQL);
        if (!$arquivo) {
            $this->messageErro = 'Arquivo não encontrado.';
            $this->getErroSimec();
            return false;
        }

        $caminho = $this->getSchema($arqid);
        if (!$caminho) {
            $this->messageErro = 'Path not found!';
            $this->getErroSimec();
            return false;
        }

        $filename = str_replace(' ', '_', $arquivo['arqnome']);
        $filename = "$filename.{$arquivo['arqextensao']}";

        header('Content-type:'. $arquivo['arqtipo']);
        header('Content-Disposition:attachment;filename='.$filename);
        readfile($caminho);
        exit();
    }

    /**
     *
     */
    private function getErro()
    {
        if ($this->messageErro) {
            echo '<script type="text/javascript"> alert(" '.$this->messageErro.'");</script>';
        }
    }

    /**
     * @return bool
     */
    private function getErroSimec()
    {
        if ($this->messageErro) {
            $this->rollback();
            $this->getErro();
            return false;
        }else{
            return true;
        }
    }
}