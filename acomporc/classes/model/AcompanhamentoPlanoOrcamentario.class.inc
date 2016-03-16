<?php

class Acomporc_Model_AcompanhamentoPlanoOrcamentario extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.acompanhamentoplanoorcamentario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "acpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acpid' => null,
        'sspid' => null,
        'aclid' => null,
        'prfid' => null,
        'metafisicarealizada' => null,
        'dataultimatualizacao' => null
    );

    public function gravarAcompanhamento(array $camposAcompanhamentoPO)
    {
        foreach($camposAcompanhamentoPO as $campos){
            $this->popularDadosObjeto($campos);
            if($this->arAtributos['acpid'] == ''){
                $this->inserir();
                continue;
            }
            $this->alterar();

        }
        return $this->commit();
    }

    public function pegaAcompanhamentoPlanosOrcamentarios(array $criterio)
    {
        $sql = <<<DML
SELECT acp.metafisicarealizada,
       spo.plocod,
       ssl.loccod
  FROM {$this->stNomeTabela} acp
    INNER JOIN acomporc.snapshotplanoorcamentario spo USING(sspid)
    INNER JOIN acomporc.snapshotlocalizador ssl ON spo.prfid = ssl.prfid AND spo.acaid = ssl.acaid
  WHERE acp.aclid = :aclid
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParam('aclid', $criterio['aclid']);

        return $this->carregar($dml);
    }
}