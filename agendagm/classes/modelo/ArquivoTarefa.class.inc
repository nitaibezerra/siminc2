<?php
/**
 * Classe de mapeamento da entidade agendagm.arquivotarefa.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade agendagm.arquivotarefa.
 *
 * @see Modelo
 */
class Agendagm_Model_ArquivoTarefa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'agendagm.arquivotarefa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'artid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'trfid' => array('tabela' => 'agendagm.tarefa', 'pk' => 'trfid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'artid' => null,
        'trfid' => null,
        'arqid' => null,
    );

    public function anexarArquivoTarefa( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){

        $trfid = $this->trfid;
               
        $file_count = count( $_FILES['arquivos']['name']) ;
        $file_keys  = array_keys( $_FILES['arquivos'] );

        for ($i=0; $i<$file_count; $i++) {
            foreach ($file_keys as $key) {
                $_FILES[$i][$key] = $_FILES['arquivos'][$key][$i];
            }
        }
        unset($_FILES['arquivos']);
        $array_files = $_FILES;        
        
        if( is_array($array_files) ){
            
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            
            $file = new FilesSimec("arquivotarefa", array(), "agendagm");

            foreach($array_files as $k => $arq){
                if( is_file($array_files[$k]["tmp_name"]) ){
                    $file->setUpload('Agenda GM - Anexos dos Enacaminhamentos-Tarefa', $k, false, 'arqid');
                }
                $arqid = $file->getIdArquivo();

                if( $arqid > 0 ){
                    $sql = "
                        INSERT INTO agendagm.arquivotarefa( trfid, arqid ) VALUES ( {$trfid}, {$arqid} ) RETURNING arqid
                    ";
                    $result = $this->pegaUm($sql);
                }
            }
        }

        if( $result > 0 ){
            return true;
        }else{
            return false;
        }
    }
    
    public function excluirArquivoTarefa( $arqid ){
        global $url;

        $sql = "DELETE FROM agendagm.arquivotarefa WHERE arqid = {$arqid} RETURNING trfid";
        $result = $this->pegaUm($sql);

        if( $result > 0 ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            if( $arqid ){
                $file = new FilesSimec("arquivotarefa", array(), "agendagm");
                $resp = $file->excluiArquivoFisico($arqid);
            }
            return true;
        }

        return false;
    }
    
    public function downloadArquivoTarefa( $arqid ){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if( $arqid ){
            $file = new FilesSimec("arquivotarefa", array(), "agendagm");
            $file->getDownloadArquivo( $arqid );
        }
    }

}