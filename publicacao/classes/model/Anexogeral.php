<?php
/**
 * Classe de mapeamento da entidade publicacao.anexogeral.
 *
 * $Id$
 */
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
/**
 * Mapeamento da entidade publicacao.anexogeral.
 *
 * @see Modelo
 */
class Publicacao_Model_Anexogeral extends Modelo
{
    /**
     *
     * @var Simec_Helper_FlashMessage
     */
    protected $_message;
    
    public function __construct() {
        $this->_message = new Simec_Helper_FlashMessage('publicacao/comunicados');
    }
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'publicacao.anexogeral';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'angid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'angid' => null,
        'angdsc' => null,
        'arqid' => null,
        'angtip' => null,
        'angano' => null,
        'solid' => null,
    );

public function cadastrarAnexo($solid){
    /* Verificando extensão. */
//    if (array_search(
//            strtolower(end(explode('.', $_FILES['file']['name']))), 
//            $this->_arExtencoes) === false) {
//        $this->_message->addMensagem('Por favor, envie arquivos somente com as extensões permitidas: <code>'.  implode(',', $this->_arExtencoes) .'</code>.', Simec_Helper_FlashMessage::AVISO);
//        return false;
//    } else {
    //caso exista anexo, remove e inclui novo arquivo
    if($this->existeAnexo($solid)){
        $this->deletarFile($this->existeAnexo($solid));
    }
    $campos = array(
        "angdsc"   => "'{$_FILES['solicitacao_anexo']['name']}'",
        "angtip"   => "''",
        "angano"   => "'{$_SESSION['exercicio']}'",
        "solid"    => $solid   
    );
    $file = new FilesSimec("anexogeral", $campos, "publicacao");

    if($file->setUpload($_FILES ['solicitacao_anexo']['name'] , '', true)){
        return $_FILES ['solicitacao_anexo']['value'] = $file->getIdArquivo();
    }else{
        return false;
    }
}

public function downloadAnexo( $solidAnexo ){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $sql = "SELECT arqid FROM publicacao.anexogeral as pag INNER JOIN publicacao.solicitacao ps ON ps.solid = pag.solid WHERE ps.solid = {$solidAnexo}";
    $arqid = $this->pegaUm($sql);
    if( $arqid ){
        $file = new FilesSimec("anexogeral", array(), "publicacao");
        $file->getDownloadArquivo( $arqid );
    }
}

public function deletarFile($arqid){
     $sql = "DELETE FROM {$this->stNomeTabela} WHERE arqid = {$arqid}";
     $this->executar($sql);
     if($this->commit()){
         $this->_message->addMensagem('Arquivo deletado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
         return true;
     }else{
         $this->_message->addMensagem('Falha ao deletar arquivo.', Simec_Helper_FlashMessage::ERRO);
         return false;
     }
 }
 public function existeAnexo( $solidAnexo ){
    $sql = "SELECT arqid FROM publicacao.anexogeral as pag INNER JOIN publicacao.solicitacao ps ON ps.solid = pag.solid WHERE ps.solid = {$solidAnexo}";
    $arqid = $this->pegaUm($sql);
    if($arqid){
        return $arqid;
    }
}
 
}