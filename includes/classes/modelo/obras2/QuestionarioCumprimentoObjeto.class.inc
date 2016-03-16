<?php
class QuestionarioCumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questao_cumprimento_obj_documentacao";

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
        'qcodid' => null,
        'cumid' => null,
        'qstid' => null,
        'qcodresposta' => null,
        'qcodobs' => null,
        'qcodstatus' => null
    );

    public $codid;
    public $obrid;
    public $questoes;
    public $escopoQDCO = true;
    public $descricaoItem;
    public $escopo = 'QDCO';
    public $itcid;
    private $obra;

    public function montaQuestionario($obra,$escopo = true)
    {
        $this->escopoQDCO = $escopo;
        $this->codid = $obra['codid'];
        $this->obrid = $obra['obrid'];
        $this->escopo = $this->escopoQDCO ? 'QDCO' : 'QDCOC';
        $this->obra = $obra;
        
        $questao = new Questao();
        $questionario = $questao->pegaTodaEstruturaCumprimentoObjeto($this->codid, $this->escopo);
        $this->populaQuestionario($questionario);
        $this->populaDescricao($questionario);
        $this->populaITCID($questionario);
        return $this;
    }

    public function respostas($obra,$escopo = true)
    {
        $this->escopoQDCO = $escopo;
        $this->escopo = $this->escopoQDCO ? 'QDCO' : 'QDCOC';
        $this->obrid = $obra['obrid'];

        $questao = new Questao();
        $questionario = $questao->pegaTodaEstruturaCumprimentoObjeto($obra['codid'], $this->escopo);
        $respostas = $this->populaRespostas($questionario);
        return $respostas;
    }

    public function populaRespostas($questionario)
    {
        $fracasso = 0;
        foreach($questionario as $questao) {
            $questaoCumprimento = new QuestaoCumprimentoObjeto();
            $saida = $questaoCumprimento->pegaResposta($this->obrid, $questao);
            if($saida['numero'] == 6) {
                if($saida['resposta'] == 't') {
                    $fracasso++;
                }
                continue;
            }
            if($saida['resposta'] == 'f') {
                $fracasso++;
            }
        }
        return $fracasso;
    }

    public function populaQuestionario($questionario)
    {
        $this->questoes = array();
        foreach($questionario as $questao) {
            $questaoCumprimento = new QuestaoCumprimentoObjeto();
            $questaoCumprimento->trataQuestao($this->obrid, $questao);
            $this->questoes[] = $questaoCumprimento;
        }
    }

    public function populaDescricao($questionario)
    {
        $descricao = $questionario[0]['itcdsc'];
        if(!$this->escopoQDCO) {
            $descricao .= ' ('.mascaraglobal($this->obra['entnumcpfcnpj'],"##.###.###/####-##").') '.$this->obra['entnome'];
        }
        $this->descricaoItem = $descricao;
    }

    public function populaITCID($questionario)
    {
        $this->itcid = $questionario[0]['itcid'];
    }

    public function verificaExistencia()
    {
        $query = <<<DML
            SELECT qcodid FROM {$this->stNomeTabela} WHERE cumid = {$this->arAtributos['cumid']} AND qstid = {$this->arAtributos['qstid']} AND qcodstatus = 'A';
DML;
        return $this->pegaUm($query);
    }

    public function inativaResposta($qcodid)
    {
        $query = <<<DML
            UPDATE {$this->stNomeTabela} SET qcodresposta = 'f' WHERE qcodid = $qcodid;
DML;
        $this->executar($query);
        $this->commit();
    }

}