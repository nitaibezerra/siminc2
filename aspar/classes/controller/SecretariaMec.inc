<?php
/**
 * Classe de mapeamento da entidade aspar.secretariamec
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_SecretariaMec
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_SecretariaMec extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $secretariamec = new Aspar_Model_SecretariaMec();
        $secretariamec->popularDadosObjeto();
        $url .= '&scmid=' . $secretariamec->scmid;

        try{
            $sucesso = $secretariamec->salvar();
            $secretariamec->commit();
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
        $secretariamec = new Aspar_Model_SecretariaMec();        
        $secretariamec->popularDadosObjeto();
        try{
            $secretariamec->excluir();            
            $secretariamec->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $scmid
    * @return JSON
    */
    public function recuperarPorId($scmid)
    {        
        $secretariamec = new Aspar_Model_SecretariaMec();        
        $secretariamec->carregarPorId($scmid);        
        echo json_encode(Array('scmdsc' => utf8_encode($secretariamec->scmdsc)));
        
    }    



}//end Class
?>