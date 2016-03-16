<?php
/**
 * Classe de mapeamento da entidade aspar.partidos
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Partidos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_Partidos extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $partidos = new Aspar_Model_Partidos();
        $partidos->popularDadosObjeto();
        $url .= '&prtid=' . $partidos->prtid;

        try{
            $sucesso = $partidos->salvar();
            $partidos->commit();
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
        $partidos = new Aspar_Model_Partidos();        
        $partidos->popularDadosObjeto();
        try{
            $partidos->excluir();            
            $partidos->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $prtid
    * @return JSON
    */
    public function recuperarPorId($prtid)
    {        
        $partidos = new Aspar_Model_Partidos();        
        $partidos->carregarPorId($prtid);        
        echo json_encode(Array('prtdsc' => utf8_encode($partidos->prtdsc)));
        
    }   



}//end Class
?>