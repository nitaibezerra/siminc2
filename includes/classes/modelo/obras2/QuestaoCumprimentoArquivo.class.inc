<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class QuestaoCumprimentoArquivo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questao_cumprimento_arquivo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qcodid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qcodid' => NULL,
        'arqid' => NULL,
        'status' => NULL
    );

    public function novo($qstid, $qcodid)
    {
        $campos	= array(
            "qcodid"  => $qcodid,
            "status" => "'A'"
        );
        $file = new FilesSimec("questao_cumprimento_arquivo", $campos ,"obras2");
        return $file->setUpload(NULL, $qstid);
    }

    public function download($arqid)
    {
        if(!$arqid){
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $file = new FilesSimec();
        $file->getDownloadArquivo($arqid);
    }

    public function excluir($arqid)
    {
        if(!$arqid){
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        
        $file = new FilesSimec('questao_cumprimento_arquivo',NULL,'obras2');
        $file->excluiArquivoFisico($arqid);
        $query = <<<HTML
            UPDATE {$this->stNomeTabela} SET status = 'I' WHERE arqid = $arqid
HTML;
        $this->executar($query);
        $this->commit();
        //Não há mais necessidade de alterar o valor da resposta uma vez que a resposta valida será sempre a do validador.
        //$this->verificaQuestao($arqid);
    }

    private function verificaQuestao($arqid)
    {
        $query = <<<HTML
            SELECT
                a.qcodid AS qcodid,
                (SELECT COUNT(qcodid) FROM {$this->stNomeTabela} WHERE qcodid = a.qcodid AND status = 'A') AS total
            FROM (SELECT qcodid FROM {$this->stNomeTabela} WHERE arqid = $arqid) a;
HTML;
        $resultado = $this->pegaLinha($query);
        if($resultado['total'] == '0') {
            $questionarioCumprimentoObjeto = new QuestionarioCumprimentoObjeto();
            $questionarioCumprimentoObjeto->inativaResposta($resultado['qcodid']);
        }
    }
}