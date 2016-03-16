<?php
class Acomporc_Model_QuestionarioLocalizadorRespostas extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionariolocalizadorrespostas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qlrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qlrid' => null,
        'aclid' => null,
        'prfid' => null,
        'qprid' => null,
        'acacod' => null,
        'unicod' => null,
        'loccod' => null,
        'qlrresposta' => null
    );



    /**
     * Popular qprid, asaid antes de chamar função.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaResposta()
    {
        $query = <<<DML
            SELECT
                qsrid,
                qsrresposta
            FROM acomporc.questionariosubacaorespostas
            WHERE qprid = {$this->arAtributos['qprid']}
                AND asaid = {$this->arAtributos['asaid']}
DML;
        return $this->pegaLinha($query);
    }

    public function salvarQuestao($camposResposta)
    {
        if(!$camposResposta || !is_array($camposResposta)){
            return false;
        }
        foreach($camposResposta as $resposta){
            $this->popularDadosObjeto($resposta);
            if($resposta['qlrid'] == null || trim($resposta['qlrid']) == ''){
                $this->inserir();
            }else{
                $this->alterar();
            }
        }

        return $this->commit();
    }

    public function pegaRespostasQuetionario(array $criterio)
    {
        $sql = <<<DML
SELECT loccod,
       qprpergunta,
       qlrresposta
  FROM acomporc.questionariolocalizadorrespostas qsr
    INNER JOIN acomporc.questionarioperguntas qsp USING(qprid)
  WHERE unicod = :unicod
    AND acacod = :acacod
    AND prfid = :prfid
  ORDER BY loccod,
           qprordem
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParam('unicod', $criterio['unicod'], true)
            ->addParam('acacod', $criterio['acacod'], true)
            ->addParam('prfid', $criterio['prfid']);

        return $this->carregar($dml);
    }
}
