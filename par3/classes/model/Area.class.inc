<?php

class Par3_Model_Area extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.area";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "areid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'areid' => null,
									  	'dimid' => null,
									  	'aredsc' => null,
									  	'arecod' => null,
    									'arestatus' => null,
									  );

	protected $stOrdem = null;

	public function recuperarAreasGuia($dimid)
	{
		$sql = "SELECT DISTINCT
					ad.areid,
					ad.arecod,
					ad.aredsc,
					ad.dimid
			    FROM {$this->stNomeTabela} ad
			    INNER JOIN par3.dimensao d ON d.dimid = ad.dimid
			    	AND d.dimstatus = 'A'
			    WHERE ad.arestatus = 'A'
			    AND ad.dimid = {$dimid}
			    ORDER BY ad.arecod
			    --limit 1";

		return $this->carregar($sql);
	}

	public function recuperarOrdemAreaPorDimid($dimid)
	{
		$sql = "SELECT
					max(arecod) as ordem
				FROM {$this->stNomeTabela}
				WHERE dimid = {$dimid}";

		$this->stOrdem =  $this->pegaUm($sql)+1;
	}

	public function verificaSubitensGuia($areid)
	{

		$sql = "SELECT
					count(areid)
				FROM par3.indicador
				WHERE areid = {$areid}";

		if($this->pegaUm($sql) > 0){
			return false;
		} else {
			return true;
		}

	}

	public function deletarAreaGuia($areid)
	{

		if(!$this->verificaSubitensGuia($areid)){
		    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'Não foi possível excluir a área, existem subitens cadastrados.');
			return false;
		}

		$this->areid = $areid;
		$this->excluir();
		$this->commit();
	}

	public function preparaSalvar(){

	    $areid = $_POST['areid'] ? $_POST['areid'] : null;

	    $this->areid 		= $areid;
	    $this->arecod 		= $_POST['ordcod'];
	    $this->aredsc 		= $_POST['aredsc'];
	    $this->arestatus 	= 'A';
	    $this->dimid 		= $_POST['dimid'];
	    $this->salvar();
	    $this->commit();
	}

}