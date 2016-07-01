<?php

class Par3_Model_Indicador extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.indicador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "indid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'indid' => null,
									  	'areid' => null,
									  	'inddsc' => null,
									  	'indajuda' => null,
									  	'indstatus' => null,
    									'indcod' => null,
									  );

	protected $stOrdem = null;

	public function recuperarIndicadoresGuia($areid)
	{
		$sql = "SELECT distinct
				    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.indajuda,
				    ind.areid
				FROM {$this->stNomeTabela} ind
				INNER JOIN par3.area ard on ind.areid = ard.areid
				INNER JOIN par3.dimensao dim on dim.dimid = ard.dimid
				    	AND dim.dimstatus = 'A'
				WHERE ind.indstatus = 'A'
				AND ind.areid = {$areid}
				ORDER BY ind.indcod
				--limit 1";

		return $this->carregar($sql);
	}

	public function recuperarDados($indid)
	{
		$sql = "SELECT distinct
				    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.indajuda,
				    ind.areid
				FROM {$this->stNomeTabela} ind
				WHERE
				    ind.indid = {$indid}";

		return $this->pegaLinha($sql);
	}

	public function recuperarOrdemIndicadorPorAreid($areid)
	{
		$sql = "SELECT
					max(indcod) as ordem
				FROM {$this->stNomeTabela}
				WHERE areid = {$areid}";

		$this->stOrdem = $this->pegaUm($sql)+1;
	}

	public function verificaSubitensGuia($indid)
	{

		$sql = "SELECT
					count(c.crtid)
				FROM par3.criterio c
				WHERE c.indid = {$indid} AND c.crtstatus = 'A'";

		$nrCriterio = $this->pegaUm($sql);

		if($nrCriterio > 0){
			return false;
		} else {
			return true;
		}

	}

	public function deletarIndicadorGuia($indid)
	{

		if(!$this->verificaSubitensGuia($indid)){
		    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'Não foi possível excluir o indicador, existem subitens cadastrados.');
			return false;
		}

		$this->indid = $indid;
		$this->excluir();
		$this->commit();
	}

	public function preparaSalvar(){

	    $indid = $_POST['indid'] ? $_POST['indid'] : null;

	    $this->indid 		= $indid;
	    $this->indcod 	    = $_POST['ordcod'];
	    $this->inddsc 	    = $_POST['inddsc'];
	    $this->indajuda	    = $_POST['indajuda'];
	    $this->indstatus 	= 'A';
	    $this->areid 		= $_POST['areid'];
	    $this->salvar();
	    $this->commit();

	    return $this->indid;
	}
}