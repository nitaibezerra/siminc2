<?php

class Par3_Model_Dimensao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.dimensao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dimid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'dimid' => null,
									  	'dimcod' => null,
									  	'dimdsc' => null,
									  	'dimstatus' => null,
    									'itrid' => null,
									  );
	protected $stOrdem = null;

	public function recuperarDimensoesGuia($itrid)
	{
		$sql = "SELECT DISTINCT
					d.dimid,
					d.dimcod,
					d.dimdsc,
					d.itrid
				FROM {$this->stNomeTabela} d
				INNER JOIN par3.instrumento i ON i.itrid = d.itrid
				WHERE d.dimstatus = 'A'
				AND d.itrid = {$itrid}
				ORDER BY d.dimcod
				--limit 1
				";

		return $this->carregar($sql);
	}

	public function recuperarOrdemDimensaoPorItrid($itrid)
	{
		$sql = "SELECT
					max(dimcod) as ordem
				FROM {$this->stNomeTabela}
				WHERE itrid = {$itrid}";

		$this->stOrdem =  $this->pegaUm($sql)+1;
	}

	public function verificaSubitensGuia($dimid)
	{

		$sql = "SELECT
					count(areid)
				FROM par3.area
				WHERE dimid = {$dimid}";

		if($this->pegaUm($sql) > 0){
			return false;
		} else {
			return true;
		}

	}

	public function deletarDimensaoGuia($dimid)
	{

		if(!$this->verificaSubitensGuia($dimid)){
		    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'Não foi possível excluir a dimensão, existem subitens cadastrados.');
			return false;
		}

		$this->dimid = $dimid;
		$this->excluir();
		$this->commit();
	}

	public function preparaSalvar(){

	    $dimid = $_POST['dimid'] ? $_POST['dimid'] : null;

	    $this->dimid 		= $dimid;
	    $this->dimcod 		= $_POST['ordcod'];
	    $this->dimdsc 		= $_POST['dimdsc'];
	    $this->dimstatus 	= 'A';
	    $this->itrid 		= $_POST['itrid'];
	    $this->salvar();
	    $this->commit();
	}

}