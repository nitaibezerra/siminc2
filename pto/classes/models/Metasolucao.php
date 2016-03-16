<?php

class Model_Metasolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'metasolucao';
    public $entity = array();
    const CORPO_LEI_ID = 99999999;

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['metid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => '');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => '');
        $this->entity['mpneid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => '');
    }

    public function getOptionsMeta($where = null, $dados = array())
    {
        if (!empty($dados) && is_null($where) && !empty($dados['temid'])) {
            $strTemid = implode(',', $dados['temid']);
            $where = " OR temid IN ( {$strTemid} ) ";
        }
        $metas = $this->getMetas($where);
        return $this->getOptions($metas, array('prompt' => 'nenhuma', 'prompt_value' => 'nenhuma'), 'mpneid');
    }

    public function getMetas($where = null)
    {
        if (empty($where)) {
            $where = 'OR 1=1';
        }
        $corpoLei = Model_Metasolucao::CORPO_LEI_ID;
        $sql = "SELECT mpneid as codigo, mpnenome as descricao FROM pde.ae_metapne WHERE mpneid = {$corpoLei} {$where} ORDER BY mpneordem ";
        return $this->_db->carregar($sql);
    }

    public function salvarMeta($arrayMetas, $idSolucao)
    {
        foreach ($arrayMetas as $metaID) {
            $this->setAttributeValue('solid', $idSolucao);
            $this->setAttributeValue('mpneid', $metaID);
            $id = $this->save();
            if ($id == false) {
                throw new Exception('Erro ao inserir Metas PNE.');
            }
        }
    }

    public function getMetaPainel($arrayIds)
    {
        if (is_array($arrayIds) and count($arrayIds) > 0) {
            $ids = implode(',', $arrayIds);
            $sql = "SELECT mpneid, mpnenome
                        FROM pde.ae_metapne
                        WHERE mpneid in ({$ids})
                        ORDER BY mpneid";
            $dados = $this->_db->carregar($sql);

            $arrayDescricao = array();
            if ($dados) {
                foreach ($dados as $valor) {
                    $nome = explode(':', $valor['mpnenome']);
                    $arrayDescricao[] = $nome[0];
                }
            }
            return implode(', ', $arrayDescricao);
        } else {
            return false;
        }
    }

    public function getMetaIndicadorPainel($arrayIds, $solid, $estrategia = false)
    {
        $joinEstrategia = '';
        $joinIndicador = '';
        if ($estrategia) {
            $joinEstrategia = '	INNER JOIN pto.metasolucao metasol ON metasol.mpneid = metapne.mpneid
            					INNER JOIN pde.ae_estrategia ae_estrategia ON ae_estrategia.metid = metapne.mpneid';
        } else {
            $joinIndicador = '
                            INNER JOIN pto.metasolucao metasol ON metasol.mpneid = metapne.mpneid
                            LEFT JOIN pde.ae_metapnexindicador metapnexindicador ON metapnexindicador.mpneid = metapne.mpneid
                            LEFT JOIN painel.indicador indicador ON indicador.indid = metapnexindicador.indid
                            LEFT JOIN painel.periodicidade per ON per.perid = indicador.perid
                        ';
        }

        if (is_array($arrayIds) and count($arrayIds) > 0) {
            $ids = implode(',', $arrayIds);
            $sql = "SELECT *
                        FROM pde.ae_metapne metapne
                        {$joinIndicador}
                        {$joinEstrategia}
                        WHERE 	metapne.mpneid in ({$ids})
                        		AND  metasol.solid = {$solid}
                        ORDER BY metapne.mpneordem";
//ver($sql);
            $dados = $this->_db->carregar($sql);

            if ($estrategia) {
                foreach ($dados as $valor) {
                    $arrayEstrategiasMeta[$valor['mpnenome']][] = $valor['estnome'];
                }
                return $arrayEstrategiasMeta;
            } else {
                foreach ($dados as $valor) {
                    $arrayIndicadoresMeta[$valor['mpnenome']][] = $valor;
                }
                return $arrayIndicadoresMeta;
            }
        } else {
            return false;
        }
    }

    public function metasInvalidas($arrayMetas)
    {
        $flag = is_array($arrayMetas) && (array_search('nenhuma', $arrayMetas) !== false || array_search('99999999', $arrayMetas) !== false);
        return $flag;
    }

    public function validaCorpoLei($arrayMetas)
    {
        $posIdCorpoLei = false;
        if (is_array($arrayMetas)) {
            $posIdCorpoLei = array_search(Model_Metasolucao::CORPO_LEI_ID, $arrayMetas);
        }
        return $posIdCorpoLei;
    }
}
