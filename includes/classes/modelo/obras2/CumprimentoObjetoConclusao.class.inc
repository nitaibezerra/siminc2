<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of CumprimentoObjetoConclusao
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoConclusao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_conclusao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cocid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cocid' => null,
        'coid' => null,
        'valor_devolucao' => null,
        'aprovacao' => null,
    );

    public function salvarConclusao($dados)
    {
        $dados['valordevolucao'] = str_replace(',','.',str_replace('.','',$dados['valordevolucao']));
        
        $file = new FilesSimec('arquivo', null, 'obras2');
        $file->setPasta('obras2');
        if($dados['cocid']) {
            $sql = <<<DML
                UPDATE {$this->stNomeTabela} SET valor_devolucao = {$dados['valordevolucao']}, aprovacao = '{$dados['aprovacao']}' WHERE cocid = {$dados['cocid']}
DML;
            $this->executar($sql);
            $this->commit();
        } else {
            $sql = <<<DML
                INSERT INTO {$this->stNomeTabela} (coid, aprovacao, valor_devolucao) VALUES ({$dados['coid']}, '{$dados['aprovacao']}', {$dados['valordevolucao']});
DML;
            $this->executar($sql);
            $this->commit();
        }
    }

    public function capturaDados($coid)
    {
        if(!$coid) {
            return false;
        }
        $sql = <<<DML
            SELECT
                coc.cocid,
                coc.valor_devolucao,
                coc.aprovacao
            FROM {$this->stNomeTabela} coc
            WHERE coc.coid = $coid;
DML;
        $dados = $this->pegaLinha($sql);
        $this->popularDadosObjeto($dados);
        return $dados;
    }

    public function excluir($arqid,$colunaArqid)
    {
        if(!$arqid || !$colunaArqid){
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $sql = <<<DML
            UPDATE {$this->stNomeTabela} SET $colunaArqid = NULL WHERE $colunaArqid = $arqid;
DML;
        $this->executar($sql);
        $this->commit();
        $file = new FilesSimec('cumprimento_objeto_diligencia',$this->arAtributos,'obras2');
        $file->setPulaTableEschema(true);
        $file->setRemoveUpload($arqid);
    }

    public function tabelaParcelas($preid)
    {
        if(!$preid) {
            return false;
        }
        $sql = <<<DML
            SELECT
                p.pagparcela,
                pagvalorparcela,
                pagdatapagamento
            FROM par.pagamento p
            JOIN (
                SELECT preid, pagid, pobvalorpagamento as valor
                FROM par.pagamentoobra
                UNION
                SELECT preid, pagid, popvalorpagamento as valor
                FROM par.pagamentoobrapar) po ON po.pagid = p.pagid
            WHERE preid = $preid
                AND p.pagstatus = 'A'
                AND p.pagsituacaopagamento = '2
DML;
        return $this->carregar($sql);
    }

    public function verificaExistencia($coid)
    {
        $sql = <<<DML
            SELECT coid, cocid, valor_devolucao, aprovacao FROM {$this->stNomeTabela} WHERE coid = $coid;
DML;
        $dados = $this->pegaLinha($sql);
        if($dados) {
            $this->popularDadosObjeto($dados);
            return true;
        }
    }
}