<?php

class ParametrosSistema extends Modelo {

    const PERIODO_DETALHAMENTO_AVALIACAO = "PERIODO_DETALHAMENTO_AVALIACAO";
    
     /** 
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.parametrossistema";   

    /** 
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "psid" );
    
    /** 
     * Atributos
     * @var array
     * @access protected
     */    

    protected $arAtributos = array(
                                    'psid' => null, 
                                    'psnome' => null,
                                    'psdsc' => null, 
                                    'psvalor' => null
                                  );  
    
                                  
    protected $stCampos = "psid, psnome, psdsc, psvalor";
                         
    public function getId(){
        return $this->arAtributos['psid'];
    }
    
    public function getNome(){
        return $this->arAtributos['psnome'];
    }
    
    public function getDescricao(){
        return $this->arAtributos['psdsc'];
    }
    
    public function getValor(){
        return $this->arAtributos['psvalor'];
    }
    
    public function setId($id){
        $this->arAtributos['psid'] = $id;
    }
    
    public function setNome($nome){
        $this->arAtributos['psnome'] = $nome;
    }
    
    public function setDescricao($descricao){
        $this->arAtributos['psdsc'] = $descricao;
    }
    
    public function setValor($valor){
        $this->arAtributos['psvalor'] = $valor;
    }
    
    /**
     *Retorna um array de dados de Parametros do Sistema
     * @param type $nomeParametroSistema
     * @return type array 
     */
    public function recuperaPeloNome($nomeParametroSistema){
        
        $sql = "select * from $this->stNomeTabela where psnome = '$nomeParametroSistema'";
        $dados = $this->pegaLinha($sql);
        
        $this->psid = $dados['psid'];
        $this->psnome = $dados['psnome'];
        $this->psdsc = $dados['psdsc'];
        $this->psvalor = $dados['psvalor'];
        
        return $dados;
    }
    
    /**
     * Recupera o valor do Parametro do Sistema
     */
    public function recuperaValorPeloNome($nomeParametroSistema){
        $sql = "select psvalor from $this->stNomeTabela where psnome = '$nomeParametroSistema'";
        $dados =  $this->pegaLinha($sql);
        return $dados['psvalor']==NULL?0:$dados['psvalor'];
    }
    
	//FIXME: melhorar lógica de pegar o objeto sem passar por dois foreach
	public function salvar(){
		$novoIdMemorando = parent::salvar($arAtributos);
		$listaDeOs = $this->getListaDeOrdensDeServico();
		foreach ($listaDeOs as $arrayDeOs){
			foreach ($arrayDeOs as $os){
//					echo $os->getId();
				$this->atribuirMemorandoAOrdensDeServico($novoIdMemorando, $os->getId());
			}
		}
	}
	
	public function atualize($parametroSistema){
		$valor = $parametroSistema->getValor();
		$descricao = $parametroSistema->getDescricao();
		$nome = $parametroSistema->getNome();
		$id = $parametroSistema->getId();
		$sql = "update $this->stNomeTabela set psvalor = '$valor', psdsc = '$descricao', psnome = '$nome' where psid = '$id'";
		parent::executar($sql);
	}

}
