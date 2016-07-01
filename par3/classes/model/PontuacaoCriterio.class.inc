<?php

class Par3_Model_PontuacaoCriterio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.pontuacaocriterio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'ptcid' => null,
									  	'ptoid' => null,
									  	'crtid' => null
									  );

    public function recuperarDados($ptoid){
        $sql = "SELECT
                    pc.ptcid,
                    pc.crtid
                FROM
                    {$this->stNomeTabela} pc
                WHERE
                    pc.ptoid = {$ptoid}";

        return $this->carregar($sql);
    }

	public function preparaExcluir($ptcid){

	    $this->ptcid = $ptcid;
	    $this->excluir();
	    $this->commit();
	}

	public function preparaSalvar($ptoid, $crtid){

	    $this->ptoid = $ptoid;
	    $this->crtid = $crtid;
	    $this->salvar();
	    $this->commit();
	}
}