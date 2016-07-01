<?php

/**
 * Description of CumprimentoObjetoQuestionario
 *
 * @author lindalbertofilho
 */
class FotosQuestionarioCumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.fotos_questionario_cumprimento_objeto";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'descricao' => null,
        'coqid' => null,
        'arqid' => null,
        'status' => null,
        'usucpf' => null,
        'dtinclusao' => null
    );


    public function salvarDados($dados)
    {
        $arrArqid = $dados['arquivo_qco'] ? $dados['arquivo_qco'] : array();
        if ($_FILES['arquivo_qco']['name'][0] != '') {
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_qco']['name'] as $key => $value) {
                if(empty($value))
                    continue;

                $files =  array(
                    'name' => $arquivos['arquivo_qco']['name'][$key],
                    'type' => $arquivos['arquivo_qco']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_qco']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_qco']['error'][$key],
                    'size' => $arquivos['arquivo_qco']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload($dados['arquivo_descricao'][$key], 'arquivo', false);
                $arqid = $file->getIdArquivo();
                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $this->popularDadosObjeto(array('coqid' => $dados['coqid'],'arqid' => $arqid, 'usucpf' => $_SESSION['usucpf']));
                    $this->salvar();
                }
            }
            $this->commit();
        }
        $this->verificaArqid($dados['coqid'], $arrArqid);
    }

    function verificaArqid($coqid, $arqid)
    {
        if(is_array($arqid)) {
            foreach ($arqid as $key => $id) {
                $arquivo = new Arquivo($id);
                $arquivo->arqdescricao = $_POST['arquivo_descricao'][$key];
                $arquivo->salvar();
                $arquivo->commit();
            }
        }

        $arqid = (is_array($arqid)) ? $arqid : array();
        $sql = "
            SELECT fqco.arqid
            FROM obras2.fotos_questionario_cumprimento_objeto fqco
            JOIN arquivo a ON a.arqid = fqco.arqid
            WHERE fqco.status = 'A' AND fqco.coqid = $coqid
        ";
        $arquivos  = $this->carregar($sql);

        if(!empty($arquivos)) {
            foreach ($arquivos as $arquivo) {
                if (array_search($arquivo['arqid'], $arqid) === false) {
                    $sql = "UPDATE obras2.fotos_questionario_cumprimento_objeto SET status = 'I' WHERE arqid = {$arquivo['arqid']}";
                    $this->executar($sql);
                    $this->commit();
                }
            }
        }
    }

    public function importaDados()
    {
        if($this->coqid){
            $sql = <<<DML
                SELECT fqco.arqid, arq.arqnome, arq.arqdescricao, arq.arqextensao
                FROM {$this->stNomeTabela} fqco
                INNER join public.arquivo arq ON fqco.arqid = arq.arqid
                WHERE fqco.coqid = $this->coqid
                    AND fqco.status = 'A'
                    AND arq.arqstatus = 'A'
DML;
            $dados = $this->carregar($sql);
            return $dados;
        }
    }

    public function verificaFotos()
    {
        if($this->coqid) {
            $sql = <<<DML
                SELECT COUNT(1) FROM {$this->stNomeTabela} WHERE status = 'A' AND coqid = {$this->coqid}
DML;
            if($this->pegaUm($sql) > 0) return true;
        }
    }
}