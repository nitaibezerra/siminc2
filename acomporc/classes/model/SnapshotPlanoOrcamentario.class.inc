<?php

class Acomporc_Model_SnapshotPlanoOrcamentario extends Modelo {

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
    protected $arChavePrimaria = array( "sspid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sspid' => null,
        'sslid' => null, //Referencia para o localizador ao qual este po pertecente
        'ploid' => null, //Referencia para monitora.planoorcamentario
        'acaid' => null, //Referencia para monitora.acao
        'prfid' => null, //Referencia para o periodo de referencia de acompanhamento do localizador
        'plocod' => null, //Codigo do plano orcamentario
        'dotacaoatual' => null, //Valor da dotacao atual do plano orcamentario
        'liquidado' => null, //Valor liquidado da dotacao do plano orcamentario
        'empenhado' => null, //Valor empenhado da dotacao do plano orcamentario
        'pago' => null, //Valor pago da dotacao do plano orcamentario
        'prddescricao' => null, //Descricao do produto do plano orcamentario
        'unmdescricao' => null, //Descricao da unidade de medida do plano orcamentario
        'metafisica' => null,  //Meta fisica do plano orcamentario
        'dataultimatualizacao' => null, //Momento em que ocorreu a ultima atualizacao nesta linha
        'aclid' => null //id do acompanhamento localizador. Não existe na tabela.
    );

    public function listaPlanoOrcamentario()
    {
        $leftJoin = "";
        $columns = array(
            'sp.sspid',
            'sp.plocod',
            'sp.plodsc',
            'sp.prddescricao',
            'sp.unmdescricao',
            'sp.dotacaoatual',
            'sp.empenhado',
            'sp.liquidado',
            'sp.pago',
            'sp.metafisica'
            );
        if(!is_null($this->arAtributos['aclid'])){
            $leftJoin = "LEFT JOIN acomporc.acompanhamentoplanoorcamentario apo ON (sp.sspid = apo.sspid AND sp.prfid = apo.prfid AND apo.aclid = {$this->arAtributos['aclid']})";
            $columns[] = 'apo.metafisicarealizada';
            $columns[] = 'apo.acpid';
        }
        $columns = implode(',',$columns);
        $query = <<<DML
            SELECT
                {$columns}
            FROM acomporc.snapshotplanoorcamentario sp
            JOIN acomporc.snapshotlocalizador sl ON (sp.acaid = sl.acaid)
            {$leftJoin}
            WHERE sl.sslid = {$this->arAtributos['sslid']}
                AND sp.prfid = {$this->arAtributos['prfid']}
DML;
        return $this->carregar($query);
    }
}