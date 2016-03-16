<?php

class Par3_Model_Pendencias extends Modelo
{
	/*
	 * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.vm_total_pendencias";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'obrid' => null,
									  	'preid' => null,
									  	'predescricao' => null,
									  	'estuf' => null,
									  	'descricao' => null,
									  	'muncod' => null,
									  	'descricao' => null,
									  	'situacao' => null,
									  	'pendencia' => null
									  );

    public function __construct()
    {

    }
    
    /**
     * Função montarFiltroPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function montarFiltroPendencias($arrPost){
    	
    	$where = array();
    
    	if ($_SESSION['par']['estuf'] != '') $where[] = "estuf = '{$_SESSION['par']['estuf']}'";
    	
    	if ($_SESSION['par']['muncod'] != '') $where[] = "muncod = '{$_SESSION['par']['muncod']}'";

    	if ($_SESSION['par']['obrid'] != '') $where[] = "obrid = '{$_SESSION['par']['obrid']}'";
    	
    	if ($_SESSION['par']['preid'] != '') $where[] = "dopstatus = '{$_SESSION['par']['preid']}'";
    	 
    	return $where;
    
    }//end motnarFiltroPAR()
    
	public function recuperarPendenciasEntidade($inuid, $estuf = null, $muncod = null, $itrid = null){
		
		$GLOBALS['nome_bd'] = 'simec_espelho_producao';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = '';
		
	
 		$where = " estuf = '{$GLOBALS['estuf']}'";
		if ($itrid === '2') $where = " muncod = '".$GLOBALS['muncod']."'";
		
		$sql = "select
				obrid,preid,predescricao,estuf,descricao,muncod,descricao,situacao,pendencia
				from obras2.vm_total_pendencias
				 WHERE ".$where ." order by pendencia";
		
		$dados = $this->carregar($sql);
// 	dbg($dados);
		return $dados;
	}

	public function recuperarTiposPendencia(){
	
		$GLOBALS['nome_bd'] = '';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '5432';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = '';
	
		$sql = "select
				distinct pendencia
				from obras2.vm_total_pendencias order by pendencia";
			
		$dados = $this->carregarColuna($sql);

		return $dados;
	}
	
	public function existePendenciaPorTipo($pendencia, $inuid, $estuf = null, $muncod = null, $itrid = null){
	
		$GLOBALS['nome_bd'] = '';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '5432';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = '';
		
		$where = " estuf = '{$GLOBALS['estuf']}'";
		if ($itrid === '2') $where = " muncod = '".$GLOBALS['muncod']."'";
	
		$sql = "select
				obrid
				from obras2.vm_total_pendencias
				WHERE ".$where." AND pendencia = '$pendencia' order by pendencia ";
			
		$dados = $this->pegaUm($sql);
	
		return $dados;
	}

}
