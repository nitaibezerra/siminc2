<?php

class QuestaoSolicitacaoVinculada extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questaosolicitacaovinculada";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qsvid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qsvid' => null,
        'slvid' => null,
        'qstid' => null,
        'qsvresposta' => null,
        'qsvobs' => null,
        'qsvstatus' => null,
    );


    public function carregaPorQstideSlvid($qstid, $slvid)
    {
        $dados = $this->pegaLinha("SELECT * FROM {$this->stNomeTabela} WHERE qstid = $qstid AND slvid = $slvid");
        if(!empty($dados)) {
            $this->popularDadosObjeto($dados);
            return true;
        }
        return false;
    }
}