<?php
/**
 * Description of CumprimentoObjetoProcesso
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoProcesso extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_processo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("copid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'copid' => null,
        'numprocesso' => null,
        'arqid_pf' => null,
        'arqid_pf_revisado' => null,
        'valor_devolucao' => null,
        'aprovacao' => null,
    );

    public function salvarConclusao($dados)
    {
        $file = new FilesSimec('arquivo', null, 'obras2');
        $file->setPasta('obras2');
        if($dados['copid']) {
            $setExtra = '';
            if($_FILES['parecerfinal']) {
                $file->setUpload('', 'parecerfinal', false);
                $arqidParecerFinal = $file->getIdArquivo();
                $setExtra .= ', arqid_pf = '.$arqidParecerFinal;
            }
            if($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                $file->setUpload('', 'parecerfinalrevisado', false);
                $arqidParecerFinalRevisado = $file->getIdArquivo();
                $setExtra .= ', arqid_pf_revisado = '.$arqidParecerFinalRevisado;
            }
            $sql = <<<DML
                UPDATE {$this->stNomeTabela} SET aprovacao = '{$dados['aprovacao']}', valor_devolucao = {$dados['valor_devolucao']} {$setExtra} WHERE numprocesso = '{$dados['numprocesso']}'
DML;
            $this->executar($sql);
            $this->commit();
        } else {
            $file->setUpload('', 'parecerfinal', false);
            $arqidParecerFinal = $file->getIdArquivo();
            $arqidParecerFinalRevisado = 'NULL';
            if($_FILES['parecerfinalrevisado'] && $_FILES['parecerfinalrevisado']['error'] == 0 && $_FILES['parecerfinalrevisado']['size'] > 0) {
                $file->setUpload('', 'parecerfinalrevisado', false);
                $arqidParecerFinalRevisado = $file->getIdArquivo();
            } 

            $sql = <<<DML
                INSERT INTO {$this->stNomeTabela} (numprocesso, arqid_pf, arqid_pf_revisado, valor_devolucao, aprovacao) VALUES ('{$dados['numprocesso']}', '{$arqidParecerFinal}',{$arqidParecerFinalRevisado}, {$dados['valor_devolucao']}, '{$dados['aprovacao']}');
DML;
            $this->executar($sql);
            $this->commit();
        }
    }

    public function capturaDados($numprocesso)
    {
        $sql = <<<DML
            SELECT
                cop.copid,
                cop.arqid_pf,
                cop.arqid_pf_revisado,
                cop.valor_devolucao,
                cop.aprovacao,
                arq.arqnome AS pfnome,
                arq.arqextensao AS pfextensao,
                arq.usucpf AS pfusu,
                usu.usunome AS pfusunome,
                TO_CHAR(arq.arqdata,'DD/MM/YYYY') AS pfdata,
                arq2.arqnome AS pfrnome,
                arq2.arqextensao AS pfrextensao,
                arq2.usucpf AS pfrusu,
                usu2.usunome AS pfrusunome,
                TO_CHAR(arq2.arqdata,'DD/MM/YYYY') AS pfrdata
            FROM {$this->stNomeTabela} cop
            LEFT JOIN public.arquivo arq ON (cop.arqid_pf = arq.arqid)
            LEFT JOIN seguranca.usuario usu ON (arq.usucpf = usu.usucpf)
            LEFT JOIN public.arquivo arq2 ON (cop.arqid_pf_revisado = arq2.arqid)
            LEFT JOIN seguranca.usuario usu2 ON (arq2.usucpf = usu2.usucpf)
            WHERE cop.numprocesso = '$numprocesso';
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
        $file = new FilesSimec('cumprimento_objeto_processo',$this->arAtributos,'obras2');
        $file->setPulaTableEschema(true);
        $file->setRemoveUpload($arqid);
    }

    public function verificaObrasPorProcesso($processo,$recuperaObras = true)
    {
        $sql = <<<DML
            SELECT
                CASE WHEN
                    (SELECT count(*) FROM obras2.obras WHERE obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo')) <> (SELECT count(*) FROM obras2.obras obr INNER JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) INNER JOIN obras2.cumprimento_objeto_conclusao coc ON (co.coid = coc.coid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid))
                    THEN '0'
                    ELSE '1'
                    END
DML;
        $result = $this->pegaUm($sql);
        if($recuperaObras) {
            $obras = $this->retornaObrasPorProcesso($processo);
        }
        return array('result' => $result == '1' ? true : false, 'obras' => $obras);
    }

    public function retornaObrasPorProcesso($processo)
    {
        $sql = <<<DML
            SELECT obr.obrid, co.docid, co.coid FROM obras2.obras obr LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid);
DML;
        return $this->carregar($sql);
    }

    public function verificaObrasPorProcessoCondicao($processo,$coid)
    {
        $sql = <<<DML
            SELECT
                CASE WHEN
                    (SELECT count(*) FROM obras2.obras WHERE obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo')) <> (SELECT count(*) FROM obras2.obras obr INNER JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid) INNER JOIN workflow.documento doc ON (co.docid = doc.docid) WHERE obr.obrid IN (SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '$processo' ORDER BY obrid) AND doc.esdid = (select esdid from workflow.documento where docid = (select docid from obras2.cumprimento_objeto where coid = $coid)))
                    THEN '0'
                    ELSE '1'
                    END
DML;
        return $this->pegaUm($sql) == '0' ? false : true;
    }
}