<?php

class ValidadorQuestao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.validador_questao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vdqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(

        'vdqid' => 'null',
        'qstid' => 'null',
        'vdrid' => 'null',
        'vdqstatus' => 'null',
        'vdqobrigatorio' => 'null',
        'vdqdatainclusao' => 'null',
    );

    public function carregarPorQstid($qstid)
    {
        $linha = $this->pegaLinha("SELECT * FROM obras2.validador_questao WHERE qstid = $qstid AND vdqstatus = 'A'");
        if($linha)
            $this->popularDadosObjeto($linha);
    }

    public function check($qstid, $obrid)
    {
        $this->carregarPorQstid($qstid);

        if($this->vdqid == 'null')
            return null;

        $validador = new Validador();
        return $validador->check($obrid, $this->vdrid);
    }
}