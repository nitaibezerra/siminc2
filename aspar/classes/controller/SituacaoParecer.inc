<?php
/**
 * Classe de mapeamento da entidade aspar.situacaoparecer
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Situacao_Parecer
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_SituacaoParecer extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $situacaoparecer = new Aspar_Model_SituacaoParecer();
        $situacaoparecer->popularDadosObjeto();
        $url .= '&stpid=' . $situacaoparecer->stpid;

        try{
            $sucesso = $situacaoparecer->salvar();
            $situacaoparecer->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }        
    }//end gravar()


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function excluir()
    {
        global $url;       
        $situacaoparecer = new Aspar_Model_SituacaoParecer();        
        $situacaoparecer->popularDadosObjeto();
        try{
            $situacaoparecer->excluir();            
            $situacaoparecer->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $stpid
    * @return JSON
    */
    public function recuperarPorId($stpid)
    {        
        $situacaoparecer = new Aspar_Model_SituacaoParecer();        
        $situacaoparecer->carregarPorId($stpid);        
        echo json_encode(Array('stpdsc' => utf8_encode($situacaoparecer->stpdsc)));
        
    }
}//end Class
?>