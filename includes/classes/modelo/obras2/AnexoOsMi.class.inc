<?php

class AnexoOsMi extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.anexoosmi";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("aomid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                   'aomid' => null,
                                   'osmid' => null,
                                   'taoid' => null,
                                   'arqid' => null,
                                  );

    public function getAnexoExecucaoSondagem($osmid){
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi
                WHERE
                    osmid = $osmid AND taoid = 1 AND aomstatus = 'A'";
        $row = $this->pegaLinha($sql);
        return $row;
    }
    
    public function getAnexoExecucao($osmid){
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi
                WHERE
                    osmid = $osmid AND taoid = 8 AND aomstatus = 'A' ORDER BY aomid DESC";
        $row = $this->pegaLinha($sql);
        return $row;
    }

    public function deletaAnexoPorArqid($arqid){        
        if(empty($arqid)){
            return false;
        }
        
        $sql = " update obras2.anexoosmi set aomstatus = 'I' where arqid = $arqid ";
        $this->executar($sql);
        $this->commit();
    }

    public function getAnexoExecucaoImplantacao($osmid){
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi aom
                JOIN
                    obras2.tipoanexoosmi tao on aom.taoid = tao.taoid
                WHERE
                    aom.osmid = $osmid AND tao.tomid = 3 AND aomstatus = 'A'";
        $row = $this->carregar($sql);
        return $row;
    }
    
    public function getOsAssinada($osmid){
                
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi aom
                JOIN
                    obras2.tipoanexoosmi tao on aom.taoid = tao.taoid
                WHERE
                    aom.osmid = $osmid AND tao.taoid = 8 AND aomstatus = 'A' ORDER BY aomid DESC";
        
        $row = $this->pegaLinha($sql);
        
        $row = ($row) ? $row : array();
        
        return $row;
    }

    public function getOsArt($osmid){

        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi aom
                JOIN
                    obras2.tipoanexoosmi tao on aom.taoid = tao.taoid
                WHERE
                    aom.osmid = $osmid AND tao.taoid = 13 AND aomstatus = 'A' ORDER BY aomid DESC";

        $row = $this->pegaLinha($sql);

        $row = ($row) ? $row : array();

        return $row;
    }
    
    public function getAnexoExecucaoTermoRecebimento($osmid){
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi aom
                JOIN
                    obras2.tipoanexoosmi tao on aom.taoid = tao.taoid
                WHERE
                    aom.osmid = $osmid AND tao.taoid = 11 AND aomstatus = 'A' ORDER BY aomid DESC";
        $row = $this->pegaLinha($sql);
        $row = ($row) ? $row : array();
        return $row;
    }
    
    public function getAnexoExecucaoPlanilhaMedicaoAcumulada($osmid){
        $sql = "SELECT
                    *
                FROM
                    obras2.anexoosmi aom
                JOIN
                    obras2.tipoanexoosmi tao on aom.taoid = tao.taoid
                WHERE
                    aom.osmid = $osmid AND tao.taoid = 12 AND aomstatus = 'A' ORDER BY aomid DESC";
        $row = $this->pegaLinha($sql);
        $row = ($row) ? $row : array();
        return $row;
    }
    
    /**
     * Salva os arquivos:
     *  - OSs assinadas
     *  - OS de Execução:
     *      - Termo de Recebimento
     *      - Planilha de Medição Acumulada
     */
    public function salvaArquivosOs() {

        $anexoOsMi = new AnexoOsMi();
        $anexoOs   = new AnexoOsMi();
        $tomid     = ($_REQUEST['tomid'] != '') ? $_REQUEST['tomid'] : $_SESSION['tomid'];

        $ordemservico = new OrdemServicoMI();
        $os           = $ordemservico->recuperarTodos("osmid, docid", array("osmstatus = 'A' AND obrid = " . $_SESSION['obras2']['obrid'], "tomid = " . $tomid), 'osmid ASC');
        if(empty($os)){
            die(' <script type="text/javascript"> alert("Erro ao salvar os arquivos."); window.location.reload(); </script>');
        }
        
        $os = array(array_pop($os));
        
        if(empty($os[0]['osmid'])){
            die(' <script type="text/javascript"> alert("Erro ao salvar os arquivos."); window.location.reload(); </script>');
        }
        
        $ordemservico->carregarPorId($os[0]['osmid']);
        $ordemservico->salvar();

        //OS de Exec - Planilha de Medição Acumulada
        if($_FILES["arquivo_os_pma"]['error'] == 0){
            $anexoOsMi = new AnexoOsMi();
            try{
                $anexo          = $anexoOs->getAnexoExecucaoPlanilhaMedicaoAcumulada($os[0]['osmid']);
                $anexo['taoid'] = 12;
                $arqid          = null;
                $arquivo        = $_FILES["arquivo_os_pma"];

                if ($_FILES["arquivo_os_tr"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo_os_pma', false);
                    $arqid = $file->getIdArquivo();
                }

                $anexo['osmid'] = $os[0]['osmid'];
                $anexo['arqid'] = $arqid;

                $anexoOsMi->popularDadosObjeto($anexo);
                $anexoOsMi->salvar();
                $anexoOsMi->commit();
            } catch (Exception $ex) {
                $anexoOsMi->rollback();
                die(' <script type="text/javascript"> alert("Erro ao salvar o arquivo Planilha de Medição Acumulada! Erro: '.$ex->getMessage().'");</script>');
            }
        }

        //OS de Exec - Termo de Recebimento
        if($_FILES["arquivo_os_tr"]['error'] == 0){
            try{
                $anexoOsMi = new AnexoOsMi();
                $anexo          = $anexoOs->getAnexoExecucaoTermoRecebimento($os[0]['osmid']);
                $anexo['taoid'] = 11;
                $arqid          = null;
                $arquivo        = $_FILES["arquivo_os_tr"];

                if ($_FILES["arquivo_os_tr"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo_os_tr', false);
                    $arqid = $file->getIdArquivo();
                }

                $anexo['osmid'] = $os[0]['osmid'];
                $anexo['arqid'] = $arqid;

                $anexoOsMi->popularDadosObjeto($anexo);
                $anexoOsMi->salvar();
                $anexoOsMi->commit();
            } catch (Exception $ex) {
                $anexoOsMi->rollback();
                die(' <script type="text/javascript"> alert("Erro ao salvar o arquivo Termo de Recebimento! Erro: '.$ex->getMessage().'");</script>');
            }

        }

        //OS Assinada
        if($_FILES["arquivo_s"]['error'] == 0){
            try{
                $anexoOsMi = new AnexoOsMi();
                $anexo          = $anexoOs->getOsAssinada($os[0]['osmid']);

                $arqid          = null;
                $arquivo        = $_FILES["arquivo_s"];

                if ($_FILES["arquivo_s"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo_s', false);
                    $arqid = $file->getIdArquivo();
                }

                $anexo['taoid'] = 8;
                $anexo['osmid'] = $os[0]['osmid'];
                $anexo['arqid'] = $arqid;

                $anexoOsMi->popularDadosObjeto($anexo);
                $anexoOsMi->salvar();
                $anexoOsMi->commit();
            } catch (Exception $ex) {
                $anexoOsMi->rollback();
                die(' <script type="text/javascript"> alert("Erro ao salvar o arquivo Ordem de Serviço Assinada! Erro: '.$ex->getMessage().'");</script>');
            }
        }

        //ART/RRT
        if($_FILES["arquivo_art"]['error'] == 0){
            try{
                $anexoOsMi = new AnexoOsMi();
                $anexo          = $anexoOs->getOsArt($os[0]['osmid']);

                $arqid          = null;
                $arquivo        = $_FILES["arquivo_art"];

                if ($_FILES["arquivo_art"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo_art', false);
                    $arqid = $file->getIdArquivo();
                }

                $anexo['taoid'] = 13;
                $anexo['osmid'] = $os[0]['osmid'];
                $anexo['arqid'] = $arqid;

                $anexoOsMi->popularDadosObjeto($anexo);
                $anexoOsMi->salvar();
                $anexoOsMi->commit();
            } catch (Exception $ex) {
                $anexoOsMi->rollback();
                die(' <script type="text/javascript"> alert("Erro ao salvar o arquivo ART/RRT! Erro: '.$ex->getMessage().'");</script>');
            }
        }
    }

}