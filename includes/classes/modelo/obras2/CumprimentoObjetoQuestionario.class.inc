<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of CumprimentoObjetoQuestionario
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoQuestionario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_questionario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("coqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'coqid' => null,
        'coid' => null,
        'qrpid' => null,
        'coqstatus' => null,
        'usucpf' => null,
        'coqdatainclusao' => null
    );

    public $usunome;

    public function verificaExistencia($coid, $cria = false)
    {
        $sql = <<<DML
            SELECT coq.coqid, coq.coid, coq.qrpid, usu.usucpf, usu.usunome, to_char(coq.coqdatainclusao,'DD/MM/YYYY') AS coqdatainclusao
            FROM {$this->stNomeTabela} coq
            INNER JOIN seguranca.usuario usu ON coq.usucpf = usu.usucpf
            WHERE coid = $coid;
DML;
        $dados = $this->pegaLinha($sql);
        if(!$dados && $cria) {
            $arParam = array("queid" => QUEID_QUEST_CHKLST_CUMPRIMENTO, "titulo" => "Checklist do Cumprimento (" . $coid . " - Checklist do Cumprimento)");
            $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
            $this->popularDadosObjeto(array('coid' => $coid, 'qrpid' => $qrpid, 'usucpf' => "'".$_SESSION['usucpf']."'"));
            $this->salvar();
            $this->commit();
            $this->verificaExistencia($coid);
            return;
        }
        $this->usunome = $dados['usunome'];
        $this->popularDadosObjeto($dados);
    }

    public function verificaRespostaItem5()
    {
        $queid = QUEID_QUEST_CHKLST_CUMPRIMENTO;
        $sql = <<<DML
            SELECT itpid FROM questionario.resposta WHERE qrpid = {$this->qrpid}
                AND perid = (SELECT perid FROM questionario.pergunta WHERE queid = $queid AND perordem = 5 LIMIT 1)
DML;
        return $this->pegaUm($sql);
    }

    public function verificaPendencias()
    {
        if($this->qrpid) {
            $queid = QUEID_QUEST_CHKLST_CUMPRIMENTO;
            $sql = <<<DML
            SELECT
                (select CASE WHEN count((select perid from questionario.resposta where perid = p.perid and qrpid = {$this->qrpid})) = count(1) then 'S' else 'N' end from questionario.pergunta p where p.queid = $queid ) as respostas_zeradas,
                CASE (SELECT itpid FROM questionario.resposta WHERE qrpid = {$this->qrpid} AND perid = (SELECT perid FROM questionario.pergunta WHERE queid = $queid AND perordem = 5 LIMIT 1)) WHEN 5577 THEN 'S' ELSE 'N' END AS resposta_fotos
DML;
        return $this->pegaLinha($sql);
        }
    }
}