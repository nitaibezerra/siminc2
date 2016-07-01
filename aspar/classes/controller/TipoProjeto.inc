<?php
/**
 * Classe de mapeamento da entidade aspar.tipoprojeto
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_TipoProjeto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_TipoProjeto extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $tipoprojeto = new Aspar_Model_TipoProjeto();
        $tipoprojeto->popularDadosObjeto();
        $url .= '&tpjid=' . $tipoprojeto->tpjid;

        try{
            $sucesso = $tipoprojeto->salvar();
            $tipoprojeto->commit();
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
        $tipoprojeto = new Aspar_Model_TipoProjeto();        
        $tipoprojeto->popularDadosObjeto();
        try{
            $tipoprojeto->excluir();            
            $tipoprojeto->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $tpjid
    * @return JSON
    */
    public function recuperarPorId($tpjid)
    {        
        $tipoprojeto = new Aspar_Model_TipoProjeto();        
        $tipoprojeto->carregarPorId($tpjid);        
        echo json_encode(Array('tpjdsc' => utf8_encode($tipoprojeto->tpjdsc)));
        
    }
}//end Class
?>