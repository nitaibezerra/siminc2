<?php

class Model_vwAvaliacaoSimec extends Abstract_Model
{

    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'carga';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'vw_avaliacao_simec';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     *
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['DISCENTE'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '');
        $this->entity['CPF_DISCENTE'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '');
        $this->entity['BOLSISTA'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['INICIO_ATIVIDADE'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['FIM_ATIVIDADE'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['CODIGO_IES'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['NOME_IES'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['CODIGO_GRUPOPET'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['NOME_GRUPOPET'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['ABRANGENCIA'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['TUTOR'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['CPF_TUTOR'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '');
        $this->entity['INICIO TUTORIA'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '');
        $this->entity['CURSO'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
    }

    public function getListaGrupos(){
        try{
            $sql = '
                 SELECT "NOME_GRUPOPET", "CODIGO_GRUPOPET", "ABRANGENCIA"  FROM carga.vw_avaliacao_simec
        ';
//        ver($sql, d); "NOME_GRUPOPET",  DISTINCT ("CODIGO_GRUPOPET")
            $dados = $this->_db->carregar($sql);
        }catch (Exception $e){
            ver($e, d);
        }

        $dados = $dados ? $dados : array();
//ver($dados, d);
        $listagem = new Simec_Listagem();
        $listagem->setTamanhoPagina(10);
//        $listagem->addCallbackDeCampo(array('usunome', 'pfldsc'), 'alinhaParaEsquerda');
//        $listagem->esconderColunas(array('docid', 'dmdprazo', 'acjid'));
        $listagem->setCabecalho(array(
            'Grupo',
            'Abrangência',
        ));
        $listagem->setDados($dados);
        $listagem->render();
    }
}
