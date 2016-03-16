<?php

class Par3_Model_Pontuacao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.pontuacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'ptoid' => null,
									  	'inuid' => null,
									  	'ptopontuacao' => null,
									  	'ptodata' => null,
									  	'usucpf' => null,
    									'ptojustificativa' => null,
    									'indid' => null
									  );

    public function buscaPontuacao($inuid, $indid){
        $sql = "SELECT
                    p.ptoid
                FROM
                    {$this->stNomeTabela} p
                WHERE
                    p.inuid = {$inuid} AND
                    p.indid = {$indid}";

        return $this->pegaUm($sql);
    }

	public function preparaSalvar($ptoid = null, $inuid, $ptopontuacao, $usucpf, $ptojustificativa, $indid){

	    $ptoid = $ptoid ? $ptoid : null;

	    $this->ptoid            = $ptoid;
	    $this->inuid            = $inuid;
	    $this->ptopontuacao     = $ptopontuacao;
	    $this->usucpf           = $usucpf;
	    $this->ptojustificativa = $ptojustificativa;
	    $this->indid            = $indid;
	    $this->salvar();
	    $this->commit();

	    return $this->ptoid;
	}
}