<?php
/**
 * Classe de mapeamento da entidade aspar.situacao
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Situacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_Situacao extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $situacao = new Aspar_Model_Situacao();
        $situacao->popularDadosObjeto();
        $url .= '&sitid=' . $situacao->sitid;

        try{
            $sucesso = $situacao->salvar();
            $situacao->commit();
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
        $situacao = new Aspar_Model_Situacao();        
        $situacao->popularDadosObjeto();
        try{
            $situacao->excluir();            
            $situacao->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $sitid
    * @return JSON
    */
    public function recuperarPorId($sitid)
    {        
        $situacao = new Aspar_Model_Situacao();        
        $situacao->carregarPorId($sitid);        
        echo json_encode(Array('sitdsc' => utf8_encode($situacao->sitdsc)));
        
    }

}//end Class
?>