<?php

class Acomporc_Model_QuestionarioPerguntas extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionarioperguntas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qprid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qprid' => null,
        'qstid' => null,
        'qprpergunta' => null,
        'qprnumcaracteres' => null,
        'qprobrigatorio' => null,
        'qprrespostapadrao' => null,
        'qprordem' => null,
    );
    
    public function buscaPorId(){
        $query = <<<DML
            SELECT 
                qprid, 
                qprpergunta, 
                qprnumcaracteres, 
                qprobrigatorio, 
                qprrespostapadrao, 
                qprordem 
            FROM {$this->stNomeTabela}
            WHERE qprid = {$this->arAtributos['qprid']}
DML;
        return $this->pegaLinha($query);
    }

    public function pegaUltimoOrdem()
    {
        $query = <<<DML
            SELECT 
                qprordem + 1
            FROM acomporc.questionarioperguntas 
            WHERE qstid = {$this->arAtributos['qstid']} 
            ORDER BY qprordem DESC 
            LIMIT 1
DML;
        $retorno = $this->pegaUm($query);
        if(!$retorno){
            return 1;
        }
        return $retorno;
    }
}