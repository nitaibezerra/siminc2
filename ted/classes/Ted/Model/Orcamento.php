<?php

class Ted_Model_Orcamento extends modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.orcamentario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('proid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'proid' => NULL,
        'tcpid' => NULL,
        'ptrid' => NULL,
        'pliid' => NULL,
        'provalor' => NULL,
        'ndpid' => NULL,
        'proanoreferencia' => NULL,
        'prostatus' => NULL
    );

    protected $arAtributosObrigatorios = array(
        'tcpid',
    );

    public function getByTcpid($tcpid = null, $prostatus = 'A')
    {
        if (!$tcpid) {
            return false;
        }

        $strSQL = <<<DML
SELECT orc.proid,
       orc.proanoreferencia AS ptrano,
       orc.tcpid,
       orc.ptrid,
       ptr.ptres,
       ptr.funcod,
       ptr.sfucod,
       ptr.prgcod,
       ptr.unicod,
       ptr.loccod,
       aca.acacod,
       aca.acadsc,
       orc.pliid,
       pli.plicod,
       pli.plidsc,
       orc.ndpid,
       ndp.ndpid,
       ndp.ndpcod,
       ndp.ndpdsc,
       orc.provalor,
       orc.prostatus
  FROM ted.orcamentario orc
    INNER JOIN monitora.ptres ptr USING(ptrid)
    INNER JOIN monitora.acao aca USING(acaid)
    INNER JOIN monitora.pi_planointerno pli USING(pliid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
  WHERE tcpid = {$tcpid}
    AND prostatus = '{$prostatus}'
DML;

        $rs = $this->carregar($strSQL);
        return ($rs) ? $rs : null;
    }

    /**
     * Retorna dados para um input[select] de meses de execução
     * @return string
     */
    public function getIntervaloMeses()
    {
        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        $html.= '<option value="1" label="1 Mês">1 Mês</option>';
        foreach (range(2, 50) as $month) {
            $html.= "<option value=\"{$month}\" label=\"{$month} Meses\">{$month} Meses</option>";
        }
        return $html;
    }

    /**
     * Organiza o array com os dados para o metodo populate da classe model
     * @param array $post
     * @return array|bool
     */
    public function prepareData(array $post)
    {
        if (!count($post)) return false;

        $arrayData = $arrTemp = array();
        $keyMaps = array_keys($post);

        foreach ($post['proid'] as $k => $v) {
            foreach ($keyMaps as $postKey) {
                $arrTemp[$postKey] = $post[$postKey][$k];
            }
            $arrayData[] = $arrTemp;
            $arrTemp = array();
        }

        return $arrayData;
    }

    public function antesSalvar()
    {
        $this->arAtributos['provalor'] = str_replace(
            array('.', ','),
            array('', '.'),
            $this->arAtributos['provalor']
        );

        return parent::antesSalvar();
    }

    /**
     * @param $dados
     * @return bool
     */
    public function salvarDados($dados)
    {
        $this->popularDadosObjeto($dados);
        $this->arAtributos['provalor'] = str_replace(',', '.', str_replace('.', '', $this->arAtributos['provalor']));
        if (!empty($this->arAtributos['proid'])) {
            return $this->atualizar();
        } else {
            $this->arAtributos['proid'] = $this->pegaUm("select max(proid)+1 from {$this->stNomeTabela}");
            return $this->cadastrar();
        }

        return false;
    }

    /**
     */
    private function cadastrar()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->arAtributos['proid'] = $this->inserir();
            return $this->commit();
        }

        return false;
    }

    /**
     */
    private function atualizar()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->alterar();
            return $this->commit();
        }
        return false;
    }

    /**
     * Valida campos obrigatorios no objeto populado
     */
    private function validaCamposObrigatorios()
    {
        foreach ($this->arAtributosObrigatorios as $valor)
            if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
                return false;

        return true;
    }

    /**
     * @param $proid
     * @return bool
     */
    public function delete($proid)
    {
        $strSQL = "
            update {$this->stNomeTabela} set prostatus = 'I' where proid = {$proid}
        ";

        $this->executar($strSQL);
        return ($this->commit()) ? true : false;
    }
}