<?php

class QuestaoSolicitacaoDesembolso extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questaosolicitacaodesembolso";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qsdid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qsdid' => null,
        'sldid' => null,
        'qstid' => null,
        'qsdresposta' => null,
        'qsdobs' => null,
        'qsdstatus' => null,
    );


    public function carregaPorQstideSldid($qstid, $sldid)
    {
        $dados = $this->pegaLinha("SELECT * FROM {$this->stNomeTabela} WHERE qstid = $qstid AND sldid = $sldid");
        if(!empty($dados)) {
            $this->popularDadosObjeto($dados);
            return true;
        }
        return false;
    }
}