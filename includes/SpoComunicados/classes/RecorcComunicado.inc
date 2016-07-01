<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of RecorcComunicado
 *
 * @author LindalbertoFilho
 */
class RecorcComunicado extends Modelo{
    
    protected $stNomeTabela = 'recorc.anexogeral';
    protected $arChavePrimaria = array('angid');
    protected $arAtributos = array(
        'angid'  => NULL,
        'angdsc' => NULL,
        'arqid'  => NULL,
        'angtip' => NULL,
        'capid'  => NULL
    );

    protected $_arExtencoes = array('pdf');
    /**
     *
     * @var Simec_Helper_FlashMessage
     */
    protected $_message;
    
    public function __construct() {
        $this->_message = new Simec_Helper_FlashMessage(MODULO.'/comunicados');
    }
    
    public function buscar()
    {
        if(!$this->verificaValor(array('arqid' => $this->arAtributos['arqid']))){
            return false;
        }
        $query = <<<DML
            SELECT
                con.arqid,
                con.angdsc,
                '<a href=\"recorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo
            FROM {$this->stNomeTabela} con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            WHERE con.arqid = {$this->arAtributos['arqid']}
DML;
        $dados = $this->pegaLinha($query);
        if($dados){
            return $dados;
        }else{
            $this->_message->addMensagem('Arquivo não encontrado.', Simec_Helper_FlashMessage::ERRO);
            return false;
        }
    }
    
    public function cadastrar()
    {
        /* Verificando extensão. */
        if (array_search(
                strtolower(end(explode('.', $_FILES['file']['name']))), 
                $this->_arExtencoes) === false) {
            $this->_message->addMensagem('Por favor, envie arquivos somente com as extensões permitidas: <code>'.  implode(',', $this->_arExtencoes) .'</code>.', Simec_Helper_FlashMessage::AVISO);
            return false;
        } else {
            $campos = array(
                "angdsc"       => "'{$_POST['nomeArquivo']}'",
                "angtip"       => "'CO'",
                "angano" => "'{$_SESSION['exercicio']}'"
            );
            $file = new FilesSimec("anexogeral", $campos, "recorc");
            if($file->setUpload($_FILES ['file'] ['name'], '', true)){
                $this->_message->addMensagem('Arquivo cadastrado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
                return true;
            }else{
                $this->_message->addMensagem('Falha ao cadastrar arquivo.', Simec_Helper_FlashMessage::ERRO);
                return false;
            }
            
        }
    }
    
    public function atualizar()
    {
        if(!$this->verificaValor(array('angdsc' => $this->arAtributos['angdsc'],'arqid' => $this->arAtributos['arqid']))){
            return false;
        }
        $query = <<<DML
            UPDATE {$this->stNomeTabela} 
                SET angdsc = '{$this->arAtributos['angdsc']}'
            WHERE arqid = '{$this->arAtributos['arqid']}'
DML;
        $this->executar($query);
        if($this->commit()){
            $this->_message->addMensagem('Arquivo atualizado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            return true;
        }else{
            $this->_message->addMensagem('Falha ao atualizar arquivo.', Simec_Helper_FlashMessage::ERRO);
            return false;
        }
            
    }
    
    public function verificaValor($valor){        
        if(is_array($valor)){
            $retorno = true;
            foreach($valor as $key => $val){
                if(!$val || $val == null || trim($val) == ''){
                    trigger_error( "O ATRIBUTO ({$key}) NÃO ESTÁ POPULADO!" );
                    $retorno = false;
                }
            }
            return $retorno;
        }
    }
    
    public function deletar(){
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE arqid = {$this->arAtributos['arqid']}";
        $this->executar($sql);
        if($this->commit()){
            $this->_message->addMensagem('Arquivo deletado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            return true;
        }else{
            $this->_message->addMensagem('Falha ao deletar arquivo.', Simec_Helper_FlashMessage::ERRO);
            return false;
        }
    }
    
    /**
     * @return type Simec_Listagem
     */
    public function listar(){

        $sql = <<<DML
            SELECT
                con.arqid,
                '<a href=\"recorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
                con.angdsc,
                arq.arqtamanho,
                to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                usu.usunome
            FROM {$this->stNomeTabela} con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
            WHERE con.angtip = 'CO'
            AND angano = '{$_SESSION['exercicio']}'
            ORDER BY angtip
DML;
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(array ("Download","Descrição","Tamanho(bytes)","Data inclusão","Responsável"));
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setAcoes(array('edit' => 'editLimite','delete' => 'deleteLimite','send'  => 'comunicadoEmail'));
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    }
    
    public function listaInicial(){       
        $sql = <<<HTML
            SELECT DISTINCT 
                arq.arqid,
                arq.arqnome,
                arq.arqextensao,
                con.angdsc
            FROM {$this->stNomeTabela} con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
            WHERE con.angtip = 'CO'
            AND angano = '{$_SESSION['exercicio']}'
            ORDER BY angdsc
HTML;
        return $this->carregar($sql);
    }
    
    public function buscarDescricao(){
        $query = <<<DML
            SELECT 
                con.angdsc 
            FROM {$this->stNomeTabela} con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
            WHERE con.arqid = {$this->arAtributos['arqid']}
            ORDER BY angtip
DML;
        return $this->pegaUm($query);
    }
}
