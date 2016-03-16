<?php

class Par3_Model_CriterioVinculacao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.criterio_vinculacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "crvid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'crvid' => null,
									  	'crtid' => null,
									  	'crtidpai' => null,
									  	'crvvinculo' => null,
									  );

	protected $stOrdem = null;

	public function recuperarDadosListaVinculacao($crtid){
	    $sql = "SELECT
	                cv.crvid,
            	    cv.crtid,
            	    cv.crvvinculo
        	    FROM {$this->stNomeTabela} cv
        	    WHERE cv.crtidpai = {$crtid}";

	    return $this->carregar($sql);
	}

	public function recuperaQuantidadeVinculacao($crtid){
	    $sql = "SELECT
	                count(cv.crvid) as quantidade
        	    FROM {$this->stNomeTabela} cv
        	    WHERE (cv.crtidpai = {$crtid} OR cv.crtid = {$crtid})";

	    return $this->pegaUm($sql);
	}

	public function preparaSalvar(){

	    $crtid = $_POST['crtid'] ? $_POST['crtid'] : null;

	    $this->crtid 		= $crtid;
	    $this->crtdsc 	    = $_POST['crtdsc'];
	    $this->crtcod       = $_POST['ordcod'];
	    $this->crtstatus 	= 'A';
	    $this->indid 		= $_POST['indid'];
	    $this->crtaplica 	= $_POST['crtaplica'];
	    $this->salvar();
	    $this->commit();
	}

}