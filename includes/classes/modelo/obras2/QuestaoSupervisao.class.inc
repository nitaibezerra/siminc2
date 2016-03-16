<?php

class QuestaoSupervisao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questaosupervisao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qtsid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'qtsid' => null,
                                    'supid' => null,
                                    'qstid' => null,
                                    'emsid' => null,
                                    'sueid' => null,
                                    'smiid' => null,
                                    'sfndeid' => null,
                                    'qtsresposta' => null,
                                    'qtsobs' => null,
                                    'qtsstatus' => null,
                                   );

    public function apagaRespostaPorQuestao($qstid) {
        $sql = "UPDATE obras2.questaosupervisao SET qtsstatus = 'I' WHERE qstid = {$qstid}";

        $this->executar($sql);
    }

    public function pegaRespostaPorTipoSueid($qstid, $sueid) {
        if (empty($qstid) || empty($sueid)) {
            return false;
        }

        $sql = "SELECT * FROM obras2.questaosupervisao WHERE qtsstatus = 'A' AND sueid = {$sueid} AND qstid = {$qstid}";

        return $this->pegaLinha($sql);
    }

    public function pegaOcorrenciaRespostaContundentePorSueid($sueid) {

        $sql = "SELECT
                        COUNT(qs.qtsid)
                FROM
                        obras2.questaosupervisao qs
                JOIN obras2.questao q ON q.qstid = qs.qstid AND
                                         q.qststatus = 'A' AND
                                         q.qstcontundente = true
                WHERE
                        qs.qtsresposta = 'f' AND
                        qs.sueid = {$sueid};";

        return $this->pegaUm($sql);
    }

}
