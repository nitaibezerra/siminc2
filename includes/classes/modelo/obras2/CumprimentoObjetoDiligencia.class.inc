<?php
/**
 * Description of DiligenciaCumprimentoObjeto
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoDiligencia extends modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_diligencia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("arqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'coid' => null,
        'usucpf' => null,
        'tipo' => null,
        'arqid' => null,
    );

    public function salvarDados($dados)
    {
        $file = new FilesSimec('arquivo', null, 'obras2');
        $file->setPasta('obras2');
        $file->setUpload($dados['descricao'], 'arquivo', false);
        $arqid = $file->getIdArquivo();
        if ($arqid) {
            $sql = <<<DML
                INSERT INTO {$this->stNomeTabela} (coid, usucpf, tipo, arqid) VALUES ({$dados['coid']}, '{$_SESSION['usucpf']}','{$dados['tipo']}', $arqid);
DML;
            $this->executar($sql);
            $this->commit();
        }
    }

    public function getDados($coid, $tipo)
    {
        $sql = <<<DML
            SELECT
                usu.usunome,
                arq.arqdescricao
            FROM {$this->stNomeTabela} cod
            INNER JOIN public.arquivo arq ON (arq.arqid = cod.arqid)
            INNER JOIN seguranca.usuario usu ON (usu.usucpf = cod.usucpf)
DML;
    }

    public function excluir($arqid)
    {
        if(!$arqid){
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }

        $file = new FilesSimec('cumprimento_objeto_diligencia',$this->arAtributos,'obras2');
        $file->setPulaTableEschema();
        $file->setRemoveUpload($arqid);
    }
}