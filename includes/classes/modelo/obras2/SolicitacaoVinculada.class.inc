<?php

class SolicitacaoVinculada extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_vinculada";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("slvid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'docid' => null,
        'slvid' => null,
        'obrid' => null,
        'supid' => null,
        'usucpf' => null,
        'slvpercultvistoria' => null,
        'slvpercnovocontrato' => null,
        'slvjustificativa' => null,
        'slvobs' => null,
        'arqid' => null,
        'slvstatus' => null,
        'slvdatainclusao' => null,
        'slvnovocontrato' => null
    );

    public function verificaRestricaoObraVinculadaExistente($obrid)
    {
        $sql = "SELECT
                    COUNT(*)
                FROM obras2.restricao
                WHERE
                rstdsc = 'Solicitação de criação de obra vinculada.'
                AND obrid = $obrid
                AND tprid = 12
                AND fsrid = 5
                AND rstitem = 'R'
                AND rststatus = 'A'";
        $c = $this->pegaUm($sql);
        return ($c > 0) ? true : false;
    }

    public function criarRestricaoObraVinculada($obrid)
    {
        if($this->verificaRestricaoObraVinculadaExistente($obrid))
            return;

        $questao = new Questao();
        $questionario = $questao->pegaTodaEstruturaSolicitacaoVinculada("NULL");

        $providencia = "Deve ser solicitada a criação da obra vinculada no link \"Solicitar Obra Vinculada\" presente na barra de ferramentas da obra.\nOs seguintes itens devem ser atendidos para realizar a solicitação de obra vinculada:";
        foreach($questionario as $key => $questao){
            $validador = new Validador();
            if ($validador->check($obrid, $questao['vdrid'])) {
                $providencia .=  "\n - " . $questao['qstobs'];
            }
        }
        $date = new DateTime(date('Y-m-d'));
        $date->modify('+30 day');
        $restricao = new Restricao();
        $arDado = array(
            'tprid' => 12,
            'fsrid' => 5,
            'rstitem' => 'R',
            'empid' => NULL,
            'obrid' => $obrid,
            'usucpf' => '',
            'rstdsc' => 'Solicitação de criação de obra vinculada.',
            'rstdtprevisaoregularizacao' => $date->format('Y-m-d'),
            'rstdscprovidencia' => $providencia,
        );
        $restricao->popularDadosObjeto($arDado)
            ->salvar(true, true, array());
        $restricao->atualizaDocidNullRetricao($restricao->rstid);
        $restricao->clearDados();
        $restricao->commit();

    }
}