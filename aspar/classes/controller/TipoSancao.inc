<?php
/**
 * Classe de mapeamento da entidade aspar.tiposancao
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_TipoSancao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Controller_TipoSancao extends Modelo
{
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $tiposancao = new Aspar_Model_TipoSancao();
        $tiposancao->popularDadosObjeto();
        $url .= '&tpsid=' . $tiposancao->tpsid;

        try{
            $sucesso = $tiposancao->salvar();
            $tiposancao->commit();
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
        $tiposancao = new Aspar_Model_TipoSancao();        
        $tiposancao->popularDadosObjeto();
        try{
            $tiposancao->excluir();            
            $tiposancao->commit();
            echo json_encode('success');
        } catch (Simec_Db_Exception $e) {
            echo json_encode($e);
        }
    }//end excluir()
    
   /**
    * Retorna a descrição da Situação do Projeto    
    * @param type $tpsid
    * @return JSON
    */
    public function recuperarPorId($tpsid)
    {        
        $tiposancao = new Aspar_Model_TipoSancao();        
        $tiposancao->carregarPorId($tpsid);        
        echo json_encode(Array('tpsdsc' => utf8_encode($tiposancao->tpsdsc)));
        
    }
}//end Class
?>