<?php

class Acomporc_Model_AcompanhamentoSubacaoPtres extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.acompanhamentosubacaoptres";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aspid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aspid' => null,
        'prfid' => null,
        'sbacod' => null,
        'metafisicareprogramada' => null,
        'analiseexecucao' => null,
        'usucpf' => null,
        'dataultimaatualizacao' => null,
        'ptres' => null,
        'asaid' => null
    );

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * Captura listagem de ações para a subação.
     * Obs.: Popular asaid com __set antes de chamar esta função.
     * @return array
     */
    public function listar()
    {
        $lista = $this->carregar($this->_pegaQuery());
        if(!$lista){
            return array();
        }
        return $this->_formataLista($lista);
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $lista
     * @return array Retorna o array formatado para inserção no Simec_Listagem
     */
    protected function _formataLista($lista = array())
    {
        $retorno = array();
        foreach($lista as $dado){
            $aspid = explode(',',$dado['aspid']);
            $ptres = explode(',',$dado['ptres']);
            $plocod = explode('|@|@|',$dado['plocod']);
            $plodsc = explode('|@|@|',$dado['plodsc']);
            $prodsc = explode(',',$dado['prodsc']);
            $metafisica = explode(',',$dado['metafisica']);
            $analiseexecucao = explode('|@|@|',$dado['analiseexecucao']);
            $metafisicareprogramada = explode(',',$dado['metafisicareprogramada']);
            $vlrdotacao = explode(',',$dado['vlrdotacao']);
            $vlrempenhado = explode(',',$dado['vlrempenhado']);
            $vlrliquidado = explode(',',$dado['vlrliquidado']);
            $vlrpago = explode(',',$dado['vlrpago']);

            $contador = count($aspid);
            $dadosPtres = array();
            for($i = 0; $i < $contador; $i++){
                $dadosPtres[] = array(
                    'aspid' => preg_replace('/[{}"]/','',$aspid[$i]),
                    'ptres' => preg_replace('/[{}"]/','',$ptres[$i]),
                    'plocod' => $plocod[$i] == '' ? 'N/A' : $plocod[$i] .' - '. $plodsc[$i],
                    'prodsc' => preg_replace('/[{}"]/','',$prodsc[$i]),
                    'metafisica' => preg_replace('/[{}"]/','',$metafisica[$i]),
                    'analiseexecucao' => $analiseexecucao[$i],
                    'metafisicareprogramada' => preg_replace('/[{}"]/','',$metafisicareprogramada[$i]),
                    'vlrdotacao' => preg_replace('/[{}"]/','',$vlrdotacao[$i]),
                    'vlrempenhado' => preg_replace('/[{}"]/','',$vlrempenhado[$i]),
                    'vlrliquidado' => preg_replace('/[{}"]/','',$vlrliquidado[$i]),
                    'vlrpago' => preg_replace('/[{}"]/','',$vlrpago[$i])
                    );
            }

            $retorno[] = array(
                'acacod' => $dado['acacod'],
                'acadsc' => $dado['acadsc'],
                'datareferencia' => $dado['datareferencia'],
                'dadosPtres' => $dadosPtres
                );
        }
        return $retorno;
    }

    /**
     * Query da listagem de ações/ptres
     * Obs.: Popular 'asaid' antes de chamar função.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return string
     */
    protected function _pegaQuery()
    {
        $query = <<<DML
            SELECT
		ma.acacod,
		ma.acadsc,
                to_char(max(ss.dataultimaatualizacao),'dd/mm/yyyy') as datareferencia,
		array_agg(sap.aspid) as aspid,
		array_agg(ss.ptres) as ptres,
		array_to_string(array_agg(COALESCE(ss.plocod,'')),'|@|@|') as plocod,
                array_to_string(array_agg(COALESCE(ss.plodsc,'')),'|@|@|') as plodsc,
                array_agg(coalesce(ss.prddescricao,'N/A')) as prodsc,
                array_agg(coalesce(ss.metafisica::varchar,'N/A')) as metafisica,
                array_to_string(array_agg(COALESCE(sap.analiseexecucao,'')),'|@|@|') as analiseexecucao,
                array_agg(COALESCE(sap.metafisicareprogramada,0)) as metafisicareprogramada,
                array_agg(ss.vlrdotacao) as vlrdotacao,
                array_agg(ss.vlrempenhado) as vlrempenhado,
                array_agg(ss.vlrliquidado) as vlrliquidado,
                array_agg(ss.vlrpago) as vlrpago
            FROM acomporc.snapshotsubacao ss
            LEFT JOIN {$this->stNomeTabela} sap ON sap.sbacod = ss.sbacod AND sap.prfid = ss.prfid AND ss.ptres = sap.ptres
            JOIN monitora.ptres mp ON ss.ptres = mp.ptres and mp.ptrano = '{$_SESSION['exercicio']}'
            JOIN monitora.acao ma ON mp.acaid = ma.acaid
            WHERE ss.sbacod = '{$this->arAtributos['sbacod']}' AND ss.prfid = {$this->arAtributos['prfid']}
            GROUP BY 1, 2
DML;
        return $query;
    }

    public function gravarAcompanhamento($dados)
    {
        if(!$dados || !is_array($dados)){
            return false;
        }
        foreach($dados['acompanhamento'] as $acomp){
            if($acomp['aspid'] == null || trim($acomp['aspid']) == ''){
                $this->_novoAcompanhamento(array('prfid' => $dados['prfid'],'sbacod' => $dados['sbacod'],'asaid' => $dados['asaid'],'ptres' => $acomp['ptres'],'metafisicareprogramada' => $acomp['metafisicareprogramada'],'analiseexecucao' => $acomp['analiseexecucao']));
            }else{
                $this->_atualizaAcompanhamento(array('asaid' => $dados['asaid'],'metafisicareprogramada' => $acomp['metafisicareprogramada'],'analiseexecucao' => $acomp['analiseexecucao'],'aspid' => $acomp['aspid']));
            }
        }

        return $this->commit();
    }

    protected function _novoAcompanhamento($dados)
    {
        $queryInsert = <<<DML
            INSERT INTO acomporc.acompanhamentosubacaoptres
            (prfid, sbacod, asaid, usucpf, ptres, metafisicareprogramada, analiseexecucao, asaid)
            VALUES ('{$dados['prfid']}', '{$dados['sbacod']}', '{$dados['asaid']}', '{$_SESSION['usucpf']}','{$dados['ptres']}', '{$dados['metafisicareprogramada']}', '{$dados['analiseexecucao']}', '{$dados['asaid']}')
            returning ;
DML;
        $this->executar($queryInsert);
    }

    protected function _atualizaAcompanhamento($dados)
    {
        $queryUpdate = <<<DML
            UPDATE acomporc.acompanhamentosubacaoptres
            SET asaid = '{$dados['asaid']}',
                usucpf = '{$_SESSION['usucpf']}',
                metafisicareprogramada = '{$dados['metafisicareprogramada']}',
                analiseexecucao = '{$dados['analiseexecucao']}'
            WHERE aspid = '{$dados['aspid']}';
DML;
        $this->executar($queryUpdate);
    }
}